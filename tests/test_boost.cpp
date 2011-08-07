/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GPLv3) – see LICENSE file for details
 **/
#include "gtest/gtest.h"
#include <iostream>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

TEST(boost, filesystem)
{
    file_size(__FILE__);
    EXPECT_TRUE(true);
}