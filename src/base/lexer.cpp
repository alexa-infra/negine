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

namespace base
{

LexerPolicy::LexerPolicy(const std::string& whitespaces, u32 mask)
    : setup(mask)
{
    memset( whiteCharacters, 0, 128 );
    for(u32 i=0; i<whitespaces.size(); i++)
        whiteCharacters[(u8)whitespaces[i]] = 1;
}

void LexerPolicy::setWhiteCharValue( char ch, bool whitespace )
{
    whiteCharacters[( u8 )ch] = whitespace ? 1 : 0;
}

LexerPolicy LexerPolicy::defaultPolicy()
{
    return LexerPolicy(" \t,\r\n*:/", cStyleComment | cppStyleComment | pythonComment | strings);
}

Lexer::Lexer( const std::string& filename, const LexerPolicy& policy )
    : file_( new FileText(filename) )
    , maxTokenSize( 512 )
    , token_( NULL )
    , policy_( policy )
{
    token_ = new char[maxTokenSize];
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
            *tokenChar++ = file_->bump_char();
        }
    }

    *tokenChar = '\0';
    return token_;
}

f32 Lexer::ReadFloat()
{
    ReadToken();
    return ( f32 )atof( token_ );
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
