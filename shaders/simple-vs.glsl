#version 400 core

in vec3 position;
in vec2 vertex_uv;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 fragment_uv;

void main()
{
  gl_Position = projection * view * model * vec4(position, 1.0);
  fragment_uv = vertex_uv;
}
