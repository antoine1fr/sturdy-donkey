#pragma once

#include <vector>
#include "StackAllocator.hpp"

namespace donkey
{

template <typename T>
using StackVector = std::vector<T, StackAllocator<T>>;

}
