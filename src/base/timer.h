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

public:
    Timer();

    f32 Reset();
    f32 Elapsed();
private:
    static u64 GetClock();
    static f32 GetMillis( u64 range );
};

} // namespace base