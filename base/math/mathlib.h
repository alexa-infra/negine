/**
 * @file    base/math/mathlib.h
 * @brief   Common math definitions
 *
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) – see LICENSE file for details
 **/
#pragma once

#include "base/types.h"
#include <math.h>
#include <algorithm>

namespace base {
namespace math {

const f32 eps = 0.000001f;
const f32 pi = 3.1415f;

inline bool Equal(const f32& x, const f32& y) { return fabs(x - y) < eps; }
inline void Swap(f32& x, f32& y) {
	std::swap(x, y);
}

}
}
