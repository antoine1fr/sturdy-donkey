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

uniform sampler2D albedo_texture;
uniform sampler2D light_texture;

in vec2 fragment_uv;
out vec4 color;

void main()
{
  vec4 albedo = texture(albedo_texture, fragment_uv);
  vec4 light = texture(light_texture, fragment_uv);
  color = albedo * light;
}
