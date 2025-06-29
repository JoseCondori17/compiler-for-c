#ifndef BREAK_STATEMENT_H
#define BREAK_STATEMENT_H

#include "statement.h"

class BreakStatement : public Stmt
{
public:
    BreakStatement() {}
    ~BreakStatement() {}
    int accept(Visitor *visitor) override;
};

#endif // BREAK_STATEMENT_H