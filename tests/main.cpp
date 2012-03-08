/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://hg.alexadotlife.com/negine)
 * Licensed under LGPLv3 (or GPLv3) - see LICENSE file for details
 **/
#include "gtest/gtest.h"

int main(int argc, char* argv[])
{
        ::testing::InitGoogleTest(&argc, argv);
        ::testing::GTEST_FLAG(print_time) = true; 
        return RUN_ALL_TESTS();
}
