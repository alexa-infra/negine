/**
 * \file
 * \brief       Vector 2D, 3D, 4D classes
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include <iostream>

namespace base
{
namespace math
{

class Vector2;
class Vector3;
class Vector4;

class Vector2
{
public:
    union {
        struct {
            f32 x, y;
        };
        f32 array[2];
    };
    Vector2();
    Vector2( f32 vx, f32 vy );
    Vector2( const Vector2& v );
    explicit Vector2( const Vector3& v );
    explicit Vector2( const Vector4& v );
    explicit Vector2( const f32* v );
    explicit Vector2( f32 v );

    void set( f32 vx, f32 vy );
    void set( const f32* v );
    void set( f32 v );

    Vector2& operator= ( const Vector2& v );
    Vector2& operator+= ( const Vector2& v );
    Vector2& operator-= ( const Vector2& v );
    Vector2& operator*= ( const Vector2& v );
    Vector2& operator*= ( const f32 v );
    Vector2& operator/= ( const Vector2& v );
    Vector2& operator/= ( const f32 v );
    Vector2 operator+ ( const Vector2& v ) const;
    Vector2 operator- ( const Vector2& v ) const;
    Vector2 operator* ( const Vector2& v ) const;
    Vector2 operator* ( const f32 v ) const;
    Vector2 operator/ ( const Vector2& v ) const;
    Vector2 operator/ ( const f32 v ) const;
    Vector2 operator- () const;
    f32& operator[] ( const int i );
    const f32& operator[] ( const int i ) const;
    bool operator== ( const Vector2& v ) const;
    bool operator!= ( const Vector2& v ) const;
    bool Equal( const Vector2& v ) const;
    bool Valid() const;
    f32 Length2() const;
    f32 Length() const;
    void SetLength( f32 len );
    Vector2& Normalize();
    Vector2 Normalized() const;
};

Vector2 Normalize( const Vector2& v );
f32 Dot( const Vector2& a, const Vector2& b );

class Vector3
{
public:
    union {
        struct {
            f32 x, y, z;
        };
        struct {
            f32 array[3];
        };
    };
    Vector3();
    Vector3( f32 vx, f32 vy, f32 vz );
    Vector3( const Vector3& v );
    explicit Vector3( const Vector2& v );
    explicit Vector3( const Vector4& v );
    explicit Vector3( const f32* v );
    explicit Vector3( f32 v );

    void set( f32 vx, f32 vy, f32 vz );
    void set( const f32* v );
    void set( f32 v );

    Vector3& operator= ( const Vector3& v );
    Vector3& operator+= ( const Vector3& v );
    Vector3& operator-= ( const Vector3& v );
    Vector3& operator*= ( const Vector3& v );
    Vector3& operator*= ( const f32 v );
    Vector3& operator/= ( const Vector3& v );
    Vector3& operator/= ( const f32 v );
    Vector3 operator+ ( const Vector3& v ) const;
    Vector3 operator- ( const Vector3& v ) const;
    Vector3 operator* ( const Vector3& v ) const;
    Vector3 operator* ( const f32 v ) const;
    Vector3 operator/ ( const Vector3& v ) const;
    Vector3 operator/ ( const f32 v ) const;
    Vector3 operator- () const;
    f32& operator[] ( const int i );
    const f32& operator[] ( const int i ) const;
    bool operator== ( const Vector3& v ) const;
    bool operator!= ( const Vector3& v ) const;
    bool Equal( const Vector3& v ) const;
    bool Valid() const;
    f32 Length2() const;
    f32 Length() const;
    void SetLength( f32 len );
    Vector3& Normalize();
    Vector3 Normalized() const;

    friend std::ostream& operator<< ( std::ostream& o, const Vector3& v );
};

Vector3 Normalize( const Vector3& v );
Vector3 Cross( const Vector3& a, const Vector3& b );
f32 Dot( const Vector3& a, const Vector3& b );
std::ostream& operator<< ( std::ostream& o, const Vector3& v );

class Vector4
{
public:
    union {
        struct {
            f32 x, y, z, w;
        };
        struct {
            f32 array[4];
        };
    };
    Vector4();
    Vector4( f32 vx, f32 vy, f32 vz, f32 vw );
    Vector4( const Vector4& v );
    Vector4( const Vector3& v, f32 f );
    explicit Vector4( const Vector2& v );
    explicit Vector4( const Vector3& v );
    explicit Vector4( const f32* v );
    explicit Vector4( f32 v );

    void set( f32 vx, f32 vy, f32 vz, f32 vw );
    void set( const f32* v );
    void set( f32 v );

    Vector3 xyz() const;

    Vector4& operator= ( const Vector4& v );
    Vector4& operator+= ( const Vector4& v );
    Vector4& operator-= ( const Vector4& v );
    Vector4& operator*= ( const Vector4& v );
    Vector4& operator*= ( const f32 v );
    Vector4& operator/= ( const Vector4& v );
    Vector4& operator/= ( const f32 v );
    Vector4 operator+ ( const Vector4& v ) const;
    Vector4 operator- ( const Vector4& v ) const;
    Vector4 operator* ( const Vector4& v ) const;
    Vector4 operator* ( const f32 v ) const;
    Vector4 operator/ ( const Vector4& v ) const;
    Vector4 operator/ ( const f32 v ) const;
    Vector4 operator- () const;
    f32& operator[] ( const int i );
    const f32& operator[] ( const int i ) const;
    bool operator== ( const Vector4& v ) const;
    bool operator!= ( const Vector4& v ) const;
    bool Equal( const Vector4& v ) const;
    bool Valid() const;
    f32 Length2() const;
    f32 Length() const;
    void SetLength( f32 len );
    Vector4& Normalize();
    Vector4 Normalized() const;
    friend std::ostream& operator<< ( std::ostream& o, const Vector4& v );
};

Vector4 Normalize( const Vector4& v );
f32 Dot( const Vector4& a, const Vector4& b );
std::ostream& operator<< ( std::ostream& o, const Vector4& v );

}
}
