/**
 * \file
 * \brief       timer class
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"

namespace base
{

class Timer
{
private:
    u64 start_time_;
#ifdef OS_WIN
    u64 frequency_;
#endif

public:
    Timer();

    f32 Reset();
    f32 Elapsed();
private:
    u64 GetClock();
    f32 GetMillis( u64 range );
};

} // namespace base