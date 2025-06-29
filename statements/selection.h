#ifndef SELECTION_STATEMENT_H
#define SELECTION_STATEMENT_H

#include "Statement.h"
class Exp;

class SelectionStatement : public Stmt
{
private:
    Exp *condition;
    Stmt *thenStatement;
    Stmt *elseStatement;

public:
    SelectionStatement(Exp *cond, Stmt *thenStmt, Stmt *elseStmt = nullptr)
        : condition(cond), thenStatement(thenStmt), elseStatement(elseStmt) {}
    ~SelectionStatement()
    {
        delete condition;
        delete thenStatement;
        delete elseStatement;
    }

    int accept(Visitor *visitor) override;
    Exp *getCondition() const { return condition; }
    Stmt *getThenStatement() const { return thenStatement; }
    Stmt *getElseStatement() const { return elseStatement; }
};

#endif // SELECTION_STATEMENT_H