#pragma once

#include <math.h>

namespace base {
namespace math {

class Vector2 {
public:
    union {
        struct {
            float x, y;
        };
        float array[2];
    };
    Vector2() { set(0.f, 0.f); }
    Vector2(float vx, float vy) { set(vx, vy); }
    Vector2(const Vector2& v) { set(v.x, v.y); }
    explicit Vector2(const Vector3& v);
    explicit Vector2(const Vector4& v);
    explicit Vector2(const float* v) { set(v); }
    explicit Vector2(float v) { set(v); }

    inline void set(float vx, float vy) { x = vx; y = vy; }
    inline void set(const float* v) { set(v[0], v[1]); }
    inline void set(float v) { x = v; y = v; }

    inline Vector2& operator= (const Vector2& v) {
        x = v.x; y = v.y;
        return *this; 
    }
    inline Vector2& operator+= (const Vector2& v) { 
        x += v.x; y += v.y;
        return *this; 
    }
    inline Vector2& operator-= (const Vector2& v) { 
        x -= v.x; y -= v.y; 
        return *this;
    }
    inline Vector2& operator*= (const Vector2& v) { 
        x *= v.x; y *= v.y; 
        return *this; 
    }
    inline Vector2& operator*= (const float v) { 
        x *= v; y *= v; 
        return *this; 
    }
    inline Vector2& operator/= (const Vector2& v) { 
        x /= v.x; y /= v.y; 
        return *this;
    }
    inline Vector2& operator/= (const float v) { 
        x /= v; y /= v; 
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
    inline Vector2 operator* (const float v) const { 
        return Vector2(x * v, y * v); 
    }
    inline Vector2 operator/ (const Vector2& v) const { 
        return Vector2(x / v.x, y / v.y); 
    }
    inline Vector2 operator/ (const float v) const { 
        return Vector2(x / v, y / v); 
    }
    inline Vector2 operator- () const { 
        return Vector2( -x, -y ); 
    }
    inline float& operator[] (const int i) { 
        return array[i]; 
    }
    inline const float& operator[] (const int i) const { 
        return array[i]; 
    }
    inline bool operator== (const Vector2& v) const { 
        return (x == v.x && y == v.y); 
    }
    inline bool operator!= (const Vector2& v) const { 
        return (x != v.x || y != v.y); 
    }
    inline bool Equal (const Vector2& v) const { 
        return *this == v;
    }
    inline bool Valid() const { 
        return true; 
    }
    inline float Length2() const { 
        return x * x + y * y; 
    }
    inline float Length () const {
        return sqrt(Length2());
    }
    inline void SetLength(float len) {
        float magnitude = Length() / len;
        if (magnitude == 0.)
            return;
        *this /= magnitude;
    }
    inline Vector2& Normalize () {         
        SetLength(1.0);
    }
    inline Vector2 Normalized () const {
        Vector2 normalized(x, y);
        normalized.Normalize();
        return normalized;
    }
    inline float Dot (const Vector2& v) const { 
        return (x * v.x + y * v.y); 
    }
};

class Vector3 {
public:
    union {
        float x, y, z;
        struct {
            float array[3];
        };
    };
    Vector3() { set(0.f, 0.f, 0.f); }
    Vector3(float vx, float vy, float vz) { set(vx, vy, vz); }
    Vector3(const Vector3& v) { set(v.x, v.y, v.z); }
    explicit Vector3(const Vector2& v) { set(v.x, v.y, 0.f); }
    explicit Vector3(const Vector4& v);
    explicit Vector3(const float* v) { set(v); }
    explicit Vector3(float v) { set(v); }

    inline void set(float vx, float vy, float vz) { x = vx; y = vy; z = vz; }
    inline void set(const float* v) { set(v[0], v[1], v[2]); }
    inline void set(float v) { x = v; y = v; z = v; }

    inline Vector3& operator= (const Vector3& v) {
        x = v.x; y = v.y; z = v.z;
        return *this; 
    }
    inline Vector3& operator+= (const Vector3& v) { 
        x += v.x; y += v.y; z += v.z;
        return *this; 
    }
    inline Vector3& operator-= (const Vector3& v) { 
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }
    inline Vector3& operator*= (const Vector3& v) { 
        x *= v.x; y *= v.y; z *= v.z;
        return *this; 
    }
    inline Vector3& operator*= (const float v) { 
        x *= v; y *= v; z *= v;
        return *this; 
    }
    inline Vector3& operator/= (const Vector3& v) { 
        x /= v.x; y /= v.y; z /= v.z;
        return *this;
    }
    inline Vector3& operator/= (const float v) { 
        x /= v; y /= v; z /= v;
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
    inline Vector3 operator* (const float v) const { 
        return Vector3(x * v, y * v, z * v); 
    }
    inline Vector3 operator/ (const Vector3& v) const { 
        return Vector3(x / v.x, y / v.y, z / v.z); 
    }
    inline Vector3 operator/ (const float v) const { 
        return Vector3(x / v, y / v, z / v); 
    }
    inline Vector3 operator- () const { 
        return Vector3( -x, -y -z ); 
    }
    inline float& operator[] (const int i) { 
        return array[i]; 
    }
    inline const float& operator[] (const int i) const { 
        return array[i]; 
    }
    inline bool operator== (const Vector3& v) const { 
        return (x == v.x && y == v.y && z == v.z); 
    }
    inline bool operator!= (const Vector3& v) const { 
        return (x != v.x || y != v.y || z != v.z); 
    }
    inline bool Equal (const Vector3& v) const { 
        return *this == v;
    }
    inline bool Valid() const { 
        return true; 
    }
    inline float Length2() const { 
        return x * x + y * y + z * z; 
    }
    inline float Length () const {
        return sqrt(Length2());
    }
    inline void SetLength(float len) {
        float magnitude = Length() / len;
        if (magnitude == 0.f)
            return;
        *this /= magnitude;
    }
    inline Vector3& Normalize () {         
        SetLength(1.0f);
    }
    inline Vector3 Normalized () const {
        Vector3 normalized(x, y);
        normalized.Normalize();
        return normalized;
    }
    inline float Dot (const Vector3& v) const { 
        return (x * v.x + y * v.y + z * v.z); 
    }

};

class Vector4 {
public:
    union {
        float x, y, z, w;
        struct {
            float array[4];
        };
    };
    Vector4() { set(0.f, 0.f, 0.f, 0.f); }
    Vector4(float vx, float vy, float vz, float vw) { set(vx, vy, vz, vw); }
    Vector4(const Vector4& v) { set(v.x, v.y, v.z, v.w); }
    explicit Vector4(const Vector2& v) { set(v.x, v.y, 0.f, 0.f); }
    explicit Vector4(const Vector3& v) { set(v.x, v.y, v.z, 0.f); }
    explicit Vector4(const float* v) { set(v); }
    explicit Vector4(float v) { set(v); }

    inline void set(float vx, float vy, float vz, float vw) { 
        x = vx; y = vy; z = vz; w = vw;
    }
    inline void set(const float* v) { set(v[0], v[1], v[2], v[3]); }
    inline void set(float v) { x = v; y = v; z = v; w = v; }

    inline Vector4& operator= (const Vector4& v) {
        x = v.x; y = v.y; z = v.z; w = v.w;
        return *this; 
    }
    inline Vector4& operator+= (const Vector4& v) { 
        x += v.x; y += v.y; z += v.z; w = v.w;
        return *this; 
    }
    inline Vector4& operator-= (const Vector4& v) { 
        x -= v.x; y -= v.y; z -= v.z; w = v.w;
        return *this;
    }
    inline Vector4& operator*= (const Vector4& v) { 
        x *= v.x; y *= v.y; z *= v.z; w = v.w;
        return *this; 
    }
    inline Vector4& operator*= (const float v) { 
        x *= v; y *= v; z *= v; w *= v;
        return *this; 
    }
    inline Vector4& operator/= (const Vector3& v) { 
        x /= v.x; y /= v.y; z /= v.z; w /= v.w;
        return *this;
    }
    inline Vector4& operator/= (const float v) { 
        x /= v; y /= v; z /= v; w /= v;
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
    inline Vector4 operator* (const float v) const { 
        return Vector4(x * v, y * v, z * v, w *v); 
    }
    inline Vector4 operator/ (const Vector4& v) const { 
        return Vector4(x / v.x, y / v.y, z / v.z, w / v.w); 
    }
    inline Vector4 operator/ (const float v) const { 
        return Vector4(x / v, y / v, z / v, w / v); 
    }
    inline Vector4 operator- () const { 
        return Vector3( -x, -y -z, -w ); 
    }
    inline float& operator[] (const int i) { 
        return array[i]; 
    }
    inline const float& operator[] (const int i) const { 
        return array[i]; 
    }
    inline bool operator== (const Vector4& v) const { 
        return (x == v.x && y == v.y && z == v.z && w = v.w); 
    }
    inline bool operator!= (const Vector4& v) const { 
        return (x != v.x || y != v.y || z != v.z || w != v.w); 
    }
    inline bool Equal (const Vector4& v) const { 
        return *this == v;
    }
    inline bool Valid() const { 
        return true; 
    }
    inline float Length2() const { 
        return x * x + y * y + z * z + w * w; 
    }
    inline float Length () const {
        return sqrt(Length2());
    }
    inline void SetLength(float len) {
        float magnitude = Length() / len;
        if (magnitude == 0.f)
            return;
        *this /= magnitude;
    }
    inline Vector3& Normalize () {
        SetLength(1.0f);
    }
    inline Vector4 Normalized () const {
        Vector4 normalized(x, y);
        normalized.Normalize();
        return normalized;
    }
    inline float Dot (const Vector4& v) const { 
        return (x * v.x + y * v.y + z * v.z + w * v.w); 
    }
};

Vector2::Vector2(const Vector3& v) {
    set(v.x, v.y);
}

Vector2::Vector2(const Vector4& v) {
    set(v.x, v.y);
}

Vector3::Vector3(const Vector4& v) {
    set(v.x, v.y, v.z);
}

}
}
