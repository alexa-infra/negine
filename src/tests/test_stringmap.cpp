/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/
#include "gtest/gtest.h"
#include "base/stringmap.h"

using base::StringMap;

namespace TestEnums
{
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
StringMap<TestEnum, TestEnums::Count> map( strs );

TEST( enums, basic )
{
    std::string res;
    EXPECT_TRUE( map.to_string( TestEnums::Second, res ) );
    EXPECT_STREQ( res.c_str(), "Second" );
    TestEnum en;
    EXPECT_TRUE( map.from_string( "Fourth", en ) );
    EXPECT_EQ( en, TestEnums::Fourth );
}

TEST( enums, not_found )
{
    TestEnum en;
    EXPECT_FALSE( map.from_string( "bla-bla-bla", en ) );
}