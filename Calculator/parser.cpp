#include "parser.hpp"
#include <variant>


// Функция проверки выхода за пределы массива
bool AtEnd(const std::vector<Token>& tokens, size_t pos) {
  return pos >= tokens.size();
}

// Функция длоя получения текущего токена
const Token& Current(const std::vector<Token>& tokens, size_t pos) {
  if (AtEnd(tokens, pos)) {
    throw WrongExpressionError();
  }
  return tokens[pos];
}

// Функция парсера = Выражние - Слагаемое (+|- слагаемое)*
std::unique_ptr<IExpression> ParseExpression(const std::vector<Token>& tokens, size_t& pos) {
  auto expr = ParseTerm(tokens, pos);
  while (!AtEnd(tokens, pos)) {
    const Token& current = Current(tokens, pos);

    if (std::holds_alternative<PlusToken>(current)) {
      ++pos;
      auto right = ParseTerm(tokens, pos);
      expr = std::make_unique<Sum>(std::move(expr), std::move(right));
    } else if (std::holds_alternative<MinusToken>(current)) {
      ++pos;
      auto right = ParseTerm(tokens, pos);
      expr = std::make_unique<Subtract>(std::move(expr), std::move(right));
    } else {
      break;  // Встретили токен другого приоритета или закрывающуюся скобку
    }
  }
  return expr;
}

// Функция парсера = Слагаемое - степень (*|/|%- степеь)*
std::unique_ptr<IExpression> ParseTerm(const std::vector<Token>& tokens, size_t& pos) {
  auto expr = ParsePower(tokens, pos);
  
  while (!AtEnd(tokens, pos)) {
    const Token& current = Current(tokens, pos);

    if (std::holds_alternative<MultiplyToken>(current)) {
      ++pos;
      auto right = ParsePower(tokens, pos);
      expr = std::make_unique<Multiply>(std::move(expr), std::move(right));
    } else if (std::holds_alternative<DivideToken>(current)) {
      ++pos;
      auto right = ParsePower(tokens, pos);
      expr = std::make_unique<Divide>(std::move(expr), std::move(right));
    } else if (std::holds_alternative<ResidualToken>(current)) {
      ++pos;
      auto right = ParsePower(tokens, pos);
      expr = std::make_unique<Residual>(std::move(expr), std::move(right));
    } else {
      break;  // Встретили токен другого приоритета или закрывающуюся скобку
    }
  }
  return expr;
}

// Функция парсера = Степень - Множитель (^ Степень)
std::unique_ptr<IExpression> ParsePower(const std::vector<Token>& tokens, size_t& pos) {
  auto expr = ParseFactor(tokens, pos);

  if (!AtEnd(tokens, pos) && std::holds_alternative<PowToken>(Current(tokens, pos))) {
    ++pos;
    auto right = ParsePower(tokens, pos);
    return std::make_unique<Power>(std::move(expr), std::move(right));
  }
  return expr;
}

// Множитель - число | '(' Выражение ')' | Унарный оператор Множитель
std::unique_ptr<IExpression> ParseFactor(const std::vector<Token>& tokens, size_t& pos) {
  if (AtEnd(tokens, pos)) {
    throw WrongExpressionError();
  }
  const Token& current = Current(tokens, pos);

  // Обработка унарного плюса
  if (std::holds_alternative<PlusToken>(current)) {
    ++pos;
    auto operand = ParseFactor(tokens, pos);
    return std::make_unique<Plus>(std::move(operand));
  }

  // Обработка унарного минуса
  if (std::holds_alternative<MinusToken>(current)) {
    ++pos;
    auto operand = ParseFactor(tokens, pos);
    return std::make_unique<Minus>(std::move(operand));
  }

  // Обработка скобок
  if (std::holds_alternative<OpeningBracketToken>(current)) {
    ++pos;
    auto expr = ParseExpression(tokens, pos);
    if (AtEnd(tokens, pos) || !std::holds_alternative<ClosingBracketToken>(Current(tokens, pos))) {
      throw WrongExpressionError();
    }
    ++pos;
    return expr;
  }

  // Обработка числа
  if (const auto* number_token = std::get_if<NumberToken>(&current)) {
    ++pos;
    return std::make_unique<Constant>(number_token->value);
  }

  // Если встретили что-то иное
  throw WrongExpressionError();
}