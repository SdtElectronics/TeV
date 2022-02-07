// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2022 SdtElectronics
// All rights reserved. Use of this source code is governed by
// a BSD-style license that can be found in the LICENSE file.

#ifndef tevdefs
#define tevdefs

#ifdef TEVHDRONLY
#define TEVINLINE inline
#else
#define TEVINLINE 
#endif

// move forward reference in lambda capture if C++14 or above is supported
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201402L) || __cplusplus >= 201402L)
#define TEVCAPTMOVE(v) v=std::move(v)
#else
#define TEVCAPTMOVE(v) v
#endif

#define TEVISMOVABLE(T) \
static_assert(std::is_nothrow_move_constructible<T>::value, ""); \
static_assert(std::is_nothrow_move_assignable<T>::value, "");

#define TEVISCLONEABLE(T) \
static_assert(std::is_copy_constructible<T>::value, ""); \
static_assert(std::is_copy_assignable<T>::value, "");

#endif