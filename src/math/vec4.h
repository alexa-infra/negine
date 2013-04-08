/**
 * \file
 * \brief       Vector 2D, 3D, 4D classes
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "math/vec3.h"

namespace base
{
namespace math
{

template<typename T>
struct vec4
{
    union {
        struct {
            T x, y, z, w;
        };
        T arr_[4];
    };
    vec4() {}

    template<typename R>
    vec4(R a, R b, R c, R d) : x(static_cast<T>(a)), y(static_cast<T>(b)), z(static_cast<T>(c)), w(static_cast<T>(d)) {}

    vec4(const vec4<T>& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

    template<typename R> explicit vec4(const vec4<R>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(v.w)) {}
    template<typename R> explicit vec4(R a) : x(static_cast<T>(a)), y(static_cast<T>(a)), z(static_cast<T>(a)), w(static_cast<T>(a)) {}
    template<typename R> vec4(const vec2<R>& v, T vz, T vw) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(vz), w(vw) {}
    template<typename R> vec4(const vec3<R>& v, T vw) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(vw) {}

    template<typename R> inline vec4<T>& operator=(const vec4<R>& v) { x = static_cast<T>(v.x); y = static_cast<T>(v.y); z = static_cast<T>(v.z); w = static_cast<T>(v.w); return *this; }

    template<typename R> inline vec4 operator+(R a) const { return vec4<T>(x + a, y + a, z + a, w + a); }
    template<typename R> inline vec4 operator-(R a) const { return vec4<T>(x - a, y - a, z - a, w - a); }
    template<typename R> inline vec4 operator*(R a) const { return vec4<T>(x * a, y * a, z * a, w * a); }
    template<typename R> inline vec4 operator/(R a) const { return vec4<T>(x / a, y / a, z / a, w / a); }

    template<typename R> inline vec4& operator+=(R a) { return *this = *this + a; }
    template<typename R> inline vec4& operator-=(R a) { return *this = *this - a; }
    template<typename R> inline vec4& operator*=(R a) { return *this = *this * a; }
    template<typename R> inline vec4& operator/=(R a) { return *this = *this / a; }

    inline vec4 operator+(const vec4<T>& v) const { return vec4<T>(x + v.x, y + v.y, z + v.z, w + v.w); }
    inline vec4 operator-(const vec4<T>& v) const { return vec4<T>(x - v.x, y - v.y, z - v.z, w - v.w); }
    inline vec4 operator*(const vec4<T>& v) const { return vec4<T>(x * v.x, y * v.y, z * v.z, w * v.w); }
    inline vec4 operator/(const vec4<T>& v) const { return vec4<T>(x / v.x, y / v.y, z / v.z, w / v.w); }

    inline vec4& operator+=(const vec4<T>& v) { return *this = *this + v; }
    inline vec4& operator-=(const vec4<T>& v) { return *this = *this - v; }
    inline vec4& operator*=(const vec4<T>& v) { return *this = *this * v; }
    inline vec4& operator/=(const vec4<T>& v) { return *this = *this / v; }

    inline bool operator==(const vec4<T>& v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
    inline bool operator!=(const vec4<T>& v) const { return x != v.x || y != v.y || z != v.z || w != v.w; }

    inline vec4 operator-() const { return vec4<T>(-x, -y, -z, -w); }
    inline vec2<T> xy() const { return vec2<T>(x, y); }
    inline vec2<T> zw() const { return vec2<T>(z, w); }
    inline vec3<T> xyz() const { return vec3<T>(x, y, z); }

    inline T& operator[](const int idx) { return arr_[idx]; }
    inline const T& operator[](const int idx) const { return arr_[idx]; }
};

template<typename T, typename R> inline vec4<T> operator+ (R a, const vec4<T>& v) { return vec4<T>(a) + v; }
template<typename T, typename R> inline vec4<T> operator- (R a, const vec4<T>& v) { return vec4<T>(a) - v; }
template<typename T, typename R> inline vec4<T> operator* (R a, const vec4<T>& v) { return vec4<T>(a) * v; }
template<typename T, typename R> inline vec4<T> operator/ (R a, const vec4<T>& v) { return vec4<T>(a) / v; }

typedef vec4<f32> vec4f;

inline f32 dot( const vec4f& a, const vec4f& b ) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
inline f32 length( const vec4f& a ) { return sqrt( dot( a, a ) ); }
inline f32 length2( const vec4f& a ) { return dot( a, a ); }
inline vec4f normalize( const vec4f& v ) { return v / length( v ); }

}
}
