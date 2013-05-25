/**
 * \file
 * \brief       Lexer parser
 * \author      Victor Sukochev <sukochevvv@gmail.com>
                Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/

#include "lexer.h"
#include "stream.h"
#include <string.h>
#include <stdlib.h>
#include <regex>

namespace base
{

LexerPolicy::LexerPolicy(u32 mask)
    : setup(mask)
    , maxTokenSize( 512 )
{
    memset( whiteCharacters, 0, 128 );
    memset( breakCharacters, 0, 128 );
}

void LexerPolicy::setWhitespaces(const std::string& whitespaces)
{
    for(u32 i=0; i<whitespaces.size(); i++)
        whiteCharacters[static_cast<u8>(whitespaces[i])] = 1;
}

void LexerPolicy::setBreakChar(const std::string& breaks)
{
    for(u32 i=0; i<breaks.size(); i++)
        breakCharacters[static_cast<u8>(breaks[i])] = 1;
}

LexerPolicy LexerPolicy::defaultPolicy()
{
    LexerPolicy policy(cStyleComment | cppStyleComment | pythonComment | strings);
    policy.setWhitespaces(" \t,\r\n*:/");
    return policy;
}

Lexer::Lexer( const std::string& filename, const LexerPolicy& policy )
    : file_( new FileText(filename) )
    , token_( NULL )
    , policy_( policy )
{
    token_ = new char[policy_.maxTokenSize];
}

Lexer::~Lexer()
{
    delete[] token_;
    delete file_;
}

void Lexer::SkipWhiteSpace()
{
    if ( !HasMoreData() ) {
        return;
    }

    while( HasMoreData() && policy_.isWhitespace( file_->current_char() ) ) {
        char previousChar = file_->bump_char();

        if ( !HasMoreData() ) {
            break;
        }

        char ch = file_->current_char();

        if (policy_.allowPythonComment() && ch == '#') {
            skipUntil('\n');
        } else if ( policy_.allowCppStyleComment() && ( ch == '/' ) && ( previousChar == '/' ) ) {
            SkipRestOfLine();
        } else if ( policy_.allowCStyleComment() && ( ch == '*' ) && ( previousChar == '/' ) ) {
            skipUntil('*', '/');
        }
    }
}

void Lexer::SkipRestOfLine()
{
    skipUntil('\n');
}


const char* Lexer::ReadToken()
{
    char* tokenChar = token_;
    *tokenChar = '\0';
    SkipWhiteSpace();

    if ( !HasMoreData() ) {
        return token_;
    }

    if ( policy_.allowString() && file_->current_char() == '"' ) {
        file_->bump_char();

        while( HasMoreData() ) {
            if ( file_->current_char() == '"' ) {
                file_->bump_char();
                break;
            }

            *tokenChar++ = file_->bump_char();
        }
    } else {
        while( HasMoreData() && !policy_.isWhitespace( file_->current_char() ) ) {
            if (policy_.allowBreakOnChar() && policy_.isBreakChar( file_->current_char() ))
                break;
            *tokenChar++ = file_->bump_char();
        }
    }

    *tokenChar = '\0';
    return token_;
}

bool Lexer::ReadNumber(f32& ret)
{
    char* tokenChar = token_;
    *tokenChar = '\0';
    SkipWhiteSpace();

    const std::string chars("0123456789+-.eE");
    while ( HasMoreData() ) {
        char ch = file_->current_char();
        if (chars.find(ch) == std::string::npos)
            break;
        *tokenChar++ = file_->bump_char();
    }
    *tokenChar = '\0';

    if ( std::regex_match( token_, std::regex( "-?(?:0|[1-9]\\d*)(?:(?:\\.\\d+)?(?:[eE][+-]?\\d+)?)?" ) ) )
    {
        ret = atof( token_ );
        return true;
    }
    return false;
}

f32 Lexer::ReadFloat()
{
    ReadToken();
    return (f32)atof( token_ );
}

bool Lexer::HasMoreData() const
{
    return ( file_->position() < file_->size() );
}

void Lexer::skipUntil(char ch)
{
    while ( HasMoreData() ) {
        if ( file_->bump_char() == ch ) {
            break;
        }
    }
}

void Lexer::skipUntil(char ch1, char ch2)
{
    while ( HasMoreData() ) {
        char previousChar = file_->bump_char();

        if ( !HasMoreData() ) {
            break;
        }

        char ch = file_->current_char();

        if ( ( ch == ch2 ) && ( previousChar == ch1 ) ) {
            break;
        }
    }
}

} // namespace base
