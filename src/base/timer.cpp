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
    startTime_ = getClock();
#ifdef OS_WIN
    LARGE_INTEGER freq;
    if ( QueryPerformanceFrequency( &freq ) == FALSE )
    {
        ERR("QueryPerformanceFrequency fails");
        abort();
    }
    frequency_ = freq.QuadPart;
#else
    frequency_ = 0;
#endif
}

f32 Timer::reset()
{
    u64 new_time = getClock();
    f32 elapsed = static_cast<f32>( convertToMillis( new_time - startTime_ ) );
    startTime_ = new_time;
    return elapsed;
}

f32 Timer::elapsed() const
{
    return static_cast<f32>( convertToMillis( getClock() - startTime_ ) );
}

#ifdef OS_WIN

u64 Timer::getClock() const
{
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return time.QuadPart;
}

f64 Timer::convertToMillis( u64 range ) const
{
    return range / ( frequency_ / 1000.0 );
}

#elif defined(OS_UNIX)

u64 Timer::getClock() const
{
    timeval time;
    gettimeofday( &time, nullptr );
    return time.tv_sec * 1000000 + time.tv_usec;
}

f64 Timer::convertToMillis( u64 range ) const
{
    return range / 1000.0;
}

#endif

} // namespace base
