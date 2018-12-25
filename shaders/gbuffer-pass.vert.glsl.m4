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
