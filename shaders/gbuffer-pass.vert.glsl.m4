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
  mat4 view_model = view * model;
  gl_Position = projection * view_model * vec4(position, 1.0);
  fragment_uv = uv;
  vec3 t = normalize(vec3(view_model * vec4(tangent, 0.0)));
  vec3 b = normalize(vec3(view_model * vec4(bitangent, 0.0)));
  vec3 n = normalize(vec3(view_model * vec4(normal, 0.0)));
  tbn = mat3(t, b, n);
}
