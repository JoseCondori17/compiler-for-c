#ifndef EXPRESSION_STATEMENT_H
#define EXPRESSION_STATEMENT_H

#include "statement.h"
class Exp;

class ExpressionStatement : public Stmt
{
private:
    Exp *expression;

public:
    ExpressionStatement(Exp *exp = nullptr) : expression(exp) {}
    ~ExpressionStatement() { delete expression; }
    int accept(Visitor *visitor) override;
    Exp *getExpression() const { return expression; }
};

#endif // EXPRESSION_STATEMENT_H