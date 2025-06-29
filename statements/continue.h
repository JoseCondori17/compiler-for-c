#ifndef CONTINUE_STATEMENT_H
#define CONTINUE_STATEMENT_H

#include "statement.h"

class ContinueStatement : public Stmt
{
public:
    ContinueStatement() {}
    ~ContinueStatement() {}
    int accept(Visitor *visitor) override;
};

#endif // CONTINUE_STATEMENT_H