#ifndef PARAMETER_DECLARATION_H
#define PARAMETER_DECLARATION_H

#include "declaration.h"
#include <string>

using std::string;

class Type;

class ParameterDeclaration : public Declaration
{
private:
    Type *type;
    string identifier;

public:
    ParameterDeclaration(Type *type, const string &id) : type(type), identifier(id) {}
    ~ParameterDeclaration() { delete type; }

    Type *getType() const { return type; }
    string getIdentifier() const { return identifier; }
};

#endif // PARAMETER_DECLARATION_H