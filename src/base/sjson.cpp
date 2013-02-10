#include "base/sjson.h"
#include "base/sjson-internal.h"
#include <sstream>

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

ParseException::ParseException(const std::string& reason)
    : text(reason)
{
}

void skipWhiteSpace(std::istream* json)
{
    while(json->good()) {
        char ch = (char)json->peek();
        if (!json->good() || ch == EOF)
            break;
        if (ch == '/')
            skipComment(json);
        else if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r')
            json->get();
        else
            break;
    }
}

void checkBounds(std::istream* json) throw(ParseException)
{
    int res = json->peek();
    if (!json->good() || res == EOF)
        throw ParseException("checkBounds fails");
}

void skipComment(std::istream* json) throw(ParseException)
{
    consume(json, "/");
    checkBounds(json);
    
    char ch = (char)json->get();
    if (ch == '/') {
        while(json->good()) {
            if ((char)json->get() == '\n') {
                return;
            }
        }
    } else if (ch == '*') {
        while(json->good())
            if ((char)json->get() == '*' && (char)json->get() == '/') {
                return;
            }
        checkBounds(json);
    } else {
        throw ParseException("incorrect comment");
    }
}

void consume(std::istream* json, const std::string& str) throw(ParseException)
{
    for(u32 i=0; i<str.length(); i++) {
        checkBounds(json);
        char ch = (char)json->peek();
        if (ch != str[i])
            throw ParseException("wrong format, consumed: " + str);
        json->get();
    }
}

char next(std::istream* json) throw(ParseException)
{
    skipWhiteSpace(json);
    checkBounds(json);
    return (char)json->peek();
}

Variant parseString(std::istream* json) throw(ParseException)
{
    consume(json, "\"");
    std::string ret;
    while(json->good()) {
        checkBounds(json);
        char ch = (char)json->peek();
        if (ch == '"')
            break;
        json->get();
        if (ch == '\\')
        {
            checkBounds(json);
            ch = (char)json->get();
            if (ch == '"' || ch == '\\' || ch == '/')
                ret.push_back(ch);
            else if (ch == 't') ret.push_back('\t');
            else if (ch == 'n') ret.push_back('\n');
            else if (ch == 'r') ret.push_back('\r');
            else throw ParseException("wrong string escaping");
        }
        else
        {
            ret.push_back(ch);
        }
    }
    consume(json, "\"");
    return Variant(ret);
}

Variant parseIdentifier(std::istream* json) throw(ParseException)
{
    char ch = next(json);
    if (ch == '"')
        return parseString(json);
    std::string ret;
    while(json->good())
    {
        char ch = (char)json->peek();
        if (ch == EOF)
            break;
        if (isIdentifier(ch))
            ret.push_back(ch);
        else
            break;
        json->get();
    }
    if (ret.empty())
        throw ParseException("empty identifier");
    return Variant(ret);
}

bool isIdentifier(char ch)
{
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_');
}

Variant parseNumber(std::istream* json) throw(ParseException)
{
    const std::string chars("0123456789+-.eE");
    std::string ret;
    while(json->good()) {
        char ch = (char)json->peek();
        if (!json->good() || ch == EOF)
            break;
        if (chars.find(ch) == std::string::npos)
            break;
        ret.push_back(ch);
        json->get();
    }
    if (ret.find('.') != std::string::npos ||
        ret.find('e') != std::string::npos ||
        ret.find('E') != std::string::npos)
    {
        return Variant(atof(ret.c_str()));
    }
    return Variant((i64)atol(ret.c_str()));
}

Variant parseValue(std::istream* json) throw(ParseException)
{
    char ch = next(json);
    if (ch == '"')
        return parseString(json);
    if (ch == 't') {
        consume(json, "true");
        return Variant(true);
    }
    if (ch == 'f') {
        consume(json, "false");
        return Variant(false);
    }
    if (ch == 'n') {
        consume(json, "null");
        return Variant(typeNull);
    }
    if (ch == '{')
        return parseObject(json);
    if (ch == '[')
        return parseArray(json);
    if ((ch >= '0' && ch <= '9') || ch == '-')
        return parseNumber(json);
    throw ParseException("unknown value");
}

Variant parseObject(std::istream* json) throw(ParseException)
{
    consume(json, "{");
    Variant obj(typeDict);
    while(next(json) != '}')
    {
        std::string key = parseIdentifier(json).asString();
        if (obj.hasMember(key))
            throw ParseException("duplicate key " + key);
        skipWhiteSpace(json);
        consume(json, "=");
        skipWhiteSpace(json);
        Variant value = parseValue(json);
        obj[key] = value;
        if (next(json) == ',')
            json->get();
    }
    consume(json, "}");
    return obj;
}

Variant parseArray(std::istream* json) throw(ParseException)
{
    consume(json, "[");
    Variant arr(typeArray);
    while(next(json) != ']')
    {
        Variant value = parseValue(json);
        arr.asArray().push_back(value);
        if (next(json) == ',')
            json->get();
    }
    consume(json, "]");
    return arr;
}

Variant parseRoot(std::istream* json) throw(ParseException)
{
    Variant obj(typeDict);
    while(json->good())
    {
        skipWhiteSpace(json);
        std::string key = parseIdentifier(json).asString();
        skipWhiteSpace(json);
        consume(json, "=");
        skipWhiteSpace(json);
        Variant value = parseValue(json);
        obj[key] = value;
        skipWhiteSpace(json);
        if ((char)json->peek() == ',') {
            json->get();
        } else if (json->peek() == EOF)
            break;
    }
    return obj;
}

bool parse(const std::string& json, Variant& obj)
{
    std::istringstream ss(json);
    try
    {
        obj = parseRoot(&ss);
        return true;
    }
    catch(ParseException& e)
    {
        ERR("%s: %d", e.what(), ss.tellg());
        return false;
    }
}

void writeTabs(std::ostream* json, u32 tabs)
{
    for (u32 t=0; t<tabs; t++) *json << '\t';
}

void writeString(std::ostream* json, const std::string& str)
{
    *json << '"';
    for(u32 i=0; i<str.length(); i++)
    {
        char ch = str[i];
        if (ch == '\t') *json << "\\t";
        else if (ch == '\n') *json << "\\n";
        else if (ch == '\r') *json << "\\r";
        else if (ch == '"') *json << "\\\"";
        else *json << ch;
    }
    *json << '"';
}

void writeIdentifier(std::ostream* json, const std::string& str)
{
    for (u32 i=0; i<str.length(); i++)
        if (!isIdentifier(str[i]))
        {
            writeString(json, str);
            return;
        }
    for(u32 i=0; i<str.length(); i++)
        *json << str[i];
}

void writeArray(std::ostream* json, const Variant::Array& val, u32 tabs)
{
    *json << '[';
    if (val.size() > 0)
    {
        *json << '\n';
        for(u32 i=0; i<val.size(); i++)
        {
            writeTabs(json, tabs);
            writeValue(json, val[i], tabs + 1);
            *json << '\n';
        }
        writeTabs(json, tabs-1);
    }
    *json << ']';
}

void writeMap(std::ostream* json, const Variant::Map& val, u32 tabs)
{
    for(Variant::Map::const_iterator it=val.begin(); it!=val.end(); ++it)
    {
        writeTabs(json, tabs);
        writeIdentifier(json, it->first);
        *json << " = ";
        writeValue(json, it->second, tabs + 1);
        *json << '\n';
    }
}

void writeObject(std::ostream* json, const Variant::Map& val, u32 tabs)
{
    *json << '{';
    if (val.size() > 0)
    {
        *json << '\n';
        writeMap(json, val, tabs);
        writeTabs(json, tabs-1);
    }
    *json << '}';
}

void writeValue(std::ostream* json, const Variant& v, u32 tabs)
{
    if (v.isString())
        writeString(json, v.asString());
    else if (v.isBool())
        *json << (v.asBool() ? "true" : "false");
    else if (v.isInt())
        *json << v.asInt<i64>();
    else if (v.isFloat())
        *json << v.asFloat<f64>();
    else if (v.isNull())
        *json << "null";
    else if (v.isArray())
        writeArray(json, v.asArray(), tabs);
    else if (v.isMap())
        writeObject(json, v.asMap(), tabs);
}

std::string write(const Variant& v)
{
    std::ostringstream ss;
    writeMap(&ss, v.asMap(), 0);
    return ss.str();
}

} // namespace sjson
} // namespace base
