/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "gtest/gtest.h"
#include "math/vec2.h"

using base::math::vec2f;

const vec2f v1( 0.f, 2.f );
const vec2f v2( 1.f, 3.f );

TEST( math, vector2d_sum )
{
    vec2f v3 = v1 + v2;
    EXPECT_FLOAT_EQ( v3.x, 1.f );
    EXPECT_FLOAT_EQ( v3.y, 5.f );
}

TEST( math, vector2d_mult )
{
    vec2f v4 = v1 * v2;
    EXPECT_FLOAT_EQ( v4.x, 0.f );
    EXPECT_FLOAT_EQ( v4.y, 6.f );
}

TEST( math, vector2d_unary_minus )
{
    vec2f v5 = -v1;
    EXPECT_FLOAT_EQ( v5.x, 0.f );
    EXPECT_FLOAT_EQ( v5.y, -2.f );
}

TEST( math, vector2d_scalar )
{
    float res = dot( v1, v2 );
    EXPECT_FLOAT_EQ( res, 6.f );
}
