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

uniform sampler2D normals_texture; // normals in gbuffer_view space
uniform sampler2D depth_texture;
uniform vec3 camera_position; // Eye's position in view space.
uniform vec4 light_dir; // Light's direction in gbuffer_view space in xyz.
                        // Material's shininess in w.
uniform vec4 light_diffuse; // Light's diffuse color.
uniform vec4 light_specular; // Light's specular color.
uniform vec2 gbuffer_projection_params; // x: near plane, y: far plane
uniform mat4 gbuffer_projection_inverse;
uniform mat4 gbuffer_view;

in vec2 fragment_uv;
out vec4 color;

struct Light
{
  vec4 diffuse;
  vec4 specular;
  vec3 direction;
};

struct Material
{
  float shininess;
};

struct Fragment
{
  vec3 position;
  vec3 normal;
};

vec4 compute_specular_term(Fragment fragment, Light light, Material material,
    vec3 camera_position)
{
  vec3 camera_direction = normalize(-fragment.position);
  vec3 highlight = reflect(light.direction, fragment.normal);
  float specular =  pow(max(dot(camera_direction, highlight), 0.0),
      material.shininess);
  return specular * light.specular;
}

vec4 compute_diffuse_term(Fragment fragment, Light light, Material material)
{
  vec3 light_dir = normalize(-light.direction);
  float intensity = max(dot(fragment.normal, light_dir), 0);
  return intensity * light.diffuse;
}

vec3 unpack_position()
{
  float depth = texture(depth_texture, fragment_uv).x;
  vec4 clip_space_position = vec4(fragment_uv * 2 - 1, depth, 1);
  vec4 view_space_position = gbuffer_projection_inverse * clip_space_position;
  vec3 position = view_space_position.xyz / view_space_position.w;
  return position;
}

void main()
{
  vec3 normal = texture(normals_texture, fragment_uv).xyz;
  Material material = Material(light_dir.w);
  Light light = Light(light_diffuse, light_specular, light_dir.xyz);
  Fragment fragment = Fragment(unpack_position(), normal);
  vec4 diffuse_term = compute_diffuse_term(fragment, light, material);
  vec4 specular_term = compute_specular_term(fragment, light, material,
      camera_position);
  color = diffuse_term + specular_term;
}
