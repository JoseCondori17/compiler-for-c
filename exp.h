#ifndef EXP_H
#define EXP_H
#include <string>
#include <vector>
#include <memory>
#include "token.h"

using std::string;
using std::vector;
using std::unique_ptr;

class Visitor;

class Exp {
public:
    virtual ~Exp() = default;
    virtual void accept(Visitor* v) = 0;
};

class Stm {
public:
    virtual ~Stm() = default;
    virtual void accept(Visitor* v) = 0;
};

// Expressions
class BinaryExp : public Exp {
public:
    unique_ptr<Exp> left, right;
    Token::Type op;
    
    BinaryExp(unique_ptr<Exp> left, Token::Type op, unique_ptr<Exp> right);
    void accept(Visitor* v) override;
};

class UnaryExp : public Exp {
public:
    unique_ptr<Exp> exp;
    Token::Type op;
    
    UnaryExp(Token::Type op, unique_ptr<Exp> exp);
    void accept(Visitor* v) override;
};

class NumberExp : public Exp {
public:
    string value;
    
    NumberExp(const string& value);
    void accept(Visitor* v) override;
};

class StringExp : public Exp {
public:
    string value;
    
    StringExp(const string& value);
    void accept(Visitor* v) override;
};

class BoolExp : public Exp {
public:
    bool value;
    
    BoolExp(bool value);
    void accept(Visitor* v) override;
};

class IdentifierExp : public Exp {
public:
    string name;
    
    IdentifierExp(const string& name);
    void accept(Visitor* v) override;
};

class AssignExp : public Exp {
public:
    string var;
    unique_ptr<Exp> exp;
    
    AssignExp(const string& var, unique_ptr<Exp> exp);
    void accept(Visitor* v) override;
};

class FunctionCallExp : public Exp {
public:
    string name;
    vector<unique_ptr<Exp>> args;
    
    FunctionCallExp(const string& name, vector<unique_ptr<Exp>> args);
    void accept(Visitor* v) override;
};

class ArrayAccessExp : public Exp {
public:
    unique_ptr<Exp> array;
    unique_ptr<Exp> index;
    
    ArrayAccessExp(unique_ptr<Exp> array, unique_ptr<Exp> index);
    void accept(Visitor* v) override;
};

class MemberAccessExp : public Exp {
public:
    unique_ptr<Exp> object;
    string member;
    bool isPointer; // true for ->, false for .
    
    MemberAccessExp(unique_ptr<Exp> object, const string& member, bool isPointer);
    void accept(Visitor* v) override;
};

class ConditionalExp : public Exp {
public:
    unique_ptr<Exp> condition;
    unique_ptr<Exp> trueExp;
    unique_ptr<Exp> falseExp;
    
    ConditionalExp(unique_ptr<Exp> condition, unique_ptr<Exp> trueExp, unique_ptr<Exp> falseExp);
    void accept(Visitor* v) override;
};

class PostIncrementExp : public Exp {
public:
    unique_ptr<Exp> exp;
    bool isIncrement; // true for ++, false for --
    
    PostIncrementExp(unique_ptr<Exp> exp, bool isIncrement);
    void accept(Visitor* v) override;
};

class MemberAssignExp : public Exp {
public:
    unique_ptr<Exp> object;
    unique_ptr<Exp> value;
    
    MemberAssignExp(unique_ptr<Exp> obj, unique_ptr<Exp> val);
    void accept(Visitor *v) override;
};

class ArrayAssignExp : public Exp {
public:
    unique_ptr<Exp> array;
    unique_ptr<Exp> value;
    
    ArrayAssignExp(unique_ptr<Exp> arr, unique_ptr<Exp> val);
    void accept(Visitor *v) override;
};

class PreIncrementExp : public Exp {
public:
    unique_ptr<Exp> exp;
    bool isIncrement; // true for ++, false for --

    PreIncrementExp(unique_ptr<Exp> exp, bool isIncrement);
    void accept(Visitor* v) override;
};

// Statements
class ExpressionStm : public Stm {
public:
    unique_ptr<Exp> exp;
    
    ExpressionStm(unique_ptr<Exp> exp);
    void accept(Visitor* v) override;
};

class PrintStm : public Stm {
public:
    unique_ptr<Exp> exp;
    
    PrintStm(unique_ptr<Exp> exp);
    void accept(Visitor* v) override;
};

class IfStm : public Stm {
public:
    unique_ptr<Exp> condition;
    unique_ptr<Stm> thenStm;
    unique_ptr<Stm> elseStm;
    
    IfStm(unique_ptr<Exp> condition, unique_ptr<Stm> thenStm, unique_ptr<Stm> elseStm = nullptr);
    void accept(Visitor* v) override;
};

class WhileStm : public Stm {
public:
    unique_ptr<Exp> condition;
    unique_ptr<Stm> body;
    
    WhileStm(unique_ptr<Exp> condition, unique_ptr<Stm> body);
    void accept(Visitor* v) override;
};

class ForStm : public Stm {
public:
    unique_ptr<Exp> init;
    unique_ptr<Exp> condition;
    unique_ptr<Exp> increment;
    unique_ptr<Stm> body;
    
    ForStm(unique_ptr<Exp> init, unique_ptr<Exp> condition, unique_ptr<Exp> increment, unique_ptr<Stm> body);
    void accept(Visitor* v) override;
};

class DoWhileStm : public Stm {
public:
    unique_ptr<Stm> body;
    unique_ptr<Exp> condition;
    
    DoWhileStm(unique_ptr<Stm> body, unique_ptr<Exp> condition);
    void accept(Visitor* v) override;
};

class BlockStm : public Stm {
public:
    vector<unique_ptr<Stm>> statements;
    
    BlockStm(vector<unique_ptr<Stm>> statements);
    void accept(Visitor* v) override;
};

class ReturnStm : public Stm {
public:
    unique_ptr<Exp> exp;
    
    ReturnStm(unique_ptr<Exp> exp = nullptr);
    void accept(Visitor* v) override;
};

class BreakStm : public Stm {
public:
    BreakStm();
    void accept(Visitor* v) override;
};

class ContinueStm : public Stm {
public:
    ContinueStm();
    void accept(Visitor* v) override;
};

class VarDeclaration : public Stm {
public:
    string type;
    string name;
    unique_ptr<Exp> init;
    
    VarDeclaration(const string& type, const string& name, unique_ptr<Exp> init = nullptr);
    void accept(Visitor* v) override;
};

class FunctionDeclaration : public Stm {
public:
    string returnType;
    string name;
    vector<std::pair<string, string>> params; // (type, name) pairs
    unique_ptr<BlockStm> body;
    
    FunctionDeclaration(const string& returnType, const string& name, 
                       vector<std::pair<string, string>> params, unique_ptr<BlockStm> body);
    void accept(Visitor* v) override;
};


class StructDeclaration : public Stm {
public:
    string name;
    vector<std::pair<string, string>> members; // (type, name) pairs

    StructDeclaration(const string &name, vector<std::pair<string, string>> members);
    void accept(Visitor* v) override;
};

class Program : public Stm {
public:
    vector<unique_ptr<Stm>> declarations;
    
    Program(vector<unique_ptr<Stm>> declarations);
    void accept(Visitor* v) override;
};

#endif // EXP_H