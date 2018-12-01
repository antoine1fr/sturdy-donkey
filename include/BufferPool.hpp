#pragma once

#include <atomic>
#include "Buffer.hpp"

class BufferPool
{
  private:
    typedef std::vector<Buffer> Vector;
    typedef Buffer::Size Size;

    static Vector instances_;
    static std::atomic_size_t push_head_num_;
    static std::atomic_size_t pop_head_num_;

  public:
    static Buffer& get_push_head();
    static Buffer& get_pop_head();
    static Buffer& get_buffer(Vector::size_type n);
    static size_t get_buffer_count();
    static void add_instance(Size size = 1000);
    static void next_push_head();
    static void next_pop_head();

};
