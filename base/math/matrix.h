/**
 * @file    base/math/matrix.h
 * @brief   Matrix 4x4 class
 *
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) – see LICENSE file for details
 **/
#pragma once

#include "base/math/mathlib.h"
#include "base/math/vector.h"
#include "base/math/plane.h"

namespace base {
namespace math {

class Matrix4 {
 public:
    union {
        f32 array1d[16];
        f32 array2d[4][4];
        struct {
            f32 xx, xy, xz, xw,
                yx, yy, yz, yw,
                zx, zy, zz, zw,
                wx, wy, wz, ww;
        };
        struct {
            f32 x[4];
            f32 y[4];
            f32 z[4];
            f32 w[4];
        };
    };

 public:
    Matrix4();
    explicit Matrix4(const f32* matrix);
    Matrix4(f32 vxx, f32 vxy, f32 vxz, f32 vxw,
            f32 vyx, f32 vyy, f32 vyz, f32 vyw,
            f32 vzx, f32 vzy, f32 vzz, f32 vzw,
            f32 vwx, f32 vwy, f32 vwz, f32 vww);

    void SetIdentity();

    void Scale(const Vector3& scale);
    void Translate(const Vector3& translation);

    void Rotate(const Vector3& axis, const f32& angle);
    void RotateX(const f32& angle);
    void RotateY(const f32& angle);
    void RotateZ(const f32& angle);

    void Project(const Plane& plane);
    void Reflect(const Plane& plane);

    void Transpose();
    void Invert();

 public:
    Matrix4& operator = (const Matrix4& m);
    Matrix4& operator *= (const Matrix4& m);
    Matrix4 operator * (const Matrix4& m) const;
    Matrix4 operator * (const f32& s) const;
    Vector3 operator * (const Vector3& v) const;
    Vector4 operator * (const Vector4& v) const;
    bool operator == (const Matrix4& m);
    bool operator != (const Matrix4& m);

 public:
    static const Matrix4 Identity;
    static const Matrix4 Zero;

    static Matrix4 GetScale(const Vector3& scale);
    static Matrix4 GetTranslation(const Vector3& translation);

    static Matrix4 GetRotation(const Vector3& axis, const f32& angle);
    static Matrix4 GetRotationX(const f32& angle);
    static Matrix4 GetRotationY(const f32& angle);
    static Matrix4 GetRotationZ(const f32& angle);

    static Matrix4 GetProjection(const Plane& plane);
    static Matrix4 GetReflection(const Plane& plane);

    static Matrix4 GetOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 nearDist, f32 farDist);
 private:
    void MatrixSwap(const u8& _a, const u8& _b);
};
}
}
