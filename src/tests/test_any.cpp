#include "gtest/gtest.h"
#include "base/any.h"
#include <string>

using namespace base;

TEST(any, easy)
{
    any value(1);
    int v = any_cast<int>(value);
    EXPECT_EQ(1, v);

    any str(std::string("blah"));
    std::string str1 = any_cast<std::string>(str);
    EXPECT_STREQ("blah", str1.c_str());
}
