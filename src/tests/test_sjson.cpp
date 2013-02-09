#include "base/sjson.h"
#include "gtest/gtest.h"

using namespace base;

TEST( sjson, simple )
{
    sjson::Variant root;
    bool result = sjson::parse("property1 = 123, property2 = \"abc\"", root);
    EXPECT_TRUE(result);
    EXPECT_TRUE(root.isMap());
    EXPECT_TRUE(root.hasMember("property1"));
    EXPECT_TRUE(root.hasMember("property2"));
}

TEST( sjson, fail )
{
    sjson::Variant root;
    bool result = sjson::parse("property1 = 123, property2 = \"ab       ", root);
    EXPECT_FALSE(result);
}

void testSkipSpaces(const char* str, u32 pos)
{
    u32 index = 0;
    sjson::skipWhiteSpace(str, index);
    EXPECT_EQ(pos, index);
}

TEST( sjson, skipSpaces )
{
    testSkipSpaces("  a", 2);
    testSkipSpaces("\t a", 2);
    testSkipSpaces("\r\n\r\nbb", 4);
    testSkipSpaces("", 0);
    testSkipSpaces("afg", 0);
    testSkipSpaces("   ", 3);
}

TEST( sjson, spacesAndComments )
{
    testSkipSpaces(" // comment \n a", 14);
    testSkipSpaces(" /* comment */ a", 15);
    EXPECT_THROW({
        u32 index = 0;
        sjson::skipWhiteSpace(" /*  ", index);      
    }, sjson::ParseException);
}

TEST( sjson, bounds )
{
    EXPECT_THROW({
        sjson::checkBounds("abc", 5);
    }, sjson::ParseException);
    EXPECT_THROW({
        sjson::checkBounds("abc", 3);
    }, sjson::ParseException);
    EXPECT_NO_THROW({
        sjson::checkBounds("abc", 0);
    });
    EXPECT_NO_THROW({
        sjson::checkBounds("abc", 2);
    });
}

void testSkipComment(const char* str, u32 pos)
{
    u32 index = 0;
    sjson::skipComment(str, index);
    EXPECT_EQ(pos, index);
}

TEST( sjson, comments )
{
    testSkipComment("// a\n", 5);
    testSkipComment("//\n//\na", 3);
    testSkipComment("//dd", 4);
    testSkipComment("/**/", 4);
    testSkipComment("/*\n\n\n*/a", 7);
    testSkipComment("/*a*//*b*/tt", 5);
    EXPECT_THROW({
        u32 index=0;
        sjson::skipComment("    /*", index);
    }, sjson::ParseException);
    EXPECT_THROW({
        u32 index=0;
        sjson::skipComment("/*", index);
    }, sjson::ParseException);
    EXPECT_THROW({
        u32 index=0;
        sjson::skipComment("/s  ", index);
    }, sjson::ParseException);
}

void testConsume(const char* str, const char* check)
{
    u32 index = 0;
    sjson::consume(str, index, check);
    EXPECT_EQ(strlen(check), index);
}

TEST( sjson, consume )
{
    testConsume("test string", "test");
    testConsume("true", "true");
    EXPECT_THROW({
        u32 index = 0;
        sjson::consume("test", index, "testME");
    }, sjson::ParseException);
    EXPECT_THROW({
        u32 index = 0;
        sjson::consume("testHIM", index, "testME");
    }, sjson::ParseException);
    EXPECT_THROW({
        u32 index = 0;
        sjson::consume("test", index, "false");
    }, sjson::ParseException);
}

TEST( sjson, nextChar )
{
    u32 pos = 0;
    EXPECT_EQ('t', sjson::next(" /* */ test", pos));
    pos = 0;
    EXPECT_EQ('t', sjson::next(" /* */ /* */ test", pos));
    pos = 0;
    EXPECT_EQ('t', sjson::next("test", pos));
    EXPECT_THROW({
        u32 index = 0;
        sjson::next(" // \n", index);
    }, sjson::ParseException);
}

TEST( sjson, string )
{
    u32 pos = 0;
    sjson::Variant v = sjson::parseString("\"this is a string\" something", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_STREQ("this is a string", v.asString().c_str());
    EXPECT_EQ(18, pos);
    pos = 0;
    v = sjson::parseString("\"\" empty", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_EQ(0, v.asString().size());
    EXPECT_EQ(2, pos);
    pos = 0;
    v = sjson::parseString("\"\\n\\r\" empty", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_STREQ("\n\r", v.asString().c_str());
    EXPECT_EQ(6, pos);
    EXPECT_THROW({
        u32 index = 0;
        sjson::parseString("asffa", index);
    }, sjson::ParseException);
    EXPECT_THROW({
        u32 index = 0;
        sjson::parseString("\"asffa", index);
    }, sjson::ParseException);
    EXPECT_THROW({
        u32 index = 0;
        sjson::parseString("\\easffa", index);
    }, sjson::ParseException);
}

TEST( sjson, identifier )
{
    u32 pos = 0;
    sjson::Variant v = sjson::parseIdentifier("theValue123_aa ", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_STREQ("theValue123_aa", v.asString().c_str());
    EXPECT_EQ(14, pos);
    pos = 0;
    v = sjson::parseIdentifier("theValue", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_STREQ("theValue", v.asString().c_str());
    EXPECT_EQ(8, pos);
    pos = 0;
    v = sjson::parseIdentifier("theValue=123 ", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_STREQ("theValue", v.asString().c_str());
    EXPECT_EQ(8, pos);
    pos = 0;
    v = sjson::parseIdentifier("\"the value\"=123 ", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_STREQ("the value", v.asString().c_str());
    EXPECT_EQ(11, pos);
    EXPECT_THROW({
        u32 index = 0;
        sjson::parseIdentifier("=123", index);
    }, sjson::ParseException);
}

TEST( sjson, parseNum )
{
    u32 pos = 0;
    sjson::Variant v = sjson::parseNumber("-125", pos);
    ASSERT_TRUE(v.isInt());
    ASSERT_FALSE(v.isFloat());
    EXPECT_EQ(-125, v.asInt<i32>());
    EXPECT_EQ(4, pos);
    pos = 0;
    v = sjson::parseNumber("-12.5", pos);
    ASSERT_TRUE(v.isFloat());
    ASSERT_FALSE(v.isInt());
    EXPECT_FLOAT_EQ(-12.5, v.asFloat<f32>());
    EXPECT_EQ(5, pos);
}

TEST( sjson, parseValue )
{
    u32 pos = 0;
    sjson::Variant v = sjson::parseValue("true,", pos);
    ASSERT_TRUE(v.isBool());
    EXPECT_EQ(true, v.asBool());
    EXPECT_EQ(4, pos);
    pos = 0;
    v = sjson::parseValue("false", pos);
    ASSERT_TRUE(v.isBool());
    EXPECT_EQ(false, v.asBool());
    EXPECT_EQ(5, pos);
    pos = 0;
    v = sjson::parseValue("     false", pos);
    ASSERT_TRUE(v.isBool());
    EXPECT_EQ(false, v.asBool());
    EXPECT_EQ(10, pos);
    pos = 0;
    v = sjson::parseValue("null", pos);
    ASSERT_TRUE(v.isNull());
    EXPECT_EQ(4, pos);
    pos = 0;
    v = sjson::parseValue("  \"this is a string\" something", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_STREQ("this is a string", v.asString().c_str());
    EXPECT_EQ(20, pos);
    pos = 0;
    v = sjson::parseValue("  { prop1 = 123 }", pos);
    EXPECT_TRUE(v.isMap());
    pos = 0;
    v = sjson::parseValue("  [ 123 ]", pos);
    EXPECT_TRUE(v.isArray());
    pos = 0;
    v = sjson::parseValue("  -12.5", pos);
    ASSERT_TRUE(v.isFloat());
    ASSERT_FALSE(v.isInt());
    EXPECT_FLOAT_EQ(-12.5, v.asFloat<f32>());

    EXPECT_THROW({
        u32 index = 0;
        sjson::parseValue("", index);
    }, sjson::ParseException);
    EXPECT_THROW({
        u32 index = 0;
        sjson::parseValue("  asdf", index);
    }, sjson::ParseException);
}

TEST( sjson, parseObject )
{
    u32 pos = 0;
    sjson::Variant v = sjson::parseObject("{}", pos);
    ASSERT_TRUE(v.isMap());
    ASSERT_EQ(0, v.asMap().size());
    EXPECT_EQ(2, pos);
    pos = 0;
    v = sjson::parseObject("{ a = 123, b = true, c = null }", pos);
    ASSERT_TRUE(v.isMap());
    ASSERT_TRUE(v.hasMember("a"));
    ASSERT_TRUE(v.hasMember("b"));
    ASSERT_TRUE(v.hasMember("c"));
    ASSERT_FALSE(v.hasMember("f"));
    ASSERT_TRUE(v["a"].isInt());
    ASSERT_TRUE(v["b"].isBool());
    ASSERT_TRUE(v["c"].isNull());
    pos = 0;
    v = sjson::parseObject("{ a = 123 b = true c = null }", pos);
    ASSERT_TRUE(v.isMap());
    ASSERT_TRUE(v.hasMember("a"));
    ASSERT_TRUE(v.hasMember("b"));
    ASSERT_TRUE(v.hasMember("c"));
    ASSERT_TRUE(v["a"].isInt());
    ASSERT_TRUE(v["b"].isBool());
    ASSERT_TRUE(v["c"].isNull());
    pos = 0;
    v = sjson::parseObject("{a=123b=truec=null}", pos);
    ASSERT_TRUE(v.isMap());
    ASSERT_TRUE(v.hasMember("a"));
    ASSERT_TRUE(v.hasMember("b"));
    ASSERT_TRUE(v.hasMember("c"));
    ASSERT_TRUE(v["a"].isInt());
    ASSERT_TRUE(v["b"].isBool());
    ASSERT_TRUE(v["c"].isNull());
    pos = 0;
    v = sjson::parseObject("{a=123,b=true,c=null}", pos);
    ASSERT_TRUE(v.isMap());
    ASSERT_TRUE(v.hasMember("a"));
    ASSERT_TRUE(v.hasMember("b"));
    ASSERT_TRUE(v.hasMember("c"));
    ASSERT_TRUE(v["a"].isInt());
    ASSERT_TRUE(v["b"].isBool());
    ASSERT_TRUE(v["c"].isNull());
    pos = 0;
    v = sjson::parseObject("{a={b=123},b={c=123}}", pos);
    ASSERT_TRUE(v.isMap());
    ASSERT_TRUE(v.hasMember("a"));
    ASSERT_TRUE(v.hasMember("b"));
    sjson::Variant a = v["a"];
    ASSERT_TRUE(a.isMap());
    ASSERT_TRUE(a.hasMember("b"));
    ASSERT_TRUE(a["b"].isInt());
    a = v["b"];
    ASSERT_TRUE(a.isMap());
    ASSERT_TRUE(a.hasMember("c"));
    ASSERT_TRUE(a["c"].isInt());
}

TEST( sjson, array )
{
    u32 pos = 0;
    sjson::Variant v = sjson::parseArray("[]", pos);
    ASSERT_TRUE(v.isArray());
    EXPECT_EQ(0, v.size());
    pos = 0;
    v = sjson::parseArray("[1,2,3,4,5,123]", pos);
    ASSERT_TRUE(v.isArray());
    EXPECT_EQ(6, v.size());
    EXPECT_EQ(1, v[0].asInt<i32>());
    EXPECT_EQ(2, v[1].asInt<i32>());
    EXPECT_EQ(3, v[2].asInt<i32>());
    EXPECT_EQ(4, v[3].asInt<i32>());
    EXPECT_EQ(5, v[4].asInt<i32>());
    EXPECT_EQ(123, v[5].asInt<i32>());
    pos = 0;
    v = sjson::parseArray("[1 2 3 4 5 123]", pos);
    ASSERT_TRUE(v.isArray());
    EXPECT_EQ(6, v.size());
    pos = 0;
    v = sjson::parseArray("[{a=123} {a=234}]", pos);
    ASSERT_TRUE(v.isArray());
    EXPECT_EQ(2, v.size());
    ASSERT_TRUE(v[0].isMap());
    ASSERT_TRUE(v[1].isMap());
    EXPECT_EQ(123, v[0]["a"].asInt<i32>());
    EXPECT_EQ(234, v[1]["a"].asInt<i32>());

}
