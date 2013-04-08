#pragma once

#include "math/matrix.h"
#include "math/vec3.h"
#include <math.h>

namespace base
{
namespace math
{

inline Matrix4::Matrix4( const vec4f& col0, const vec4f& col1, const vec4f& col2, const vec4f& col3 )
    : column0_( col0 )
    , column1_( col1 )
    , column2_( col2 )
    , column3_( col3 )
{
}

inline Matrix4::Matrix4( const Matrix4& m )
    : column0_( m.column0_ )
    , column1_( m.column1_ )
    , column2_( m.column2_ )
    , column3_( m.column3_ )
{
}

inline Matrix4& Matrix4::operator =( const Matrix4& m )
{
    column0_ = m.column0_;
    column1_ = m.column1_;
    column2_ = m.column2_;
    column3_ = m.column3_;
    return *this;
}

inline Matrix4& Matrix4::SetCol0( const vec4f& c )
{
    column0_ = c;
    return *this;
}

inline Matrix4& Matrix4::SetCol1( const vec4f& c )
{
    column1_ = c;
    return *this;
}

inline Matrix4& Matrix4::SetCol2( const vec4f& c )
{
    column2_ = c;
    return *this;
}

inline Matrix4& Matrix4::SetCol3( const vec4f& c )
{
    column3_ = c;
    return *this;
}

inline Matrix4& Matrix4::SetCol( u32 i, const vec4f& c )
{
    *( &column0_ + i ) = c;
    return *this;
}

inline Matrix4& Matrix4::SetRow( u32 i, const vec4f& r )
{
    column0_[i] = r[0];
    column1_[i] = r[1];
    column2_[i] = r[2];
    column3_[i] = r[3];
    return *this;
}

inline Matrix4& Matrix4::SetElem( u32 i, u32 j, f32 a )
{
    vec4f c = Col( i );
    c[j] = a;
    SetCol( i, c );
    return *this;
}

inline const vec4f Matrix4::Col0() const
{
    return column0_;
}

inline const vec4f Matrix4::Col1() const
{
    return column1_;
}

inline const vec4f Matrix4::Col2() const
{
    return column2_;
}

inline const vec4f Matrix4::Col3() const
{
    return column3_;
}

inline const vec4f Matrix4::Col( u32 i ) const
{
    return *( &column0_ + i );
}

inline const vec4f Matrix4::Row( u32 i ) const
{
    return vec4f(
               column0_[i],
               column1_[i],
               column2_[i],
               column3_[i]
           );
}

inline f32 Matrix4::Elem( u32 i, u32 j ) const
{
    const vec4f& c = *( &column0_ + i );
    return c[j];
}

inline const Matrix4 Matrix4::operator +( const Matrix4& m ) const
{
    return Matrix4(
               column0_ + m.column0_,
               column1_ + m.column1_,
               column2_ + m.column2_,
               column3_ + m.column3_
           );
}

inline const Matrix4 Matrix4::operator -( const Matrix4& m ) const
{
    return Matrix4(
               column0_ - m.column0_,
               column1_ - m.column1_,
               column2_ - m.column2_,
               column3_ - m.column3_
           );
}

inline const Matrix4 Matrix4::operator *( f32 s ) const
{
    return Matrix4(
               column0_ * s,
               column1_ * s,
               column2_ * s,
               column3_ * s
           );
}

inline const vec4f Matrix4::operator *( const vec4f& v ) const
{
    return vec4f(
               dot( Row( 0 ), v ),
               dot( Row( 1 ), v ),
               dot( Row( 2 ), v ),
               dot( Row( 3 ), v )
           );
}

inline const vec4f Matrix4::operator *( const vec3f& v ) const
{
    vec4f t = vec4f( v, 0.0f );
    return vec4f(
               dot( Row( 0 ), t ),
               dot( Row( 1 ), t ),
               dot( Row( 2 ), t ),
               dot( Row( 3 ), t )
           );
}

inline const Matrix4 Matrix4::operator *( const Matrix4& m ) const
{
    return Matrix4(
               *this * m.column0_,
               *this * m.column1_,
               *this * m.column2_,
               *this * m.column3_
           );
}

inline bool Matrix4::operator ==( const Matrix4& m ) const
{
    return column0_ == m.column0_ && column1_ == m.column1_ && column2_ == m.column2_ && column3_ == m.column3_;
}

inline bool Matrix4::operator !=( const Matrix4& m ) const
{
    return column0_ != m.column0_ || column1_ != m.column1_ || column2_ != m.column2_ || column3_ != m.column3_;
}

inline Matrix4& Matrix4::operator +=( const Matrix4& m )
{
    *this = *this + m;
    return *this;
}

inline Matrix4& Matrix4::operator -=( const Matrix4& m )
{
    *this = *this - m;
    return *this;
}

inline Matrix4& Matrix4::operator *=( f32 s )
{
    *this = *this * s;
    return *this;
}

inline Matrix4& Matrix4::operator *=( const Matrix4& m )
{
    *this = *this * m;
    return *this;
}

inline const Matrix4 Matrix4::Identity()
{
    return Matrix4(
               vec4f( 1.0f, 0.0f, 0.0f, 0.0f ),
               vec4f( 0.0f, 1.0f, 0.0f, 0.0f ),
               vec4f( 0.0f, 0.0f, 1.0f, 0.0f ),
               vec4f( 0.0f, 0.0f, 0.0f, 1.0f )
           );
}

inline const Matrix4 Matrix4::RotationX( f32 radians )
{
    f32 c = cosf( radians );
    f32 s = sinf( radians );
    return Matrix4(
               vec4f( 1.0f, 0.0f, 0.0f, 0.0f ),
               vec4f( 0.0f,    c,    s, 0.0f ),
               vec4f( 0.0f,   -s,    c, 0.0f ),
               vec4f( 0.0f, 0.0f, 0.0f, 1.0f )
           );
}

inline const Matrix4 Matrix4::RotationY( f32 radians )
{
    f32 c = cosf( radians );
    f32 s = sinf( radians );
    return Matrix4(
               vec4f(   c, 0.0f,   -s, 0.0f ),
               vec4f( 0.0f, 1.0f, 0.0f, 0.0f ),
               vec4f(   s, 0.0f,    c, 0.0f ),
               vec4f( 0.0f, 0.0f, 0.0f, 1.0f )
           );
}

inline const Matrix4 Matrix4::RotationZ( f32 radians )
{
    f32 c = cosf( radians );
    f32 s = sinf( radians );
    return Matrix4(
               vec4f(   c,    s, 0.0f, 0.0f ),
               vec4f(  -s,    c, 0.0f, 0.0f ),
               vec4f( 0.0f, 0.0f, 1.0f, 0.0f ),
               vec4f( 0.0f, 0.0f, 0.0f, 1.0f )
           );
}

inline const Matrix4 Matrix4::Scale( const vec3f& s )
{
    return Matrix4(
               vec4f( s.x, 0.0f, 0.0f, 0.0f ),
               vec4f( 0.0f,  s.y, 0.0f, 0.0f ),
               vec4f( 0.0f, 0.0f,  s.z, 0.0f ),
               vec4f( 0.0f, 0.0f, 0.0f, 1.0f )
           );
}

inline const Matrix4 Matrix4::Translation( const vec3f& tr )
{
    return Matrix4(
               vec4f( 1.0f, 0.0f, 0.0f, 0.0f ),
               vec4f( 0.0f, 1.0f, 0.0f, 0.0f ),
               vec4f( 0.0f, 0.0f, 1.0f, 0.0f ),
               vec4f( tr, 1.0f )
           );
}

inline const Matrix4 operator *( f32 s, const Matrix4& m )
{
    return m * s;
}

inline const Matrix4 Transpose( const Matrix4& m )
{
    return Matrix4(
               m.Row( 0 ),
               m.Row( 1 ),
               m.Row( 2 ),
               m.Row( 3 )
           );
}

} // namespace math
} // namespace base
