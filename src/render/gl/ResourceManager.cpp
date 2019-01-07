/* Copyright (C) 2018 Antoine Luciani
 *
 * This file is part of Sturdy Donkey.
 *
 * Sturdy Donkey is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, version 3.
 *
 * Sturdy Donkey is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Sturdy Donkey. If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <SDL_image.h>

#include "render/gl/Material.hpp"
#include "render/gl/ResourceManager.hpp"

namespace donkey {
namespace render {
namespace gl {

const std::vector<GLenum> ResourceManager::pixel_internal_formats_ = {
  GL_RGB8,
  GL_RGBA8,
  GL_DEPTH_COMPONENT24
};

const std::vector<GLenum> ResourceManager::pixel_formats_ = {
  GL_RGB,
  GL_RGBA,
  GL_DEPTH_COMPONENT
};

const std::vector<GLenum> ResourceManager::pixel_component_types_ = {
  GL_BYTE,
  GL_UNSIGNED_BYTE,
  GL_FLOAT
};

ResourceManager::ResourceManager()
{
  textures_.push_back(Texture(0));
}

ResourceManager::~ResourceManager()
{
  cleanup();
}

void ResourceManager::cleanup()
{
  for (auto framebuffer: framebuffers_)
  {
    glDeleteFramebuffers(1, &framebuffer.handle);
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
  for (const Texture& texture: textures_)
  {
    glDeleteTextures(1, &(texture.texture));
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

SDL_Surface* ResourceManager::create_mirror_surface_(SDL_Surface* surface)
{
  uint32_t bpp = surface->format->BytesPerPixel;
  int pitch = surface->pitch;
  int width = surface->w;
  int height = surface->h;
  SDL_Surface* new_surface = SDL_CreateRGBSurfaceWithFormat(0, width, height,
      32, SDL_PIXELFORMAT_RGBA32);
  char* src_pixels = reinterpret_cast<char*>(surface->pixels);
  char* dst_pixels = reinterpret_cast<char*>(new_surface->pixels);

  SDL_LockSurface(new_surface);
  SDL_LockSurface(surface);
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      uint32_t* src_ptr = (uint32_t*)(src_pixels
        + (y * pitch) + x * bpp);
      uint32_t* dst_ptr = (uint32_t*)(dst_pixels
        + ((height - y) * pitch) + x * bpp);
      *dst_ptr = *src_ptr;
    }
  }
  SDL_UnlockSurface(surface);
  SDL_UnlockSurface(new_surface);
  return new_surface;
}

GLuint ResourceManager::load_texture_(const std::string& path)
{
  SDL_Surface* img_surface = IMG_Load(path.c_str());
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  GLenum format = sdl_to_gl_pixel_format_(img_surface->format);
  GLenum type = sdl_to_gl_pixel_type_(img_surface->format);
  SDL_Surface* mirror_surface = create_mirror_surface_(img_surface);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mirror_surface->w, mirror_surface->h,
      0, format, type, nullptr);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mirror_surface->w, mirror_surface->h,
      format, type, mirror_surface->pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  SDL_FreeSurface(img_surface);
  SDL_FreeSurface(mirror_surface);
  return texture;
}

GLuint ResourceManager::load_texture_from_file(const std::string& path)
{
  std::cout << "Loading texture from file: " << path << '\n';
  GLuint texture = load_texture_(path);
  uint32_t id = textures_.size();
  textures_.push_back(Texture(texture));
  return id;
}

uint32_t ResourceManager::create_mesh(
    const std::vector<float>& positions,
    const std::vector<float>& normals,
    const std::vector<float>& uvs,
    const std::vector<float>& tangents,
    const std::vector<float>& bitangents,
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

  GLuint normal_buffer;
  glGenBuffers(1, &normal_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(),
    &normals[0], GL_STATIC_DRAW);

  GLuint uv_buffer;
  glGenBuffers(1, &uv_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvs.size(), &uvs[0],
      GL_STATIC_DRAW);

  GLuint tangent_buffer;
  glGenBuffers(1, &tangent_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, tangent_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tangents.size(),
    &tangents[0], GL_STATIC_DRAW);

  GLuint bitangent_buffer;
  glGenBuffers(1, &bitangent_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, bitangent_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * bitangents.size(),
    &bitangents[0], GL_STATIC_DRAW);

  GLuint index_buffer;
  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
      &indices[0], GL_STATIC_DRAW);

  uint32_t id = meshes_.size();
  meshes_.push_back(Mesh(
    position_buffer,
    normal_buffer,
    uv_buffer,
    tangent_buffer,
    bitangent_buffer,
    index_buffer,
    vertex_array));
  return id;
}

uint32_t ResourceManager::create_material(uint32_t gpu_program)
{
  uint32_t id = materials_.size();
  materials_.push_back(Material(*this, gpu_program));
  return id;
}

uint32_t ResourceManager::create_texture(
    std::size_t width,
    std::size_t height,
    pixel::Format format,
    pixel::InternalFormat internal_format,
    pixel::ComponentType component_type)
{
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    pixel_internal_formats_[static_cast<std::size_t>(internal_format)],
    width,
    height,
    0,
    pixel_formats_[static_cast<std::size_t>(format)],
    pixel_component_types_[static_cast<std::size_t>(component_type)],
    nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
  uint32_t id = textures_.size();
  textures_.push_back(Texture(texture));
  return id;
}

uint32_t ResourceManager::create_framebuffer(uint32_t albedo_rt_id,
    uint32_t normal_rt_id, uint32_t depth_rt_id)
{
  GLuint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D, get_texture(albedo_rt_id).texture, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
      GL_TEXTURE_2D, get_texture(normal_rt_id).texture, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
      GL_TEXTURE_2D, get_texture(depth_rt_id).texture, 0);
  check_gl_framebuffer(GL_FRAMEBUFFER);
  uint32_t id = framebuffers_.size();
  framebuffers_.push_back(Framebuffer(framebuffer, 2));
  return id;
}

uint32_t ResourceManager::create_framebuffer(uint32_t color_rt_id,
    uint32_t depth_rt_id)
{
  GLuint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D, get_texture(color_rt_id).texture, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
      GL_TEXTURE_2D, get_texture(depth_rt_id).texture, 0);
  check_gl_framebuffer(GL_FRAMEBUFFER);
  uint32_t id = framebuffers_.size();
  framebuffers_.push_back(Framebuffer(framebuffer, 1));
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

const Texture& ResourceManager::get_texture(uint32_t id) const
{
  return textures_[id];
}

const Framebuffer& ResourceManager::get_framebuffer(uint32_t id) const
{
  return framebuffers_[id];
}

AMaterial& ResourceManager::get_material(std::uint32_t id)
{
  return materials_[id];
}

}
}
}
