/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "gtest/gtest.h"
#include "math/vector-inl.h"

using base::math::Vector2;

const Vector2 v1( 0.f, 2.f );
const Vector2 v2( 1.f, 3.f );

TEST( math, vector2d_sum )
{
    Vector2 v3 = v1 + v2;
    EXPECT_FLOAT_EQ( v3.x, 1.f );
    EXPECT_FLOAT_EQ( v3.y, 5.f );
}

TEST( math, vector2d_mult )
{
    Vector2 v4 = v1 * v2;
    EXPECT_FLOAT_EQ( v4.x, 0.f );
    EXPECT_FLOAT_EQ( v4.y, 6.f );
}

TEST( math, vector2d_unary_minus )
{
    Vector2 v5 = -v1;
    EXPECT_FLOAT_EQ( v5.x, 0.f );
    EXPECT_FLOAT_EQ( v5.y, -2.f );
}

TEST( math, vector2d_scalar )
{
    float res = Dot( v1, v2 );
    EXPECT_FLOAT_EQ( res, 6.f );
}
