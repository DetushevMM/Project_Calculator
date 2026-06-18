#include "polish_notation.hpp"
#include "expressions.hpp"
#include "tokenize.hpp"

#include <memory>
#include <variant>
#include <vector>

// Рекурсивный парсер польской нотации. pos_ — текущая позиция в массиве токенов
class Parser {
  const std::vector<Token>& tokens_;
  size_t pos_{0};

  bool AtEnd() const {
    return pos_ >= tokens_.size();
  }

  const Token& Current() const {
    if (AtEnd()) {
      throw WrongExpressionError();
    }
    return tokens_[pos_];
  }

  void Advance() {
    ++pos_;
  }

  // Проверка: после первого операнда + или - стоит ')' или конец выражения? Если да — операция унарная "(- 3)" или "(+ (* 5 2))".
  bool IsUnaryPlusMinus() const {
    return AtEnd() || std::holds_alternative<ClosingBracketToken>(Current());
  }

  // Главная рекурсивная функция построения дерева
  std::unique_ptr<IExpression> ParseExpression() {
    if(AtEnd()) {
      throw WrongExpressionError();
    }

    // Обработка скобок '(' : парсим подвыражение, затем ожидаем ')'
    if (std::holds_alternative<OpeningBracketToken>(Current())) {
      Advance();
      auto expr = ParseExpression();
      if (AtEnd() || !std::holds_alternative<ClosingBracketToken>(Current())) {
        throw WrongExpressionError();
      }
      Advance();
      return expr;
    }
    // Число — лист дерева
    if (const auto* number = std::get_if<NumberToken>(&Current())) {
      Advance();
      return std::make_unique<Constant>(number->value);
    }

    // Операция: читаем токен и рекурсивно строим операнды
    Token op = Current();
    Advance();

    // '+' и '-' могут быть унарными или бинарными — определяем после 1-го операнда
    if (std::holds_alternative<PlusToken>(op)) {
      auto lhs = ParseExpression();
      if (IsUnaryPlusMinus()) {
        return std::make_unique<Plus>(std::move(lhs));
      }
      auto rhs = ParseExpression();
      return std::make_unique<Sum>(std::move(lhs), std::move(rhs));
    }
    if (std::holds_alternative<MinusToken>(op)) {
      auto lhs = ParseExpression();
      if (IsUnaryPlusMinus()) {
        return std::make_unique<Minus>(std::move(lhs));
      }
      auto rhs = ParseExpression();
      return std::make_unique<Subtract>(std::move(lhs), std::move(rhs));
    }

    // Остальные операции имеют фиксированную арность
    if (std::holds_alternative<MultiplyToken>(op)) {
      auto lhs = ParseExpression();
      auto rhs = ParseExpression();
      return std::make_unique<Multiply>(std::move(lhs), std::move(rhs));
    }

    if (std::holds_alternative<DivideToken>(op)) {
      auto lhs = ParseExpression();
      auto rhs = ParseExpression();
      return std::make_unique<Divide>(std::move(lhs), std::move(rhs));
    }

    if (std::holds_alternative<ResidualToken>(op)) {
      auto lhs = ParseExpression();
      auto rhs = ParseExpression();
      return std::make_unique<Residual>(std::move(lhs), std::move(rhs));
    }

    if (std::holds_alternative<MinToken>(op)) {
      auto lhs = ParseExpression();
      auto rhs = ParseExpression();
      return std::make_unique<Minimum>(std::move(lhs), std::move(rhs));
    }

    if (std::holds_alternative<MaxToken>(op)) {
      auto lhs = ParseExpression();
      auto rhs = ParseExpression();
      return std::make_unique<Maximum>(std::move(lhs), std::move(rhs));
    }

    if (std::holds_alternative<AbsToken>(op)) {
      auto operand = ParseExpression();
      return std::make_unique<AbsoluteValue>(std::move(operand));
    }

    if (std::holds_alternative<SqrToken>(op)) {
      auto operand = ParseExpression();
      return std::make_unique<Square>(std::move(operand));
    }

    if (std::holds_alternative<PowToken>(op)) {
      auto lhs = ParseExpression();
      auto rhs = ParseExpression();
      return std::make_unique<Power>(std::move(lhs), std::move(rhs));
    }

    // прочие неизвестные в этой части — некорректное выражение
    throw WrongExpressionError();
  }
 public:
  explicit Parser(const std::vector<Token>& tokens) : tokens_(tokens) {
  }

  std::unique_ptr<IExpression> Parse() {
    auto root = ParseExpression();
    if (!AtEnd()) {
      throw WrongExpressionError();
    }
    return root;
  }
};

int CalculatePolishNotation(std::string_view input) {
  // Токенизация
  auto tokens = Tokenize(input);
  // Проверка на неизвестные символы
  for (const auto& token : tokens) {
    if (std::holds_alternative<UnknownToken>(token)) {
      throw UnknownSymbolError();
    }
  }
  // Построение дерева разбора
  Parser parser(tokens);
  auto expression = parser.Parse();
  // Вычисление результата
  return expression->Calculate();
}