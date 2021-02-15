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

#include <function>
#include <iostream>
#pragma warning(push)
#pragma warning(disable : 26812 26819)
#include <SDL_image.h>
#pragma warning(pop)

#include "render/gl/Material.hpp"
#include "render/gl/ResourceManager.hpp"

namespace donkey {
namespace render {
namespace gl {

const std::array<GLenum, 4> ResourceManager::pixel_internal_formats_ = {
    GL_RGB8, GL_RGB16F, GL_RGBA8, GL_DEPTH_COMPONENT24};

const std::array<GLenum, 3> ResourceManager::pixel_formats_ = {
    GL_RGB, GL_RGBA, GL_DEPTH_COMPONENT};

const std::array<GLenum, 3> ResourceManager::pixel_component_types_ = {
    GL_BYTE, GL_UNSIGNED_BYTE, GL_FLOAT};

ResourceManager::ResourceManager() {
  textures_.push_back(Texture(0));
}

ResourceManager::~ResourceManager() {
  cleanup();
}

void ResourceManager::cleanup() {
  for (const auto& framebuffer : framebuffers_) {
    glDeleteFramebuffers(1, &(framebuffer.handle));
  }
  for (const auto& program : gpu_programs_) {
    glDeleteProgram(program.handle);
  }
  for (const auto& mesh : meshes_) {
    glDeleteBuffers(1, &(mesh.vertex_buffer));
    glDeleteBuffers(1, &(mesh.index_buffer));
    glDeleteVertexArrays(1, &(mesh.vertex_array));
  }
  for (const auto& texture : textures_) {
    glDeleteTextures(1, &(texture.texture));
  }
}

GLuint ResourceManager::build_shader_(GLenum type, const std::string& sources) {
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

GLuint ResourceManager::build_vertex_shader_(const std::string& sources) {
  return build_shader_(GL_VERTEX_SHADER, sources);
}

GLuint ResourceManager::build_fragment_shader_(const std::string& sources) {
  return build_shader_(GL_FRAGMENT_SHADER, sources);
}

GLuint ResourceManager::link_gpu_program_(GLuint vertex_shader,
                                          GLuint fragment_shader) {
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

uint32_t ResourceManager::load_gpu_program_from_file(
    const std::string& vs_path,
    const std::string& fs_path) {
  GLuint vertex_shader_id = load_shader_<GL_VERTEX_SHADER>(vs_path);
  GLuint fragment_shader_id = load_shader_<GL_FRAGMENT_SHADER>(fs_path);
  GLuint program_id = link_gpu_program_(vertex_shader_id, fragment_shader_id);

  GpuProgram program;
  program.handle = program_id;

  program.position_location = glGetAttribLocation(program_id, "position");
  program.uv_location = glGetAttribLocation(program_id, "uv");
  fetch_gpu_program_infos_(program);

  uint32_t id = static_cast<uint32_t>(gpu_programs_.size());
  gpu_programs_.push_back(program);
  return id;
}

void ResourceManager::fetch_gpu_program_infos_(GpuProgram& program) {
  GLint block_count;
  GLuint handle = program.handle;
  std::hash<std::string> hash;

  glGetProgramiv(handle, GL_ACTIVE_UNIFORM_BLOCKS, &block_count);
  program.uniform_blocks.reserve(block_count);
  for (GLint block_index = 0; block_index < block_count; ++block_index) {
    GLint name_length;
    std::vector<GLchar> name_vec;
    std::string name;

    glGetActiveUniformBlockiv(handle, block_index,
                              GL_UNIFORM_BLOCK_NAME_LENGTH, &name_length);
    name_vec.resize(name_length);
    glGetActiveUniformBlockName(handle, block_index, name_length, nullptr,
                                name_vec.data());
    name.assign(name_vec.begin(), name_vec.end() - 1);
    program.uniform_blocks[hash(name)] = block_index;
  }
}

GLenum ResourceManager::sdl_to_gl_pixel_format_(SDL_PixelFormat* format) {
  uint32_t r_mask = format->Rmask;
  uint8_t bpp = format->BitsPerPixel;

  if (bpp == 32) {
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
  return 0;  // will never happen but makes MSVC happy
}

GLenum ResourceManager::sdl_to_gl_pixel_type_(SDL_PixelFormat* format) {
  switch (format->BitsPerPixel) {
    case 24:
    case 32:
      return GL_UNSIGNED_BYTE;
    default:
      std::cerr << "Sorry, we only support 24- and 32-bit images.";
      assert(false);
  }
  return 0;  // should never arrive because of assert(false)
}

GLuint ResourceManager::load_texture_(uint8_t* pixels, int width, int height) {
  GLenum type = GL_UNSIGNED_BYTE;
  GLuint texture;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, type,
               nullptr);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, type, pixels);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  return texture;
}

// uint32_t ResourceManager::load_texture_from_file(const std::string& path)
//{
//  std::cout << "Loading texture from file: " << path << '\n';
//  GLuint texture = load_texture_(path);
//  uint32_t id = textures_.size();
//  textures_.push_back(Texture(texture));
//  return id;
//}

uint32_t ResourceManager::load_texture_from_memory(uint8_t* pixels,
                                                   int width,
                                                   int height) {
  std::cout << "Loading texture from memory.\n";
  GLuint texture = load_texture_(pixels, width, height);
  uint32_t id = static_cast<uint32_t>(textures_.size());
  textures_.push_back(Texture(texture));
  return id;
}

uint32_t ResourceManager::create_mesh(
    const std::vector<float>& vertices,
    const std::vector<unsigned int>& indices) {
  GLuint vertex_array;
  glGenVertexArrays(1, &vertex_array);
  glBindVertexArray(vertex_array);

  GLuint vertex_buffer;
  glGenBuffers(1, &vertex_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0],
               GL_STATIC_DRAW);

  GLuint index_buffer;
  glGenBuffers(1, &index_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
               &indices[0], GL_STATIC_DRAW);

  uint32_t id = static_cast<uint32_t>(meshes_.size());
  meshes_.push_back(Mesh(vertex_buffer, index_buffer, vertex_array));
  return id;
}

uint32_t ResourceManager::create_material(uint32_t gpu_program) {
  uint32_t id = static_cast<uint32_t>(materials_.size());
  materials_.push_back(Material(*this, gpu_program));
  return id;
}

uint32_t ResourceManager::create_texture(std::size_t width,
                                         std::size_t height,
                                         pixel::Format format,
                                         pixel::InternalFormat internal_format,
                                         pixel::ComponentType component_type) {
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(
      GL_TEXTURE_2D, 0,
      pixel_internal_formats_[static_cast<std::size_t>(internal_format)],
      static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0,
      pixel_formats_[static_cast<std::size_t>(format)],
      pixel_component_types_[static_cast<std::size_t>(component_type)],
      nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
  uint32_t id = static_cast<uint32_t>(textures_.size());
  textures_.push_back(Texture(texture));
  return id;
}

uint32_t ResourceManager::create_framebuffer(
    uint32_t depth_rt_id,
    const std::vector<uint32_t>& color_rt_ids) {
  GLuint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  for (size_t i = 0; i < color_rt_ids.size(); ++i) {
    uint32_t id = color_rt_ids[i];
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(i),
                           GL_TEXTURE_2D, get_texture(id).texture, 0);
  }
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         get_texture(depth_rt_id).texture, 0);
  check_gl_framebuffer(GL_FRAMEBUFFER);
  uint32_t id = static_cast<uint32_t>(framebuffers_.size());
  framebuffers_.push_back(
      Framebuffer(framebuffer, static_cast<GLenum>(color_rt_ids.size())));
  return id;
}

uint32_t ResourceManager::create_framebuffer(
    const std::list<const donkey::render::Texture*>& render_targets) {
  GLuint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  size_t color_rt_id = 0;
  for (auto texture : render_targets) {
    if (texture->format == pixel::Format::kDepthComponent) {
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                             get_texture(texture->gpu_resource_id).texture, 0);
    } else {
      glFramebufferTexture2D(
          GL_FRAMEBUFFER,
          GL_COLOR_ATTACHMENT0 + static_cast<GLenum>(color_rt_id),
          GL_TEXTURE_2D, get_texture(texture->gpu_resource_id).texture, 0);
      color_rt_id += 1;
    }
  }
  check_gl_framebuffer(GL_FRAMEBUFFER);
  uint32_t id = static_cast<uint32_t>(framebuffers_.size());
  framebuffers_.push_back(
      Framebuffer(framebuffer, static_cast<GLenum>(color_rt_id)));
  return id;
}

uint32_t ResourceManager::create_state(const render::State& state) {
  states_.push_back(State(state));
  return static_cast<uint32_t>(states_.size()) - 1;
}

uint32_t ResourceManager::create_buffer() {
  uint32_t buffer;
  uint32_t id = static_cast<uint32_t>(buffers_.size());
  glGenBuffers(1, &buffer);
  buffers_.push_back(buffer);
  return id;
}

const GpuProgram& ResourceManager::get_gpu_program(uint32_t id) const {
  return gpu_programs_[id];
}

const Mesh& ResourceManager::get_mesh(uint32_t id) const {
  return meshes_[id];
}

const Texture& ResourceManager::get_texture(uint32_t id) const {
  return textures_[id];
}

const Framebuffer& ResourceManager::get_framebuffer(uint32_t id) const {
  return framebuffers_[id];
}

const State& ResourceManager::get_state(uint32_t id) const {
  return states_[id];
}

AMaterial& ResourceManager::get_material(std::uint32_t id) {
  return materials_[id];
}

}  // namespace gl
}  // namespace render
}  // namespace donkey
