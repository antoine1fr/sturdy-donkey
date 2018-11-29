#pragma once

#include <GL/gl3w.h>
#include <SDL.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include "common.hpp"

class Material;

class ResourceManager
{
  private:
    std::vector<GpuProgram> gpu_programs_;
    std::vector<Mesh> meshes_;
    std::vector<GLuint> textures_;
    std::vector<Material> materials_;
    std::vector<GLuint> framebuffers_;

  private:
    template <GLenum type>
      GLuint load_shader_(const std::string& path);
    GLuint link_gpu_program_(GLuint vertex_shader, GLuint fragment_shader);
    GLuint build_shader_(GLenum type, const std::string& sources);
    GLuint build_vertex_shader_(const std::string& sources);
    GLuint build_fragment_shader_(const std::string& sources);
    GLenum sdl_to_gl_pixel_format_(SDL_PixelFormat* format);
    GLenum sdl_to_gl_pixel_type_(SDL_PixelFormat* format);
    GLuint load_texture_(const std::string& path);

  public:
    ResourceManager();
    ~ResourceManager();
    void cleanup();
    uint32_t load_texture_from_file(const std::string& path);
    uint32_t load_gpu_program_from_file(const std::string& vs_path,
      const std::string& fs_path);
    uint32_t create_mesh(const std::vector<float>& positions,
      const std::vector<float>& uvs,
      const std::vector<uint32_t>& indices);
    uint32_t create_material(uint32_t gpu_program);
    uint32_t register_material(Material&& material);
    uint32_t create_texture(int width, int height, GLenum format,
        GLenum internal_format, GLenum component_type);
    uint32_t create_framebuffer(uint32_t albedo_rt_id, uint32_t normal_rt_id,
        uint32_t depth_rt_id);

    const GpuProgram& get_gpu_program(uint32_t id) const;
    const Mesh& get_mesh(uint32_t id) const;
    GLuint get_texture(uint32_t id) const;
    const Material& get_material(uint32_t id) const;
    GLuint get_framebuffer(uint32_t id) const;
};

template <GLenum type>
GLuint ResourceManager::load_shader_(const std::string& path)
{
  std::cout << "Loading shader: " << path << "\n";
  std::ifstream stream(path);
  stream.seekg(0, std::ios_base::end);
  size_t length = stream.tellg();
  stream.seekg(0, std::ios_base::beg);
  std::string sources(length, '\0');
  stream.read(&sources[0], length);
  return build_shader_(type, sources);
}
