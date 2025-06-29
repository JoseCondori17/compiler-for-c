#ifndef POSTFIX_EXPRESSION_H
#define POSTFIX_EXPRESSION_H

#include "expression.h"
#include <vector>
using std::vector;

class PostfixExpression : public Exp
{
private:
    Exp *primary;
    vector<Exp *> postfixOps;

public:
    PostfixExpression(Exp *primary) : primary(primary) {}
    ~PostfixExpression() {
        delete primary;
        for (auto op : postfixOps) {
            delete op;
        }
    }

    int accept(Visitor *visitor) override;
    void addPostfixOp(Exp *op) { postfixOps.push_back(op); }
    Exp *getPrimary() const { return primary; }
    vector<Exp *> &getPostfixOps() { return postfixOps; }
};

class ArrayAccessExpression : public Exp
{
private:
    Exp *array;
    Exp *index;

public:
    ArrayAccessExpression(Exp *array, Exp *index) : array(array), index(index) {}
    ~ArrayAccessExpression()
    {
        delete array;
        delete index;
    }

    int accept(Visitor *visitor) override;
    Exp *getArray() const { return array; }
    Exp *getIndex() const { return index; }
};

class FunctionCallExpression : public Exp
{
private:
    string functionName;
    vector<Exp *> arguments;

public:
    FunctionCallExpression(const string &name) : functionName(name) {}
    ~FunctionCallExpression()
    {
        for (auto arg : arguments)
        {
            delete arg;
        }
    }

    int accept(Visitor *visitor) override;
    void addArgument(Exp *arg) { arguments.push_back(arg); }
    string getFunctionName() const { return functionName; }
    const vector<Exp *> &getArguments() const { return arguments; }
};

class MemberAccessExpression : public Exp
{
private:
    Exp *object;
    string member;
    bool isPointer; // true for ->, false for .

public:
    MemberAccessExpression(Exp *obj, const string &member, bool isPointer)
        : object(obj), member(member), isPointer(isPointer) {}
    ~MemberAccessExpression()
    {
        delete object;
    }

    int accept(Visitor *visitor) override;
    Exp *getObject() const { return object; }
    string getMember() const { return member; }
    bool getIsPointer() const { return isPointer; }
};

class LiteralExpression : public Exp
{
private:
    string value;
    string type; // "int", "float", "char", "string"

public:
    LiteralExpression(const string &value, const string &type)
        : value(value), type(type) {}
    int accept(Visitor *visitor) override;
    string getValue() const { return value; }
    string getType() const { return type; }
};
    
class ParenthesizedExpression : public Exp
{
private:
    Exp *expression;

public:
    ParenthesizedExpression(Exp *exp) : expression(exp) {}
    ~ParenthesizedExpression() { delete expression; }
    int accept(Visitor *visitor) override;
    Exp *getExpression() const { return expression; }
};

#endif // POSTFIX_EXPRESSION_H