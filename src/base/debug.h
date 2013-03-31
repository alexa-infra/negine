#pragma once

#include "platform.h"

#if defined(COMPILER_MSVC)
#   define debugBreak() __debugbreak()
#elif defined(COMPILER_GCC)
#   define debugBreak() __builtin_trap()
#endif

#define debugBreakOnce() \
    static bool __firstTime ## __LINE__ = true; \
    if (__firstTime) { \
        __firstTime = false; \
        debugBreak(); \
    }

#define assertMacro(exp) \
    if (!(exp)) { \
        debugBreak(); \
    }

#ifdef _DEBUG
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