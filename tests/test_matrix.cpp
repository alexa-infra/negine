#include "gtest/gtest.h"
#include "base/matrix.h"

TEST(math, matrix4)
{
    base::math::Matrix4 v;
    EXPECT_TRUE(v.xx == 1.);
}
