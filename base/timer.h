/**
 * @file    base/timer.h
 * @brief   timer class
 *
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://hg.alexadotlife.com/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once

#include "base/types.h"

namespace base {

class Timer {
private:
    u64 start_time_;

public:
    Timer();

    f32 Reset();
    f32 Elapsed();
private:
    static u64 GetClock();
    static f32 GetMillis(u64 range);
};

} // namespace base