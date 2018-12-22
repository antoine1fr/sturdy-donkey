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

#include "render/gl/Mesh.hpp"

namespace donkey
{
namespace render
{
namespace gl
{

Mesh::Mesh(
      GLuint position_buffer,
      GLuint uv_buffer,
      GLuint index_buffer,
      GLuint vertex_array):
  position_buffer(position_buffer),
  uv_buffer(uv_buffer),
  index_buffer(index_buffer),
  vertex_array(vertex_array)
{
}

}
}
}
