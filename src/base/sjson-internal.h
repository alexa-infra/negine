#pragma once

#include "sjson.h"
#include <exception>

namespace base {
namespace sjson {

    struct ParseException : public std::exception
    {
        i32 position;
        std::string text;
        ParseException(const std::string& reason, i32 pos);
        virtual ~ParseException() {}
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

} // namespace sjson
} // namespace base
