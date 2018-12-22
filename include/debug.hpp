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

#include "build.hpp"

#if defined(STURDY_DONKEY_MACOS)

# include <sys/kdebug_signpost.h>

inline void signpost_start(int a, int b, int c, int d, int e)
{
	kdebug_signpost_start(a, b, c, d, e);
}

inline void signpost_end(int a, int b, int c, int d, int e)
{
	kdebug_signpost_end(a, b, c, d, e);
}

#else

inline void signpost_start(int, int, int, int, int)
{
}

inline void signpost_end(int, int, int, int, int)
{
}

#endif // defined(STURDY_DONKEY_MACOS)

