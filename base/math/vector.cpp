/**
 * @file    base/math/vector.cpp
 * @brief   Implementation of vector classes
 *
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://hg.alexadotlife.com/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#include "base/math/vector.h"

namespace base {
namespace math {

Vector2::Vector2(const Vector3& v) {
    set(v.x, v.y);
}

Vector2::Vector2(const Vector4& v) {
    set(v.x, v.y);
}

Vector3::Vector3(const Vector4& v) {
    set(v.x, v.y, v.z);
}

}
}
