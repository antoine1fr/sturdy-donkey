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

#include <sys/mman.h>
#include <mach/vm_statistics.h>
#include <cerrno>
#include <unistd.h>
#include <iostream>

#include "BufferPool.hpp"

namespace donkey
{

BufferPool* BufferPool::instance_ = nullptr;

BufferPool::BufferPool():
  used_buffers_(static_cast<size_t>(Buffer::Tag::kCount))
{
}

BufferPool* BufferPool::get_instance()
{
  if (!instance_)
    instance_ = new BufferPool;
  return instance_;
}

void BufferPool::cleanup()
{
  if (instance_)
    delete instance_;
}

Buffer* BufferPool::get_buffer(Buffer::Tag tag, int id, size_t size)
{
  std::lock_guard<std::mutex> lock(mutex_);
  size_t real_size = size + sizeof(Buffer);

  // Loop through unused buffers by increasing capacity and return the first
  // large enough to fulfil the request.
  for (std::list<Buffer*>::iterator it = unused_buffers_.begin();
      it != unused_buffers_.end();
      it++)
  {
    if ((*it)->capacity() > real_size)
    {
      Buffer* buffer = *it;
      unused_buffers_.erase(it);
      buffer->reset();
      buffer->set_tag_and_id(tag, id);
      return buffer;
    }
  }

  // No unused buffer meeting the requirements. Let's allocate one.
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_ANONYMOUS | MAP_PRIVATE | VM_MAKE_TAG(VM_MEMORY_MALLOC_HUGE);
  size_t block_size = getpagesize();
  size_t block_count = real_size / block_size;
  if (real_size % block_size > 0)
    block_count += 1;
  size_t capacity = block_count * block_size;
  void* ptr = mmap(nullptr, capacity, prot, flags, -1, 0);
  if (ptr == MAP_FAILED)
    perror(nullptr);
  std::cout << "Page size: " << getpagesize() << '\n';
  return new(ptr) Buffer(
    tag,
    id,
    capacity - sizeof(Buffer),
    static_cast<char*>(ptr) + sizeof(Buffer));
}

void BufferPool::give_back_buffer(Buffer* buffer)
{
  std::lock_guard<std::mutex> lock(mutex_);
  size_t t = static_cast<size_t>(buffer->get_tag());
  used_buffers_[t].push_front(buffer);
}

void BufferPool::free_tag(Buffer::Tag tag, int id)
{
  std::lock_guard<std::mutex> lock(mutex_);

  // get every used buffer corresponding to the tuple (tag, id)
  std::list<std::list<Buffer*>::iterator> iterators;
  size_t t = static_cast<size_t>(tag);
  for (std::list<Buffer*>::iterator it = used_buffers_[t].begin();
      it != used_buffers_[t].end();
      it++)
  {
    Buffer* buffer = *it;
    if (buffer->get_id() == id)
      iterators.push_front(it);
  }

  // put back every buffer in unused buffer list
  for (const std::list<Buffer*>::iterator& it: iterators)
  {
    Buffer* buffer = *it;
    used_buffers_[id].erase(it);
    unused_buffers_.push_front(buffer);
  }
}

}
