#ifndef CONDITIONAL_EXPRESSION_H
#define CONDITIONAL_EXPRESSION_H

#include "expression.h"

class ConditionalExpression : public Exp
{
private:
    Exp *condition;
    Exp *trueExpression;
    Exp *falseExpression;

public:
    ConditionalExpression(Exp *cond, Exp *trueExp, Exp *falseExp)
        : condition(cond), trueExpression(trueExp), falseExpression(falseExp) {}
    ~ConditionalExpression() {
        delete condition;
        delete trueExpression;
        delete falseExpression;
    }

    int accept(Visitor *visitor) override;
    Exp *getCondition() const { return condition; }
    Exp *getTrueExpression() const { return trueExpression; }
    Exp *getFalseExpression() const { return falseExpression; }
};

#endif // CONDITIONAL_EXPRESSION_H