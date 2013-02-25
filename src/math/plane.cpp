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

void Plane::set( const Vector3* points )
{
    set( points[0], points[1], points[2] );
}

void Plane::set( const Vector3& p1, const Vector3& p2, const Vector3& p3 )
{
    normal_ = Cross( p2 - p1, p3 - p1 );
    distance_ = -Dot( normal_, p1 );
    Normalize();
}

void Plane::set( const Vector3& normal, const Vector3& planePoint )
{
    normal_ = normal;
    distance_ = -Dot( normal, planePoint );
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

Vector3 Plane::Projection( const Vector3& point ) const
{
    return ( point - normal_ * Distance( point ) );
}

f32 Plane::Distance( const Vector3& point ) const
{
    return ( Dot( normal_, point ) + distance_ );
}

void Plane::Normalize()
{
    f32 len = normal_.Length();

    if ( len < eps ) {
        return;
    }

    normal_.x /= len;
    normal_.y /= len;
    normal_.z /= len;
    distance_ /= len;
}

i8 Plane::BoxOnPlaneSide( const Vector3& mmin, const Vector3& mmax ) const
{
    Vector3 corners[2];

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
