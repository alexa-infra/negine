#pragma once

#include "platform.h"

#if defined COMPILER_MSVC
#   define debugBreak() __debugbreak()
#elif defined(COMPILER_GCC) || defined(COMPILER_CLANG)
#   define debugBreak() __builtin_trap()
#endif

#define assertMacro(exp) \
    if (!(exp)) { \
        debugBreak(); \
    }

#if defined _DEBUG
    #define ASSERT(x) assertMacro(x)
    #define ENSURE(x) assertMacro(x)
#else
    #define ASSERT(x) {}
    #define ENSURE(x) (void)(x);
#endif

#ifdef OS_WIN
    #define UNREACHABLE() __assume(0)
#else
    #define UNREACHABLE() __builtin_unreachable()
#endif
