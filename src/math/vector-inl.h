#pragma once

#include "math/vector.h"
#include "math/mathlib.h"

namespace base
{
namespace math
{

inline Vector2::Vector2() {
    set( 0.f, 0.f );
}
inline Vector2::Vector2( f32 vx, f32 vy ) {
    set( vx, vy );
}
inline Vector2::Vector2( const Vector2& v ) {
    set( v.x, v.y );
}
inline Vector2::Vector2( const f32* v ) {
    set( v );
}
inline Vector2::Vector2( f32 v ) {
    set( v );
}

inline void Vector2::set( f32 vx, f32 vy ) {
    x = vx;
    y = vy;
}
inline void Vector2::set( const f32* v ) {
    set( v[0], v[1] );
}
inline void Vector2::set( f32 v ) {
    x = v;
    y = v;
}

inline Vector2& Vector2::operator= ( const Vector2& v ) {
    x = v.x;
    y = v.y;
    return *this;
}
inline Vector2& Vector2::operator+= ( const Vector2& v ) {
    x += v.x;
    y += v.y;
    return *this;
}
inline Vector2& Vector2::operator-= ( const Vector2& v ) {
    x -= v.x;
    y -= v.y;
    return *this;
}
inline Vector2& Vector2::operator*= ( const Vector2& v ) {
    x *= v.x;
    y *= v.y;
    return *this;
}
inline Vector2& Vector2::operator*= ( const f32 v ) {
    x *= v;
    y *= v;
    return *this;
}
inline Vector2& Vector2::operator/= ( const Vector2& v ) {
    x /= v.x;
    y /= v.y;
    return *this;
}
inline Vector2& Vector2::operator/= ( const f32 v ) {
    x /= v;
    y /= v;
    return *this;
}
inline Vector2 Vector2::operator+ ( const Vector2& v ) const {
    return Vector2( x + v.x, y + v.y );
}
inline Vector2 Vector2::operator- ( const Vector2& v ) const {
    return Vector2( x - v.x, y - v.y );
}
inline Vector2 Vector2::operator* ( const Vector2& v ) const {
    return Vector2( x * v.x, y * v.y );
}
inline Vector2 Vector2::operator* ( const f32 v ) const {
    return Vector2( x * v, y * v );
}
inline Vector2 Vector2::operator/ ( const Vector2& v ) const {
    return Vector2( x / v.x, y / v.y );
}
inline Vector2 Vector2::operator/ ( const f32 v ) const {
    return Vector2( x / v, y / v );
}
inline Vector2 Vector2::operator- () const {
    return Vector2( -x, -y );
}
inline f32& Vector2::operator[] ( const int i ) {
    return array[i];
}
inline const f32& Vector2::operator[] ( const int i ) const {
    return array[i];
}
inline bool Vector2::operator== ( const Vector2& v ) const {
    return ( x == v.x && y == v.y );
}
inline bool Vector2::operator!= ( const Vector2& v ) const {
    return ( x != v.x || y != v.y );
}
inline bool Vector2::Equal( const Vector2& v ) const {
    return *this == v;
}
inline bool Vector2::Valid() const {
    return true;
}
inline f32 Vector2::Length2() const {
    return x * x + y * y;
}
inline f32 Vector2::Length() const {
    return sqrt( Length2() );
}
inline void Vector2::SetLength( f32 len ) {
    f32 magnitude = Length() / len;

    if ( magnitude < eps ) {
        return;
    }

    *this /= magnitude;
}
inline Vector2& Vector2::Normalize() {
    SetLength( 1.0 );
    return *this;
}
inline Vector2 Vector2::Normalized() const {
    Vector2 normalized( x, y );
    normalized.Normalize();
    return normalized;
}

inline Vector2 Normalize( const Vector2& v )
{
    Vector2 n = v;
    n.SetLength( 1.0f );
    return n;
}

inline f32 Dot( const Vector2& a, const Vector2& b )
{
    return ( a.x * b.x + a.y * b.y );
}

inline Vector3::Vector3() {
    set( 0.f, 0.f, 0.f );
}
inline Vector3::Vector3( f32 vx, f32 vy, f32 vz ) {
    set( vx, vy, vz );
}
inline Vector3::Vector3( const Vector3& v ) {
    set( v.x, v.y, v.z );
}
inline Vector3::Vector3( const Vector2& v ) {
    set( v.x, v.y, 0.f );
}
inline Vector3::Vector3( const f32* v ) {
    set( v );
}
inline Vector3::Vector3( f32 v ) {
    set( v );
}

inline void Vector3::set( f32 vx, f32 vy, f32 vz ) {
    x = vx;
    y = vy;
    z = vz;
}
inline void Vector3::set( const f32* v ) {
    set( v[0], v[1], v[2] );
}
inline void Vector3::set( f32 v ) {
    x = v;
    y = v;
    z = v;
}

inline Vector3& Vector3::operator= ( const Vector3& v ) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}
inline Vector3& Vector3::operator+= ( const Vector3& v ) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}
inline Vector3& Vector3::operator-= ( const Vector3& v ) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}
inline Vector3& Vector3::operator*= ( const Vector3& v ) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
}
inline Vector3& Vector3::operator*= ( const f32 v ) {
    x *= v;
    y *= v;
    z *= v;
    return *this;
}
inline Vector3& Vector3::operator/= ( const Vector3& v ) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
}
inline Vector3& Vector3::operator/= ( const f32 v ) {
    x /= v;
    y /= v;
    z /= v;
    return *this;
}
inline Vector3 Vector3::operator+ ( const Vector3& v ) const {
    return Vector3( x + v.x, y + v.y, z + v.z );
}
inline Vector3 Vector3::operator- ( const Vector3& v ) const {
    return Vector3( x - v.x, y - v.y, z - v.z );
}
inline Vector3 Vector3::operator* ( const Vector3& v ) const {
    return Vector3( x * v.x, y * v.y, z * v.z );
}
inline Vector3 Vector3::operator* ( const f32 v ) const {
    return Vector3( x * v, y * v, z * v );
}
inline Vector3 Vector3::operator/ ( const Vector3& v ) const {
    return Vector3( x / v.x, y / v.y, z / v.z );
}
inline Vector3 Vector3::operator/ ( const f32 v ) const {
    return Vector3( x / v, y / v, z / v );
}
inline Vector3 Vector3::operator- () const {
    return Vector3( -x, -y, -z );
}
inline f32& Vector3::operator[] ( const int i ) {
    return array[i];
}
inline const f32& Vector3::operator[] ( const int i ) const {
    return array[i];
}
inline bool Vector3::operator== ( const Vector3& v ) const {
    return ( x == v.x && y == v.y && z == v.z );
}
inline bool Vector3::operator!= ( const Vector3& v ) const {
    return ( x != v.x || y != v.y || z != v.z );
}
inline bool Vector3::Equal( const Vector3& v ) const {
    return *this == v;
}
inline bool Vector3::Valid() const {
    return true;
}
inline f32 Vector3::Length2() const {
    return x * x + y * y + z * z;
}
inline f32 Vector3::Length() const {
    return sqrt( Length2() );
}
inline void Vector3::SetLength( f32 len ) {
    f32 magnitude = Length() / len;

    if ( magnitude < eps ) {
        return;
    }

    *this /= magnitude;
}
inline Vector3& Vector3::Normalize() {
    SetLength( 1.0f );
    return *this;
}
inline Vector3 Vector3::Normalized() const {
    Vector3 normalized( x, y, z );
    normalized.Normalize();
    return normalized;
}

inline Vector3 Normalize( const Vector3& v )
{
    Vector3 n = v;
    n.SetLength( 1.0f );
    return n;
}

inline Vector3 Cross( const Vector3& a, const Vector3& b )
{
    return Vector3( a.y * b.z - a.z * b.y,  a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x );
}

inline f32 Dot( const Vector3& a, const Vector3& b )
{
    return ( a.x * b.x + a.y * b.y + a.z * b.z );
}

inline std::ostream& operator<< ( std::ostream& o, const Vector3& v )
{
    return o << v.x << ' ' << v.y << ' ' << v.z;
}

inline Vector4::Vector4() {
    set( 0.f, 0.f, 0.f, 0.f );
}
inline Vector4::Vector4( f32 vx, f32 vy, f32 vz, f32 vw ) {
    set( vx, vy, vz, vw );
}
inline Vector4::Vector4( const Vector4& v ) {
    set( v.x, v.y, v.z, v.w );
}
inline Vector4::Vector4( const Vector3& v, f32 f ) {
    set( v.x, v.y, v.z, f );
}
inline Vector4::Vector4( const Vector2& v ) {
    set( v.x, v.y, 0.f, 1.f );
}
inline Vector4::Vector4( const Vector3& v ) {
    set( v.x, v.y, v.z, 1.f );
}
inline Vector4::Vector4( const f32* v ) {
    set( v );
}
inline Vector4::Vector4( f32 v ) {
    set( v );
}

inline void Vector4::set( f32 vx, f32 vy, f32 vz, f32 vw ) {
    x = vx;
    y = vy;
    z = vz;
    w = vw;
}
inline void Vector4::set( const f32* v ) {
    set( v[0], v[1], v[2], v[3] );
}
inline void Vector4::set( f32 v ) {
    x = v;
    y = v;
    z = v;
    w = v;
}

inline Vector3 Vector4::xyz() const {
    return Vector3( x, y, z );
}

inline Vector4& Vector4::operator= ( const Vector4& v ) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}
inline Vector4& Vector4::operator+= ( const Vector4& v ) {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}
inline Vector4& Vector4::operator-= ( const Vector4& v ) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}
inline Vector4& Vector4::operator*= ( const Vector4& v ) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
}
inline Vector4& Vector4::operator*= ( const f32 v ) {
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return *this;
}
inline Vector4& Vector4::operator/= ( const Vector4& v ) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
}
inline Vector4& Vector4::operator/= ( const f32 v ) {
    x /= v;
    y /= v;
    z /= v;
    w /= v;
    return *this;
}
inline Vector4 Vector4::operator+ ( const Vector4& v ) const {
    return Vector4( x + v.x, y + v.y, z + v.z, w + v.w );
}
inline Vector4 Vector4::operator- ( const Vector4& v ) const {
    return Vector4( x - v.x, y - v.y, z - v.z, w - v.w );
}
inline Vector4 Vector4::operator* ( const Vector4& v ) const {
    return Vector4( x * v.x, y * v.y, z * v.z, w * v.w );
}
inline Vector4 Vector4::operator* ( const f32 v ) const {
    return Vector4( x * v, y * v, z * v, w * v );
}
inline Vector4 Vector4::operator/ ( const Vector4& v ) const {
    return Vector4( x / v.x, y / v.y, z / v.z, w / v.w );
}
inline Vector4 Vector4::operator/ ( const f32 v ) const {
    return Vector4( x / v, y / v, z / v, w / v );
}
inline Vector4 Vector4::operator- () const {
    return Vector4( -x, -y, -z, -w );
}
inline f32& Vector4::operator[] ( const int i ) {
    return array[i];
}
inline const f32& Vector4::operator[] ( const int i ) const {
    return array[i];
}
inline bool Vector4::operator== ( const Vector4& v ) const {
    return ( x == v.x && y == v.y && z == v.z && w == v.w );
}
inline bool Vector4::operator!= ( const Vector4& v ) const {
    return ( x != v.x || y != v.y || z != v.z || w != v.w );
}
inline bool Vector4::Equal( const Vector4& v ) const {
    return *this == v;
}
inline bool Vector4::Valid() const {
    return true;
}
inline f32 Vector4::Length2() const {
    return x * x + y * y + z * z + w * w;
}
inline f32 Vector4::Length() const {
    return sqrt( Length2() );
}
inline void Vector4::SetLength( f32 len ) {
    f32 magnitude = Length() / len;

    if ( magnitude == eps ) {
        return;
    }

    *this /= magnitude;
}
inline Vector4& Vector4::Normalize() {
    SetLength( 1.f );
    return *this;
}
inline Vector4 Vector4::Normalized() const {
    Vector4 normalized( x, y, z, w );
    normalized.Normalize();
    return normalized;
}

inline Vector4 Normalize( const Vector4& v )
{
    Vector4 n = v;
    n.SetLength( 1.0f );
    return n;
}

inline f32 Dot( const Vector4& a, const Vector4& b )
{
    return ( a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w );
}

inline std::ostream& operator<< ( std::ostream& o, const Vector4& v )
{
    o << v.x << ' ';
    o << v.y << ' ';
    o << v.z << ' ';
    o << v.w;
    return o;
}

inline Vector2::Vector2( const Vector3& v )
{
    set( v.x, v.y );
}

inline Vector2::Vector2( const Vector4& v )
{
    set( v.x, v.y );
}

inline Vector3::Vector3( const Vector4& v )
{
    set( v.x, v.y, v.z );
}


}
}