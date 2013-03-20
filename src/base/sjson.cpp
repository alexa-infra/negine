#include "base/sjson.h"
#include "base/sjson-internal.h"
#include <sstream>
#include <fstream>
#include "base/log.h"

namespace base {
namespace sjson {

Variant::Variant() : type(typeNull)
{
}

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

Variant::Variant(bool val) : type(typeBool) {
    boolVal = std::make_shared<bool>(val);
}

Variant::Variant(i64 val) : type(typeInt) {
    intVal = std::make_shared<i64>(val);
}

Variant::Variant(f64 val) : type(typeFloat) {
    floatVal = std::make_shared<f64>(val);
}

Variant::Variant(const std::string& val) : type(typeString) {
    strVal = std::make_shared<std::string>(val);
}

bool Variant::isBool() const {
    return type == typeBool && boolVal;
}

bool Variant::isInt() const {
    return type == typeInt && intVal;
}

bool Variant::isFloat() const {
    return type == typeFloat && floatVal;
}

bool Variant::isString() const {
    return type == typeString && strVal;
}

bool Variant::isMap() const {
    return type == typeDict && mapVal;
}

bool Variant::isArray() const {
    return type == typeArray && arrVal;
}
bool Variant::isNull() const {
    return type == typeNull;
}

bool& Variant::asBool() const {
    ASSERT(isBool());
    return *boolVal;
}

std::string& Variant::asString() const {
    ASSERT(isString());
    return *strVal;
}

Variant::Array& Variant::asArray() const {
    ASSERT(isArray());
    return *arrVal;
}

Variant::Map& Variant::asMap() const {
    ASSERT(isMap());
    return *mapVal;
}

bool Variant::hasMember(const std::string& name) const {
    return asMap().count(name) == 1;
}

Variant& Variant::operator[](const std::string& name) {
    return asMap()[name];
}

const Variant& Variant::operator[](const std::string& name) const {
    const Map& mmap = asMap();
    Map::const_iterator found = mmap.find(name);
    if (found != mmap.end())
        return found->second;
    static Variant retNull(ValueType::typeNull);
    return retNull;
}

Variant& Variant::operator[](const int idx) {
    return asArray()[idx];
}

const Variant& Variant::operator[](const int idx) const {
    return asArray()[idx];
}

size_t Variant::size() const {
    ASSERT(isMap() || isArray());
    if (isMap()) return asMap().size();
    if (isArray()) return asArray().size();
    return 0;
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

Variant parseValueJSON(std::istream* json) throw(ParseException)
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
        return parseObjectJSON(json);
    if (ch == '[')
        return parseArrayJSON(json);
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

Variant parseObjectJSON(std::istream* json) throw(ParseException)
{
    consume(json, "{");
    Variant obj(typeDict);
    while(next(json) != '}')
    {
        if (obj.size() > 0) {
            consume(json, ",");
            skipWhiteSpace(json);
        }
        std::string key = parseString(json).asString();
        if (obj.hasMember(key))
            throw ParseException("duplicate key " + key);
        skipWhiteSpace(json);
        consume(json, ":");
        skipWhiteSpace(json);
        Variant value = parseValueJSON(json);
        obj[key] = value;
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

Variant parseArrayJSON(std::istream* json) throw(ParseException)
{
    consume(json, "[");
    Variant arr(typeArray);
    while(next(json) != ']')
    {
        if (arr.size() > 0) {
            consume(json, ",");
            skipWhiteSpace(json);
        }
        Variant value = parseValue(json);
        arr.asArray().push_back(value);
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

bool parse(std::istream* ss, Variant& obj, bool simplified)
{
    try
    {
        if (simplified)
        {
            obj = parseRoot(ss);
        }
        else
        {
            obj = parseValueJSON(ss);
            if (obj.isArray() || obj.isMap())
                return true;
            ERR("array or object is expected");
            return false;
        }
        return true;
    }
    catch(ParseException& e)
    {
        ERR("%s: %d", e.what(), ss->tellg());
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

void writeArrayJSON(std::ostream* json, const Variant::Array& val, u32 tabs)
{
    *json << '[';
    if (val.size() > 0)
    {
        *json << '\n';
        for(u32 i=0; i<val.size(); i++)
        {
            writeTabs(json, tabs);
            writeValueJSON(json, val[i], tabs + 1);
            if (i != val.size()-1)
                *json << ',';
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

void writeObjectJSON(std::ostream* json, const Variant::Map& val, u32 tabs)
{
    *json << '{';
    if (val.size() > 0)
    {
        *json << '\n';
        for(Variant::Map::const_iterator it=val.begin(); it!=val.end(); ++it)
        {
            writeTabs(json, tabs);
            writeString(json, it->first);
            *json << ": ";
            writeValueJSON(json, it->second, tabs + 1);
            *json << '\n';
        }
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

void writeValueJSON(std::ostream* json, const Variant& v, u32 tabs)
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
        writeArrayJSON(json, v.asArray(), tabs);
    else if (v.isMap())
        writeObjectJSON(json, v.asMap(), tabs);
}

void write(std::ostream* ss, const Variant& v, bool simplified)
{
    if (simplified)
    {
        writeMap(ss, v.asMap(), 0);
    }
    else
    {
        ASSERT(v.isMap() || v.isArray());
        if (v.isMap()) writeObjectJSON(ss, v.asMap(), 1);
        if (v.isArray()) writeArrayJSON(ss, v.asArray(), 1);
    }
}

bool parse(const std::string& json, Variant& obj, bool simplified)
{
    std::istringstream ss(json);
    return parse(&ss, obj, simplified);
}
    
std::string write(const Variant& v, bool simplified)
{
    std::ostringstream ss;
    write(&ss, v, simplified);
    return ss.str();
}

bool readFile(const std::string& filename, Variant& obj, bool simplified)
{
    std::ifstream ifs(filename.c_str());
    if (!ifs.is_open() || !ifs.good())
    {
        ERR("File '%s' does not exists", filename.c_str());
        return false;
    }
    return parse(&ifs, obj, simplified);
}

void writeFile(const std::string& filename, Variant& obj, bool simplified)
{
    std::ofstream ofs(filename.c_str());
    if (!ofs.is_open() || !ofs.good())
    {
        ERR("File '%s' could not be created", filename.c_str());
        return;
    }
    write(&ofs, obj, simplified);
}

} // namespace sjson
} // namespace base
