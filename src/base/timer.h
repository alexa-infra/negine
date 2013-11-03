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

//! Timer class, count time in seconds (with decimal part)
//! TODO: add f64 time return
class NEGINE_API Timer
{
public:
    Timer();

    //! Reset timer to zero, returns current elapsed time in seconds
    f32 reset();

    //! Returns current elapsed time in seconds
    f32 elapsed() const;

private:
	//! Gets current timer value
    u64 getClock() const;
    
    //! Convert internal representation to milliseconds
    f64 convertToMillis( u64 range ) const;

private:
    u64 startTime_;
    u64 frequency_;
};

} // namespace base