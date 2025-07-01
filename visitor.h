#ifndef VISITOR_H
#define VISITOR_H
#include "exp.h"
#include "environment.h"
#include <iostream>
#include <unordered_map>
#include <map>
#include <sstream>
#include <string>
#include <stack>

// Visitor pattern
class Visitor
{
public:
    virtual ~Visitor() = default;

    // Expression visitors
    virtual void visit(BinaryExp *exp) = 0;
    virtual void visit(UnaryExp *exp) = 0;
    virtual void visit(NumberExp *exp) = 0;
    virtual void visit(StringExp *exp) = 0;
    virtual void visit(BoolExp *exp) = 0;
    virtual void visit(IdentifierExp *exp) = 0;
    virtual void visit(AssignExp *exp) = 0;
    virtual void visit(FunctionCallExp *exp) = 0;
    virtual void visit(ArrayAccessExp *exp) = 0;
    virtual void visit(MemberAccessExp *exp) = 0;
    virtual void visit(ConditionalExp *exp) = 0;
    virtual void visit(MemberAssignExp *exp) = 0;
    virtual void visit(ArrayAssignExp *exp) = 0;
    virtual void visit(PostIncrementExp *exp) = 0;
    virtual void visit(PreIncrementExp *exp) = 0;

    // Statement visitors
    virtual void visit(ExpressionStm *stm) = 0;
    virtual void visit(PrintStm *stm) = 0;
    virtual void visit(IfStm *stm) = 0;
    virtual void visit(WhileStm *stm) = 0;
    virtual void visit(ForStm *stm) = 0;
    virtual void visit(DoWhileStm *stm) = 0;
    virtual void visit(BlockStm *stm) = 0;
    virtual void visit(ReturnStm *stm) = 0;
    virtual void visit(BreakStm *stm) = 0;
    virtual void visit(ContinueStm *stm) = 0;
    virtual void visit(VarDeclaration *stm) = 0;
    virtual void visit(FunctionDeclaration *stm) = 0;
    virtual void visit(StructDeclaration *stm) = 0;
    virtual void visit(Program *stm) = 0;
};

class PrintVisitor : public Visitor
{
private:
    int nivel_indentacion = 0;
    std::ostringstream output;

    std::string getIndent();

public:
    // Expressions
    void visit(BinaryExp *exp) override;
    void visit(UnaryExp *exp) override;
    void visit(NumberExp *exp) override;
    void visit(StringExp *exp) override;
    void visit(BoolExp *exp) override;
    void visit(IdentifierExp *exp) override;
    void visit(AssignExp *exp) override;
    void visit(FunctionCallExp *exp) override;
    void visit(ArrayAccessExp *exp) override;
    void visit(MemberAccessExp *exp) override;
    void visit(ConditionalExp *exp) override;
    void visit(MemberAssignExp *exp) override;
    void visit(ArrayAssignExp *exp) override;
    void visit(PostIncrementExp *exp) override;
    void visit(PreIncrementExp *exp) override;

    // Statements
    void visit(ExpressionStm *stm) override;
    void visit(PrintStm *stm) override;
    void visit(IfStm *stm) override;
    void visit(WhileStm *stm) override;
    void visit(ForStm *stm) override;
    void visit(DoWhileStm *stm) override;
    void visit(BlockStm *stm) override;
    void visit(ReturnStm *stm) override;
    void visit(BreakStm *stm) override;
    void visit(ContinueStm *stm) override;
    void visit(VarDeclaration *stm) override;
    void visit(FunctionDeclaration *stm) override;
    void visit(StructDeclaration *stm) override;
    void visit(Program *stm) override;

    std::string getOutput();
    void imprimir(Program *program);
};

class TypeVisitor : public Visitor
{
private:
    Environment env;
    std::unordered_map<std::string, FunctionDeclaration *> fdecs;
    std::unordered_map<std::string, StructDeclaration *> struct_registry;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> struct_fields;

    string currFun;

    // almacenar tipos de expresiones
    unordered_map<Exp *, string> expTypes;

    void setExpType(Exp *e, const string &type);
    string getExpType(Exp *e);
    bool isNumericType(const string &type);
    bool isIntegerType(const string &type);
    bool isAssignmentCompatible(const string &target, const string &source);

    string extractStructName(const string &type);
    bool isStructType(const string &type);

public:
    TypeVisitor() = default;

    // Expressions
    void visit(BinaryExp *e) override;
    void visit(UnaryExp *e) override;
    void visit(NumberExp *e) override;
    void visit(StringExp *e) override;
    void visit(BoolExp *e) override;
    void visit(IdentifierExp *e) override;
    void visit(AssignExp *e) override;
    void visit(FunctionCallExp *e) override;
    void visit(ArrayAccessExp *e) override;
    void visit(MemberAccessExp *e) override;
    void visit(ConditionalExp *e) override;
    void visit(PostIncrementExp *e) override;
    void visit(PreIncrementExp *e) override;
    void visit(MemberAssignExp *e) override;
    void visit(ArrayAssignExp *e) override;

    // Statements
    void visit(ExpressionStm *s) override;
    void visit(PrintStm *s) override;
    void visit(IfStm *s) override;
    void visit(WhileStm *s) override;
    void visit(ForStm *s) override;
    void visit(DoWhileStm *s) override;
    void visit(BlockStm *s) override;
    void visit(ReturnStm *s) override;
    void visit(BreakStm *s) override;
    void visit(ContinueStm *s) override;

    // Declarations
    void visit(VarDeclaration *s) override;
    void visit(FunctionDeclaration *s) override;
    void visit(StructDeclaration *s) override;
    void visit(Program *p) override;
};

struct LoopContext {
    string break_label;
    string continue_label;
};

class GenCodeVisitor : public Visitor {
private:
    Environment env;
    ostream* output;
    int label_counter;
    int stack_offset;
    unordered_map<string, int> var_offsets; // Variable name -> stack offset
    stack<LoopContext> loop_contexts; // Para manejar break/continue
    
    string next_label();
    void push_loop_context(const string& break_label, const string& continue_label);
    void pop_loop_context();
    string get_break_label();
    string get_continue_label();

public:
    GenCodeVisitor(ostream* output);
    
    // Expression visitors
    void visit(BinaryExp* exp) override;
    void visit(UnaryExp* exp) override;
    void visit(NumberExp* exp) override;
    void visit(StringExp* exp) override;
    void visit(BoolExp* exp) override;
    void visit(IdentifierExp* exp) override;
    void visit(AssignExp* exp) override;
    void visit(FunctionCallExp* exp) override;
    void visit(ArrayAccessExp* exp) override;
    void visit(MemberAccessExp* exp) override;
    void visit(ConditionalExp* exp) override;
    void visit(MemberAssignExp* exp) override;
    void visit(ArrayAssignExp* exp) override;
    void visit(PostIncrementExp* exp) override;
    void visit(PreIncrementExp* exp) override;
    
    // Statement visitors
    void visit(ExpressionStm* stm) override;
    void visit(PrintStm* stm) override;
    void visit(IfStm* stm) override;
    void visit(WhileStm* stm) override;
    void visit(ForStm* stm) override;
    void visit(DoWhileStm* stm) override;
    void visit(BlockStm* stm) override;
    void visit(ReturnStm* stm) override;
    void visit(BreakStm* stm) override;
    void visit(ContinueStm* stm) override;
    void visit(VarDeclaration* stm) override;
    void visit(FunctionDeclaration* stm) override;
    void visit(StructDeclaration* stm) override;
    void visit(Program* stm) override;
    
    void generate(Program* p);
};
#endif
