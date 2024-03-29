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

#if defined(__APPLE__)
#define STURDY_DONKEY_MACOS
#elif defined(_MSC_VER)
#define STURDY_DONKEY_WINDOWS
#else
#define STURDY_DONKEY_LINUX
#endif

#if defined(STURDY_DONKEY_MACOS) || defined(STURDY_DONKEY_LINUX)
#define STURDY_DONKEY_UNIX
#endif
