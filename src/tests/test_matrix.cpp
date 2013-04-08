/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "gtest/gtest.h"
#include "math/matrix-inl.h"
#include "math/vec3.h"
#include "math/vec4.h"

using base::math::Matrix4;
using base::math::vec4f;
using base::math::pi;

TEST( math, matrix4_1 )
{
    Matrix4 a( vec4f( 1.0f, 1.0f, 0.0f, 0.0f ),
               vec4f( 1.0f, 1.0f, 0.0f, 1.0f ),
               vec4f( 0.0f, 2.0f, 1.0f, 0.0f ),
               vec4f( 3.0f, 1.0f, 0.0f, 1.0f ) );
    Matrix4 b = a * Matrix4::Identity();
    EXPECT_TRUE( a == b );
}
/*TEST(math, matrix4)
{
    Matrix4 v;
    EXPECT_TRUE(v[0] == 1.);
}

TEST(math, matrix4_eq)
{
    Matrix4 v;
    v.Translate(vec3f(1.f, 2.f, 3.f));

    Matrix4 m;
    m.Translate(vec3f(1.f, 2.f, 3.f));

    EXPECT_TRUE(m == v);
}

TEST(math, matrix4_neq)
{
    Matrix4 v;
    v.Translate(vec3f(1.f, 2.f, 3.f));

    Matrix4 m;
    m.RotateX(pi);

    EXPECT_TRUE(m != v);
}

TEST(math, matrix4_transpose)
{
    Matrix4 m(1.0f, 1.0f, 1.0f, 1.0f,
              0.0f, 1.0f, 1.0f, 1.0f,
              0.0f, 0.0f, 1.0f, 1.0f,
              0.0f, 0.0f, 0.0f, 1.0f);
    Matrix4 v(1.0f, 0.0f, 0.0f, 0.0f,
              1.0f, 1.0f, 0.0f, 0.0f,
              1.0f, 1.0f, 1.0f, 0.0f,
              1.0f, 1.0f, 1.0f, 1.0f);
    m.Transpose();
    EXPECT_TRUE(m == v);
}

TEST(math, matrix4_invert)
{
    Matrix4 v;
    v.Translate(vec3f(0.f, 0.f, 10.f));
    v.RotateZ(0.5f);

    Matrix4 m = v;
    m.Invert();

    Matrix4 r = m * v;

    EXPECT_TRUE(r == Matrix4::Identity);
}

TEST(math, matrix4_translate)
{
    Matrix4 v;
    v.Translate(vec3f(1.f, 2.f, 3.f));

    EXPECT_FLOAT_EQ(v.array2d[0][3], 1.f);
    EXPECT_FLOAT_EQ(v.array2d[1][3], 2.f);
    EXPECT_FLOAT_EQ(v.array2d[2][3], 3.f);
}

TEST(math, matrix4_assign)
{
    Matrix4 v;
    v.Translate(vec3f(1.f, 2.f, 3.f));
    Matrix4 m;
    m = v;
    EXPECT_TRUE(m == v);
}

*/
