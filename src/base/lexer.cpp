/**
 * \file
 * \brief       Lexer parser
 * \author      Victor Sukochev <sukochevvv@gmail.com>
                Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/

#include "lexer.h"
#include <string.h>
#include <stdlib.h>

namespace base {

Lexer::Lexer(const std::string &filename)
    : file_(filename)
    , maxTokenSize(512) 
    , token_(NULL)
    , whiteCharacters(NULL)
{

    token_ = new char[maxTokenSize];

    whiteCharacters = new char[128];
    memset(whiteCharacters, 0, 128);

    whiteCharacters[(u8)' '] = 1;
    whiteCharacters[(u8)'\t'] = 1;
    whiteCharacters[(u8)','] = 1;
//    whiteCharacters[(u8)'('] = 1;
//    whiteCharacters[(u8)')'] = 1;
    whiteCharacters[(u8)'\r'] = 1;
    whiteCharacters[(u8)'\n'] = 1;
    whiteCharacters[(u8)'*'] = 1;
    whiteCharacters[(u8)':'] = 1;
    whiteCharacters[(u8)'/'] = 1;
}

Lexer::~Lexer(){

    delete[] whiteCharacters;
    delete[] token_;
}

void Lexer::SkipWhiteSpace() {
    
    if (!HasMoreData())
        return;  
 
    while(HasMoreData() && IsWhiteChar(file_.current_char()))
    {
        char previousChar = file_.bump_char();
        
        if (!HasMoreData())
            break;
        char ch = file_.current_char();

        if (ch == '#')
        {
            SkipRestOfLine();
        }
        else if ((ch == '/') && (previousChar == '/'))
        {
            SkipRestOfLine();
        }
        else if ((ch == '*') && (previousChar == '/'))
        {
            //Skip until */ is found
            while (HasMoreData())
            {
                previousChar = file_.bump_char();
                
                if (!HasMoreData())
                    break;
                ch = file_.current_char();

                if ((ch == '/') && (previousChar == '*')) {
                    break;
                }
            }
        }
    }
}

void Lexer::SkipRestOfLine() {

    while (HasMoreData()) {
        if (file_.bump_char() == '\n')
            break;     
    }
}


const char* Lexer::ReadToken() {
    
    char* tokenChar = token_;
    *tokenChar = '\0';
    SkipWhiteSpace();

    if (!HasMoreData())
        return token_;
    
    if (file_.current_char() == '"') 
    {
        file_.bump_char();
        while(HasMoreData())
        {
            if (file_.current_char() == '"')
            {
                file_.bump_char();
                break;
            }
            *tokenChar++ = file_.bump_char();
        }
    }
    else 
    {
        while(HasMoreData() && !IsWhiteChar(file_.current_char()))
        {
            *tokenChar++ = file_.bump_char();
        }
    }
    *tokenChar = '\0';
    return token_;
}

bool Lexer::IsWhiteChar(char character) const {

    return whiteCharacters[(u8)character] == 1;
}

f32 Lexer::ReadFloat() {

    ReadToken(); 
    return (float)atof(token_);
}

bool Lexer::HasMoreData() const {
    
    return (file_.position() < file_.size());
}

void Lexer::SetWhiteCharValue(char c, char value) {
    
    whiteCharacters[(u8)c] = value;
}

} // namespace base
