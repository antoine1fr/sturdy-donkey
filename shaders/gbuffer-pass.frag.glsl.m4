#version 410 core

uniform sampler2D diffuse_texture;
uniform sampler2D normal_map;
uniform mat4 model;
uniform mat4 view;

in vec2 fragment_uv;
layout (location = 0) out vec4 gbuffer_albedo;
layout (location = 1) out vec4 gbuffer_normal;

void main()
{
  // just copy input color into the albedo render target
  gbuffer_albedo = texture(diffuse_texture, fragment_uv);
  // write view-space normal into the normal render target
  gbuffer_normal = (view * model * texture(normal_map, fragment_uv));
  //albedo = vec4(1.0, 0.0, 1.0, 1.0);
  //normal = vec4(0.0, 1.0, 1.0, 1.0);
}
