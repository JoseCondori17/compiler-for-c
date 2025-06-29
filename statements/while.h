#ifndef WHILE_STATEMENT_H
#define WHILE_STATEMENT_H

#include "statement.h"
class Exp;

class WhileStatement : public Stmt
{
private:
    Exp *condition;
    Stmt *body;

public:
    WhileStatement(Exp *cond, Stmt *body) : condition(cond), body(body) {}
    ~WhileStatement()
    {
        delete condition;
        delete body;
    }

    int accept(Visitor *visitor) override;
    Exp *getCondition() const { return condition; }
    Stmt *getBody() const { return body; }
};

#endif // WHILE_STATEMENT_H