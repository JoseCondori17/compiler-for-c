#include "visitor.h"
#include <utility>

// BinaryExp
BinaryExp::BinaryExp(unique_ptr<Exp> left, Token::Type op, unique_ptr<Exp> right) 
    : left(std::move(left)), op(op), right(std::move(right)) {}

void BinaryExp::accept(Visitor* v) {
    v->visit(this);
}

// UnaryExp
UnaryExp::UnaryExp(Token::Type op, unique_ptr<Exp> exp) 
    : op(op), exp(std::move(exp)) {}

void UnaryExp::accept(Visitor* v) {
    v->visit(this);
}

// NumberExp
NumberExp::NumberExp(const string& value) : value(value) {}

void NumberExp::accept(Visitor* v) {
    v->visit(this);
}

// StringExp
StringExp::StringExp(const string& value) : value(value) {}

void StringExp::accept(Visitor* v) {
    v->visit(this);
}

// BoolExp
BoolExp::BoolExp(bool value) : value(value) {}

void BoolExp::accept(Visitor* v) {
    v->visit(this);
}

// IdentifierExp
IdentifierExp::IdentifierExp(const string& name) : name(name) {}

void IdentifierExp::accept(Visitor* v) {
    v->visit(this);
}

// AssignExp
AssignExp::AssignExp(const string& var, unique_ptr<Exp> exp) 
    : var(var), exp(std::move(exp)) {}

void AssignExp::accept(Visitor* v) {
    v->visit(this);
}

// FunctionCallExp
FunctionCallExp::FunctionCallExp(const string& name, vector<unique_ptr<Exp>> args) 
    : name(name), args(std::move(args)) {}

void FunctionCallExp::accept(Visitor* v) {
    v->visit(this);
}

// ArrayAccessExp
ArrayAccessExp::ArrayAccessExp(unique_ptr<Exp> array, unique_ptr<Exp> index) 
    : array(std::move(array)), index(std::move(index)) {}

void ArrayAccessExp::accept(Visitor* v) {
    v->visit(this);
}

// MemberAccessExp
MemberAccessExp::MemberAccessExp(unique_ptr<Exp> object, const string& member, bool isPointer) 
    : object(std::move(object)), member(member), isPointer(isPointer) {}

void MemberAccessExp::accept(Visitor* v) {
    v->visit(this);
}

// ConditionalExp
ConditionalExp::ConditionalExp(unique_ptr<Exp> condition, unique_ptr<Exp> trueExp, unique_ptr<Exp> falseExp)
    : condition(std::move(condition)), trueExp(std::move(trueExp)), falseExp(std::move(falseExp)) {}

void ConditionalExp::accept(Visitor* v) {
    v->visit(this);
}

// PostIncrementExp
PostIncrementExp::PostIncrementExp(unique_ptr<Exp> exp, bool isIncrement) 
    : exp(std::move(exp)), isIncrement(isIncrement) {}

void PostIncrementExp::accept(Visitor* v) {
    v->visit(this);
}
// MemberExp
MemberAssignExp::MemberAssignExp(unique_ptr<Exp> obj, unique_ptr<Exp> val)
    : object(std::move(obj)), value(std::move(val)) {}
void MemberAssignExp::accept(Visitor* v) {
    v->visit(this);
}
// ArrayAssignExp
ArrayAssignExp::ArrayAssignExp(unique_ptr<Exp> arr, unique_ptr<Exp> val)
    : array(std::move(arr)), value(std::move(val)) {}
void ArrayAssignExp::accept(Visitor* v) {
    v->visit(this);
}
// PreIncrementExp
PreIncrementExp::PreIncrementExp(unique_ptr<Exp> exp, bool isIncrement)
    : exp(std::move(exp)), isIncrement(isIncrement) {}

void PreIncrementExp::accept(Visitor* v) {
    v->visit(this);
}

// ExpressionStm
ExpressionStm::ExpressionStm(unique_ptr<Exp> exp) : exp(std::move(exp)) {}

void ExpressionStm::accept(Visitor* v) {
    v->visit(this);
}

// PrintStm
PrintStm::PrintStm(unique_ptr<Exp> exp) : exp(std::move(exp)) {}

void PrintStm::accept(Visitor* v) {
    v->visit(this);
}

// IfStm
IfStm::IfStm(unique_ptr<Exp> condition, unique_ptr<Stm> thenStm, unique_ptr<Stm> elseStm)
    : condition(std::move(condition)), thenStm(std::move(thenStm)), elseStm(std::move(elseStm)) {}

void IfStm::accept(Visitor* v) {
    v->visit(this);
}

// WhileStm
WhileStm::WhileStm(unique_ptr<Exp> condition, unique_ptr<Stm> body) 
    : condition(std::move(condition)), body(std::move(body)) {}

void WhileStm::accept(Visitor* v) {
    v->visit(this);
}

// ForStm
ForStm::ForStm(unique_ptr<Exp> init, unique_ptr<Exp> condition, unique_ptr<Exp> increment, unique_ptr<Stm> body)
    : init(std::move(init)), condition(std::move(condition)), increment(std::move(increment)), body(std::move(body)) {}

void ForStm::accept(Visitor* v) {
    v->visit(this);
}

// DoWhileStm
DoWhileStm::DoWhileStm(unique_ptr<Stm> body, unique_ptr<Exp> condition) 
    : body(std::move(body)), condition(std::move(condition)) {}

void DoWhileStm::accept(Visitor* v) {
    v->visit(this);
}

// BlockStm
BlockStm::BlockStm(vector<unique_ptr<Stm>> statements) : statements(std::move(statements)) {}

void BlockStm::accept(Visitor* v) {
    v->visit(this);
}

// ReturnStm
ReturnStm::ReturnStm(unique_ptr<Exp> exp) : exp(std::move(exp)) {}

void ReturnStm::accept(Visitor* v) {
    v->visit(this);
}

// BreakStm
BreakStm::BreakStm() {}

void BreakStm::accept(Visitor* v) {
    v->visit(this);
}

// ContinueStm
ContinueStm::ContinueStm() {}

void ContinueStm::accept(Visitor* v) {
    v->visit(this);
}

// VarDeclaration
VarDeclaration::VarDeclaration(const string& type, const string& name, unique_ptr<Exp> init) 
    : type(type), name(name), init(std::move(init)) {}

void VarDeclaration::accept(Visitor* v) {
    v->visit(this);
}

// FunctionDeclaration
FunctionDeclaration::FunctionDeclaration(const string& returnType, const string& name, 
                                       vector<std::pair<string, string>> params, unique_ptr<BlockStm> body)
    : returnType(returnType), name(name), params(std::move(params)), body(std::move(body)) {}

void FunctionDeclaration::accept(Visitor* v) {
    v->visit(this);
}
// struct declaration
StructDeclaration::StructDeclaration(const string& name, vector<std::pair<string, string>> members)
    : name(name), members(std::move(members)) {}

void StructDeclaration::accept(Visitor* v) {
    v->visit(this);
}

// Program
Program::Program(vector<unique_ptr<Stm>> declarations) : declarations(std::move(declarations)) {}

void Program::accept(Visitor* v) {
    v->visit(this);
}