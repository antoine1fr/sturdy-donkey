#pragma once

#include <vector>
#include <string>

#include "render/AMaterial.hpp"
#include "render/Mesh.hpp"
#include "render/GpuProgram.hpp"
#include "render/pixel.hpp"

namespace donkey {
namespace render {

class AResourceManager
{
  public:
    virtual void cleanup() = 0;

    virtual uint32_t load_texture_from_file(const std::string& path) = 0;

    virtual uint32_t load_gpu_program_from_file(
      const std::string& vs_path,
      const std::string& fs_path) = 0;

    virtual uint32_t create_mesh(
      const std::vector<float>& positions,
      const std::vector<float>& uvs,
      const std::vector<uint32_t>& indices) = 0;

    virtual uint32_t create_material(uint32_t gpu_program) = 0;

    virtual uint32_t create_texture(
        std::size_t width,
        std::size_t height,
        pixel::Format format,
        pixel::InternalFormat internal_format,
        pixel::ComponentType component_type) = 0;

    virtual uint32_t create_framebuffer(
        uint32_t albedo_rt_id,
        uint32_t normal_rt_id,
        uint32_t depth_rt_id) = 0;

    virtual AMaterial& get_material(std::uint32_t id) = 0;
};

}
}
