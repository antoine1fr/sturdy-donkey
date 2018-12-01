#include "BufferPool.hpp"

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
