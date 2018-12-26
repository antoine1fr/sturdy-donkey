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
