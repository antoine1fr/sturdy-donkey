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

#include "BufferPool.hpp"

namespace donkey
{

template <typename T>
struct StackAllocator
{
  typedef T value_type;
  typedef T* pointer;
  typedef std::size_t size_type;

  StackAllocator()
  {
  }

  ~StackAllocator()
  {
  }

  pointer allocate(size_type size)
  {
    Buffer& buffer = BufferPool::get_push_head();
    return static_cast<pointer>(buffer.allocate(size * sizeof(T)));
  }

  void deallocate(pointer, size_type size)
  {
    // Nothing to do.
  }

  void* get_pointer() const
  {
    Buffer& buffer = BufferPool::get_push_head();
    return buffer.ptr();
  }

  void rollback(pointer pointer)
  {
    Buffer& buffer = BufferPool::get_push_head();
    buffer.set_pointer(pointer);
  }
};

}
