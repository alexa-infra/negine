/**
 * @file    base/math/vector.h
 * @brief   Vector in 2D, 3D, 4D classes
 *
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#pragma once

#include "base/math/mathlib.h"

namespace base {
namespace math {

class Vector2;
class Vector3;
class Vector4;

class Vector2 {
 public:
    union {
        struct {
            f32 x, y;
        };
        f32 array[2];
    };
    Vector2() { set(0.f, 0.f); }
    Vector2(f32 vx, f32 vy) { set(vx, vy); }
    Vector2(const Vector2& v) { set(v.x, v.y); }
    explicit Vector2(const Vector3& v);
    explicit Vector2(const Vector4& v);
    explicit Vector2(const f32* v) { set(v); }
    explicit Vector2(f32 v) { set(v); }

    inline void set(f32 vx, f32 vy) {
        x = vx;
        y = vy;
    }
    inline void set(const f32* v) { set(v[0], v[1]); }
    inline void set(f32 v) {
        x = v;
        y = v;
    }

    inline Vector2& operator= (const Vector2& v) {
        x = v.x;
        y = v.y;
        return *this;
    }
    inline Vector2& operator+= (const Vector2& v) {
        x += v.x;
        y += v.y;
        return *this;
    }
    inline Vector2& operator-= (const Vector2& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }
    inline Vector2& operator*= (const Vector2& v) {
        x *= v.x;
        y *= v.y;
        return *this;
    }
    inline Vector2& operator*= (const f32 v) {
        x *= v;
        y *= v;
        return *this;
    }
    inline Vector2& operator/= (const Vector2& v) {
        x /= v.x;
        y /= v.y;
        return *this;
    }
    inline Vector2& operator/= (const f32 v) {
        x /= v;
        y /= v;
        return *this;
    }
    inline Vector2 operator+ (const Vector2& v) const {
        return Vector2(x + v.x, y + v.y);
    }
    inline Vector2 operator- (const Vector2& v) const {
        return Vector2(x - v.x, y - v.y);
    }
    inline Vector2 operator* (const Vector2& v) const {
        return Vector2(x * v.x, y * v.y);
    }
    inline Vector2 operator* (const f32 v) const {
        return Vector2(x * v, y * v);
    }
    inline Vector2 operator/ (const Vector2& v) const {
        return Vector2(x / v.x, y / v.y);
    }
    inline Vector2 operator/ (const f32 v) const {
        return Vector2(x / v, y / v);
    }
    inline Vector2 operator- () const {
        return Vector2( -x, -y );
    }
    inline f32& operator[] (const int i) {
        return array[i];
    }
    inline const f32& operator[] (const int i) const {
        return array[i];
    }
    inline bool operator== (const Vector2& v) const {
        return (x == v.x && y == v.y);
    }
    inline bool operator!= (const Vector2& v) const {
        return (x != v.x || y != v.y);
    }
    inline bool Equal(const Vector2& v) const {
        return *this == v;
    }
    inline bool Valid() const {
        return true;
    }
    inline f32 Length2() const {
        return x * x + y * y;
    }
    inline f32 Length() const {
        return sqrt(Length2());
    }
    inline void SetLength(f32 len) {
        f32 magnitude = Length() / len;
        if (magnitude < eps)
            return;
        *this /= magnitude;
    }
    inline Vector2& Normalize() {
        SetLength(1.0);
        return *this;
    }
    inline Vector2 Normalized() const {
        Vector2 normalized(x, y);
        normalized.Normalize();
        return normalized;
    }
    inline f32 Dot(const Vector2& v) const {
        return (x * v.x + y * v.y);
    }
};

class Vector3 {
 public:
    union {
        struct {
            f32 x, y, z;
        };
        struct {
            f32 array[3];
        };
    };
    Vector3() { set(0.f, 0.f, 0.f); }
    Vector3(f32 vx, f32 vy, f32 vz) { set(vx, vy, vz); }
    Vector3(const Vector3& v) { set(v.x, v.y, v.z); }
    explicit Vector3(const Vector2& v) { set(v.x, v.y, 0.f); }
    explicit Vector3(const Vector4& v);
    explicit Vector3(const f32* v) { set(v); }
    explicit Vector3(f32 v) { set(v); }

    inline void set(f32 vx, f32 vy, f32 vz) {
        x = vx;
        y = vy;
        z = vz;
    }
    inline void set(const f32* v) { set(v[0], v[1], v[2]); }
    inline void set(f32 v) {
        x = v;
        y = v;
        z = v;
    }

    inline Vector3& operator= (const Vector3& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
    inline Vector3& operator+= (const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    inline Vector3& operator-= (const Vector3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    inline Vector3& operator*= (const Vector3& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    inline Vector3& operator*= (const f32 v) {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }
    inline Vector3& operator/= (const Vector3& v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        return *this;
    }
    inline Vector3& operator/= (const f32 v) {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }
    inline Vector3 operator+ (const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
    inline Vector3 operator- (const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    inline Vector3 operator* (const Vector3& v) const {
        return Vector3(x * v.x, y * v.y, z * v.z);
    }
    inline Vector3 operator* (const f32 v) const {
        return Vector3(x * v, y * v, z * v);
    }
    inline Vector3 operator/ (const Vector3& v) const {
        return Vector3(x / v.x, y / v.y, z / v.z);
    }
    inline Vector3 operator/ (const f32 v) const {
        return Vector3(x / v, y / v, z / v);
    }
    inline Vector3 operator- () const {
        return Vector3( -x, -y, -z );
    }
    inline Vector3 operator ^ (const Vector3& v) const {
        return Vector3(y*v.z - z*v.y,  z*v.x - x*v.z, x*v.y - y*v.x);
    }
    inline f32& operator[] (const int i) {
        return array[i];
    }
    inline const f32& operator[] (const int i) const {
        return array[i];
    }
    inline bool operator== (const Vector3& v) const {
        return (x == v.x && y == v.y && z == v.z);
    }
    inline bool operator!= (const Vector3& v) const {
        return (x != v.x || y != v.y || z != v.z);
    }
    inline bool Equal(const Vector3& v) const {
        return *this == v;
    }
    inline bool Valid() const {
        return true;
    }
    inline f32 Length2() const {
        return x * x + y * y + z * z;
    }
    inline f32 Length() const {
        return sqrt(Length2());
    }
    inline void SetLength(f32 len) {
        f32 magnitude = Length() / len;
        if (magnitude < eps)
            return;
        *this /= magnitude;
    }
    inline Vector3& Normalize() {
        SetLength(1.0f);
        return *this;
    }
    inline Vector3 Normalized() const {
        Vector3 normalized(x, y, z);
        normalized.Normalize();
        return normalized;
    }
    inline f32 Dot(const Vector3& v) const {
        return (x * v.x + y * v.y + z * v.z);
    }
};

class Vector4 {
 public:
    union {
        struct {
            f32 x, y, z, w;
        };
        struct {
            f32 array[4];
        };
    };
    Vector4() { set(0.f, 0.f, 0.f, 0.f); }
    Vector4(f32 vx, f32 vy, f32 vz, f32 vw) { set(vx, vy, vz, vw); }
    Vector4(const Vector4& v) { set(v.x, v.y, v.z, v.w); }
    explicit Vector4(const Vector2& v) { set(v.x, v.y, 0.f, 0.f); }
    explicit Vector4(const Vector3& v) { set(v.x, v.y, v.z, 0.f); }
    explicit Vector4(const f32* v) { set(v); }
    explicit Vector4(f32 v) { set(v); }

    inline void set(f32 vx, f32 vy, f32 vz, f32 vw) {
        x = vx;
        y = vy;
        z = vz;
        w = vw;
    }
    inline void set(const f32* v) { set(v[0], v[1], v[2], v[3]); }
    inline void set(f32 v) {
        x = v;
        y = v;
        z = v;
        w = v;
    }

    inline Vector4& operator= (const Vector4& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
        return *this;
    }
    inline Vector4& operator+= (const Vector4& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }
    inline Vector4& operator-= (const Vector4& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }
    inline Vector4& operator*= (const Vector4& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        w *= v.w;
        return *this;
    }
    inline Vector4& operator*= (const f32 v) {
        x *= v;
        y *= v;
        z *= v;
        w *= v;
        return *this;
    }
    inline Vector4& operator/= (const Vector4& v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        w /= v.w;
        return *this;
    }
    inline Vector4& operator/= (const f32 v) {
        x /= v;
        y /= v;
        z /= v;
        w /= v;
        return *this;
    }
    inline Vector4 operator+ (const Vector4& v) const {
        return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
    }
    inline Vector4 operator- (const Vector4& v) const {
        return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
    }
    inline Vector4 operator* (const Vector4& v) const {
        return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
    }
    inline Vector4 operator* (const f32 v) const {
        return Vector4(x * v, y * v, z * v, w *v);
    }
    inline Vector4 operator/ (const Vector4& v) const {
        return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
    }
    inline Vector4 operator/ (const f32 v) const {
        return Vector4(x / v, y / v, z / v, w / v);
    }
    inline Vector4 operator- () const {
        return Vector4( -x, -y, -z, -w );
    }
    inline f32& operator[] (const int i) {
        return array[i];
    }
    inline const f32& operator[] (const int i) const {
        return array[i];
    }
    inline bool operator== (const Vector4& v) const {
        return (x == v.x && y == v.y && z == v.z && w == v.w);
    }
    inline bool operator!= (const Vector4& v) const {
        return (x != v.x || y != v.y || z != v.z || w != v.w);
    }
    inline bool Equal(const Vector4& v) const {
        return *this == v;
    }
    inline bool Valid() const {
        return true;
    }
    inline f32 Length2() const {
        return x * x + y * y + z * z + w * w;
    }
    inline f32 Length() const {
        return sqrt(Length2());
    }
    inline void SetLength(f32 len) {
        f32 magnitude = Length() / len;
        if (magnitude == eps)
            return;
        *this /= magnitude;
    }
    inline Vector3& Normalize() {
        SetLength(1.0f);
    }
    inline Vector4 Normalized() const {
        Vector4 normalized(x, y, z, w);
        normalized.Normalize();
        return normalized;
    }
    inline f32 Dot(const Vector4& v) const {
        return (x * v.x + y * v.y + z * v.z + w * v.w);
    }
};
}
}
