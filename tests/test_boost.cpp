#include "gtest/gtest.h"
#include <iostream>
#include <boost/filesystem.hpp>

using namespace boost::filesystem;

TEST(boost, filesystem)
{
    file_size(__FILE__);
    EXPECT_TRUE(true);
}