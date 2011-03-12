/**
 * @file    base/types.h
 * @brief   basic types for platform
 **/
#pragma once

#include "types/platform.h"

// Define NULL if it does not defined at platform, note that 
// NULL could be defined differently
#if !defined(NULL)
#   define NULL 0
#endif

#if OS_POSIX
#include <stdint.h>

typedef int8_t      i8;
typedef uint8_t     u8;
typedef int16_t     i16;
typedef uint16_t    u16;
typedef int32_t     i32;
typedef uint32_t    u32;
typedef int64_t     i64;
typedef uint64_t    u64;

#else
#error Include stdint.h
#endif

typedef float       f32;
typedef double      f64;

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
   TypeName(const TypeName&);               \
   void operator=(const TypeName&)
