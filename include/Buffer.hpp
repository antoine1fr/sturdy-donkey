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

namespace donkey
{

class Buffer
{
  public:
    typedef std::size_t Size;

    Buffer(Size capacity);
    Buffer(Buffer&& buffer) noexcept;
    Buffer(const Buffer&) = delete;
    ~Buffer();
    void* allocate(Size size);
    void reset();
    Size size() const;
    Size capacity() const;
    void* ptr() const;
    void set_pointer(void* ptr);

    template <typename T>
      T* placement_new();

  private:
    Size capacity_;
    Size size_;
    char* ptr_;
};

template <typename T>
T* Buffer::placement_new()
{
  void* ptr = allocate(sizeof(T));
  return new (ptr) T;
}

}
