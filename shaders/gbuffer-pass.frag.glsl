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

#version 410 core

// uniforms that are changed at the pass level
uniform pass
{
  mat4 view;
} pass;

// uniforms that are changed at render object level
uniform object
{
  sampler2D diffuse_texture;
  sampler2D normal_map;
} object;

in vec2 fragment_uv;
in mat3 tbn;

layout (location = 0) out vec4 gbuffer_albedo;
layout (location = 1) out vec3 gbuffer_normal;

void main()
{
  // just copy input color into the albedo render target
  gbuffer_albedo = texture(object.diffuse_texture, fragment_uv);
  // write view-space normal into the normal render target
  gbuffer_normal = texture(object.normal_map, fragment_uv).xyz;
  gbuffer_normal = normalize(gbuffer_normal * 2.0 - 1.0);
  gbuffer_normal = normalize(tbn * gbuffer_normal);
}
