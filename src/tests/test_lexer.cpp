/**
 * \file
 * \author      Alexey Vasilyev <alexa.infra@gmail.com>
 * \brief       Test for lexer class - creates file and try parse it
 * \copyright   MIT License
 **/
#include "gtest/gtest.h"
#include "base/lexer.h"
#include <iostream>
#include <fstream>
#include <stdio.h>

using base::Lexer;
using std::ofstream;

const char filename[] = "tempfile.txt";

class LexerTest : public ::testing::Test {
protected:
    static void SetUpTestCase() {
        ofstream f;
        f.open (filename);
        f << "            this is a test" << std::endl;
        f << "   second \"line\"" << std::endl;
        f << "value # with comment" << std::endl;
        f << "thevalue" << std::endl;
        f << "/* block" << std::endl;
        f << " comment*/ value1" << std::endl;
        f.close();
    }

    static void TearDownTestCase() {
        remove(filename);
    }
};

TEST_F(LexerTest, Basic) {
    Lexer lex(filename);
    EXPECT_EQ(lex.ReadToken(), "this");
    EXPECT_EQ(lex.ReadToken(), "is");
    EXPECT_EQ(lex.ReadToken(), "a");
    EXPECT_EQ(lex.ReadToken(), "test");
    EXPECT_EQ(lex.ReadToken(), "second");
    EXPECT_EQ(lex.ReadToken(), "line");
    
    EXPECT_EQ(lex.ReadToken(), "value");
    EXPECT_EQ(lex.ReadToken(), "thevalue");
    EXPECT_EQ(lex.ReadToken(), "value1");
}
