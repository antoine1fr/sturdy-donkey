#version 400 core

uniform sampler2D diffuse_texture;

in vec2 fragment_uv;
out vec4 color;

void main()
{
  color = texture(diffuse_texture, fragment_uv);
}
