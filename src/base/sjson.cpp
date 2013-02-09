#include "base/sjson.h"

namespace base {
namespace sjson {

Variant::Variant(ValueType v)
    : type(v)
{
    switch(type) {
    case typeBool:      boolVal = std::make_shared<bool>();         break;
    case typeInt:       intVal = std::make_shared<i64>();           break;
    case typeFloat:     floatVal = std::make_shared<f64>();       break;
    case typeString:    strVal = std::make_shared<std::string>();   break;
    case typeDict:      mapVal = std::make_shared<Map>();           break;
    case typeArray:     arrVal = std::make_shared<Array>();         break;
    default: break;
    }
}

ParseException::ParseException(const std::string& reason, u32 pos)
    : position(pos)
    , text(reason)
{
}

void skipWhiteSpace(const std::string& json, u32& index)
{
    while(index < json.length()) {
        char ch = json[index];
        if (ch == '/')
            skipComment(json, index);
        else if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r')
            index++;
        else
            break;
    }
}

void checkBounds(const std::string& json, u32 index) throw(ParseException)
{
    if (index >= json.length())
        throw ParseException("checkBounds fails", index);
}

void skipComment(const std::string& json, u32& index) throw(ParseException)
{
    consume(json, index, "/");
    checkBounds(json, index);
    
    if (json[index] == '/') {
        while(++index < json.length())
            if (json[index] == '\n') {
                index++;
                return;
            }
    } else if (json[index] == '*') {
        while(++index + 1 < json.length())
            if (json[index] == '*' && json[index+1] == '/') {
                index += 2;
                return;
            }
        checkBounds(json, ++index);
    } else {
        WARN("Wrong comment char '%c' at %d in '%s'", json[index], index, json.c_str());
        throw ParseException("incorrect comment", index);
    }
}

void consume(const std::string& json, u32& index, const std::string& str) throw(ParseException)
{
    for(u32 i=0; i<str.length(); i++, index++) {
        checkBounds(json, index);
        if (json[index] != str[i])
            throw ParseException("wrong format, consumed: " + str, index);
    }
}

char next(const std::string& json, u32& index) throw(ParseException)
{
    skipWhiteSpace(json, index);
    checkBounds(json, index);
    return json[index];
}

Variant parseString(const std::string& json, u32& index) throw(ParseException)
{
    consume(json, index, "\"");
    std::string ret;
    while(index < json.length()) {
        char ch = json[index];
        if (ch == '"')
            break;
        if (ch == '\\')
        {
            checkBounds(json, ++index);
            ch = json[index];
            if (ch == '"' || ch == '\\' || ch == '/')
                ret.push_back(ch);
            else if (ch == 't') ret.push_back('\t');
            else if (ch == 'n') ret.push_back('\n');
            else if (ch == 'r') ret.push_back('\r');
            else throw ParseException("wrong string escaping", index);
        }
        else
        {
            ret.push_back(ch);
        }
        index++;
    }
    consume(json, index, "\"");
    return Variant(ret);
}

Variant parseIdentifier(const std::string& json, u32& index) throw(ParseException)
{
    char ch = next(json, index);
    if (ch == '"')
        return parseString(json, index);
    std::string ret;
    while(index < json.length())
    {
        char ch = json[index];
        if (isIdentifier(ch))
            ret.push_back(ch);
        else
            break;
        index++;
    }
    if (ret.empty())
        throw ParseException("empty identifier", index);
    return Variant(ret);
}

bool isIdentifier(char ch)
{
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_');
}

Variant parseNumber(const std::string& json, u32& index)throw(ParseException)
{
    const std::string chars("0123456789+-.eE");
    std::string ret;
    while(index < json.length()) {
        char ch = json[index];
        if (chars.find(ch) == std::string::npos)
            break;
        ret.push_back(ch);
        index++;
    }
    if (ret.find('.') != std::string::npos ||
        ret.find('e') != std::string::npos ||
        ret.find('E') != std::string::npos)
    {
        return Variant(atof(ret.c_str()));
    }
    return Variant((i64)atol(ret.c_str()));
}

Variant parseValue(const std::string& json, u32& index) throw(ParseException)
{
    char ch = next(json, index);
    if (ch == '"')
        return parseString(json, index);
    if (ch == 't') {
        consume(json, index, "true");
        return Variant(true);
    }
    if (ch == 'f') {
        consume(json, index, "false");
        return Variant(false);
    }
    if (ch == 'n') {
        consume(json, index, "null");
        return Variant(typeNull);
    }
    if (ch == '{')
        return parseObject(json, index);
    if (ch == '[')
        return parseArray(json, index);
    if ((ch >= '0' && ch <= '9') || ch == '-')
        return parseNumber(json, index);
    throw ParseException("unknown value", index);
}

Variant parseObject(const std::string& json, u32& index) throw(ParseException)
{
    consume(json, index, "{");
    Variant obj(typeDict);
    while(next(json, index) != '}')
    {
        std::string key = parseIdentifier(json, index).asString();
        if (obj.hasMember(key))
            throw ParseException("duplicate key " + key, index);
        skipWhiteSpace(json, index);
        consume(json, index, "=");
        skipWhiteSpace(json, index);
        Variant value = parseValue(json, index);
        obj[key] = value;
        if (next(json, index) == ',')
            index++;
    }
    consume(json, index, "}");
    return obj;
}

Variant parseArray(const std::string& json, u32& index) throw(ParseException)
{
    consume(json, index, "[");
    Variant arr(typeArray);
    while(next(json, index) != ']')
    {
        Variant value = parseValue(json, index);
        arr.asArray().push_back(value);
        if (next(json, index) == ',')
            index++;
    }
    consume(json, index, "]");
    return arr;
}

Variant parseRoot(const std::string& json) throw(ParseException)
{
    Variant obj(typeDict);
    u32 index = 0;
    while(index < json.length())
    {
        skipWhiteSpace(json, index);
        std::string key = parseIdentifier(json, index).asString();
        skipWhiteSpace(json, index);
        consume(json, index, "=");
        skipWhiteSpace(json, index);
        Variant value = parseValue(json, index);
        obj[key] = value;
        skipWhiteSpace(json, index);
        if (json[index] == ',') {
            index++;
            skipWhiteSpace(json, index);
        }
    }
    return obj;
}

bool parse(const std::string& json, Variant& obj)
{
    try
    {
        obj = parseRoot(json);
        return true;
    }
    catch(ParseException& e)
    {
        ERR("%s: %d", e.what(), e.position);
        return false;
    }
}

} // namespace sjson
} // namespace base
