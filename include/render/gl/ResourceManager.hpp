#pragma once

#include <GL/gl3w.h>
#include <SDL.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "common.hpp"
#include "render/AResourceManager.hpp"
#include "render/gl/GpuProgram.hpp"
#include "render/gl/Mesh.hpp"
#include "render/gl/Texture.hpp"

namespace donkey {
namespace render {

class AMaterial;

namespace gl {

class Material;

class ResourceManager: public AResourceManager
{
  private:
    std::vector<GpuProgram> gpu_programs_;
    std::vector<Mesh> meshes_;
    std::vector<Texture> textures_;
    std::vector<GLuint> framebuffers_;
    std::vector<Material> materials_;
    static const std::vector<GLenum> pixel_internal_formats_;
    static const std::vector<GLenum> pixel_formats_;
    static const std::vector<GLenum> pixel_component_types_;

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
    virtual ~ResourceManager();
    virtual void cleanup();
    virtual uint32_t load_texture_from_file(const std::string& path);
    virtual uint32_t load_gpu_program_from_file(const std::string& vs_path,
      const std::string& fs_path);
    virtual uint32_t create_mesh(const std::vector<float>& positions,
      const std::vector<float>& uvs,
      const std::vector<uint32_t>& indices);
    virtual uint32_t create_material(uint32_t gpu_program);
    virtual uint32_t create_texture(
        std::size_t width,
        std::size_t height,
        pixel::Format format,
        pixel::InternalFormat internal_format,
        pixel::ComponentType component_type);
    virtual uint32_t create_framebuffer(
        uint32_t albedo_rt_id,
        uint32_t normal_rt_id,
        uint32_t depth_rt_id);

    virtual AMaterial& get_material(std::uint32_t id);

    const GpuProgram& get_gpu_program(uint32_t id) const;
    const Mesh& get_mesh(uint32_t id) const;
    const Texture& get_texture(uint32_t id) const;
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

}
}
}
