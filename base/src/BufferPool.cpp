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

#include "build.hpp"

#include "BufferPool.hpp"
#if defined(STURDY_DONKEY_UNIX)
# include "UnixPageAllocator.hpp"
#elif defined(STURDY_DONKEY_WINDOWS)
# include "WindowsPageAllocator.hpp"
#endif

namespace donkey
{

BufferPool* BufferPool::instance_ = nullptr;

BufferPool::BufferPool():
  used_buffers_(static_cast<size_t>(Buffer::Tag::kCount)),
#if defined(STURDY_DONKEY_UNIX)
  page_allocator_(new UnixPageAllocator())
#elif defined(STURDY_DONKEY_WINDOWS)
  page_allocator_(new WindowsPageAllocator())
#endif
{
}

BufferPool::~BufferPool()
{
  delete page_allocator_;
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
  // large enough to fulfill the request.
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
  size_t block_size = page_allocator_->get_page_size();
  size_t block_count = real_size / block_size;
  if (real_size % block_size > 0)
    block_count += 1;
  size_t capacity = block_count * block_size;
  void* ptr = page_allocator_->allocate(capacity);
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
