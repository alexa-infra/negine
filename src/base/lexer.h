/**
 * \file
 * \brief       Lexer parser
 * \author      Victor Sukochev <sukochevvv@gmail.com>
                Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/

#pragma once

#include "base/types.h"
#include <string>

namespace base
{

class FileText;

class LexerPolicy
{
public:
    LexerPolicy(u32 mask);

    enum Policy {
        POLICY_CSTYLE_COMMENT   = 0x1,
        POLICY_CPPSTYLE_COMMENT = 0x2,
        POLICY_PYTHON_COMMENT   = 0x4,
        POLICY_QUOTED_STRINGS   = 0x8,
        POLICY_BREAK_CHAR       = 0x10
    };

    void setWhitespaces(const std::string& whitespaces);
    void setBreakChar(const std::string& breaks);

    inline bool allowCStyleComment() const   { return (setup & POLICY_CSTYLE_COMMENT) == POLICY_CSTYLE_COMMENT; }
    inline bool allowCppStyleComment() const { return (setup & POLICY_CPPSTYLE_COMMENT) == POLICY_CPPSTYLE_COMMENT; }
    inline bool allowPythonComment() const   { return (setup & POLICY_PYTHON_COMMENT) == POLICY_PYTHON_COMMENT; }
    inline bool allowString() const          { return (setup & POLICY_QUOTED_STRINGS) == POLICY_QUOTED_STRINGS; }
    inline bool isWhitespace(char ch) const  { return whiteCharacters[static_cast<u8>(ch)] == 1; }
    inline bool isBreakChar(char ch) const   { return breakCharacters[static_cast<u8>(ch)] == 1; }
    inline bool allowBreakOnChar() const     { return (setup & POLICY_BREAK_CHAR) == POLICY_BREAK_CHAR; }

    static LexerPolicy defaultPolicy();

    u32 setup;
    u8 whiteCharacters[128];
    u8 breakCharacters[128];
    u32 maxTokenSize;
};

class Lexer
{
public:
    Lexer( const std::string& filename, const LexerPolicy& policy = LexerPolicy::defaultPolicy() );
    ~Lexer();

    void   skipWhiteSpace();
    void   skipRestOfLine();
    const char* readToken();
    const char* currentToken() const {
        return token_;
    }
    std::string currentTokenStr() const {
        return token_;
    }
    f32    readFloat();
    bool   readNumber(f32& ret);
    bool   hasMoreData() const;

    void   skipUntil(char ch);
    void   skipUntil(char ch1, char ch2);

protected:
    FileText*   file_;
    char*       token_;
    LexerPolicy policy_;
};

}
