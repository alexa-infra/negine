#include "base/sjson.h"
#include "base/sjson-internal.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace base;

TEST( sjson, simple )
{
    sjson::Variant root;
    bool result = sjson::parse("property1 = 123, property2 = \"abc\" property3 = { a = 123, b = false, c = null, d = [ 1 2 3 4 5 ] e = [] f = {} }", root);
    ASSERT_TRUE(result);
    ASSERT_TRUE(root.isMap());
    EXPECT_TRUE(root.hasMember("property1"));
    EXPECT_TRUE(root.hasMember("property2"));

    std::string res = sjson::write(root);
    SUCCEED();
}

TEST( sjson, simpleJSON )
{
    sjson::Variant root;
    bool result = sjson::parse(" { \"property1\" : 123, \"property2\" : \"abc\", \"property3\" : { \"a\" : 123, \"b\" : false, \"c\" : null, \"d\" : [ 1, 2, 3, 4, 5 ], \"e\" : [], \"f\" : {} } }", root, false);
    ASSERT_TRUE(result);
    ASSERT_TRUE(root.isMap());
    EXPECT_TRUE(root.hasMember("property1"));
    EXPECT_TRUE(root.hasMember("property2"));

    std::string res = sjson::write(root);
    SUCCEED();
}

TEST( sjson, fail )
{
    sjson::Variant root;
    bool result = sjson::parse("property1 = 123, property2 = \"ab       ", root);
    EXPECT_FALSE(result);
}

bool testSkipSpaces(const char* str, i32& pos)
{
    std::istringstream ss(str);
    if (!sjson::skipWhiteSpace(&ss))
        return false;
    pos = static_cast<i32>(ss.tellg());
    if (pos == -1) pos = strlen(str);
    return true;
}

TEST( sjson, skipSpaces )
{
    i32 index;
    EXPECT_TRUE(testSkipSpaces("  a", index));
    EXPECT_EQ(2, index);
    EXPECT_TRUE(testSkipSpaces("\t a", index));
    EXPECT_EQ(2, index);
    EXPECT_TRUE(testSkipSpaces("\r\n\r\nbb", index));
    EXPECT_EQ(4, index);
    EXPECT_TRUE(testSkipSpaces("", index));
    EXPECT_EQ(0, index);
    EXPECT_TRUE(testSkipSpaces("afg", index));
    EXPECT_EQ(0, index);
    EXPECT_TRUE(testSkipSpaces("   ", index));
    EXPECT_EQ(3, index);
}

TEST( sjson, spacesAndComments )
{
    i32 index;
    EXPECT_TRUE(testSkipSpaces(" // comment \n a", index));
    EXPECT_EQ(14, index);
    EXPECT_TRUE(testSkipSpaces(" /* comment */ a", index));
    EXPECT_EQ(15, index);
    EXPECT_FALSE(testSkipSpaces(" /*  ", index));
}

bool testCheckBounds(const char* str)
{
    std::istringstream ss(str);
    return sjson::checkBounds(&ss);
}

TEST( sjson, bounds )
{
    EXPECT_FALSE(testCheckBounds(""));
    EXPECT_TRUE(testCheckBounds("abc"));
}

bool testSkipComment(const char* str, i32& pos)
{
    std::istringstream ss(str);
    if (!sjson::skipComment(&ss))
        return false;
    pos = static_cast<i32>(ss.tellg());
    if (pos == -1) pos = strlen(str);
    return true;
}

TEST( sjson, comments )
{
    i32 index;
    EXPECT_TRUE(testSkipComment("// a\n", index));
    EXPECT_EQ(5, index);
    EXPECT_TRUE(testSkipComment("//\n//\na", index));
    EXPECT_EQ(3, index);
    EXPECT_TRUE(testSkipComment("//dd", index));
    EXPECT_EQ(4, index);
    EXPECT_TRUE(testSkipComment("/**/", index));
    EXPECT_EQ(4, index);
    EXPECT_TRUE(testSkipComment("/*\n\n\n*/a", index));
    EXPECT_EQ(7, index);
    EXPECT_TRUE(testSkipComment("/*a*//*b*/tt", index));
    EXPECT_EQ(5, index);
    EXPECT_FALSE(testSkipComment("    /*", index));
    EXPECT_FALSE(testSkipComment("/*", index));
    EXPECT_FALSE(testSkipComment("/s  ", index));
}

bool testConsume(const char* str, const char* check, i32& index)
{
    std::istringstream ss(str);
    if (!sjson::consume(&ss, check))
        return false;
    index = static_cast<i32>(ss.tellg());
    if (index == -1) index = strlen(str);
    return true;
}

TEST( sjson, consume )
{
    i32 index = 0;
    EXPECT_TRUE(testConsume("test string", "test", index));
    EXPECT_EQ(4, index);
    EXPECT_TRUE(testConsume("true", "true", index));
    EXPECT_EQ(4, index);
    EXPECT_FALSE(testConsume("test", "testME", index));
    EXPECT_FALSE(testConsume("testHIM", "testME", index));
    EXPECT_FALSE(testConsume("test", "false", index));
}

bool testNextChar(const char* str, char& ret)
{
    std::istringstream ss(str);
    return sjson::next(&ss, ret);
}

TEST( sjson, nextChar )
{
    char ch;
    EXPECT_TRUE(testNextChar(" /* */ test", ch));
    EXPECT_EQ('t', ch);
    EXPECT_TRUE(testNextChar(" /* */ /* */ test", ch));
    EXPECT_EQ('t', ch);
    EXPECT_TRUE(testNextChar("test", ch));
    EXPECT_EQ('t', ch);
    EXPECT_FALSE(testNextChar(" // \n", ch));
}

sjson::Variant testParseString(const char* str, i32& pos)
{
    std::istringstream ss(str);
    sjson::Variant ret;
    if (!sjson::parseString(&ss, ret))
        return sjson::Variant(sjson::ValueType::typeNull);
    pos = static_cast<i32>(ss.tellg());
    if (pos == -1) pos = strlen(str);
    return ret;
}

TEST( sjson, string )
{
    i32 pos = 0;
    sjson::Variant v = testParseString("\"this is a string\" something", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_STREQ("this is a string", v.asString().c_str());
    EXPECT_EQ(18, pos);
    pos = 0;
    v = testParseString("\"\" empty", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_EQ(0, v.asString().size());
    EXPECT_EQ(2, pos);
    pos = 0;
    v = testParseString("\"\\n\\r\" empty", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_STREQ("\n\r", v.asString().c_str());
    EXPECT_EQ(6, pos);
    pos = 0;
    v = testParseString("asffa", pos);
    EXPECT_TRUE(v.isNull());
    pos = 0;
    v = testParseString("\"asffa", pos);
    EXPECT_TRUE(v.isNull());
    pos = 0;
    v = testParseString("\\easffa", pos);
    EXPECT_TRUE(v.isNull());
}

sjson::Variant testParseIdentifier(const char* str, i32& pos)
{
    std::istringstream ss(str);
    sjson::Variant ret;
    if (!sjson::parseIdentifier(&ss, ret))
        return sjson::Variant(sjson::ValueType::typeNull);
    pos = static_cast<i32>(ss.tellg());
    if (pos == -1) pos = strlen(str);
    return ret;
}

TEST( sjson, identifier )
{
    i32 pos = 0;
    sjson::Variant v = testParseIdentifier("theValue123_aa ", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_STREQ("theValue123_aa", v.asString().c_str());
    EXPECT_EQ(14, pos);
    pos = 0;
    v = testParseIdentifier("theValue", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_STREQ("theValue", v.asString().c_str());
    EXPECT_EQ(8, pos);
    pos = 0;
    v = testParseIdentifier("theValue=123 ", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_STREQ("theValue", v.asString().c_str());
    EXPECT_EQ(8, pos);
    pos = 0;
    v = testParseIdentifier("\"the value\"=123 ", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_STREQ("the value", v.asString().c_str());
    EXPECT_EQ(11, pos);
    pos = 0;
    v = testParseIdentifier("=123", pos);
    ASSERT_TRUE(v.isNull());
}

sjson::Variant testParseNumber(const char* str, i32& pos)
{
    std::istringstream ss(str);
    sjson::Variant ret;
    if (!sjson::parseNumber(&ss, ret))
        return sjson::Variant(sjson::ValueType::typeNull);
    pos = static_cast<i32>(ss.tellg());
    if (pos == -1) pos = strlen(str);
    return ret;
}

TEST( sjson, parseNum )
{
    i32 pos = 0;
    sjson::Variant v = testParseNumber("-125", pos);
    ASSERT_TRUE(v.isInt());
    ASSERT_FALSE(v.isFloat());
    EXPECT_EQ(-125, v.asInt<i32>());
    EXPECT_EQ(4, pos);
    pos = 0;
    v = testParseNumber("-12.5", pos);
    ASSERT_TRUE(v.isFloat());
    ASSERT_FALSE(v.isInt());
    EXPECT_FLOAT_EQ(-12.5, v.asFloat<f32>());
    EXPECT_EQ(5, pos);
}

sjson::Variant testParseValue(const char* str, i32& pos)
{
    std::istringstream ss(str);
    sjson::Variant ret;
    if (!sjson::parseValue(&ss, ret))
        return sjson::Variant(sjson::ValueType::typeNull);
    pos = static_cast<i32>(ss.tellg());
    if (pos == -1) pos = strlen(str);
    return ret;
}

TEST( sjson, parseValue )
{
    i32 pos = 0;
    sjson::Variant v = testParseValue("true,", pos);
    ASSERT_TRUE(v.isBool());
    EXPECT_TRUE(v.asBool());
    EXPECT_EQ(4, pos);
    pos = 0;
    v = testParseValue("false", pos);
    ASSERT_TRUE(v.isBool());
    EXPECT_FALSE(v.asBool());
    EXPECT_EQ(5, pos);
    pos = 0;
    v = testParseValue("     false", pos);
    ASSERT_TRUE(v.isBool());
    EXPECT_FALSE(v.asBool());
    EXPECT_EQ(10, pos);
    pos = 0;
    v = testParseValue("null", pos);
    ASSERT_TRUE(v.isNull());
    EXPECT_EQ(4, pos);
    pos = 0;
    v = testParseValue("  \"this is a string\" something", pos);
    ASSERT_TRUE(v.isString());
    EXPECT_STREQ("this is a string", v.asString().c_str());
    EXPECT_EQ(20, pos);
    pos = 0;
    v = testParseValue("  { prop1 = 123 }", pos);
    EXPECT_TRUE(v.isMap());
    pos = 0;
    v = testParseValue("  [ 123 ]", pos);
    EXPECT_TRUE(v.isArray());
    pos = 0;
    v = testParseValue("  -12.5", pos);
    ASSERT_TRUE(v.isFloat());
    ASSERT_FALSE(v.isInt());
    EXPECT_FLOAT_EQ(-12.5, v.asFloat<f32>());

    pos = 0;
    v = testParseValue("", pos);
    ASSERT_TRUE(v.isNull());
    pos = 0;
    v = testParseValue("  asdf", pos);
    ASSERT_TRUE(v.isNull());
}

sjson::Variant testParseObject(const char* str, i32& pos)
{
    std::istringstream ss(str);
    sjson::Variant ret;
    if (!sjson::parseObject(&ss, ret))
        return sjson::Variant(sjson::ValueType::typeNull);
    pos = static_cast<i32>(ss.tellg());
    if (pos == -1) pos = strlen(str);
    return ret;
}

TEST( sjson, parseObject )
{
    i32 pos = 0;
    sjson::Variant v = testParseObject("{}", pos);
    ASSERT_TRUE(v.isMap());
    ASSERT_EQ(0, v.asMap().size());
    EXPECT_EQ(2, pos);
    pos = 0;
    v = testParseObject("{ a = 123, b = true, c = null }", pos);
    ASSERT_TRUE(v.isMap());
    ASSERT_TRUE(v.hasMember("a"));
    ASSERT_TRUE(v.hasMember("b"));
    ASSERT_TRUE(v.hasMember("c"));
    ASSERT_FALSE(v.hasMember("f"));
    ASSERT_TRUE(v["a"].isInt());
    ASSERT_TRUE(v["b"].isBool());
    ASSERT_TRUE(v["c"].isNull());
    pos = 0;
    v = testParseObject("{ a = 123 b = true c = null }", pos);
    ASSERT_TRUE(v.isMap());
    ASSERT_TRUE(v.hasMember("a"));
    ASSERT_TRUE(v.hasMember("b"));
    ASSERT_TRUE(v.hasMember("c"));
    ASSERT_TRUE(v["a"].isInt());
    ASSERT_TRUE(v["b"].isBool());
    ASSERT_TRUE(v["c"].isNull());
    pos = 0;
    v = testParseObject("{a=123b=truec=null}", pos);
    ASSERT_TRUE(v.isMap());
    ASSERT_TRUE(v.hasMember("a"));
    ASSERT_TRUE(v.hasMember("b"));
    ASSERT_TRUE(v.hasMember("c"));
    ASSERT_TRUE(v["a"].isInt());
    ASSERT_TRUE(v["b"].isBool());
    ASSERT_TRUE(v["c"].isNull());
    pos = 0;
    v = testParseObject("{a=123,b=true,c=null}", pos);
    ASSERT_TRUE(v.isMap());
    ASSERT_TRUE(v.hasMember("a"));
    ASSERT_TRUE(v.hasMember("b"));
    ASSERT_TRUE(v.hasMember("c"));
    ASSERT_TRUE(v["a"].isInt());
    ASSERT_TRUE(v["b"].isBool());
    ASSERT_TRUE(v["c"].isNull());
    pos = 0;
    v = testParseObject("{a={b=123},b={c=123}}", pos);
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

sjson::Variant testParseArray(const char* str, i32& pos)
{
    std::istringstream ss(str);
    sjson::Variant ret;
    if (!sjson::parseArray(&ss, ret))
        return sjson::Variant(sjson::ValueType::typeNull);
    pos = static_cast<i32>(ss.tellg());
    if (pos == -1) pos = strlen(str);
    return ret;
}

TEST( sjson, array )
{
    i32 pos = 0;
    sjson::Variant v = testParseArray("[]", pos);
    ASSERT_TRUE(v.isArray());
    EXPECT_EQ(0, v.size());
    pos = 0;
    v = testParseArray("[1,2,3,4,5,123]", pos);
    ASSERT_TRUE(v.isArray());
    EXPECT_EQ(6, v.size());
    EXPECT_EQ(1, v[0].asInt<i32>());
    EXPECT_EQ(2, v[1].asInt<i32>());
    EXPECT_EQ(3, v[2].asInt<i32>());
    EXPECT_EQ(4, v[3].asInt<i32>());
    EXPECT_EQ(5, v[4].asInt<i32>());
    EXPECT_EQ(123, v[5].asInt<i32>());
    pos = 0;
    v = testParseArray("[1 2 3 4 5 123]", pos);
    ASSERT_TRUE(v.isArray());
    EXPECT_EQ(6, v.size());
    pos = 0;
    v = testParseArray("[{a=123} {a=234}]", pos);
    ASSERT_TRUE(v.isArray());
    EXPECT_EQ(2, v.size());
    ASSERT_TRUE(v[0].isMap());
    ASSERT_TRUE(v[1].isMap());
    EXPECT_EQ(123, v[0]["a"].asInt<i32>());
    EXPECT_EQ(234, v[1]["a"].asInt<i32>());

}
