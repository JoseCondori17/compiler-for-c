#ifndef FUNCTION_DECLARATION_H
#define FUNCTION_DECLARATION_H

#include "declaration.h"
#include <vector>
#include <string>

using std::string;
using std::vector;

class Type;
class ParameterDeclaration;
class BodyStatement;

class FunctionDeclaration : public Declaration
{
private:
    Type *returnType;
    string identifier;
    vector<ParameterDeclaration *> parameters;
    BodyStatement *body;

public:
    FunctionDeclaration(Type *returnType, const string &id, BodyStatement *body)
        : returnType(returnType), identifier(id), body(body) {}
    ~FunctionDeclaration() {
        delete returnType;
        for (auto param : parameters) {
            delete param;
        }
        delete body;
    }

    void addParameter(ParameterDeclaration *param) { parameters.push_back(param); }
    Type *getReturnType() const { return returnType; }
    string getIdentifier() const { return identifier; }
    vector<ParameterDeclaration *> &getParameters() { return parameters; }
    BodyStatement *getBody() const { return body; }
};

#endif // FUNCTION_DECLARATION_H