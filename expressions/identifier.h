#ifndef IDENTIFIER_EXPRESSION_H
#define IDENTIFIER_EXPRESSION_H

#include "expression.h"
#include <string>

class IdentifierExpression : public Exp
{
private:
    string identifier;

public:
    IdentifierExpression(const string &id) : identifier(id) {}
    int accept(Visitor *visitor) override;
    string getIdentifier() const { return identifier; }
};

#endif // IDENTIFIER_EXPRESSION_H