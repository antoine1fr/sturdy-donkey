#pragma once

#include <vector>
#include "StackAllocator.hpp"

template <typename T>
using StackVector = std::vector<T, StackAllocator<T>>;
