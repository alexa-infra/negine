/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "gtest/gtest.h"
#include "math/quat.h"
#include "math/vec3.h"

using base::math::vec3f;
using base::math::Quat;
using base::math::Matrix4;
using base::f32;

TEST( quat, sum )
{
    const Quat a( 1.f, 2.f, 3.f, 4.f );
    const Quat b( 2.f, 3.f, 4.f, 5.f );
    Quat c = a + b;
    EXPECT_TRUE( vec3f( 3.f, 5.f, 7.f ) == c.getXYZ() );
    EXPECT_FLOAT_EQ( 9., c.w );
    c = b - a;
    EXPECT_TRUE( vec3f( 1.f, 1.f, 1.f ) == c.getXYZ() );
    EXPECT_FLOAT_EQ( 1.f, c.w );
}

TEST( quat, scalar )
{
    const Quat a( 1.f, 2.f, 3.f, 4.f );
    const Quat c = a * 2.f;
    EXPECT_TRUE( Quat( 2.f, 4.f, 6.f, 8.f ) == c );
}

TEST( quat, cross_product )
{
    const vec3f v1( 1.f, 2.f, 3.f );
    const f32 w1 = 4.f;
    const vec3f v2( 2.f, 3.f, 4.f );
    const f32 w2 = 5.f;
    Quat a( v1.x, v1.y, v1.z, w1 );
    Quat b( v2.x, v2.y, v2.z, w2 );
    Quat c = a * b;
    vec3f expectXYZ = cross( v1, v2 ) + v2 * w1 + v1 * w2;
    f32 expectW = w1 * w2 - dot( v1, v2 );
    Quat expected( expectXYZ );
    expected.w = expectW;
    EXPECT_EQ( expected, c );
}

TEST( quat, norm_and_length )
{
    const Quat a( 1.f, 2.f, 3.f, 4.f );
    EXPECT_FLOAT_EQ( 1.f * 1.f + 2.f * 2.f + 3.f * 3.f + 4.f * 4.f, a.GetNorm() );
    EXPECT_FLOAT_EQ( sqrtf( 1.f * 1.f + 2.f * 2.f + 3.f * 3.f + 4.f * 4.f ), a.GetLength() );
}

TEST( quat, conjugate )
{
    Quat a( 1.f, 2.f, 3.f, 4.f );
    Quat b = a.GetConjugated();
    EXPECT_EQ( Quat( -1.f, -2.f, -3.f, 4.f ), b );
}

TEST( quat, rotate_vector )
{
    const Quat a( 1.f, 2.f, 3.f, 4.f );
    const vec3f b( 2.f, 3.f, 4.f );
    vec3f c = a.RotatePoint( b );
    Quat temp = a * Quat( b.x, b.y, b.z, 0.f ) * a.GetConjugated();
    vec3f expected = temp.getXYZ();
    EXPECT_EQ( expected, c );
}
