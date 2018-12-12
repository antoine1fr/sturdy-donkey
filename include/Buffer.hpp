#pragma once

#include <vector>

namespace donkey
{

class Buffer
{
  public:
    typedef std::size_t Size;

    Buffer(Size capacity);
    Buffer(Buffer&& buffer) noexcept;
    Buffer(const Buffer&) = delete;
    ~Buffer();
    void* allocate(Size size);
    void reset();
    Size size() const;
    Size capacity() const;
    void* ptr() const;
    void set_pointer(void* ptr);

    template <typename T>
      T* placement_new();

  private:
    Size capacity_;
    Size size_;
    char* ptr_;
};

template <typename T>
T* Buffer::placement_new()
{
  void* ptr = allocate(sizeof(T));
  return new (ptr) T;
}

}
