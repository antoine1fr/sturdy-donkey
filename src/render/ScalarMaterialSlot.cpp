#include <GL/gl3w.h>
#include "render/ScalarMaterialSlot.hpp"

namespace render
{

template <>
void ScalarMaterialSlot<float>::bind(const ResourceManager&)
{
  glUniform1fv(location, 1, &storage);
}

template <>
void ScalarMaterialSlot<glm::vec2>::bind(const ResourceManager&)
{
  glUniform2fv(location, 1, &(storage[0]));
}

template <>
void ScalarMaterialSlot<glm::vec3>::bind(const ResourceManager&)
{
  glUniform3fv(location, 1, &(storage[0]));
}

template <>
void ScalarMaterialSlot<glm::vec4>::bind(const ResourceManager&)
{
  glUniform4fv(location, 1, &(storage[0]));
}

template <>
void ScalarMaterialSlot<glm::mat2>::bind(const ResourceManager&)
{
  glUniformMatrix2fv(location, 1, GL_FALSE, &(storage[0][0]));
}

template <>
void ScalarMaterialSlot<glm::mat3>::bind(const ResourceManager&)
{
  glUniformMatrix3fv(location, 1, GL_FALSE, &(storage[0][0]));
}

template <>
void ScalarMaterialSlot<glm::mat4>::bind(const ResourceManager&)
{
  glUniformMatrix4fv(location, 1, GL_FALSE, &(storage[0][0]));
}

template <>
void ScalarMaterialSlot<int>::bind(const ResourceManager&)
{
  glUniform1iv(location, 1, &storage);
}

template <>
void ScalarMaterialSlot<glm::tvec2<int>>::bind(const ResourceManager&)
{
  glUniform2iv(location, 1, &(storage[0]));
}

template <>
void ScalarMaterialSlot<glm::tvec3<int>>::bind(const ResourceManager&)
{
  glUniform3iv(location, 1, &(storage[0]));
}

template <>
void ScalarMaterialSlot<glm::tvec4<int>>::bind(const ResourceManager&)
{
  glUniform4iv(location, 1, &(storage[0]));
}

template <>
void ScalarMaterialSlot<unsigned int>::bind(const ResourceManager&)
{
  glUniform1uiv(location, 1, &storage);
}

template <>
void ScalarMaterialSlot<glm::tvec2<unsigned int>>::bind(const ResourceManager&)
{
  glUniform2uiv(location, 1, &(storage[0]));
}

template <>
void ScalarMaterialSlot<glm::tvec3<unsigned int>>::bind(const ResourceManager&)
{
  glUniform3uiv(location, 1, &(storage[0]));
}

template <>
void ScalarMaterialSlot<glm::tvec4<unsigned int>>::bind(const ResourceManager&)
{
  glUniform4uiv(location, 1, &(storage[0]));
}

}
