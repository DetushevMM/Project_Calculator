#pragma once

#include <string_view>
#include <vector>
#include <variant>
#include <string>

// Создадим базовые структуры для токенов
struct PlusToken{};
struct MinusToken{};
struct MultiplyToken{};
struct DivideToken{};
struct ResidualToken{};
struct OpeningBracketToken{};
struct ClosingBracketToken{};
struct SqrToken{};
struct MaxToken{};
struct MinToken{};
struct AbsToken{};
struct PowToken{};

struct NumberToken{
  int value;
};

struct UnknownToken{
  std::string value;
};

// Операторы сравнения для пустых токенов
inline bool operator==(PlusToken, PlusToken) {
  return true;
}

inline bool operator==(MinusToken, MinusToken) {
  return true;
}

inline bool operator==(MultiplyToken, MultiplyToken) {
  return true;
}

inline bool operator==(DivideToken, DivideToken) {
  return true;
}

inline bool operator==(ResidualToken, ResidualToken) {
  return true;
}

inline bool operator==(OpeningBracketToken, OpeningBracketToken) {
  return true;
}

inline bool operator==(ClosingBracketToken, ClosingBracketToken) {
  return true;
}

inline bool operator==(SqrToken, SqrToken) {
  return true;
}

inline bool operator==(MaxToken, MaxToken) {
  return true;
}

inline bool operator==(MinToken, MinToken) {
  return true;
}

inline bool operator==(AbsToken, AbsToken) {
  return true;
}

inline bool operator==(PowToken, PowToken) {
  return true;
}

inline bool operator==(const NumberToken& lhs, const NumberToken& rhs) {
  return lhs.value == rhs.value;
}

inline bool operator==(const UnknownToken& lhs, const UnknownToken& rhs) {
  return lhs.value == rhs.value;
}

// Тип Token как вариант всех возможных токенов
using Token = std::variant<
  PlusToken,
  MinusToken,
  MultiplyToken,
  DivideToken,
  ResidualToken,
  OpeningBracketToken,
  ClosingBracketToken,
  SqrToken,
  MaxToken,
  MinToken,
  AbsToken,
  PowToken,
  NumberToken,
  UnknownToken
>;

// Прототип функции токенизации
std::vector<Token> Tokenize(std::string_view input);