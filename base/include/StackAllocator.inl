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

namespace donkey {

extern thread_local std::vector<Buffer*> buffers_;

template <typename T, size_t alignment>
StackAllocator<T, alignment>::StackAllocator(Buffer::Tag tag, size_t id):
  tag(tag),
  id(id)
{
}

template <typename T, size_t alignment>
template <typename U>
StackAllocator<T, alignment>::StackAllocator(
    const StackAllocator<U, alignment>& allocator):
  tag(allocator.tag),
  id(allocator.id)
{
}

template <typename T, size_t alignment>
StackAllocator<T, alignment>::~StackAllocator()
{
}

template <typename T, size_t alignment>
typename StackAllocator<T, alignment>::pointer
  StackAllocator<T, alignment>::allocate(size_type n)
{
  size_t size = n * sizeof(T);
  size_t real_size = size + alignment;
  size_t buffer_id = static_cast<size_t>(tag);
  Buffer* buffer = buffers_[buffer_id];
  BufferPool* buffer_pool = BufferPool::get_instance();
  void* ptr;

  if (!buffer)
    buffer = buffer_pool->get_buffer(tag, id, real_size);
  ptr = buffer->allocate(real_size, alignment);
  if (!ptr)
  {
    buffer_pool->give_back_buffer(buffer);
    buffer = buffer_pool->get_buffer(tag, id, real_size);
    ptr = buffer->allocate(real_size, alignment);
    assert(ptr != nullptr);
  }
  buffers_[buffer_id] = buffer;
  return static_cast<pointer>(ptr);
}

template <typename T, size_t alignment>
void StackAllocator<T, alignment>::deallocate(pointer, size_type size)
{
  // Nothing to do.
}

}
