#pragma once

#include "BufferPool.hpp"

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
