#include "tokenize.hpp"

#include <gtest/gtest.h>

TEST(TokenizeTest, EmptyInput) {
    EXPECT_TRUE(Tokenize("").empty());
}

TEST(TokenizeTest, SingleNumber) {
    std::vector<Token> expected = {
        NumberToken{123}
    };

    EXPECT_EQ(Tokenize("123"), expected);
}

TEST(TokenizeTest, ArithmeticOperators) {
    std::vector<Token> expected = {
        PlusToken{},
        MinusToken{},
        MultiplyToken{},
        DivideToken{},
        ResidualToken{},
        PowToken{}
    };

    EXPECT_EQ(Tokenize("+-*/%^"), expected);
}

TEST(TokenizeTest, Brackets) {
    std::vector<Token> expected = {
        OpeningBracketToken{},
        ClosingBracketToken{}
    };

    EXPECT_EQ(Tokenize("()"), expected);
}

TEST(TokenizeTest, Functions) {
    std::vector<Token> expected = {
        SqrToken{},
        MaxToken{},
        MinToken{},
        AbsToken{}
    };

    EXPECT_EQ(Tokenize("sqr max min abs"), expected);
}

TEST(TokenizeTest, MixedExpression) {
    std::vector<Token> expected = {
        NumberToken{12},
        PlusToken{},
        NumberToken{34},
        MultiplyToken{},
        OpeningBracketToken{},
        NumberToken{5},
        MinusToken{},
        NumberToken{2},
        ClosingBracketToken{}
    };

    EXPECT_EQ(Tokenize("12+34*(5-2)"), expected);
}

TEST(TokenizeTest, UnknownWord) {
    std::vector<Token> expected = {
        UnknownToken{"hello"}
    };

    EXPECT_EQ(Tokenize("hello"), expected);
}

TEST(TokenizeTest, UnknownSymbol) {
    std::vector<Token> expected = {
        UnknownToken{"@"}
    };

    EXPECT_EQ(Tokenize("@"), expected);
}

TEST(TokenizeTest, IgnoreSpaces) {
    std::vector<Token> expected = {
        NumberToken{15},
        PlusToken{},
        NumberToken{27}
    };

    EXPECT_EQ(Tokenize("   15   +   27   "), expected);
}