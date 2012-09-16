#include "base/math/matrix.h"

namespace base
{
namespace math
{

inline Matrix4::Matrix4( const Vector4& col0, const Vector4& col1, const Vector4& col2, const Vector4& col3 )
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

inline Matrix4& Matrix4::SetCol0( const Vector4& c )
{
    column0_ = c;
    return *this;
}

inline Matrix4& Matrix4::SetCol1( const Vector4& c )
{
    column1_ = c;
    return *this;
}

inline Matrix4& Matrix4::SetCol2( const Vector4& c )
{
    column2_ = c;
    return *this;
}

inline Matrix4& Matrix4::SetCol3( const Vector4& c )
{
    column3_ = c;
    return *this;
}

inline Matrix4& Matrix4::SetCol( u32 i, const Vector4& c )
{
    *( &column0_ + i ) = c;
    return *this;
}

inline Matrix4& Matrix4::SetRow( u32 i, const Vector4& r )
{
    column0_[i] = r[0];
    column1_[i] = r[1];
    column2_[i] = r[2];
    column3_[i] = r[3];
    return *this;
}

inline Matrix4& Matrix4::SetElem( u32 i, u32 j, f32 a )
{
    Vector4 c = Col( i );
    c[j] = a;
    SetCol( i, c );
    return *this;
}

inline const Vector4 Matrix4::Col0() const
{
    return column0_;
}

inline const Vector4 Matrix4::Col1() const
{
    return column1_;
}

inline const Vector4 Matrix4::Col2() const
{
    return column2_;
}

inline const Vector4 Matrix4::Col3() const
{
    return column3_;
}

inline const Vector4 Matrix4::Col( u32 i ) const
{
    return *( &column0_ + i );
}

inline const Vector4 Matrix4::Row( u32 i ) const
{
    return Vector4(
               column0_[i],
               column1_[i],
               column2_[i],
               column3_[i]
           );
}

inline f32 Matrix4::Elem( u32 i, u32 j ) const
{
    const Vector4& c = *( &column0_ + i );
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

inline const Vector4 Matrix4::operator *( const Vector4& v ) const
{
    return Vector4(
               Dot( Row( 0 ), v ),
               Dot( Row( 1 ), v ),
               Dot( Row( 2 ), v ),
               Dot( Row( 3 ), v )
           );
}

inline const Vector4 Matrix4::operator *( const Vector3& v ) const
{
    Vector4 t = Vector4( v, 0.0f );
    return Vector4(
               Dot( Row( 0 ), t ),
               Dot( Row( 1 ), t ),
               Dot( Row( 2 ), t ),
               Dot( Row( 3 ), t )
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
    *this = *this + m;
    return *this;
}

inline const Matrix4 Matrix4::Identity()
{
    return Matrix4(
               Vector4( 1.0f, 0.0f, 0.0f, 0.0f ),
               Vector4( 0.0f, 1.0f, 0.0f, 0.0f ),
               Vector4( 0.0f, 0.0f, 1.0f, 0.0f ),
               Vector4( 0.0f, 0.0f, 0.0f, 1.0f )
           );
}

inline const Matrix4 Matrix4::RotationX( f32 radians )
{
    f32 c = cosf( radians );
    f32 s = sinf( radians );
    return Matrix4(
               Vector4( 1.0f, 0.0f, 0.0f, 0.0f ),
               Vector4( 0.0f,    c,    s, 0.0f ),
               Vector4( 0.0f,   -s,    c, 0.0f ),
               Vector4( 0.0f, 0.0f, 0.0f, 1.0f )
           );
}

inline const Matrix4 Matrix4::RotationY( f32 radians )
{
    f32 c = cosf( radians );
    f32 s = sinf( radians );
    return Matrix4(
               Vector4(   c, 0.0f,   -s, 0.0f ),
               Vector4( 0.0f, 1.0f, 0.0f, 0.0f ),
               Vector4(   s, 0.0f,    c, 0.0f ),
               Vector4( 0.0f, 0.0f, 0.0f, 1.0f )
           );
}

inline const Matrix4 Matrix4::RotationZ( f32 radians )
{
    f32 c = cosf( radians );
    f32 s = sinf( radians );
    return Matrix4(
               Vector4(   c,    s, 0.0f, 0.0f ),
               Vector4(  -s,    c, 0.0f, 0.0f ),
               Vector4( 0.0f, 0.0f, 1.0f, 0.0f ),
               Vector4( 0.0f, 0.0f, 0.0f, 1.0f )
           );
}

inline const Matrix4 Matrix4::Scale( const Vector3& s )
{
    return Matrix4(
               Vector4( s.x, 0.0f, 0.0f, 0.0f ),
               Vector4( 0.0f,  s.y, 0.0f, 0.0f ),
               Vector4( 0.0f, 0.0f,  s.z, 0.0f ),
               Vector4( 0.0f, 0.0f, 0.0f, 1.0f )
           );
}

inline const Matrix4 Matrix4::Translation( const Vector3& tr )
{
    return Matrix4(
               Vector4( 1.0f, 0.0f, 0.0f, 0.0f ),
               Vector4( 0.0f, 1.0f, 0.0f, 0.0f ),
               Vector4( 0.0f, 0.0f, 1.0f, 0.0f ),
               Vector4( tr, 1.0f )
           );
}

inline std::ostream& operator<< ( std::ostream& o, const Matrix4& v )
{
    o << " [" << v.Col0() << "] ";
    o << " [" << v.Col1() << "] ";
    o << " [" << v.Col2() << "] ";
    o << " [" << v.Col3() << "] ";
    return o;
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
