#include <iostream>
#include <SDL_image.h>
#include "ResourceManager.hpp"

ResourceManager::ResourceManager()
{
  framebuffers_.push_back(0);
  textures_.push_back(0);
}

ResourceManager::~ResourceManager()
{
  cleanup();
}

void ResourceManager::cleanup()
{
  for (auto framebuffer: framebuffers_)
  {
    glDeleteFramebuffers(1, &framebuffer);
  }
  for (auto program: gpu_programs_)
  {
    glDeleteProgram(program.handle);
  }
  for (auto mesh: meshes_)
  {
    glDeleteBuffers(1, &mesh.position_buffer);
    glDeleteBuffers(1, &mesh.uv_buffer);
    glDeleteBuffers(1, &mesh.index_buffer);
    glDeleteVertexArrays(1, &mesh.vertex_array);
  }
  for (auto texture: textures_)
  {
    glDeleteTextures(1, &texture);
  }
}

GLuint ResourceManager::build_shader_(GLenum type, const std::string& sources)
{
  GLuint shader = glCreateShader(type);
  CHECK_GL_ERROR;
  const char* source_ptr = sources.c_str();
  glShaderSource(shader, 1, &source_ptr, nullptr);
  CHECK_GL_ERROR;
  glCompileShader(shader);
  CHECK_GL_ERROR;
  char info_log[1000];
  GLsizei log_length;
  glGetShaderInfoLog(shader, sizeof(info_log), &log_length, info_log);
  CHECK_GL_ERROR;
  if (log_length > 0)
    std::cout << "Shader build log: " << info_log << "\n";
  else
    std::cout << "Shader built.\n";
  return shader;
}

GLuint ResourceManager::build_vertex_shader_(const std::string& sources)
{
  return build_shader_(GL_VERTEX_SHADER, sources);
}

GLuint ResourceManager::build_fragment_shader_(const std::string& sources)
{
  return build_shader_(GL_FRAGMENT_SHADER, sources);
}

GLuint ResourceManager::link_gpu_program_(GLuint vertex_shader,
    GLuint fragment_shader)
{
  GLuint gpu_program = glCreateProgram();
  CHECK_GL_ERROR;
  glAttachShader(gpu_program, vertex_shader);
  CHECK_GL_ERROR;
  glAttachShader(gpu_program, fragment_shader);
  CHECK_GL_ERROR;
  glLinkProgram(gpu_program);
  CHECK_GL_ERROR;
  char info_log[1000];
  GLsizei log_length;
  glGetProgramInfoLog(gpu_program, sizeof(info_log), &log_length, info_log);
  CHECK_GL_ERROR;
  if (log_length > 0)
    std::cout << "GPU program link log: " << info_log << "\n";
  else
    std::cout << "GPU program linked.\n";
  return gpu_program;
}

uint32_t ResourceManager::load_gpu_program_from_file(const std::string& vs_path,
    const std::string& fs_path)
{
  GLuint vertex_shader_id = load_shader_<GL_VERTEX_SHADER>(vs_path);
  GLuint fragment_shader_id = load_shader_<GL_FRAGMENT_SHADER>(fs_path);
  GLuint program_id = link_gpu_program_(vertex_shader_id, fragment_shader_id);

  GpuProgram program;
  program.handle = program_id;

  program.model_location = glGetUniformLocation(program_id, "model");
  program.view_location = glGetUniformLocation(program_id, "view");
  program.projection_location = glGetUniformLocation(program_id, "projection");
  program.position_location = glGetAttribLocation(program_id, "position");
  program.uv_location = glGetAttribLocation(program_id, "uv");

  uint32_t id = gpu_programs_.size();
  gpu_programs_.push_back(program);
  return id;
}

GLenum ResourceManager::sdl_to_gl_pixel_format_(SDL_PixelFormat* format)
{
  uint32_t r_mask = format->Rmask;
  uint8_t bpp = format->BitsPerPixel;

  if (bpp == 32)
  {
    if (r_mask == 0x0000ff00)
      return GL_RGBA;
    else if (r_mask == 0xff000000)
      return GL_BGRA;
    std::cerr << "Sorry, we only support RGB, RGBA, BGR and BGRA images.\n";
    assert(false);
  }
  if (r_mask == 0x00ff0000)
    return GL_BGR;
  else if (r_mask == 0x000000ff)
    return GL_RGB;
  std::cerr << "Sorry, we only support RGB, RGBA, BGR and BGRA images.\n";
  assert(false);
}

GLenum ResourceManager::sdl_to_gl_pixel_type_(SDL_PixelFormat* format)
{
  switch (format->BitsPerPixel)
  {
    case 24:
    case 32:
      return GL_UNSIGNED_BYTE;
    default:
      std::cerr << "Sorry, we only support 24- and 32-bit images.";
      assert(false);
  }
}

GLuint ResourceManager::load_texture_(const std::string& path)
{
  SDL_Surface* img_surface = IMG_Load(path.c_str());
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  GLenum format = sdl_to_gl_pixel_format_(img_surface->format);
  GLenum type = sdl_to_gl_pixel_type_(img_surface->format);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_surface->w, img_surface->h,
      0, format, type, nullptr);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, img_surface->w, img_surface->h,
      format, type, img_surface->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  SDL_FreeSurface(img_surface);
  return texture;
}

GLuint ResourceManager::load_texture_from_file(const std::string& path)
{
  GLuint texture = load_texture_(path);
  uint32_t id = textures_.size();
  textures_.push_back(texture);
  return id;
}

uint32_t ResourceManager::create_mesh(const std::vector<float>& positions,
    const std::vector<float>& uvs,
    const std::vector<unsigned int>& indices)
{
  GLuint vertex_array;
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);

  GLuint position_buffer;
  glGenBuffers(1, &position_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(),
    &positions[0], GL_STATIC_DRAW);

  GLuint uv_buffer;
  glGenBuffers(1, &uv_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvs.size(), &uvs[0],
      GL_STATIC_DRAW);

  GLuint index_buffer;
  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
      &indices[0], GL_STATIC_DRAW);

  uint32_t id = meshes_.size();
  meshes_.push_back({position_buffer, uv_buffer, index_buffer, indices.size(),
      GL_UNSIGNED_INT, vertex_array});
  return id;
}

uint32_t ResourceManager::create_material(uint32_t gpu_program)
{
  uint32_t id = materials_.size();
  materials_.push_back(Material(*this, gpu_program));
  return id;
}

uint32_t ResourceManager::create_texture(GLsizei width, GLsizei height,
    GLenum format, GLenum internal_format, GLenum component_type)
{
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, internal_format,
      component_type, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  uint32_t id = textures_.size();
  textures_.push_back(texture);
  return id;
}

uint32_t ResourceManager::create_framebuffer(uint32_t albedo_rt_id,
    uint32_t normal_rt_id, uint32_t depth_rt_id)
{
  GLuint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D, get_texture(albedo_rt_id), 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
      GL_TEXTURE_2D, get_texture(normal_rt_id), 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
      GL_TEXTURE_2D, get_texture(depth_rt_id), 0);
  check_gl_framebuffer(GL_FRAMEBUFFER);
  uint32_t id = framebuffers_.size();
  framebuffers_.push_back(framebuffer);
  return id;
}

const GpuProgram& ResourceManager::get_gpu_program(uint32_t id) const
{
  return gpu_programs_[id];
}

const Mesh& ResourceManager::get_mesh(uint32_t id) const
{
  return meshes_[id];
}

GLuint ResourceManager::get_texture(uint32_t id) const
{
  return textures_[id];
}

const Material& ResourceManager::get_material(uint32_t id) const
{
  return materials_[id];
}

GLuint ResourceManager::get_framebuffer(uint32_t id) const
{
  return framebuffers_[id];
}

uint32_t ResourceManager::register_material(Material&& material)
{
  uint32_t id = materials_.size();
  materials_.push_back(material);
  return id;
}
