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

#include "render/RenderPass.hpp"
#include "render/ResourceManager.hpp"
#include "render/Window.hpp"
#include "render/gl/Driver.hpp"

namespace donkey {
namespace render {

class Pipeline {
 public:
  Pipeline(ResourceManager& resource_manager);
  void register_texture(const std::string& name,
                        int width,
                        int height,
                        pixel::Format format,
                        pixel::InternalFormat internal_format,
                        pixel::ComponentType component_type);
  void register_pass(const std::list<std::string>& input_textures,
                     const std::list<std::string>& render_targets,
                     const std::string& vertex_shader_path,
                     const std::string& fragment_shader_path);
  void register_pass(const std::list<std::string>& render_targets);

 private:
  ResourceManager& resource_manager;
};

}  // namespace render
}  // namespace donkey
