#ifndef ASSIGNMENT_EXPRESSION_H
#define ASSIGNMENT_EXPRESSION_H

#include "expression.h"
#include <string>

using std::string;

class AssignmentExpression : public Exp
{
private:
    string id;
    Exp *expression;
    BinaryOp assignOp; // =, +=, -=, *=, /=, %=

public:
    AssignmentExpression(const string &id, Exp *exp, BinaryOp op = BinaryOp::EQ_OP)
        : id(id), expression(exp), assignOp(op) {}
    ~AssignmentExpression()
    {
        delete expression;
    }
    int accept(Visitor *visitor) override;
    string getId() const { return id; }
    Exp *getExpression() const { return expression; }
    BinaryOp getAssignOp() const { return assignOp; }
};

#endif // ASSIGNMENT_EXPRESSION_H