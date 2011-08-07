/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GLPv3) – see LICENSE file for details
 **/
#include "gtest/gtest.h"

int main(int argc, char* argv[])
{
        ::testing::InitGoogleTest(&argc, argv);
        ::testing::GTEST_FLAG(print_time) = true; 
        return RUN_ALL_TESTS();
}
