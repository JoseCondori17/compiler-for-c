#ifndef BINARY_EXPRESSION_H
#define BINARY_EXPRESSION_H

#include "expression.h"
#include <string>

using std::string;

class BinaryExpression : public Exp
{
protected:
    Exp *left;
    Exp *right;
    BinaryOp op;

public:
    BinaryExpression(Exp *left, Exp *right, BinaryOp op)
        : left(left), right(right), op(op) {}
    ~BinaryExpression() {
        delete left;
        delete right;
    }

    int accept(Visitor *visitor) override;
    Exp *getLeft() const { return left; }
    Exp *getRight() const { return right; }
    BinaryOp getOperator() const { return op; }
};

class LogicalOrExpression : public BinaryExpression
{
public:
    LogicalOrExpression(Exp *left, Exp *right)
        : BinaryExpression(left, right, BinaryOp::OR_OP) {}
};

class LogicalAndExpression : public BinaryExpression
{
public:
    LogicalAndExpression(Exp *left, Exp *right)
        : BinaryExpression(left, right, BinaryOp::AND_OP) {}
};

class EqualityExpression : public BinaryExpression
{
public:
    EqualityExpression(Exp *left, Exp *right, BinaryOp op)
        : BinaryExpression(left, right, op) {} // op "==" or "!="
};

class RelationalExpression : public BinaryExpression
{
public:
    RelationalExpression(Exp *left, Exp *right, BinaryOp op)
        : BinaryExpression(left, right, op) {} // op "<", "<=", ">", ">="
};

class AdditiveExpression : public BinaryExpression
{
public:
    AdditiveExpression(Exp *left, Exp *right, BinaryOp op)
        : BinaryExpression(left, right, op) {} // op "+" or "-"
};

class MultiplicativeExpression : public BinaryExpression
{
public:
    MultiplicativeExpression(Exp *left, Exp *right, BinaryOp op)
        : BinaryExpression(left, right, op) {} // op "*", "/", or "%"
};

#endif // BINARY_EXPRESSION_H