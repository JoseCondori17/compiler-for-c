#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H

#include "statement.h"
class Exp;

class ReturnStatement : public Stmt
{
private:
    Exp *expression;

public:
    ReturnStatement(Exp *exp = nullptr) : expression(exp) {}
    ~ReturnStatement() { delete expression; }
    int accept(Visitor *visitor) override;
    Exp *getExpression() const { return expression; }
};

#endif // RETURN_STATEMENT_H