/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "math/plane.h"

namespace base
{
namespace math
{

Plane::Plane()
{
    set( 0.f, 0.f, 0.f, 0.f );
}

void Plane::set( const vec3f* points )
{
    set( points[0], points[1], points[2] );
}

void Plane::set( const vec3f& p1, const vec3f& p2, const vec3f& p3 )
{
    normal_ = cross( p2 - p1, p3 - p1 );
    distance_ = -dot( normal_, p1 );
    Normalize();
}

void Plane::set( const vec3f& normal, const vec3f& planePoint )
{
    normal_ = normal;
    distance_ = -dot( normal, planePoint );
    Normalize();
}

void Plane::set( const f32& a, const f32& b, const f32& c, const f32& d )
{
    normal_.x = a;
    normal_.y = b;
    normal_.z = c;
    distance_ = d;
    Normalize();
}

vec3f Plane::Projection( const vec3f& point ) const
{
    return ( point - normal_ * Distance( point ) );
}

f32 Plane::Distance( const vec3f& point ) const
{
    return ( dot( normal_, point ) + distance_ );
}

void Plane::Normalize()
{
    f32 len = length(normal_);

    if ( len < eps ) {
        return;
    }

    normal_.x /= len;
    normal_.y /= len;
    normal_.z /= len;
    distance_ /= len;
}

i8 Plane::BoxOnPlaneSide( const vec3f& mmin, const vec3f& mmax ) const
{
    vec3f corners[2];

    for ( i32 i = 0; i < 3; i++ ) {
        if ( normal_[i] < 0 ) {
            corners[0][i] = mmin[i];
            corners[1][i] = mmax[i];
        } else {
            corners[1][i] = mmin[i];
            corners[0][i] = mmax[i];
        }
    }

    u8 sides = 0;

    if ( Distance( corners[0] ) >= 0 ) {
        sides = 1;
    }

    if ( Distance( corners[1] ) < 0 ) {
        sides |= 2;
    }

    // 0 - intersect
    // 1 - both plus
    // 2 - both minus
    // 3 - intersect
    return sides % 3;
}

}
}
