#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include "token.h"
#include "expressions/module.h"
#include "statements/module.h"
#include "declarations/module.h"

class Parser
{
private:
    Scanner *scanner;
    Token *current, *previous;
    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();
    void expect(Token::Type type, const string &message);

    Type *parseType();
    Type *parseBasicType();
    int parsePointerLevel();

    Declaration *parseDeclaration();
    VariableDeclaration *parseVariableDeclaration();
    FunctionDeclaration *parseFunctionDeclaration();
    StructDeclaration *parseStructDeclaration();
    ParameterDeclaration *parseParameterDeclaration();
    vector<ParameterDeclaration *> parseParameterList();

    Stmt *parseStatement();
    Stmt *parseExpressionStatement();
    Stmt *parseBodyStatement();
    Stmt *parseSelectionStatement();
    Stmt *parseWhileStatement();
    Stmt *parseForStatement();
    Stmt *parseReturnStatement();
    Stmt *parseBreakStatement();
    Stmt *parseContinueStatement();

    Exp *parseExpression();
    Exp *parseAssignmentExpression();
    Exp *parseConditionalExpression();
    Exp *parseLogicalOrExpression();
    Exp *parseLogicalAndExpression();
    Exp *parseEqualityExpression();
    Exp *parseRelationalExpression();
    Exp *parseAddExpression();
    Exp *parseMultExpression();
    Exp *parsePostfixExpression();
    Exp *parseUnaryExpression();
    Exp *parsePrimaryExpression();

    vector<Exp *> parseArgumentList();

public:
    Parser(Scanner *scanner);
};
#endif // PARSER_H