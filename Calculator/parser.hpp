#pragma once
#include "tokenize.hpp"
#include "expressions.hpp"
#include <vector>
#include <memory>
#include <stdexcept>

#include "calculator.hpp"

// Функция парсера = Выражние - Слагаемое (+|- слагаемое)*
std::unique_ptr<IExpression> ParseExpression(const std::vector<Token>& tokens, size_t& pos);

// Функция парсера = Слагаемое - Степень (*|/|%- степеь)*
std::unique_ptr<IExpression> ParseTerm(const std::vector<Token>& tokens, size_t& pos);

// Функция парсера = Степень - Множитель (^ Степень)
std::unique_ptr<IExpression> ParsePower(const std::vector<Token>& tokens, size_t& pos);

// Функция парсера = Множитель - Число | '(' Выражение ')' | унарный минус/ плюс Множитель
std::unique_ptr<IExpression> ParseFactor(const std::vector<Token>& tokens, size_t& pos);