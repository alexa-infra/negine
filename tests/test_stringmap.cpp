/**
 * @author  Alexey Vasilyev <alexa.infra@gmail.com>
 * @section LICENSE
 * 
 * This file is part of Negine (https://alexadotlife.com/hg/negine)
 * Licensed under LGPLv3 (or GLPv3) – see LICENSE file for details
 **/
#include "gtest/gtest.h"
#include "base/stringmap.h"

using namespace base;

namespace TestEnums {
    enum TestEnum {
        First, Second, Third, Fourth, Count
    };
}
typedef TestEnums::TestEnum TestEnum;

StringMap<TestEnum, TestEnums::Count>::Entry strs[TestEnums::Count] = {
    { "First",  TestEnums::First  },
    { "Second", TestEnums::Second },
    { "Third",  TestEnums::Third  },
    { "Fourth", TestEnums::Fourth }
};

TEST(enums, basic)
{
    StringMap<TestEnum, TestEnums::Count> map(strs);
    EXPECT_STREQ(map.to_string(TestEnums::Second), "Second");
    EXPECT_EQ(map.from_string("Fourth"), TestEnums::Fourth);
}