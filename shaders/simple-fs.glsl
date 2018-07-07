#version 400 core

uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform mat4 model;
uniform mat4 light_model;

in vec2 fragment_uv;
out vec4 color;

struct DirectionalLight
{
  vec3 direction;
  vec3 color;
};

struct Material
{
  vec3 ambient;
};

void main()
{
  Material material = Material(vec3(0.0, 0.0, 0.0));
  DirectionalLight light = DirectionalLight(
    vec3(light_model * vec4(0.0, -1.0, 0.0, 0.0)),
    vec3(1.0, 1.0, 1.0));
  vec3 ambient_light = vec3(0, 0, 0);
  vec3 ambient_term = material.ambient + ambient_light;
  vec3 diffuse_color = texture(diffuse_map, fragment_uv).xyz;
  vec3 normal = (model * texture(normal_map, fragment_uv)).xyz;
  float n_dot_l = dot(normalize(normal), normalize(light.direction));
  vec3 diffuse_term = (light.color + diffuse_color) * n_dot_l;
  color = vec4(ambient_term + diffuse_term, 1.0);
}
