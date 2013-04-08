/**

 * \brief       Matrix 4x4 class
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "math/vec4.h"

namespace base
{
namespace math
{

//! 4x4 matrix, column-major order
class Matrix4
{
private:
    vec4f column0_;
    vec4f column1_;
    vec4f column2_;
    vec4f column3_;

public:
    Matrix4() {}
    Matrix4( const vec4f& col0, const vec4f& col1, const vec4f& col2, const vec4f& col3 );
    Matrix4( const Matrix4& m );
    Matrix4& operator =( const Matrix4& m );

public:
    Matrix4& SetCol0( const vec4f& c );
    Matrix4& SetCol1( const vec4f& c );
    Matrix4& SetCol2( const vec4f& c );
    Matrix4& SetCol3( const vec4f& c );
    Matrix4& SetCol( u32 i, const vec4f& c );
    Matrix4& SetRow( u32 i, const vec4f& r );
    Matrix4& SetElem( u32 i, u32 j, f32 a );
    const vec4f Col0() const;
    const vec4f Col1() const;
    const vec4f Col2() const;
    const vec4f Col3() const;
    const vec4f Col( u32 i ) const;
    const vec4f Row( u32 i ) const;
    f32 Elem( u32 i, u32 j ) const;

public:
    const Matrix4 operator +( const Matrix4& m ) const;
    const Matrix4 operator -( const Matrix4& m ) const;
    const Matrix4 operator *( f32 s ) const;
    const vec4f operator *( const vec4f& v ) const;
    const vec4f operator *( const vec3f& v ) const;
    const Matrix4 operator *( const Matrix4& m ) const;
    bool operator ==( const Matrix4& m ) const;
    bool operator !=( const Matrix4& m ) const;

public:
    Matrix4& operator +=( const Matrix4& m );
    Matrix4& operator -=( const Matrix4& m );
    Matrix4& operator *=( f32 s );
    Matrix4& operator *=( const Matrix4& m );

public:
    static const Matrix4 Identity();
    static const Matrix4 RotationX( f32 radians );
    static const Matrix4 RotationY( f32 radians );
    static const Matrix4 RotationZ( f32 radians );
    static const Matrix4 Scale( const vec3f& sc );
    static const Matrix4 Translation( const vec3f& tr );

public:
    static const Matrix4 LookAt( const vec3f& eyePos, const vec3f& lookAtPos, const vec3f& up );
    static const Matrix4 Perspective( f32 fovyRadians, f32 aspect, f32 zNear, f32 zFar );
    static const Matrix4 Frustum( f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar );
    static const Matrix4 Orthographic( f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar );

private:
    friend std::ostream& operator<< ( std::ostream& o, const Matrix4& m );
};

const Matrix4 operator *( f32 s, const Matrix4& m );
const Matrix4 Transpose( const Matrix4& m );
const Matrix4 Inverse( const Matrix4& m );
const Matrix4 AffineInverse( const Matrix4& m );
const Matrix4 OrthoInverse( const Matrix4& m );
f32 Determinant( const Matrix4& m );

}
}
