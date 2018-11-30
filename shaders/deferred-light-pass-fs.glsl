#version 400 core

uniform sampler2D albedo_tex;
uniform sampler2D normals_tex; // normals in view space
uniform sampler2DShadow depth_tex;
uniform vec3 camera_direction; // Eye's position in view space.
uniform vec4 ambient;
uniform vec4 light_dir; // Light's direction in view space in xyz. Material's
                        // shininess in w.
uniform vec4 light_diffuse; // Light's diffuse color.
uniform vec4 light_specular; // Light's specular color.
uniform vec2 projection_params; // x: near plane, y: far plane
uniform mat4 projection_inverse;
uniform mat4 view;

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
    vec3 camera_direction)
{
  vec3 highlight = normalize(camera_direction + fragment.position.xyz);
  float n_dot_h = pow(dot(fragment.normal, highlight), material.shininess);
  return n_dot_h * light.specular;
}

vec4 compute_diffuse_term(Fragment fragment, Light light, Material material)
{
  float n_dot_l = dot(normalize(fragment.normal), normalize(light.direction));
  float intensity = max(0, n_dot_l);
  vec4 albedo = texture(albedo_tex, fragment_uv);
  return intensity * (albedo + light.diffuse);
}

vec3 unpack_position()
{
  float linear_depth = texture(depth_tex, vec3(fragment_uv, 1));
  float depth = (projection_params.x - projection_params.y) * linear_depth;
  vec4 position = projection_inverse * vec4(fragment_uv, depth, 0);
  return position.xyz;
}

void main()
{
  vec3 normal = texture(normals_tex, fragment_uv).xyz;
  Material material = Material(light_dir.w);
  vec3 light_dir = (view * vec4(light_dir.xyz, 0)).xyz;
  Light light = Light(light_diffuse, light_specular, light_dir);
  Fragment fragment = Fragment(unpack_position(), normal);
  vec4 diffuse_term = compute_diffuse_term(fragment, light, material);
  vec4 specular_term = compute_specular_term(fragment, light, material,
      camera_direction);
  color = ambient + diffuse_term + specular_term;
}
