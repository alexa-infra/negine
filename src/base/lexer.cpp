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
    , maxTokenSize(256) 
    , token(NULL)
    , tokenChar(NULL)
    , whiteCharacters(NULL)
{
    token = new char[maxTokenSize];
    memset(token, 0, maxTokenSize);
    whiteCharacters = new char[128];
    memset(whiteCharacters, 0, 128);

    whiteCharacters[' '] = 1;
    whiteCharacters['\t'] = 1;
    whiteCharacters[','] = 1;
    whiteCharacters['('] = 1;
    whiteCharacters[')'] = 1;
    whiteCharacters['\r'] = 1;
    whiteCharacters['\n'] = 1;
    whiteCharacters['*'] = 1;
    whiteCharacters[':'] = 1;
    whiteCharacters['/'] = 1;
}

Lexer::~Lexer(){
    delete[] token;
    delete[] whiteCharacters;
}

void Lexer::SkipWhiteSpace() {
    
    if (!HasMoreData())
        return;  
 
    char ch = file_.read_type_nomove<char>();
    while(HasMoreData() && IsWhiteChar(ch))
    {
        char previousChar = file_.read_type<char>();
        ch = file_.read_type_nomove<char>();

        if (ch == '#')
        {
            SkipRestOfLine();
        }
        
        if ((ch == '/') && (previousChar == '/'))
        {
            SkipRestOfLine();
            SkipWhiteSpace();
            return;
        }
        
        if ((ch == '*') && (previousChar == '/'))
        {
            //Skip until */ is found
            while (HasMoreData())
            {
                previousChar = file_.read_type<char>();
                ch = file_.read_type_nomove<char>();

                if ((ch == '/') && (previousChar == '*'))
                    break;
            }
            
            if (HasMoreData())
            {
                file_.read_type<char>();
                SkipWhiteSpace();
                return;
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


string Lexer::ReadToken() {
    
    tokenChar = token;
    *tokenChar = '\0';
    SkipWhiteSpace();


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
        *tokenChar = file_.read_type<char>();
        while(HasMoreData() && !IsWhiteChar(file_.read_type_nomove<char>()))
        {
            *tokenChar++;
            *tokenChar = file_.read_type<char>();
        }
        *tokenChar++;
    }
    *tokenChar = '\0';
    return token;
}

bool Lexer::IsWhiteChar(char character) {
    return whiteCharacters[character] == 1;
}

f32 Lexer::ReadReal() {
    ReadToken(); 
    return (float)atof(token);
}

i32 Lexer::HasMoreData() {
    
    return (file_.position() < file_.size());
}
void Lexer::SetWhiteCharValue(char c, char value) {
    
    whiteCharacters[c] = value;
}

}
