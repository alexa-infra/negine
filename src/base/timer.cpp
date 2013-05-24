/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "base/timer.h"
#include "base/log.h"

#ifdef OS_WIN
# include <windows.h>
#endif
#ifdef OS_UNIX
# include <sys/time.h>
#endif

namespace base
{

Timer::Timer()
{
    start_time_ = GetClock();
#ifdef OS_WIN
    LARGE_INTEGER freq;
    if ( QueryPerformanceFrequency( &freq ) == FALSE )
    {
        ERR("QueryPerformanceFrequency fails");
        abort();
    }
    frequency_ = freq.QuadPart;
#endif
}

f32 Timer::Reset()
{
    u64 new_time = GetClock();
    f32 elapsed = GetMillis( new_time - start_time_ );
    start_time_ = new_time;
    return elapsed;
}

f32 Timer::Elapsed()
{
    return GetMillis( GetClock() - start_time_ );
}

#ifdef OS_WIN

u64 Timer::GetClock()
{
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return time.QuadPart;
}

f32 Timer::GetMillis( u64 range )
{
    return ( f32 )( range / ( frequency_ / 1000.0f ) );
}

#elif defined(OS_UNIX)

u64 Timer::GetClock()
{
    timeval time;
    gettimeofday( &time, NULL );
    return time.tv_sec * 1000000 + time.tv_usec;
}

f32 Timer::GetMillis( u64 range )
{
    return ( f32 )( range / ( f32 )1000 );
}

#endif

} // namespace base
