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

#define CHECK_FALSE(x) if(!x) return false

bool skipWhiteSpace(std::istream* json)
{
    while(json->good()) {
        char ch = (char)json->peek();
        if (!json->good() || ch == EOF)
            break;
        if (ch == '/') {
            CHECK_FALSE(skipComment(json));
        } else if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r')
            json->get();
        else
            break;
    }
    return true;
}

bool checkBounds(std::istream* json)
{
    int res = json->peek();
    if (!json->good() || res == EOF) {
        ERR("check bounds fails");
        return false;
    }
    return true;
}

bool skipComment(std::istream* json)
{
    CHECK_FALSE(consume(json, "/"));
    CHECK_FALSE(checkBounds(json));
    
    char ch = (char)json->get();
    if (ch == '/') {
        while(json->good()) {
            if ((char)json->get() == '\n') {
                return true;
            }
        }
    } else if (ch == '*') {
        while(json->good())
            if ((char)json->get() == '*' && (char)json->get() == '/') {
                return true;
            }
        CHECK_FALSE(checkBounds(json));
    } else {
        ERR("incorrect comment");
        return false;
    }
    return true;
}

bool consume(std::istream* json, const std::string& str)
{
    for(u32 i=0; i<str.length(); i++) {
        CHECK_FALSE(checkBounds(json));
        char ch = (char)json->peek();
        if (ch != str[i]) {
            ERR("wrong format, consumed: %s", str.c_str());
            return false;
        }
        json->get();
    }
    return true;
}

bool next(std::istream* json, char& ch)
{
    CHECK_FALSE(skipWhiteSpace(json));
    CHECK_FALSE(checkBounds(json));
    ch = (char)json->peek();
    return true;
}

bool parseString(std::istream* json, Variant& result)
{
    CHECK_FALSE(consume(json, "\""));
    std::string ret;
    while(json->good()) {
        CHECK_FALSE(checkBounds(json));
        char ch = (char)json->peek();
        if (ch == '"')
            break;
        json->get();
        if (ch == '\\')
        {
            CHECK_FALSE(checkBounds(json));
            ch = (char)json->get();
            if (ch == '"' || ch == '\\' || ch == '/')
                ret.push_back(ch);
            else if (ch == 't') ret.push_back('\t');
            else if (ch == 'n') ret.push_back('\n');
            else if (ch == 'r') ret.push_back('\r');
            else {
                ERR("wrong string escaping");
                return false;
            }
        }
        else
        {
            ret.push_back(ch);
        }
    }
    CHECK_FALSE(consume(json, "\""));
    result = Variant(ret);
    return true;
}

bool parseIdentifier(std::istream* json, Variant& result)
{
    char ch;
    CHECK_FALSE(next(json, ch));
    if (ch == '"') {
        CHECK_FALSE(parseString(json, result));
        return true;
    }
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
    if (ret.empty()) {
        ERR("empty identifier");
        return false;
    }
    result = Variant(ret);
    return true;
}

bool isIdentifier(char ch)
{
    return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_');
}

bool parseNumber(std::istream* json, Variant& result)
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
        result = Variant(atof(ret.c_str()));
        return true;
    }
    result = Variant((i64)atol(ret.c_str()));
    return true;
}

bool parseValue(std::istream* json, Variant& result)
{
    char ch; 
    CHECK_FALSE(next(json, ch));
    if (ch == '"')
        return parseString(json, result);
    if (ch == 't') {
        CHECK_FALSE(consume(json, "true"));
        result = Variant(true);
        return true;
    }
    if (ch == 'f') {
        CHECK_FALSE(consume(json, "false"));
        result = Variant(false);
        return true;
    }
    if (ch == 'n') {
        CHECK_FALSE(consume(json, "null"));
        result = Variant(typeNull);
        return true;
    }
    if (ch == '{')
        return parseObject(json, result);
    if (ch == '[')
        return parseArray(json, result);
    if ((ch >= '0' && ch <= '9') || ch == '-')
        return parseNumber(json, result);
    ERR("unknown value");
    return false;
}

bool parseValueJSON(std::istream* json, Variant& result)
{
    char ch; 
    CHECK_FALSE(next(json, ch));
    if (ch == '"')
        return parseString(json, result);
    if (ch == 't') {
        CHECK_FALSE(consume(json, "true"));
        result = Variant(true);
        return true;
    }
    if (ch == 'f') {
        CHECK_FALSE(consume(json, "false"));
        result = Variant(false);
        return true;
    }
    if (ch == 'n') {
        CHECK_FALSE(consume(json, "null"));
        result = Variant(typeNull);
        return true;
    }
    if (ch == '{')
        return parseObjectJSON(json, result);
    if (ch == '[')
        return parseArrayJSON(json, result);
    if ((ch >= '0' && ch <= '9') || ch == '-')
        return parseNumber(json, result);
    ERR("unknown value");
    return false;
}

bool parseObject(std::istream* json, Variant& result)
{
    CHECK_FALSE(consume(json, "{"));
    Variant obj(typeDict);
    char ch;
    while(next(json, ch) && ch != '}')
    {
        Variant keyVar;
        CHECK_FALSE(parseIdentifier(json, keyVar));
        std::string key = keyVar.asString();
        if (obj.hasMember(key)) {
            ERR("duplicate key %s", key.c_str());
            return false;
        }
        CHECK_FALSE(skipWhiteSpace(json));
        CHECK_FALSE(consume(json, "="));
        CHECK_FALSE(skipWhiteSpace(json));
        Variant value;
        CHECK_FALSE(parseValue(json, value));
        obj[key] = value;
        if (next(json, ch) && ch == ',')
            json->get();
    }
    CHECK_FALSE(consume(json, "}"));
    result = obj;
    return true;
}

bool parseObjectJSON(std::istream* json, Variant& result)
{
    CHECK_FALSE(consume(json, "{"));
    Variant obj(typeDict);
    char ch;
    while(next(json, ch) && ch != '}')
    {
        if (obj.size() > 0) {
            CHECK_FALSE(consume(json, ","));
            CHECK_FALSE(skipWhiteSpace(json));
        }
        Variant keyVar;
        CHECK_FALSE(parseString(json, keyVar));
        std::string key = keyVar.asString();
        if (obj.hasMember(key)) {
            ERR("duplicate key %s", key.c_str());
            return false;
        }
        CHECK_FALSE(skipWhiteSpace(json));
        CHECK_FALSE(consume(json, ":"));
        CHECK_FALSE(skipWhiteSpace(json));
        Variant value;
        CHECK_FALSE(parseValueJSON(json, value));
        obj[key] = value;
    }
    CHECK_FALSE(consume(json, "}"));
    result = obj;
    return true;
}

bool parseArray(std::istream* json, Variant& result)
{
    CHECK_FALSE(consume(json, "["));
    Variant arr(typeArray);
    char ch;
    while(next(json, ch) && ch != ']')
    {
        Variant value;
        CHECK_FALSE(parseValue(json, value));
        arr.asArray().push_back(value);
        if (next(json, ch) && ch == ',')
            json->get();
    }
    CHECK_FALSE(consume(json, "]"));
    result = arr;
    return true;
}

bool parseArrayJSON(std::istream* json, Variant& result)
{
    CHECK_FALSE(consume(json, "["));
    Variant arr(typeArray);
    char ch;
    while(next(json, ch) && ch != ']')
    {
        if (arr.size() > 0) {
            CHECK_FALSE(consume(json, ","));
            CHECK_FALSE(skipWhiteSpace(json));
        }
        Variant value;
        CHECK_FALSE(parseValue(json, value));
        arr.asArray().push_back(value);
    }
    CHECK_FALSE(consume(json, "]"));
    result = arr;
    return true;
}

bool parseRoot(std::istream* json, Variant& result)
{
    Variant obj(typeDict);
    while(json->good())
    {
        skipWhiteSpace(json);
        std::string key;
        Variant keyVar;
        CHECK_FALSE(parseIdentifier(json, keyVar));
        key = keyVar.asString();
        skipWhiteSpace(json);
        consume(json, "=");
        skipWhiteSpace(json);
        Variant value;
        CHECK_FALSE(parseValue(json, value));
        obj[key] = value;
        CHECK_FALSE(skipWhiteSpace(json));
        if ((char)json->peek() == ',') {
            json->get();
        } else if (json->peek() == EOF)
            break;
    }
    result = obj;
    return true;
}

bool parse(std::istream* ss, Variant& obj, bool simplified)
{
    if (simplified)
    {
        return parseRoot(ss, obj);
    }
    else
    {
        bool result = parseValueJSON(ss, obj);
        if (!result)
            return false;
        if (obj.isArray() || obj.isMap())
            return true;
        ERR("array or object is expected");
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
