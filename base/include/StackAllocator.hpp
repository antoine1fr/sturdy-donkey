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

#pragma once

#include <vector>

#include "Buffer.hpp"
#include "BufferPool.hpp"

namespace donkey {

template <typename T, std::size_t alignment>
struct StackAllocator {
  template <typename U>
  struct rebind {
    typedef StackAllocator<U, alignment> other;
  };

  typedef T value_type;
  typedef T* pointer;
  typedef std::size_t size_type;

  Buffer::Tag tag;
  size_t id;

  StackAllocator(Buffer::Tag tag, size_t id);

  template <typename U>
  StackAllocator(const StackAllocator<U, alignment>& allocator);

  ~StackAllocator();
  pointer allocate(size_type size);
  void deallocate(pointer, size_type size);
};

}  // namespace donkey

#include "StackAllocator.inl"
