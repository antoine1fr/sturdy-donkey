#pragma once

#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace std
{
  template <>
    struct hash<glm::vec2>
    {
      typedef glm::vec2 argument_type;
      typedef std::size_t result_type;

      std::size_t operator () (const glm::vec2& v) const noexcept
      {
        std::size_t seed = 0;
        hash_combine(seed, v.x);
        hash_combine(seed, v.y);
        return seed;
      }
    };

  template <>
    struct hash<glm::vec3>
    {
      typedef glm::vec3 argument_type;
      typedef std::size_t result_type;

      std::size_t operator () (const glm::vec3& v) const noexcept
      {
        std::size_t seed = 0;
        hash_combine(seed, v.x);
        hash_combine(seed, v.y);
        hash_combine(seed, v.z);
        return seed;
      }
    };

  template <typename T, typename U>
    struct hash<std::pair<T, U>>
    {
      typedef std::pair<T, U> argument_type;
      typedef std::size_t result_type;

      std::size_t operator () (const std::pair<T, U>& pair) const noexcept
      {
        std::size_t seed = 0;
        hash_combine<T>(seed, pair.first);
        hash_combine<U>(seed, pair.second);
        return seed;
      }
    };
}
