/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "gtest/gtest.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"

using namespace base;
using namespace base::math;

TEST( math, vector_2d_init )
{
    const vec2f a( 1.0f, 2.0f );
    EXPECT_FLOAT_EQ( 1.0f, a.x );
    EXPECT_FLOAT_EQ( 2.0f, a.y );
    vec2f b( a );
    EXPECT_FLOAT_EQ( 1.0f, b.x );
    EXPECT_FLOAT_EQ( 2.0f, b.y );
    vec2f d( 1, 2 );
    EXPECT_FLOAT_EQ( 1.0f, d.x );
    EXPECT_FLOAT_EQ( 2.0f, d.y );
    vec2f e( 5.0f );
    EXPECT_FLOAT_EQ( 5.0f, e.x );
    EXPECT_FLOAT_EQ( 5.0f, e.y );
    vec2f c = a;
    EXPECT_FLOAT_EQ( 1.0f, c.x );
    EXPECT_FLOAT_EQ( 2.0f, c.y );
}

TEST( math, vector_2d_arithm )
{
    const vec2f a( 1.0f, 2.0f );
    vec2f b = a + 1;
    EXPECT_FLOAT_EQ( 2.0f, b.x );
    EXPECT_FLOAT_EQ( 3.0f, b.y );
    b = a - 1.0f;
    EXPECT_FLOAT_EQ( 0.0f, b.x );
    EXPECT_FLOAT_EQ( 1.0f, b.y );
    b = a * 2.0f;
    EXPECT_FLOAT_EQ( 2.0f, b.x );
    EXPECT_FLOAT_EQ( 4.0f, b.y );
    b = a / 2.0f;
    EXPECT_FLOAT_EQ( 0.5f, b.x );
    EXPECT_FLOAT_EQ( 1.0f, b.y );
    b = a;
    b += 1.0f;
    EXPECT_FLOAT_EQ( 2.0f, b.x );
    EXPECT_FLOAT_EQ( 3.0f, b.y );
    b = a;
    b -= 1.0f;
    EXPECT_FLOAT_EQ( 0.0f, b.x );
    EXPECT_FLOAT_EQ( 1.0f, b.y );
    b = a;
    b *= 2.0f;
    EXPECT_FLOAT_EQ( 2.0f, b.x );
    EXPECT_FLOAT_EQ( 4.0f, b.y );
    b = a;
    b /= 2.0f;
    EXPECT_FLOAT_EQ( 0.5f, b.x );
    EXPECT_FLOAT_EQ( 1.0f, b.y );
}

TEST( math, vector_2d_arithm_2 )
{
    const vec2f a(1.0f, 2.0f);
    const vec2f b(3.0f, 5.0f);
    vec2f c = a + b;
    EXPECT_FLOAT_EQ( 4.0f, c.x );
    EXPECT_FLOAT_EQ( 7.0f, c.y );
    c = a - b;
    EXPECT_FLOAT_EQ( -2.0f, c.x );
    EXPECT_FLOAT_EQ( -3.0f, c.y );
    c = a * b;
    EXPECT_FLOAT_EQ( 3.0f, c.x );
    EXPECT_FLOAT_EQ( 10.0f, c.y );
    c = a / b;
    EXPECT_FLOAT_EQ( 1.0f / 3.0f, c.x );
    EXPECT_FLOAT_EQ( 2.0f / 5.0f, c.y );
    c = a;
    c += b;
    EXPECT_FLOAT_EQ( 4.0f, c.x );
    EXPECT_FLOAT_EQ( 7.0f, c.y );
    c = a;
    c -= b;
    EXPECT_FLOAT_EQ( -2.0f, c.x );
    EXPECT_FLOAT_EQ( -3.0f, c.y );
    c = a;
    c *= b;
    EXPECT_FLOAT_EQ( 3.0f, c.x );
    EXPECT_FLOAT_EQ( 10.0f, c.y );
    c = a;
    c /= b;
    EXPECT_FLOAT_EQ( 1.0f / 3.0f, c.x );
    EXPECT_FLOAT_EQ( 2.0f / 5.0f, c.y );
}

TEST( math, vector_2d_op)
{
    const vec2f a( 2.0f, 4.0f );
    EXPECT_FLOAT_EQ( 2.0f * 2.0f + 4.0f * 4.0f, length2( a ));
    EXPECT_FLOAT_EQ( sqrt( 2.0f * 2.0f + 4.0f * 4.0f ), length( a ));
    const vec2f na = normalize( a );
    EXPECT_FLOAT_EQ( 1.0f, length( na ) );
    const vec2f b = -a;
    EXPECT_FLOAT_EQ( -2.0f, b.x );
    EXPECT_FLOAT_EQ( -4.0f, b.y );
    const vec2f c( 3.0f, 7.0f );
    EXPECT_FLOAT_EQ( a.x * c.x + a.y * c.y, dot( a, c ));
}

TEST( math, vector_3d_init )
{
    vec3f a(3.0f, 4.0f, 5.0f);
    EXPECT_FLOAT_EQ(3.0f, a.x);
    EXPECT_FLOAT_EQ(4.0f, a.y);
    EXPECT_FLOAT_EQ(5.0f, a.z);
    vec3f b(2, 3, 4);
    EXPECT_FLOAT_EQ(2.0f, b.x);
    EXPECT_FLOAT_EQ(3.0f, b.y);
    EXPECT_FLOAT_EQ(4.0f, b.z);
    a = b;
    EXPECT_FLOAT_EQ(2.0f, a.x);
    EXPECT_FLOAT_EQ(3.0f, a.y);
    EXPECT_FLOAT_EQ(4.0f, a.z);
    vec3f c(a);
    EXPECT_FLOAT_EQ(2.0f, c.x);
    EXPECT_FLOAT_EQ(3.0f, c.y);
    EXPECT_FLOAT_EQ(4.0f, c.z);
    vec2f d(1.0f, 2.0f);
    vec3f e(d, 5.0f);
    EXPECT_FLOAT_EQ(1.0f, e.x);
    EXPECT_FLOAT_EQ(2.0f, e.y);
    EXPECT_FLOAT_EQ(5.0f, e.z);
}

TEST( math, vector_3d_arithm )
{
    const vec3f a(2,3,5);
    vec3f c = a + 2;
    EXPECT_FLOAT_EQ(4.0f, c.x);
    EXPECT_FLOAT_EQ(5.0f, c.y);
    EXPECT_FLOAT_EQ(7.0f, c.z);
    c = a - 2.0f;
    EXPECT_FLOAT_EQ(0.0f, c.x);
    EXPECT_FLOAT_EQ(1.0f, c.y);
    EXPECT_FLOAT_EQ(3.0f, c.z);
    c = a * 2.0f;
    EXPECT_FLOAT_EQ(4.0f, c.x);
    EXPECT_FLOAT_EQ(6.0f, c.y);
    EXPECT_FLOAT_EQ(10.0f, c.z);
    c = a / 2.0f;
    EXPECT_FLOAT_EQ(1.0f, c.x);
    EXPECT_FLOAT_EQ(1.5f, c.y);
    EXPECT_FLOAT_EQ(2.5f, c.z);
    c = a;
    c += 2.0f;
    EXPECT_FLOAT_EQ(4.0f, c.x);
    EXPECT_FLOAT_EQ(5.0f, c.y);
    EXPECT_FLOAT_EQ(7.0f, c.z);
    c = a;
    c -= 2.0f;
    EXPECT_FLOAT_EQ(0.0f, c.x);
    EXPECT_FLOAT_EQ(1.0f, c.y);
    EXPECT_FLOAT_EQ(3.0f, c.z);
    c = a;
    c *= 2.0f;
    EXPECT_FLOAT_EQ(4.0f, c.x);
    EXPECT_FLOAT_EQ(6.0f, c.y);
    EXPECT_FLOAT_EQ(10.0f, c.z);
    c = a;
    c /= 2.0f;
    EXPECT_FLOAT_EQ(1.0f, c.x);
    EXPECT_FLOAT_EQ(1.5f, c.y);
    EXPECT_FLOAT_EQ(2.5f, c.z);
}

TEST( math, vector_3d_arithm_2 )
{
    const vec3f a(1,2,3);
    const vec3f b(4,7,5);
    vec3f c = a + b;
    EXPECT_FLOAT_EQ(5.0f, c.x);
    EXPECT_FLOAT_EQ(9.0f, c.y);
    EXPECT_FLOAT_EQ(8.0f, c.z);
    c = a - b;
    EXPECT_FLOAT_EQ(-3.0f, c.x);
    EXPECT_FLOAT_EQ(-5.0f, c.y);
    EXPECT_FLOAT_EQ(-2.0f, c.z);
    c = a * b;
    EXPECT_FLOAT_EQ(4.0f, c.x);
    EXPECT_FLOAT_EQ(14.0f, c.y);
    EXPECT_FLOAT_EQ(15.0f, c.z);
    c = a / b;
    EXPECT_FLOAT_EQ(1.0f/4.0f, c.x);
    EXPECT_FLOAT_EQ(2.0f/7.0f, c.y);
    EXPECT_FLOAT_EQ(3.0f/5.0f, c.z);
    c = a;
    c += b;
    EXPECT_FLOAT_EQ(5.0f, c.x);
    EXPECT_FLOAT_EQ(9.0f, c.y);
    EXPECT_FLOAT_EQ(8.0f, c.z);
    c = a;
    c -= b;
    EXPECT_FLOAT_EQ(-3.0f, c.x);
    EXPECT_FLOAT_EQ(-5.0f, c.y);
    EXPECT_FLOAT_EQ(-2.0f, c.z);
    c = a;
    c *= b;
    EXPECT_FLOAT_EQ(4.0f, c.x);
    EXPECT_FLOAT_EQ(14.0f, c.y);
    EXPECT_FLOAT_EQ(15.0f, c.z);
    c = a;
    c /= b;
    EXPECT_FLOAT_EQ(1.0f/4.0f, c.x);
    EXPECT_FLOAT_EQ(2.0f/7.0f, c.y);
    EXPECT_FLOAT_EQ(3.0f/5.0f, c.z);
}

TEST( math, vector_3d_op )
{
    const vec3f a(3.0f, 4.0f, 7.0f);
    const vec3f b(2.0f, 5.0f, 6.0f);
    vec3f t = -a;
    EXPECT_FLOAT_EQ( -3.0f, t.x );
    EXPECT_FLOAT_EQ( -4.0f, t.y );
    EXPECT_FLOAT_EQ( -7.0f, t.z );
    EXPECT_FLOAT_EQ( 2.0f*3.0f + 4.0f*5.0f + 7.0f*6.0f, dot(a,b) );
    EXPECT_FLOAT_EQ( 2.0f*2.0f + 5.0f*5.0f + 6.0f*6.0f, length2(b) );
    vec3f na = normalize( a );
    EXPECT_FLOAT_EQ( 1.0f, length( na ) );
    const vec3f x(1,0,0);
    const vec3f y(0,1,0);
    const vec3f z(0,0,1);
    EXPECT_FLOAT_EQ(0.0f, dot(x, y));
    EXPECT_FLOAT_EQ(0.0f, dot(y, z));
    EXPECT_FLOAT_EQ(0.0f, dot(x, z));
    vec3f n = cross(x, y);
    EXPECT_FLOAT_EQ(z.x, n.x);
    EXPECT_FLOAT_EQ(z.y, n.y);
    EXPECT_FLOAT_EQ(z.z, n.z);
    n = cross(y,x);
    EXPECT_FLOAT_EQ(-z.x, n.x);
    EXPECT_FLOAT_EQ(-z.y, n.y);
    EXPECT_FLOAT_EQ(-z.z, n.z);
}
