#include "expressions/module.h"
#include "statements/module.h"
#include "visitor.h"

int BinaryExpression::accept(Visitor *visitor) { return visitor->visit(this); }
int ConditionalExpression::accept(Visitor *visitor) { return visitor->visit(this); }
int UnaryExpression::accept(Visitor *visitor) { return visitor->visit(this); }
int AssignmentExpression::accept(Visitor *visitor) { return visitor->visit(this); }
int IdentifierExpression::accept(Visitor *visitor) { return visitor->visit(this); }
int PostfixExpression::accept(Visitor *visitor) { return visitor->visit(this); }
int ArrayAccessExpression::accept(Visitor *visitor) { return visitor->visit(this); }
int FunctionCallExpression::accept(Visitor *visitor) { return visitor->visit(this); }
int MemberAccessExpression::accept(Visitor *visitor) { return visitor->visit(this); }
int LiteralExpression::accept(Visitor *visitor) { return visitor->visit(this); }
int ParenthesizedExpression::accept(Visitor *visitor) { return visitor->visit(this); }

int ReturnStatement::accept(Visitor *visitor) { visitor->visit(this); return 0; }
int ContinueStatement::accept(Visitor *visitor) { visitor->visit(this); return 0; }
int ExpressionStatement::accept(Visitor *visitor) { visitor->visit(this); return 0; }
int SelectionStatement::accept(Visitor *visitor) { visitor->visit(this); return 0; }
int WhileStatement::accept(Visitor *visitor) { visitor->visit(this); return 0; }
int ForStatement::accept(Visitor *visitor) { visitor->visit(this); return 0; }
int BreakStatement::accept(Visitor *visitor) { visitor->visit(this); return 0; }
int BodyStatement::accept(Visitor *visitor) { visitor->visit(this); return 0; }

