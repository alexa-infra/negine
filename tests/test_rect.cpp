/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#include "gtest/gtest.h"
#include "base/math/rect.h"

TEST(math, rect)
{
    base::math::Rect r(0, 0, 640, 480);
    EXPECT_TRUE(r.Center().x == 320);
}
