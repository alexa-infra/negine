#pragma once

#include "sjson.h"
#include <istream>
#include <exception>

#ifdef COMPILER_MSVC
// void f1(void) throw(int) {} - is not implemented :(
#pragma warning( disable : 4290 )
#endif 

namespace base {
namespace sjson {

    struct ParseException : public std::exception
    {
        std::string text;
        ParseException(const std::string& reason);
        virtual ~ParseException() noexcept(true) {}
        const char* what() const throw() { return text.c_str(); }
    };

    void skipWhiteSpace(std::istream* json);
    void checkBounds(std::istream* json) throw(ParseException);
    void skipComment(std::istream* json) throw(ParseException);
    void consume(std::istream* json, const std::string& str) throw(ParseException);
    char next(std::istream* json) throw(ParseException);
    Variant parseString(std::istream* json) throw(ParseException);
    Variant parseIdentifier(std::istream* json) throw(ParseException);
    bool isIdentifier(char ch);
    Variant parseNumber(std::istream* json)throw(ParseException);
    Variant parseValue(std::istream* json) throw(ParseException);
    Variant parseArray(std::istream* json) throw(ParseException);
    Variant parseRoot(std::istream* json) throw(ParseException);
    Variant parseObject(std::istream* json) throw(ParseException);

    Variant parseValueJSON(std::istream* json) throw(ParseException);
    Variant parseArrayJSON(std::istream* json) throw(ParseException);
    Variant parseObjectJSON(std::istream* json) throw(ParseException);

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
} // namespace sjson
} // namespace base
