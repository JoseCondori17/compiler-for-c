#ifndef UNARY_EXPRESSION_H
#define UNARY_EXPRESSION_H

#include "expression.h"
#include <string>
using std::string;

class UnaryExpression : public Exp
{
protected:
    Exp *operand;
    BinaryOp op;
    bool isPrefix;

public:
    UnaryExpression(Exp *operand, BinaryOp op, bool isPrefix = true)
        : operand(operand), op(op), isPrefix(isPrefix) {}
    ~UnaryExpression() {
        delete operand;
    }

    int accept(Visitor *visitor) override;
    bool getIsPrefix() const { return isPrefix; }
    BinaryOp getOperator() const { return op; }
    Exp *getOperand() const { return operand; }
};

class UnaryArithmeticExpression : public UnaryExpression
{
public:
    UnaryArithmeticExpression(Exp *operand, BinaryOp op, bool isPrefix = true)
        : UnaryExpression(operand, op, isPrefix) {} // op "+", "-", "++", "--"
    int accept(Visitor *visitor) override;
};

class UnaryLogicalExpression : public UnaryExpression
{
public:
    UnaryLogicalExpression(Exp *operand)
        : UnaryExpression(operand, BinaryOp::AND_OP) {} // review this "!"
    int accept(Visitor *visitor) override;
};

class PointerExpression : public UnaryExpression
{
public:
    PointerExpression(Exp *operand, BinaryOp op)
        : UnaryExpression(operand, op) {} // op "*" (dereference) or "&" (address)
    int accept(Visitor *visitor) override;
};

#endif // UNARY_EXPRESSION_H