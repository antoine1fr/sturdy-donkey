/* Copyright (C) 2018 Antoine Luciani
 *
 * This file is part of Sturdy Donkey.
 *
 * Sturdy Donkey is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, version 3.
 *
 * Sturdy Donkey is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Sturdy Donkey. If not, see <https://www.gnu.org/licenses/>.
 */

// The function hash_combine comes from the Boost library and is subject to the
// following:

// Copyright 2005-2014 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace donkey {

template <typename T>
void hash_combine(std::size_t& seed, const T& x);

}

namespace std {
template <>
struct hash<glm::vec2> {
  typedef glm::vec2 argument_type;
  typedef std::size_t result_type;

  std::size_t operator()(const glm::vec2& v) const noexcept {
    std::size_t seed = 0;
    donkey::hash_combine(seed, v.x);
    donkey::hash_combine(seed, v.y);
    return seed;
  }
};

template <>
struct hash<glm::vec3> {
  typedef glm::vec3 argument_type;
  typedef std::size_t result_type;

  std::size_t operator()(const glm::vec3& v) const noexcept {
    std::size_t seed = 0;
    donkey::hash_combine(seed, v.x);
    donkey::hash_combine(seed, v.y);
    donkey::hash_combine(seed, v.z);
    return seed;
  }
};

template <typename T, typename U>
struct hash<std::pair<T, U>> {
  typedef std::pair<T, U> argument_type;
  typedef std::size_t result_type;

  std::size_t operator()(const std::pair<T, U>& pair) const noexcept {
    std::size_t seed = 0;
    donkey::hash_combine<T>(seed, pair.first);
    donkey::hash_combine<U>(seed, pair.second);
    return seed;
  }
};
}  // namespace std

namespace donkey {

template <typename T>
void hash_combine(std::size_t& seed, const T& x) {
  std::size_t hash = std::hash<T>{}(x);
  seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

}  // namespace donkey
