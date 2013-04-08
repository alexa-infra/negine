/**
 * \file
 * \brief       Quaternion
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "base/types.h"
#include "math/matrix.h"
#include "math/vec4.h"

namespace base
{
namespace math
{

//! Quaternion
class Quat
{
public:
    union {
        struct {
            f32 x, y, z, w;
        };
        f32 arr[4];
    };

public:
    Quat();
    Quat( const Quat& quat );
    Quat( const f32* quat );
    Quat( f32 _x, f32 _y, f32 _z, f32 _w );
    Quat( const vec3f& axis, f32 angle );
    Quat( const vec3f& v );
    Quat( const Matrix4& matrix );
    Quat operator+ ( const Quat& q ) const;
    Quat operator- ( const Quat& q ) const;
    Quat operator* ( const Quat& q ) const;
    Quat operator* ( const f32 scalar ) const;
    Quat operator/ ( const f32 scalar ) const;

    Quat& operator+= ( const Quat& q );
    Quat& operator-= ( const Quat& q );
    Quat& operator*= ( const Quat& q );

    Quat& operator*= ( f32 scalar );
    Quat& operator/= ( f32 scalar );

    bool operator== ( const Quat& q ) const;
    bool operator!= ( const Quat& q ) const;

    Quat GetInversed() const;
    Quat GetConjugated() const;
    vec3f RotatePoint( const vec3f& v ) const;
    vec3f getXYZ() const;

    void Set( const f32* quat );
    void Set( const vec3f& axis, f32 angle );
    void Set( f32 _x, f32 _y, f32 _z, f32 angle );
    void Set( const Matrix4& _matrix );
    Matrix4 GetMatrix() const;

    void Rotate( const vec3f& axis, const f32& angle );
    void RotateX( const f32& angle );
    void RotateY( const f32& angle );
    void RotateZ( const f32& angle );

    f32 GetLength() const;
    f32 GetNorm() const;

    void Normalize();
    void ComputeW();
public:
    static Quat GetRotation( const vec3f& axis, f32 angle );
    static Quat GetRotationX( f32 angle );
    static Quat GetRotationY( f32 angle );
    static Quat GetRotationZ( f32 angle );
    static Quat GetSlerp( const Quat& q0, const Quat& q1, f32 time );
    static Quat GetZero();
};

} // namespace base
} // namespace math
