#include "calculator.hpp"

#include "parser.hpp"
#include "tokenize.hpp"

#include <variant>
#include <vector>

int CalculateExpression(std::string_view input) {
  auto tokens = Tokenize(input);

  for (const auto& token : tokens) {
    if (std::holds_alternative<UnknownToken>(token)) {
      throw UnknownSymbolError();
    }
  }
  size_t pos = 0;
  auto expression = ParseExpression(tokens, pos);

  if (pos != tokens.size()) {
    throw WrongExpressionError();
  }
  return expression->Calculate();
}