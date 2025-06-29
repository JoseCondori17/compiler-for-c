#ifndef VARIABLE_DECLARATION_H
#define VARIABLE_DECLARATION_H

#include "declaration.h"
#include <vector>
#include <string>

using std::string;
using std::vector;

class Type;

class VariableDeclaration : public Declaration
{
private:
    Type *type;
    vector<string> identifiers;

public:
    VariableDeclaration(Type *type) : type(type) {}
    ~VariableDeclaration() { delete type; }

    void addIdentifier(const string &id) { identifiers.push_back(id); }
    Type *getType() const { return type; }
    vector<string> &getIdentifiers() { return identifiers; }
};

#endif // VARIABLE_DECLARATION_H