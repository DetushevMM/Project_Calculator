#pragma once
#include <stdexcept>
#include <string_view>

// Исключения для неизвестного токена, который встретился после токенизации
class UnknownSymbolError : public std::runtime_error {
 public:
  UnknownSymbolError() : std::runtime_error("Unknown symbol!") {
  }
};

// Некорректное выражение
class WrongExpressionError : public std::runtime_error {
 public:
  WrongExpressionError() : std::runtime_error("Wrong expression!") {
  }
};

// Функция токенизации строки, строит дерево и вычисляет результат
int CalculatePolishNotation(std::string_view input);