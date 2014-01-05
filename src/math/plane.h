/**
 * \file
 * \brief       plane class
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#pragma once

#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mathlib.h"

namespace base
{
namespace math
{

class Plane
{
private:
    vec3f normal_;
    f32 distance_;
public:
    Plane() = default;
    Plane( const vec3f* points ) {
        set( points[0], points[1], points[2] );
    }
    Plane( const vec3f& p1, const vec3f& p2, const vec3f& p3 ) {
        set( p1, p2, p3 );
    }
    Plane( const vec3f& normal, const vec3f& planePoint ) {
        set( normal, planePoint );
    }
    Plane( const f32& a, const f32& b, const f32& c, const f32& d ) {
        set( a, b, c, d );
    }
    Plane( const vec3f& n, const f32& d ) {
        set( n.x, n.y, n.z, d );
    }
    Plane( const vec4f& v ) {
        set( v.x, v.y, v.z, v.w );
    }

    void set( const vec3f* points );
    void set( const vec3f& p1, const vec3f& p2, const vec3f& p3 );
    void set( const vec3f& normal, const vec3f& planePoint );
    void set( const f32& a, const f32& b, const f32& c, const f32& d );
    void set( const vec4f& v ) {
        set( v.x, v.y, v.z, v.w );
    }

    f32 A() const {
        return normal_.x;
    }
    f32 B() const {
        return normal_.y;
    }
    f32 C() const {
        return normal_.z;
    }
    f32 D() const {
        return distance_;
    }

    const vec3f& Normal() const {
        return normal_;
    }

    f32 Distance( const vec3f& point ) const;
    vec3f Projection( const vec3f& point ) const;
    i8 BoxOnPlaneSide( const vec3f& mmin, const vec3f& mmax ) const;
private:
    void Normalize();
};
}
}
