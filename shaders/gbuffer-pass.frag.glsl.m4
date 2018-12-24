#version 410 core

uniform sampler2D diffuse_texture;
uniform sampler2D normal_map;
uniform mat4 model;
uniform mat4 view;

in vec2 fragment_uv;
in mat3 tbn;

layout (location = 0) out vec4 gbuffer_albedo;
layout (location = 1) out vec3 gbuffer_normal;

void main()
{
  // just copy input color into the albedo render target
  gbuffer_albedo = texture(diffuse_texture, fragment_uv);
  // write view-space normal into the normal render target
  gbuffer_normal = texture(normal_map, fragment_uv).xyz;
  gbuffer_normal = normalize(gbuffer_normal * 2.0 - 1.0);
  gbuffer_normal = normalize(tbn * gbuffer_normal);
}
