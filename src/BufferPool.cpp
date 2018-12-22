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

#include "BufferPool.hpp"

namespace donkey
{

std::atomic_size_t BufferPool::push_head_num_(0);
std::atomic_size_t BufferPool::pop_head_num_(0);
BufferPool::Vector BufferPool::instances_;

Buffer& BufferPool::get_push_head()
{
  return instances_[push_head_num_ % instances_.size()];
}

Buffer& BufferPool::get_buffer(Vector::size_type n)
{
  return instances_[n % instances_.size()];
}

Buffer& BufferPool::get_pop_head()
{
  return instances_[pop_head_num_ % instances_.size()];
}

std::size_t BufferPool::get_buffer_count()
{
  return push_head_num_ - pop_head_num_;
}

void BufferPool::add_instance(Size size)
{
  instances_.push_back(Buffer(size));
}

void BufferPool::next_push_head()
{
  push_head_num_++;
}

void BufferPool::next_pop_head()
{
  pop_head_num_++;
}

}
