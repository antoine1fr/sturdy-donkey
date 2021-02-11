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

#include "UnixPageAllocator.hpp"

#include <sys/mman.h>
#if defined(STURDY_DONKEY_MACOS)
#include <mach/vm_statistics.h>
#endif
#include <unistd.h>

#include <cerrno>

namespace donkey {

void* UnixPageAllocator::allocate(size_t size) {
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_ANONYMOUS | MAP_PRIVATE;
#if defined(STURDY_DONKEY_MACOS)
  flags |= VM_MAKE_TAG(VM_MEMORY_MALLOC_HUGE);
#endif
  void* ptr = mmap(nullptr, size, prot, flags, -1, 0);
  if (ptr == MAP_FAILED)
    perror(nullptr);
  return ptr;
}

size_t WindowsPageAllocator::get_page_size() const {
  return getpagesize();
}

}  // namespace donkey