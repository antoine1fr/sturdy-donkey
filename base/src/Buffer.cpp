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

#include <cstdint>
#include <cassert>
#include "Buffer.hpp"

namespace donkey
{

Buffer::Buffer(Tag tag, size_t id, Size capacity, char* ptr):
  tag_(tag),
  id_(id),
  capacity_(capacity),
  size_(0),
  ptr_(ptr)
{
}

Buffer::Buffer(Buffer&& buffer) noexcept:
  tag_(buffer.tag_),
  id_(buffer.id_),
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

void* Buffer::allocate(Size size, std::uintptr_t alignment)
{
  assert(alignment % 2 == 0);
  uintptr_t mask = alignment - 1;
  char* ptr = ptr_ + size_;
  std::ptrdiff_t offset = alignment - (reinterpret_cast<uintptr_t>(ptr) & mask);
  Size new_size = size + size_;
  if (new_size < capacity_)
  {
    size_ = new_size;
    char* adjusted_ptr = ptr + offset;
    *(adjusted_ptr - 1) = static_cast<uint8_t>(offset);
    return static_cast<void*>(adjusted_ptr);
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

void Buffer::set_tag_and_id(Tag tag, size_t id)
{
  tag_ = tag;
  id_ = id;
}

Buffer::Tag Buffer::get_tag() const
{
  return tag_;
}

size_t Buffer::get_id() const
{
  return id_;
}

}
