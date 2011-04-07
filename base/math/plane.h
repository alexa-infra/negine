#pragma once

#include "base/math/mathlib.h"
#include "base/math/vector.h"

namespace base {
namespace math {

class Plane {
 private:
    Vector3 normal_;
    f32 distance_;
 public:
    Plane();
    explicit Plane(const Vector3* points) {
        set(points[0], points[1], points[2]);
    }
    Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3) {
        set(p1, p2, p3);
    }
    Plane(const Vector3& normal, const Vector3& planePoint) {
        set(normal, planePoint);
    }
    Plane(const f32& a, const f32& b, const f32& c, const f32& d) {
        set(a, b, c, d);
    }

    void set(const Vector3* points);
    void set(const Vector3& p1, const Vector3& p2, const Vector3& p3);
    void set(const Vector3& normal, const Vector3& planePoint);
    void set(const f32& a, const f32& b, const f32& c, const f32& d);

    f32 A() const { return normal_.x; }
    f32 B() const { return normal_.y; }
    f32 C() const { return normal_.z; }
    f32 D() const { return distance_; }

    const Vector3& Normal() const { return normal_; }

    f32 Distance(const Vector3& point) const;
    Vector3 Projection(const Vector3& point) const;
 private:
    void Normalize();
};
}
}
