#include "tokenize.hpp"
#include <cctype>
#include <unordered_map>

// Контейнер для ключевых слов и их соответствующих токенов
const std::unordered_map<std::string, Token> kNameToken = {
  {"sqr", SqrToken{}},
  {"max", MaxToken{}},
  {"min", MinToken{}},
  {"abs", AbsToken{}}
};

// Карта для специальных символов и соответствующих токенов
const std::unordered_map<char, Token> kSymbolToken = {
  {'+', PlusToken{}},
  {'-', MinusToken{}},
  {'*', MultiplyToken{}},
  {'/', DivideToken{}},
  {'%', ResidualToken{}},
  {'^', PowToken{}},
  {'(', OpeningBracketToken{}},
  {')', ClosingBracketToken{}}
};

bool IsSpace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int ParseNumber(std::string_view input, size_t& pos) {
  int value = 0;
  while (pos < input.size() && std::isdigit(input[pos])) {
    value = value * 10 + (input[pos] - '0');
    ++pos;
  }
  return value;
}

std::string ParseWord(std::string_view input, size_t& pos) {
  std::string word;
  while (pos < input.size() && std::isalpha(input[pos])) {
    word += input[pos];
    ++pos;
  }
  return word;
}

std::vector<Token> Tokenize(std::string_view input) {
  std::vector<Token> tokens;
  size_t pos = 0;

  while (pos<input.size()) {
    char symbol = input[pos];

    if (IsSpace(symbol)) {
      ++pos;
      continue;
    }

    if (std::isdigit(symbol)) {
      int number = ParseNumber(input, pos);
      tokens.emplace_back(NumberToken{number});
      continue;
    }

    if (std::isalpha(symbol)) {
      std::string word = ParseWord(input, pos);
      auto it = kNameToken.find(word);

      if (it != kNameToken.end()) {
        tokens.push_back(it->second);
      } else {
        tokens.emplace_back(UnknownToken{word});
      }
      continue;
    }

    auto it = kSymbolToken.find(symbol);
    if (it != kSymbolToken.end()) {
      tokens.push_back(it->second);
      ++pos;
      continue;
    }
    tokens.emplace_back(UnknownToken{std::string(1, symbol)});
    ++pos;
  }
  return tokens;
}