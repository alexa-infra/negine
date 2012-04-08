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

    string  token_str_;
    char*   token_;
    char*   whiteCharacters;

    u32     size;
public: 
    Lexer(const std::string &filename);
    ~Lexer();

    void   SkipWhiteSpace();
    void   SkipRestOfLine();
    string ReadToken();
    const string& CurrentToken() const { return token_str_; }
    f32    ReadFloat();
    bool   HasMoreData() const;
    void   SetWhiteCharValue(char c, char value);
    bool   IsWhiteChar(char character) const;
    bool   CheckToken(const string &token);

private:

};

}
