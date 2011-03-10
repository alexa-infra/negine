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

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
   TypeName(const TypeName&);               \
   void operator=(const TypeName&)
