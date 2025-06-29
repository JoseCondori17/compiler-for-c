#ifndef FOR_STATEMENT_H
#define FOR_STATEMENT_H

#include "statement.h"
class Exp;

class ForStatement : public Stmt
{
private:
    Exp *init;
    Exp *condition;
    Exp *update;
    Stmt *body;

public:
    ForStatement(Exp *init, Exp *cond, Exp *update, Stmt *body)
        : init(init), condition(cond), update(update), body(body) {}
    ~ForStatement()
    {
        delete init;
        delete condition;
        delete update;
        delete body;
    }

    int accept(Visitor *visitor) override;
    Exp *getInit() const { return init; }
    Exp *getCondition() const { return condition; }
    Exp *getUpdate() const { return update; }
    Stmt *getBody() const { return body; }
};

#endif // FOR_STATEMENT_H