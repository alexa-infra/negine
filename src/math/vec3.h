/**
 * \file
 * \brief       Vector 3D
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "math/vec2.h"

namespace base
{
namespace math
{

template<typename T>
struct vec3
{
    union {
        struct {
            T x, y, z;
        };
        T arr_[3];
    };
    vec3() {}

    template<typename R>
    vec3(R a, R b, R c) : x(static_cast<T>(a)), y(static_cast<T>(b)), z(static_cast<T>(c)) {}

    vec3(const vec3<T>& v) : x(v.x), y(v.y), z(v.z) {}

    template<typename R> explicit vec3(const vec3<R>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) {}
    template<typename R> explicit vec3(R a) : x(static_cast<T>(a)), y(static_cast<T>(a)), z(static_cast<T>(a)) {}
    template<typename R> vec3(const vec2<R>& v, T vz) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(vz) {}

    template<typename R> inline vec3<T>& operator=(const vec3<R>& v) { x = static_cast<T>(v.x); y = static_cast<T>(v.y); z = static_cast<T>(v.z); return *this; }

    template<typename R> inline vec3 operator+(R a) const { return vec3<T>(x + a, y + a, z + a); }
    template<typename R> inline vec3 operator-(R a) const { return vec3<T>(x - a, y - a, z - a); }
    template<typename R> inline vec3 operator*(R a) const { return vec3<T>(x * a, y * a, z * a); }
    template<typename R> inline vec3 operator/(R a) const { return vec3<T>(x / a, y / a, z / a); }

    template<typename R> inline vec3& operator+=(R a) { return *this = *this + a; }
    template<typename R> inline vec3& operator-=(R a) { return *this = *this - a; }
    template<typename R> inline vec3& operator*=(R a) { return *this = *this * a; }
    template<typename R> inline vec3& operator/=(R a) { return *this = *this / a; }

    inline vec3 operator+(const vec3<T>& v) const { return vec3<T>(x + v.x, y + v.y, z + v.z); }
    inline vec3 operator-(const vec3<T>& v) const { return vec3<T>(x - v.x, y - v.y, z - v.z); }
    inline vec3 operator*(const vec3<T>& v) const { return vec3<T>(x * v.x, y * v.y, z * v.z); }
    inline vec3 operator/(const vec3<T>& v) const { return vec3<T>(x / v.x, y / v.y, z / v.z); }

    inline vec3& operator+=(const vec3<T>& v) { return *this = *this + v; }
    inline vec3& operator-=(const vec3<T>& v) { return *this = *this - v; }
    inline vec3& operator*=(const vec3<T>& v) { return *this = *this * v; }
    inline vec3& operator/=(const vec3<T>& v) { return *this = *this / v; }

    inline bool operator==(const vec3<T>& v) const { return x == v.x && y == v.y && z == v.z; }
    inline bool operator!=(const vec3<T>& v) const { return x != v.x || y != v.y || z != v.z; }

    inline vec3 operator-() const { return vec3<T>(-x, -y, -z); }
    inline vec2<T> xy() const { return vec2<T>(x, y); }

    inline T& operator[](const int idx) { return arr_[idx]; }
    inline const T& operator[](const int idx) const { return arr_[idx]; }
};

template<typename T, typename R> inline vec3<T> operator+ (R a, const vec3<T>& v) { return vec3<T>(a) + v; }
template<typename T, typename R> inline vec3<T> operator- (R a, const vec3<T>& v) { return vec3<T>(a) - v; }
template<typename T, typename R> inline vec3<T> operator* (R a, const vec3<T>& v) { return vec3<T>(a) * v; }
template<typename T, typename R> inline vec3<T> operator/ (R a, const vec3<T>& v) { return vec3<T>(a) / v; }

typedef vec3<f32> vec3f;
typedef vec3<i32> vec3i;

inline f32 dot( const vec3f& a, const vec3f& b ) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline f32 length( const vec3f& a ) { return sqrt( dot( a, a ) ); }
inline f32 length2( const vec3f& a ) { return dot( a, a ); }
inline vec3f normalize( const vec3f& v ) { return v / length( v ); }

template<typename T> inline vec3<T> cross( const vec3<T>& a, const vec3<T>& b ) {
    return vec3<T>( a.y * b.z - a.z * b.y,  a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x );
}
}
}
