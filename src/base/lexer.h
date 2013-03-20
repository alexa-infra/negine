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

class Lexer
{
protected:
    FileText* file_;

    const u32 maxTokenSize;

    char*   token_;
    char*   whiteCharacters;
public:
    Lexer( const std::string& filename );
    ~Lexer();

    void   SkipWhiteSpace();
    void   SkipRestOfLine();
    const char* ReadToken();
    const char* CurrentToken() const {
        return token_;
    }
    f32    ReadFloat();
    bool   HasMoreData() const;
    void   SetWhiteCharValue( char c, char value );
    bool   IsWhiteChar( char character ) const;

private:

};

}
