#include "parser.hpp"
#include "tokenize.hpp"

#include <gtest/gtest.h>

TEST(ParserTest, ParseSingleNumber) {
    auto tokens = Tokenize("42");

    size_t pos = 0;
    auto expression = ParseExpression(tokens, pos);

    EXPECT_EQ(expression->Calculate(), 42);
    EXPECT_EQ(pos, tokens.size());
}

TEST(ParserTest, ParseAddition) {
    auto tokens = Tokenize("2+3");

    size_t pos = 0;
    auto expression = ParseExpression(tokens, pos);

    EXPECT_EQ(expression->Calculate(), 5);
}

TEST(ParserTest, ParseSubtraction) {
    auto tokens = Tokenize("10-7");

    size_t pos = 0;
    auto expression = ParseExpression(tokens, pos);

    EXPECT_EQ(expression->Calculate(), 3);
}

TEST(ParserTest, ParseMultiplication) {
    auto tokens = Tokenize("6*7");

    size_t pos = 0;
    auto expression = ParseExpression(tokens, pos);

    EXPECT_EQ(expression->Calculate(), 42);
}

TEST(ParserTest, ParseDivision) {
    auto tokens = Tokenize("20/4");

    size_t pos = 0;
    auto expression = ParseExpression(tokens, pos);

    EXPECT_EQ(expression->Calculate(), 5);
}

TEST(ParserTest, ParseResidual) {
    auto tokens = Tokenize("20%6");

    size_t pos = 0;
    auto expression = ParseExpression(tokens, pos);

    EXPECT_EQ(expression->Calculate(), 2);
}

TEST(ParserTest, ParsePower) {
    auto tokens = Tokenize("2^10");

    size_t pos = 0;
    auto expression = ParseExpression(tokens, pos);

    EXPECT_EQ(expression->Calculate(), 1024);
}

TEST(ParserTest, ParsePriority) {
    auto tokens = Tokenize("2+3*4");

    size_t pos = 0;
    auto expression = ParseExpression(tokens, pos);

    EXPECT_EQ(expression->Calculate(), 14);
}

TEST(ParserTest, ParseBrackets) {
    auto tokens = Tokenize("(2+3)*4");

    size_t pos = 0;
    auto expression = ParseExpression(tokens, pos);

    EXPECT_EQ(expression->Calculate(), 20);
}

TEST(ParserTest, UnaryMinus) {
    auto tokens = Tokenize("-5");

    size_t pos = 0;
    auto expression = ParseExpression(tokens, pos);

    EXPECT_EQ(expression->Calculate(), -5);
}

TEST(ParserTest, UnaryPlus) {
    auto tokens = Tokenize("+5");

    size_t pos = 0;
    auto expression = ParseExpression(tokens, pos);

    EXPECT_EQ(expression->Calculate(), 5);
}

TEST(ParserTest, WrongExpression) {
    auto tokens = Tokenize("2+");

    size_t pos = 0;

    EXPECT_THROW(ParseExpression(tokens, pos), WrongExpressionError);
}

TEST(ParserTest, EmptyExpression) {
    std::vector<Token> tokens;

    size_t pos = 0;

    EXPECT_THROW(ParseExpression(tokens, pos), WrongExpressionError);
}