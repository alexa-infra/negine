/**
 * @file    base/timer.cpp
 * @brief   implementation of timer class
 *
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://hg.alexadotlife.com/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#include "base/timer.h"

#ifdef OS_WIN
# include <windows.h>
#endif
#ifdef OS_POSIX
# include <sys/time.h>
#endif

namespace base {

Timer::Timer()
{
    start_time_ = GetClock();
}

f32 Timer::Reset()
{
    u64 new_time = GetClock();
    f32 elapsed = GetMillis(new_time - start_time_);
    start_time_ = new_time;
    return elapsed;
}

f32 Timer::Elapsed()
{
    return GetMillis(GetClock() - start_time_);
}

#ifdef OS_WIN

u64 Timer::GetClock()
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    u64 res = 0;
    res |= ft.dwHighDateTime;
    res <<= 32;
    res |= ft.dwLowDateTime;
    res /= 10;
    
    return res;
}

f32 Timer::GetMillis(u64 range)
{
    return (f32)(range / (f32)1000);
}

#elif defined(OS_POSIX)

u64 Timer::GetClock()
{
    timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000000 + time.tv_usec;
}

f32 Timer::GetMillis(u64 range)
{
    return (f32)(range / (f32)1000);
}

#endif

} // namespace base