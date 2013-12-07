/**
 * \file
 * \brief       basic types for platform
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/platform.h"

#if defined(OS_UNIX) || defined(OS_MINGW)
#   include <stdint.h>
#elif defined(OS_WIN)
#   include <winsdkver.h>
#   if WINVER_MAXVER >= 0x0602
#       include <stdint.h>
#   else
#       include "stdint/stdint.h"
#   endif
#else
#   error Include stdint.h
#endif

namespace base {

typedef int8_t      i8;
typedef uint8_t     u8;
typedef int16_t     i16;
typedef uint16_t    u16;
typedef int32_t     i32;
typedef uint32_t    u32;
typedef int64_t     i64;
typedef uint64_t    u64;

typedef float       f32;
typedef double      f64;

#if defined(OS_ARCH_32)
typedef u32 uptr;
typedef i32 iptr;
#elif defined(OS_ARCH_64)
typedef u64 uptr;
typedef i64 iptr;
#endif

} // namespace base

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
   TypeName(const TypeName&);               \
   void operator=(const TypeName&)

#define DISALLOW_HEAP_ALLOC() \
    void *operator new(size_t size);  \
    void *operator new[](size_t size)

#if defined OS_WIN
#   define NEGINE_EXPORT __declspec(dllexport) // gcc supports it too
#   define NEGINE_IMPORT __declspec(dllimport)
#   define NEGINE_STATIC
#elif defined(COMPILER_GCC)
#   define NEGINE_EXPORT __attribute__((visibility("default")))
#   define NEGINE_IMPORT NEGINE_EXPORT
#   define NEGINE_STATIC NEGINE_EXPORT
#endif

#if defined NEGINE_SHARED_BUILD
#   if defined NEGINE_STATIC_BUILD
#       undef NEGINE_STATIC_BUILD
#   endif
#elif !defined NEGINE_STATIC_BUILD
#   define NEGINE_SHARED_BUILD
#endif

#ifndef NEGINE_API

#if defined NEGINE_SHARED_BUILD
#   if defined NEGINE_SOURCE
#       define NEGINE_API NEGINE_EXPORT
#   else
#       define NEGINE_API NEGINE_IMPORT
#   endif
#elif defined NEGINE_STATIC_BUILD
#   define NEGINE_API NEGINE_STATIC
#endif

#endif // NEGINE_API
