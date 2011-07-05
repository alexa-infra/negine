#include "gtest/gtest.h"
#include "base/math/matrix.h"

TEST(math, matrix4)
{
    base::math::Matrix4 v;
    EXPECT_TRUE(v.xx == 1.);
}

TEST(math, matrix4_transpose)
{
    base::math::Matrix4 m(1.0f, 1.0f, 1.0f, 1.0f,
                          0.0f, 1.0f, 1.0f, 1.0f,
                          0.0f, 0.0f, 1.0f, 1.0f,
                          0.0f, 0.0f, 0.0f, 1.0f);
    base::math::Matrix4 v(1.0f, 0.0f, 0.0f, 0.0f,
                          1.0f, 1.0f, 0.0f, 0.0f,
                          1.0f, 1.0f, 1.0f, 0.0f,
                          1.0f, 1.0f, 1.0f, 1.0f);
    m.Transpose();
    for (u32 i=0; i<16; i++)
        EXPECT_FLOAT_EQ(m.array1d[i], v.array1d[i]);
}

TEST(math, matrix4_invert)
{
    base::math::Matrix4 v;
    v.Translate(base::math::Vector3(0.f, 0.f, 10.f));
    v.RotateZ(0.5f);
    
    base::math::Matrix4 m = v;
    m.Invert();
    
    base::math::Matrix4 r = m * v;

    for (u32 i=0; i<16; i++)
        EXPECT_FLOAT_EQ(r.array1d[i], base::math::Matrix4::Identity.array1d[i]);
}