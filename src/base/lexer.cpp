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
    LexerPolicy policy(POLICY_CSTYLE_COMMENT | POLICY_CPPSTYLE_COMMENT | POLICY_PYTHON_COMMENT | POLICY_QUOTED_STRINGS);
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

void Lexer::skipWhiteSpace()
{
    if ( !hasMoreData() ) {
        return;
    }

    while( hasMoreData() && policy_.isWhitespace( file_->currentChar() ) ) {
        char previousChar = file_->bumpChar();

        if ( !hasMoreData() ) {
            break;
        }

        char ch = file_->currentChar();

        if (policy_.allowPythonComment() && ch == '#') {
            skipUntil('\n');
        } else if ( policy_.allowCppStyleComment() && ( ch == '/' ) && ( previousChar == '/' ) ) {
            skipRestOfLine();
        } else if ( policy_.allowCStyleComment() && ( ch == '*' ) && ( previousChar == '/' ) ) {
            skipUntil('*', '/');
        }
    }
}

void Lexer::skipRestOfLine()
{
    skipUntil('\n');
}


const char* Lexer::readToken()
{
    char* tokenChar = token_;
    *tokenChar = '\0';
    skipWhiteSpace();

    if ( !hasMoreData() ) {
        return token_;
    }

    if ( policy_.allowString() && file_->currentChar() == '"' ) {
        file_->bumpChar();

        while( hasMoreData() ) {
            if ( file_->currentChar() == '"' ) {
                file_->bumpChar();
                break;
            }

            *tokenChar++ = file_->bumpChar();
        }
    } else {
        while( hasMoreData() && !policy_.isWhitespace( file_->currentChar() ) ) {
            if (policy_.allowBreakOnChar() && policy_.isBreakChar( file_->currentChar() ))
                break;
            *tokenChar++ = file_->bumpChar();
        }
    }

    *tokenChar = '\0';
    return token_;
}

bool Lexer::readNumber(f32& ret)
{
    char* tokenChar = token_;
    *tokenChar = '\0';
    skipWhiteSpace();

    const std::string chars("0123456789+-.eE");
    while ( hasMoreData() ) {
        char ch = file_->currentChar();
        if (chars.find(ch) == std::string::npos)
            break;
        *tokenChar++ = file_->bumpChar();
    }
    *tokenChar = '\0';

    if ( std::regex_match( token_, std::regex( "-?(?:0|[1-9]\\d*)(?:(?:\\.\\d+)?(?:[eE][+-]?\\d+)?)?" ) ) )
    {
        ret = atof( token_ );
        return true;
    }
    return false;
}

f32 Lexer::readFloat()
{
    readToken();
    return (f32)atof( token_ );
}

bool Lexer::hasMoreData() const
{
    return ( file_->position() < file_->size() );
}

void Lexer::skipUntil(char ch)
{
    while ( hasMoreData() ) {
        if ( file_->bumpChar() == ch ) {
            break;
        }
    }
}

void Lexer::skipUntil(char ch1, char ch2)
{
    while ( hasMoreData() ) {
        char previousChar = file_->bumpChar();

        if ( !hasMoreData() ) {
            break;
        }

        char ch = file_->currentChar();

        if ( ( ch == ch2 ) && ( previousChar == ch1 ) ) {
            break;
        }
    }
}

} // namespace base
