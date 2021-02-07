/* Copyright (C) 2018 Antoine Luciani
 *
 * This file is part of Project Small Fawn.
 *
 * Project Small Fawn is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation, version 3.
 *
 * Project Small Fawn is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Project Small Fawn. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <string>
#include <IResourceLoaderDelegate.hpp>
#include <Scene.hpp>
#include <render/ResourceManager.hpp>

class ResourceLoaderDelegate: public donkey::IResourceLoaderDelegate
{
  public:
    //virtual void load(
    //    donkey::render::Window& window,
    //    donkey::Scene& scene,
    //    donkey::render::DeferredRenderer& renderer);
    virtual void load_game_objects(donkey::Scene& scene);
    virtual void load_render_resources(
        donkey::render::Window* window,
        donkey::render::ResourceManager* resource_manager,
        donkey::render::GpuResourceManager* gpu_resource_manager);

  private:
    //uint32_t load_mesh_(donkey::render::ResourceManager& resource_manager,
    //    const std::string& path) const;
    //void consolidate_indices_(
    //    const tinyobj::attrib_t& attributes,
    //    const std::vector<tinyobj::index_t>& tinyobj_indices,
    //    std::vector<uint32_t>& indices,
    //    std::vector<float>& positions,
    //    std::vector<float>& uvs) const;

  private:
    uint32_t boulder_mesh_id_;
    uint32_t boulder_material_id_;
    int width_;
    int height_;
};
