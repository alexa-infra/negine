/**
 * \file
 * \brief       Vector 2D
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "math/mathlib.h"

namespace base
{
namespace math
{

template<typename T>
struct vec2
{
    T x, y;
    vec2() {}

    template<typename R>
    vec2(R a, R b) : x(static_cast<T>(a)), y(static_cast<T>(b)) {}

    vec2(const vec2<T>& v) : x(v.x), y(v.y) {}

    template<typename R> explicit vec2(const vec2<R>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}
    template<typename R> explicit vec2(R a) : x(static_cast<T>(a)), y(static_cast<T>(a)) {}

    template<typename R> inline vec2<T>& operator=(const vec2<R>& v) { x = static_cast<T>(v.x); y = static_cast<T>(v.y); return *this; }

    template<typename R> inline vec2 operator+(R a) const { return vec2<T>(x + a, y + a); }
    template<typename R> inline vec2 operator-(R a) const { return vec2<T>(x - a, y - a); }
    template<typename R> inline vec2 operator*(R a) const { return vec2<T>(x * a, y * a); }
    template<typename R> inline vec2 operator/(R a) const { return vec2<T>(x / a, y / a); }

    template<typename R> inline vec2& operator+=(R a) { return *this = *this + a; }
    template<typename R> inline vec2& operator-=(R a) { return *this = *this - a; }
    template<typename R> inline vec2& operator*=(R a) { return *this = *this * a; }
    template<typename R> inline vec2& operator/=(R a) { return *this = *this / a; }

    inline vec2 operator+(const vec2<T>& v) const { return vec2<T>(x + v.x, y + v.y); }
    inline vec2 operator-(const vec2<T>& v) const { return vec2<T>(x - v.x, y - v.y); }
    inline vec2 operator*(const vec2<T>& v) const { return vec2<T>(x * v.x, y * v.y); }
    inline vec2 operator/(const vec2<T>& v) const { return vec2<T>(x / v.x, y / v.y); }

    inline vec2& operator+=(const vec2<T>& v) { return *this = *this + v; }
    inline vec2& operator-=(const vec2<T>& v) { return *this = *this - v; }
    inline vec2& operator*=(const vec2<T>& v) { return *this = *this * v; }
    inline vec2& operator/=(const vec2<T>& v) { return *this = *this / v; }

    inline bool operator==(const vec2<T>& v) const { return x == v.x && y == v.y; }
    inline bool operator!=(const vec2<T>& v) const { return x != v.x || y != v.y; }

    inline vec2 operator-() const { return vec2<T>(-x, -y); }
};

template<typename T, typename R> inline vec2<T> operator+ (R a, const vec2<T>& v) { return vec2<T>(a) + v; }
template<typename T, typename R> inline vec2<T> operator- (R a, const vec2<T>& v) { return vec2<T>(a) - v; }
template<typename T, typename R> inline vec2<T> operator* (R a, const vec2<T>& v) { return vec2<T>(a) * v; }
template<typename T, typename R> inline vec2<T> operator/ (R a, const vec2<T>& v) { return vec2<T>(a) / v; }

typedef vec2<f32> vec2f;
typedef vec2<i32> vec2i;

inline f32 dot( const vec2f& a, const vec2f& b ) { return a.x * b.x + a.y * b.y; }
inline f32 length( const vec2f& a ) { return sqrt( dot( a, a ) ); }
inline f32 length2( const vec2f& a ) { return dot( a, a ); }
inline vec2f normalize( const vec2f& v ) { return v / length( v ); }

}
}
