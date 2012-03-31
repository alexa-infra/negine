/**
 * \file
 * \brief       Lexer parser
 * \author      Victor Sukochev <sukochevvv@gmail.com>
                Alexey Vasilyev <alexa.infra@gmail.com>
 * \copyright   MIT License
 **/

#pragma once

#include "base/types.h"
#include "base/stream.h"


namespace base {

class Lexer {
protected:
    FileBinary file_;

    const u32 maxTokenSize;

    char* token;
    char* tokenChar;

    char *whiteCharacters;

public: 
    Lexer(const std::string &filename);
    ~Lexer();

    void   SkipWhiteSpace();
    void   SkipRestOfLine();
    string ReadToken();
    string GetCurrentToken() { return token; }
    f32    ReadReal();
    i32    HasMoreData();
    void   SetWhiteCharValue(char c, char value);
    bool   IsWhiteChar(char character);


private:

};

}