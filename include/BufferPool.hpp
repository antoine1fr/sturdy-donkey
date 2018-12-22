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
#include "Buffer.hpp"

namespace donkey
{

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

}
