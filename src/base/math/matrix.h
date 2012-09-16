/**

 * \brief       Matrix 4x4 class
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/math/mathlib.h"
#include "base/math/vector.h"

namespace base {
namespace math {

//! 4x4 matrix, column-major order
class Matrix4 {
 private:
    Vector4 column0_;
    Vector4 column1_;
    Vector4 column2_;
    Vector4 column3_;

 public:
    Matrix4() {}
    Matrix4(const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3);
    Matrix4(const Matrix4& m);
    Matrix4& operator =(const Matrix4& m);

 public:
    Matrix4& SetCol0(const Vector4& c);
    Matrix4& SetCol1(const Vector4& c);
    Matrix4& SetCol2(const Vector4& c);
    Matrix4& SetCol3(const Vector4& c);
    Matrix4& SetCol(u32 i, const Vector4& c);
    Matrix4& SetRow(u32 i, const Vector4& r);
    Matrix4& SetElem(u32 i, u32 j, f32 a);
    const Vector4 Col0() const;
    const Vector4 Col1() const;
    const Vector4 Col2() const;
    const Vector4 Col3() const;
    const Vector4 Col(u32 i) const;
    const Vector4 Row(u32 i) const;
    f32 Elem(u32 i, u32 j) const;

public:
    const Matrix4 operator +(const Matrix4& m) const;
    const Matrix4 operator -(const Matrix4& m) const;
    const Matrix4 operator *(f32 s) const;
    const Vector4 operator *(const Vector4& v) const;
    const Vector4 operator *(const Vector3& v) const;
    const Matrix4 operator *(const Matrix4& m) const;
    bool operator ==(const Matrix4& m) const;
    bool operator !=(const Matrix4& m) const;
    
 public:
    Matrix4& operator +=(const Matrix4& m);
    Matrix4& operator -=(const Matrix4& m);
    Matrix4& operator *=(f32 s);
    Matrix4& operator *=(const Matrix4& m);
    
 public:
    static const Matrix4 Identity();
    static const Matrix4 RotationX(f32 radians);
    static const Matrix4 RotationY(f32 radians);
    static const Matrix4 RotationZ(f32 radians);
    static const Matrix4 Scale(const Vector3& sc);
    static const Matrix4 Translation(const Vector3& tr);

 public:
    static const Matrix4 LookAt(const Vector3& eyePos, const Vector3& lookAtPos, const Vector3& up);
    static const Matrix4 Perspective(f32 fovyRadians, f32 aspect, f32 zNear, f32 zFar);
    static const Matrix4 Frustum(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);
    static const Matrix4 Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);
    
  private:
    friend std::ostream& operator<< (std::ostream& o, const Matrix4& m);
};

const Matrix4 operator *(f32 s, const Matrix4& m);
const Matrix4 Transpose(const Matrix4& m);
const Matrix4 Inverse(const Matrix4& m);
const Matrix4 AffineInverse(const Matrix4& m);
const Matrix4 OrthoInverse(const Matrix4& m);
f32 Determinant(const Matrix4& m);

}
}

#include "base/math/matrix-inl.h"
