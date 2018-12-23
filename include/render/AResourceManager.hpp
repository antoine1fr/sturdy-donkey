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
      const std::vector<float>& normals,
      const std::vector<float>& uvs,
      const std::vector<float>& tangents,
      const std::vector<float>& bitangents,
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
