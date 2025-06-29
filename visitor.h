#ifndef VISITOR_H
#define VISITOR_H

class Program;

// =========== STATEMENTS ============== //
class ReturnStatement;
class ContinueStatement;
class ExpressionStatement;
class SelectionStatement;
class WhileStatement;
class ForStatement;
class BreakStatement;
class BodyStatement;

// =========== EXPRESSIONS ============== //
class BinaryExpression;
class ConditionalExpression;
class UnaryExpression;
class AssignmentExpression;
class IdentifierExpression;
class PostfixExpression;
class ArrayAccessExpression;
class FunctionCallExpression;
class MemberAccessExpression;
class LiteralExpression;
class ParenthesizedExpression;

class Visitor
{
public:
    virtual int visit(BinaryExpression *exp) = 0;
    virtual int visit(ConditionalExpression *exp) = 0;
    virtual int visit(UnaryExpression *exp) = 0;
    virtual int visit(AssignmentExpression *exp) = 0;
    virtual int visit(IdentifierExpression *exp) = 0;
    virtual int visit(PostfixExpression *exp) = 0;
    virtual int visit(ArrayAccessExpression *exp) = 0;
    virtual int visit(FunctionCallExpression *exp) = 0;
    virtual int visit(MemberAccessExpression *exp) = 0;
    virtual int visit(LiteralExpression *exp) = 0;
    virtual int visit(ParenthesizedExpression *exp) = 0;

    virtual void visit(ReturnStatement *stmt) = 0;
    virtual void visit(ContinueStatement *stmt) = 0;
    virtual void visit(ExpressionStatement *stmt) = 0;
    virtual void visit(SelectionStatement *stmt) = 0;
    virtual void visit(WhileStatement *stmt) = 0;
    virtual void visit(ForStatement *stmt) = 0;
    virtual void visit(BreakStatement *stmt) = 0;
    virtual void visit(BodyStatement *stmt) = 0;

    virtual void visit(Program *program) = 0;
};

#endif // VISITOR_H