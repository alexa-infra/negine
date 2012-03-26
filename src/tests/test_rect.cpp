/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "gtest/gtest.h"
#include "base/math/rect.h"

TEST(math, rect)
{
    base::math::Rect r(0, 0, 640, 480);
    EXPECT_TRUE(r.Center().x == 320);
}
