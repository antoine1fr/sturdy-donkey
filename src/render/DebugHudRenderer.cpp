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

#include <imgui.h>
#include "render/DebugHudRenderer.hpp"

namespace donkey {
namespace render {

DebugHudRenderer::DebugHudRenderer(
    Window* window,
    gl::Driver* driver,
    ResourceManager* resource_manager):
  driver_(driver),
  resource_manager_(resource_manager)
{
  ImGui::CreateContext();

  int width;
  int height;
  uint8_t* pixels;
  ImGuiIO& io = ImGui::GetIO();
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

  uint32_t font_texture_id =
    resource_manager_->load_texture_from_memory(pixels, width, height);
  io.Fonts->TexID = reinterpret_cast<ImTextureID>(font_texture_id);

  (void)driver_;
}

DebugHudRenderer::~DebugHudRenderer()
{
}

void DebugHudRenderer::render(CommandBucket&)
{
}

}
}
