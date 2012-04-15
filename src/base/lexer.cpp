/**
 * \file
 * \brief       Lexer parser
 * \author      Victor Sukochev <sukochevvv@gmail.com>
                Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/

#include "lexer.h"
#include <string.h>

namespace base {

Lexer::Lexer(const std::string &filename)
    : file_(filename)
    , maxTokenSize(512) 
    , token_(NULL)
    , whiteCharacters(NULL)
{

    token_ = new char[maxTokenSize];

    size = file_.size();

    whiteCharacters = new char[128];
    memset(whiteCharacters, 0, 128);

    whiteCharacters[' '] = 1;
    whiteCharacters['\t'] = 1;
    whiteCharacters[','] = 1;
//    whiteCharacters['('] = 1;
//    whiteCharacters[')'] = 1;
    whiteCharacters['\r'] = 1;
    whiteCharacters['\n'] = 1;
    whiteCharacters['*'] = 1;
    whiteCharacters[':'] = 1;
    whiteCharacters['/'] = 1;
}

Lexer::~Lexer(){

    delete[] whiteCharacters;
    delete[] token_;
}

void Lexer::SkipWhiteSpace() {
    
    if (!HasMoreData())
        return;  
 
    while(HasMoreData() && IsWhiteChar(file_.read_type_nomove<char>()))
    {
        char previousChar = file_.read_type<char>();
        
        if (!HasMoreData())
            break;
        char ch = file_.read_type_nomove<char>();

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
                previousChar = file_.read_type<char>();
                
                if (!HasMoreData())
                    break;
                ch = file_.read_type_nomove<char>();

                if ((ch == '/') && (previousChar == '*')) {
                    break;
                }
            }
        }
    }
}

void Lexer::SkipRestOfLine() {

    while (HasMoreData()) {
        if (file_.read_type<char>() == '\n')
            break;     
    }
}


const char* Lexer::ReadToken() {
    
    char* tokenChar = token_;
    *tokenChar = '\0';
    SkipWhiteSpace();

    if (!HasMoreData())
        return token_;
    
    if (file_.read_type_nomove<char>() == '"') 
    {
        file_.read_type<char>();
        while(HasMoreData())
        {
            if (file_.read_type_nomove<char>() == '"')
            {
                file_.read_type<char>();
                break;
            }
            *tokenChar++ = file_.read_type<char>();
        }
    }
    else 
    {
        while(HasMoreData() && !IsWhiteChar(file_.read_type_nomove<char>()))
        {
            *tokenChar++ = file_.read_type<char>();
        }
    }
    *tokenChar = '\0';
    return token_;
}

bool Lexer::IsWhiteChar(char character) const {

    return whiteCharacters[character] == 1;
}

f32 Lexer::ReadFloat() {

    ReadToken(); 
    return (float)atof(token_);
}

bool Lexer::HasMoreData() const {
    
    return (file_.position() < size);
}

void Lexer::SetWhiteCharValue(char c, char value) {
    
    whiteCharacters[c] = value;
}

} // namespace base
