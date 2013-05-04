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
    LexerPolicy(const std::string& whitespaces, u32 mask);

    enum {
        cStyleComment   = 0x1,
        cppStyleComment = 0x2,
        pythonComment   = 0x4,
        strings         = 0x8
    };
    u32 setup;
    u8 whiteCharacters[128];

    inline bool allowCStyleComment() const   { return (setup & cStyleComment) == cStyleComment; }
    inline bool allowCppStyleComment() const { return (setup & cppStyleComment) == cppStyleComment; }
    inline bool allowPythonComment() const   { return (setup & pythonComment) == pythonComment; }
    inline bool allowString() const          { return (setup & strings) == strings; }
    inline bool isWhitespace(char ch) const  { return whiteCharacters[(u8)ch] == 1; }

    void setWhiteCharValue( char ch, bool whitespace );

    static LexerPolicy defaultPolicy();
};

class Lexer
{
protected:
    FileText* file_;
    const u32 maxTokenSize;
    char*   token_;
    LexerPolicy policy_;

public:
    Lexer( const std::string& filename, const LexerPolicy& policy = LexerPolicy::defaultPolicy() );
    ~Lexer();

    void   SkipWhiteSpace();
    void   SkipRestOfLine();
    const char* ReadToken();
    const char* CurrentToken() const {
        return token_;
    }
    std::string CurrentTokenStr() const {
        return token_;
    }
    f32    ReadFloat();
    bool   HasMoreData() const;

    void   skipUntil(char ch);
    void   skipUntil(char ch1, char ch2);
};

}
