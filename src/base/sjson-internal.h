#pragma once

#include "sjson.h"
#include <istream>

namespace base {
namespace sjson {

    bool skipWhiteSpace(std::istream* json);
    bool checkBounds(std::istream* json);
    bool skipComment(std::istream* json);
    bool consume(std::istream* json, const std::string& str);
    bool next(std::istream* json, char& ch);
    bool parseString(std::istream* json, Variant& result);
    bool parseIdentifier(std::istream* json, Variant& result);
    bool isIdentifier(char ch);
    bool parseNumber(std::istream* json, Variant& result);
    bool parseValue(std::istream* json, Variant& result);
    bool parseArray(std::istream* json, Variant& result);
    bool parseRoot(std::istream* json, Variant& result);
    bool parseObject(std::istream* json, Variant& result);

    bool parseValueJSON(std::istream* json, Variant& result);
    bool parseArrayJSON(std::istream* json, Variant& result);
    bool parseObjectJSON(std::istream* json, Variant& result);

    void writeTabs(std::ostream* json, u32 tabs);
    void writeString(std::ostream* json, const std::string& str);
    void writeIdentifier(std::ostream* json, const std::string& str);
    void writeArray(std::ostream* json, const Variant::Array& val, u32 tabs);
    void writeMap(std::ostream* json, const Variant::Map& val, u32 tabs);
    void writeObject(std::ostream* json, const Variant::Map& val, u32 tabs);
    void writeValue(std::ostream* json, const Variant& v, u32 tabs);

    void writeArrayJSON(std::ostream* json, const Variant::Array& val, u32 tabs);
    void writeObjectJSON(std::ostream* json, const Variant::Map& val, u32 tabs);
    void writeValueJSON(std::ostream* json, const Variant& v, u32 tabs);
    
    bool parse(const std::istream* json, Variant& obj, bool simplified = true);
    void write(std::ostream* json, const Variant& v, bool simplified = true);
} // namespace sjson
} // namespace base
