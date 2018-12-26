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

in vec3 position;
in vec3 normal;
in vec2 uv;
in vec3 tangent;
in vec3 bitangent;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 fragment_uv;
out mat3 tbn;

void main()
{
  gl_Position = projection * view * model * vec4(position, 1.0);
  fragment_uv = uv;
  mat3 normal_matrix = mat3(transpose(inverse(view * model)));
  vec3 t = normalize(vec3(normal_matrix * tangent));
  vec3 n = normalize(vec3(normal_matrix * normal));
  t = normalize(t - dot(t, n) * n);
  vec3 b = cross(n, t);
  tbn = mat3(t, b, n);
}
