#pragma once

#include <stdexcept>
#include <string_view>

// Искоючение при встрече с неизвестным символом
class UnknownSymbolError : public std::runtime_error {
 public:
  UnknownSymbolError() : std::runtime_error("Unknown symbol") {
  }
};

class WrongExpressionError : public std::runtime_error {
 public:
  WrongExpressionError() : std::runtime_error("Wrong expression!") {
  }
};

int CalculateExpression(std::string_view input);