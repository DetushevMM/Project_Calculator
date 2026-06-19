#pragma once

#include <memory>
#include "calculator.hpp"

// Базовый интерфейс любого выражения в дереве польской нотации. Рекурсивное вычисление узлов
class IExpression {
 public:
  virtual ~IExpression() = default;

  virtual int Calculate() const = 0;
};

// Лист дерева константное число
class Constant final : public IExpression {
  int number_;

 public:
  explicit Constant(int number) : number_(number) {
  }

  int Calculate() const override {
    return number_;
  }
};

// Абстрактный класс унарной операции хранит один операнд, вычисляется в Operation
class IUnaryOperation : public IExpression {
  std::unique_ptr<IExpression> operand_;

 protected:
  // Конкретный класс (Square, Abs, Plus, Minus) реализует саму математику
  virtual int Operation(int operand) const = 0;

 public:
  explicit IUnaryOperation(std::unique_ptr<IExpression> operand) : operand_(std::move(operand)) {
  }

  int Calculate() const override {
    return Operation(operand_->Calculate());
  }
};

// Абстрактный бинарный класс хранит левый и правый операнды, вычисляет их и применяет Operation()
class IBinaryOperation : public IExpression {
  std::unique_ptr<IExpression> lhs_;
  std::unique_ptr<IExpression> rhs_;

 protected:
  virtual int Operation(int lhs, int rhs) const = 0;
 
 public:
  IBinaryOperation(std::unique_ptr<IExpression> lhs, std::unique_ptr<IExpression> rhs) : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {
  }

  int Calculate() const override {
    return Operation(lhs_->Calculate(), rhs_->Calculate());
  }
};

// Унарные операции + и - внутри скобок (-3)
class Plus final : public IUnaryOperation {
  int Operation(int operand) const override {
    return operand;
  }

 public:
  using IUnaryOperation::IUnaryOperation;
};

class Minus final : public IUnaryOperation {
  int Operation(int operand) const override {
    return -operand;
  }

 public:
  using IUnaryOperation::IUnaryOperation;
};

// Бинарные арифметические операции
class Sum final : public IBinaryOperation {
  int Operation(int lhs, int rhs) const override {
    return lhs + rhs;
  }

 public:
  using IBinaryOperation::IBinaryOperation;
};

class Subtract final : public IBinaryOperation {
  int Operation(int lhs, int rhs) const override {
    return lhs - rhs;
  }

 public:
  using IBinaryOperation::IBinaryOperation;
};

class Multiply final : public IBinaryOperation {
  int Operation(int lhs, int rhs) const override {
    return lhs * rhs;
  }

 public:
  using IBinaryOperation::IBinaryOperation;
};

class Divide final : public IBinaryOperation {
  int Operation(int lhs, int rhs) const override {
    if (rhs == 0) {
      throw WrongExpressionError();
    }
    return lhs / rhs;
  }

 public:
  using IBinaryOperation::IBinaryOperation;
};

class Residual final : public IBinaryOperation {
  int Operation(int lhs, int rhs) const override {
    if (rhs == 0) {
      throw WrongExpressionError();
    }
    return lhs % rhs;
  }

 public:
  using IBinaryOperation::IBinaryOperation;
};

class Power final : public IBinaryOperation {
  int Operation(int lhs, int rhs) const override {
    int result = 1;
    int base = lhs;
    int exp = rhs;
    while (exp > 0) {
      if (exp % 2 == 1) {
        result *= base;
      }
      base *= base;
      exp /= 2;
    }
    return result;
  }

 public:
  using IBinaryOperation::IBinaryOperation;
};

class Minimum final : public IBinaryOperation {
  int Operation(int lhs, int rhs) const override {
    return lhs < rhs ? lhs : rhs;
  }

 public:
  using IBinaryOperation::IBinaryOperation;
};

class Maximum final : public IBinaryOperation {
  int Operation(int lhs, int rhs) const override {
    return lhs > rhs ? lhs : rhs;
  }

 public:
  using IBinaryOperation::IBinaryOperation;
};

// Унарные операции
class AbsoluteValue final : public IUnaryOperation {
  int Operation(int operand) const override {
    return operand >= 0 ? operand : -operand;
  }

 public:
  using IUnaryOperation::IUnaryOperation;
};

class Square final : public IUnaryOperation {
  int Operation(int operand) const override {
    return operand * operand;
  }

 public:
  using IUnaryOperation::IUnaryOperation;
};