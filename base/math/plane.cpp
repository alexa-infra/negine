/**
 * @file    base/math/plane.cpp
 * @brief   Implementation of plane class
 *
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://hg.alexadotlife.com/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#include "base/math/plane.h"

namespace base {
namespace math {

void Plane::set(const Vector3* points) {
    set(points[0], points[1], points[2]);
}

void Plane::set(const Vector3& p1, const Vector3& p2, const Vector3& p3) {
    normal_ = (p2 - p1) ^ (p3 - p1);
    distance_ = -normal_.Dot(p1);
    Normalize();
}

void Plane::set(const Vector3& normal, const Vector3& planePoint) {
    normal_ = normal;
    distance_ = -normal.Dot(planePoint);
    Normalize();
}

void Plane::set(const f32& a, const f32& b, const f32& c, const f32& d) {
    normal_.x = a;
    normal_.y = b;
    normal_.z = c;
    distance_ = d;
    Normalize();
}

Vector3 Plane::Projection(const Vector3& point) const {
    return (point - normal_*Distance(point));
}

f32 Plane::Distance(const Vector3& point) const {
    return (normal_.Dot(point) + distance_);
}

void Plane::Normalize() {
    f32 len = normal_.Length();
    if (len < eps)
        return;
    normal_.x /= len;
    normal_.y /= len;
    normal_.z /= len;
    distance_ /= len;
}
}
}
