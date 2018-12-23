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

#include <string>
#include <vector>
#include <tiny_obj_loader.h>

#include "render/ResourceManager.hpp"

namespace donkey {

class MeshLoader
{
  public:
    uint32_t load(
        render::ResourceManager& resource_manager,
        const std::string& path) const;

  private:
    void consolidate_indices_(
        const tinyobj::attrib_t& attributes,
        const std::vector<tinyobj::index_t>& tinyobj_indices,
        std::vector<uint32_t>& indices,
        std::vector<float>& positions,
        std::vector<float>& normals,
        std::vector<float>& uvs) const;
};

}
