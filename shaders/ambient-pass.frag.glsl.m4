#version 410 core

uniform sampler2D albedo_tex;
uniform vec4 ambient;

out vec4 color;

void main()
{
  color = ambient;
}
