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

#include <atomic>
#include <list>
#include <mutex>
#include <vector>

#include "Buffer.hpp"
#include "IPageAllocator.hpp"

namespace donkey {

class BufferPool {
 private:
  static BufferPool* instance_;
  std::vector<std::list<Buffer*>> used_buffers_;
  std::list<Buffer*> unused_buffers_;
  std::mutex mutex_;
  IPageAllocator* page_allocator_;

 public:
  static BufferPool* get_instance();
  static void cleanup();
  Buffer* get_buffer(Buffer::Tag tag, size_t id, size_t size);
  void give_back_buffer(Buffer* buffer);
  void free_tag(Buffer::Tag tag, size_t id);

 private:
  BufferPool();
  ~BufferPool();
};

}  // namespace donkey
