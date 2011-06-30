#include "base/timer.h"

#ifdef OS_WIN
#include <windows.h>
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

#else

#error IMPLEMENT TIMER FOR UNIX

//timeval time;
//gettimeofday(&time, NULL);
//return time.tv_sec * 1000000 + time.tv_usec;

#endif

} // namespace base