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

#include <vector>

namespace donkey {

class Buffer {
 public:
  enum class Tag {
    kFramePacket,
    kLightFramePacket,
    kAlbedoFramePacket,
    kCount
  };

  typedef std::size_t Size;

  Buffer(Tag tag, size_t id, Size capacity, char* ptr);
  Buffer(Buffer&& buffer) noexcept;
  Buffer(const Buffer&) = delete;
  ~Buffer();
  void* allocate(Size size, Size alignment);
  void reset();
  Size size() const;
  Size capacity() const;
  void* ptr() const;
  void set_pointer(void* ptr);
  Tag get_tag() const;
  size_t get_id() const;
  void set_tag_and_id(Tag tag, size_t id);

 private:
  Tag tag_;
  size_t id_;
  Size capacity_;
  Size size_;
  char* ptr_;
};

}  // namespace donkey
