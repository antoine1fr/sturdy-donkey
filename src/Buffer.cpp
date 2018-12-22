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

#include "Buffer.hpp"

namespace donkey
{

Buffer::Buffer(Size capacity):
  capacity_(capacity),
  size_(0),
  ptr_(nullptr)
{
}

Buffer::Buffer(Buffer&& buffer) noexcept:
  capacity_(buffer.capacity_),
  size_(buffer.size_),
  ptr_(buffer.ptr_)
{
  buffer.ptr_ = nullptr;
  buffer.size_ = 0;
}

Buffer::~Buffer()
{
  if (ptr_)
    delete [] ptr_;
}

void* Buffer::allocate(Size size)
{
  if (!ptr_)
    ptr_ = new char[capacity_];
  Size new_size = size + size_;
  if (new_size < capacity_)
  {
    void* ptr = static_cast<void*>(ptr_ + size_);
    size_ = new_size;
    return ptr;
  }
  return nullptr;
}

void Buffer::reset()
{
  size_ = 0;
}

Buffer::Size Buffer::size() const
{
  return size_;
}

Buffer::Size Buffer::capacity() const
{
  return capacity_;
}

void* Buffer::ptr() const
{
  return ptr_;
}

void Buffer::set_pointer(void* ptr)
{
  ptr_ = static_cast<char*>(ptr);
}

}
