/**
 * \file
 * \brief       defines preprocessor symbols
 *              operating system:   OS_WIN, OS_MAC, OS_LINUX, OS_IPHONE, OS_IPHONE_SIMULATOR, OS_MINGW
 *              unix:               OS_UNIX
 *              compiler:           COMPILER_GCC, COMPILER_MSVC, COMPILER_CLANG
 *              architecture:       OS_ARCH_32, OS_ARCH_64
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#if defined(_WIN32) || defined(__WIN32__)
#   define OS_WIN
#   if defined(__MINGW32__)
#       define OS_MINGW
#   endif
#elif defined(__APPLE__)
#   define OS_MAC
#elif defined(__linux__)
#   define OS_LINUX
#else
#   error System is not supported
#endif

#if defined(OS_MAC)
#   include <TargetConditionals.h>
#   if TARGET_OS_IPHONE
#       define OS_IPHONE
#       if TARGET_IPHONE_SIMULATOR
#           define OS_IPHONE_SIMULATOR
#       endif
#   endif
#endif

#if defined(OS_MAC) || defined(OS_LINUX)
#   define OS_UNIX
#endif

#if defined(__clang__)
#   define COMPILER_CLANG
#elif defined(__GNUC__)
#   define COMPILER_GCC
#elif defined(_MSC_VER)
#   define COMPILER_MSVC
#else
#   error Compiler is not supported
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64) || \
    defined(__alpha__) || defined(__ia64__)
#   define OS_ARCH_64
#else
#   define OS_ARCH_32
#endif
