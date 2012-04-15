/**
 * \file
 * \brief       Quaternion
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "base/math/matrix.h"
#include "base/math/vector.h"

namespace base {
namespace math {

//! Quaternion
class Quat {
public:
    union {
        struct {
            f32 x, y, z, w;
        };
        f32 arr[4];
    };

public:
    Quat();
    Quat(const f32 *quat);
    Quat(f32 _x, f32 _y, f32 _z, f32 _w);
    Quat(const Vector3& axis, f32 angle);
    Quat(const Matrix4& matrix);
    Quat operator+ (const Quat& q) const;
    Quat operator- (const Quat& q) const;
    Quat operator* (const Quat& q) const;
    Quat operator* (const f32 scalar) const;
    Quat operator/ (const f32 scalar) const;
    Quat operator* (const Vector3& v) const;

    Quat& operator+= (const Quat& q);
    Quat& operator-= (const Quat& q);
    Quat& operator*= (const Quat& q);

    Quat& operator*= (f32 scalar);
    Quat& operator/= (f32 scalar);

    Quat GetInversed() const;
    Quat GetConjugated() const;
    Vector3 RotatePoint(const Vector3& v) const;

    void Set(const f32 *quat);
    void Set(const Vector3& axis, f32 angle);
    void Set(f32 _x, f32 _y, f32 _z, f32 angle);
    void Set(const Matrix4& _matrix);
    Matrix4 GetMatrix() const;

    void Rotate(const Vector3& axis, const f32& angle);
    void RotateX(const f32& angle);
    void RotateY(const f32& angle);
    void RotateZ(const f32& angle);

    f32 GetLength() const;
    f32 GetNorm() const;

    void ComputeW();
public:
    static Quat GetRotation(const Vector3& axis, f32 angle);
    static Quat GetRotationX(f32 angle);
    static Quat GetRotationY(f32 angle);
    static Quat GetRotationZ(f32 angle);
    static Quat GetSlerp(const Quat& q0, const Quat& q1, f32 time);
    static Quat GetZero();
};

} // namespace base
} // namespace math