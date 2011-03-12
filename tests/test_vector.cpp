#include "gtest/gtest.h"
#include "base/math/vector.h"

TEST(math, vector2d)
{
    base::math::Vector2 v;
    EXPECT_TRUE(v.x == 0.);
}
