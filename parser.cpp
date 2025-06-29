#include <iostream>
#include <stdexcept>
#include <vector>
#include "token.h"
#include "scanner.h"
#include "parser.h"

using std::cout;
using std::endl;
using std::runtime_error;
using std::vector;

bool Parser::match(Token::Type ttype)
{
    if (check(ttype))
    {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype)
{
    if (isAtEnd())
        return false;
    return current->type == ttype;
}

bool Parser::advance()
{
    if (!isAtEnd())
    {
        Token *temp = current;
        if (previous)
            delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERROR))
        {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd()
{
    return (current->type == Token::END);
}

void Parser::expect(Token::Type type, const string &message)
{
    if (check(type))
    {
        advance();
        return;
    }
    throw runtime_error("Parse error: " + message);
}

/* ########################################################## LOGICA ########################################################## */
Declaration *Parser::parseDeclaration()
{
    if (match(Token::STRUCT))
    {
        return parseStructDeclaration();
    }

    Token *savePoint = current;
    Type *type = parseType();

    if (check(Token::IDENTIFIER))
    {
        string id = current->text;
        advance();

        if (check(Token::PL))
        {
            current = savePoint; // reset
            return parseFunctionDeclaration();
        }
        else
        {
            current = savePoint; // reset
            return parseVariableDeclaration();
        }
    }

    delete type;
    return nullptr;
}

VariableDeclaration *Parser::parseVariableDeclaration()
{
    Type *type = parseType();
    VariableDeclaration *varDecl = new VariableDeclaration(type);

    expect(Token::IDENTIFIER, "Expected variable name");
    varDecl->addIdentifier(previous->text);

    while (match(Token::COMMA))
    {
        expect(Token::IDENTIFIER, "Expected variable name after comma");
        varDecl->addIdentifier(previous->text);
    }

    expect(Token::SEMICOLON, "Expected ';' after variable declaration");
    return varDecl;
}

FunctionDeclaration *Parser::parseFunctionDeclaration()
{
    Type *returnType = parseType();

    expect(Token::IDENTIFIER, "Expected function name");
    string functionName = previous->text;

    expect(Token::PL, "Expected '(' after function name");

    vector<ParameterDeclaration *> parameters;
    if (!check(Token::PR))
    {
        parameters = parseParameterList();
    }

    expect(Token::PR, "Expected ')' after parameters");

    Stmt *body = parseBodyStatement();

    FunctionDeclaration *funcDecl = new FunctionDeclaration(returnType, functionName, dynamic_cast<BodyStatement *>(body));
    for (auto param : parameters)
    {
        funcDecl->addParameter(param);
    }

    return funcDecl;
}

StructDeclaration *Parser::parseStructDeclaration()
{
    expect(Token::IDENTIFIER, "Expected struct name");
    string structName = previous->text;

    StructDeclaration *structDecl = new StructDeclaration(structName);

    expect(Token::BCL, "Expected '{' after struct name");

    while (!check(Token::BCR) && !isAtEnd())
    {
        VariableDeclaration *member = parseVariableDeclaration();
        structDecl->addMember(member);
    }

    expect(Token::BCR, "Expected '}' after struct members");
    expect(Token::SEMICOLON, "Expected ';' after struct declaration");

    return structDecl;
}

vector<ParameterDeclaration *> Parser::parseParameterList()
{
    vector<ParameterDeclaration *> parameters;

    parameters.push_back(parseParameterDeclaration());

    while (match(Token::COMMA))
    {
        parameters.push_back(parseParameterDeclaration());
    }

    return parameters;
}

ParameterDeclaration *Parser::parseParameterDeclaration()
{
    Type *type = parseType();
    expect(Token::IDENTIFIER, "Expected parameter name");
    string paramName = previous->text;

    return new ParameterDeclaration(type, paramName);
}

// ################################### TYPE ########################################

Type *Parser::parseType()
{
    Type *baseType = parseBasicType();
    int pointerLevel = parsePointerLevel();
    return new Type(baseType->getBaseType(), pointerLevel, baseType->getIsStruct());
}

Type *Parser::parseBasicType()
{
    if (match(Token::INT))
    {
        return new Type("int");
    }
    else if (match(Token::FLOAT))
    {
        return new Type("float");
    }
    else if (match(Token::CHAR))
    {
        return new Type("char");
    }
    else if (match(Token::VOID))
    {
        return new Type("void");
    }
    else if (match(Token::STRUCT))
    {
        expect(Token::IDENTIFIER, "Expected struct name");
        return new Type(previous->text, 0, true);
    }
}

int Parser::parsePointerLevel()
{
    int level = 0;
    while (match(Token::ASTERISK))
    {
        level++;
    }
    return level;
}
/* ########################################################## STATEMENTS ########################################################## */
Stmt *Parser::parseStatement()
{
    if (check(Token::BCL))
    {
        return parseBodyStatement();
    }
    else if (match(Token::IF))
    {
        return parseSelectionStatement();
    }
    else if (match(Token::WHILE))
    {
        return parseWhileStatement();
    }
    else if (match(Token::FOR))
    {
        return parseForStatement();
    }
    else if (match(Token::RETURN))
    {
        return parseReturnStatement();
    }
    else if (match(Token::BREAK))
    {
        return parseBreakStatement();
    }
    else if (match(Token::CONTINUE))
    {
        return parseContinueStatement();
    }
    else
    {
        return parseExpressionStatement();
    }
}

Stmt *Parser::parseExpressionStatement()
{
    Exp *expr = nullptr;
    if (!check(Token::SEMICOLON))
    {
        expr = parseExpression();
    }
    expect(Token::SEMICOLON, "Expected ';' after expression");
    return new ExpressionStatement(expr);
}

Stmt *Parser::parseBodyStatement()
{
    expect(Token::BCL, "Expected ';' after expression");
    BodyStatement *body = new BodyStatement();
    while (!check(Token::BCR) && !isAtEnd()) {
        Token *savePoint = current;
        try {
            Declaration *decl = parseDeclaration();
            if (decl != nullptr) {
                //body->addItem(decl);
                continue;
            }
        }
        catch (...) {
            current = savePoint; // reset
        }

        Stmt *stmt = parseStatement();
        //body->addItem(stmt);
    }
    return body;
}

Stmt *Parser::parseSelectionStatement()
{
    expect(Token::PL, "Expected '(' after if");
    Exp *condition = parseExpression();
    expect(Token::PR, "Expected ')' after if condition");
    Stmt *thenStmt = parseStatement();
    Stmt *elseStmt = nullptr;
    if (match(Token::ELSE))
    {
        elseStmt = parseStatement();
    }

    return new SelectionStatement(condition, thenStmt, elseStmt);
}

Stmt *Parser::parseWhileStatement()
{
    expect(Token::PL, "Expected '(' after while");
    Exp *condition = parseExpression();
    expect(Token::PR, "Expected ')' after while condition");
    Stmt *body = parseStatement();

    return new WhileStatement(condition, body);
}

Stmt *Parser::parseForStatement()
{
    expect(Token::PL, "Expected '(' after for");
    Exp *init = nullptr;
    if (!check(Token::SEMICOLON))
    {
        init = parseExpression();
    }
    expect(Token::SEMICOLON, "Expected ';' after for init");

    Exp *condition = nullptr;
    if (!check(Token::SEMICOLON))
    {
        init = parseExpression();
    }
    expect(Token::SEMICOLON, "Expected ';' after for condition");

    Exp *update = nullptr;
    if (!check(Token::SEMICOLON))
    {
        init = parseExpression();
    }
    expect(Token::PR, "Expected '(' after for condition");
    Stmt *body = parseStatement();

    return new ForStatement(init, condition, update, body);
}

Stmt *Parser::parseReturnStatement()
{
    Exp *expr = nullptr;
    if (!check(Token::SEMICOLON))
    {
        expr = parseExpression();
    }
    expect(Token::SEMICOLON, "Expected ';' after return");
    return new ReturnStatement(expr);
}

Stmt *Parser::parseBreakStatement()
{
    expect(Token::SEMICOLON, "Expected ';' after 'break'");
    return new BreakStatement();
}

Stmt *Parser::parseContinueStatement()
{
    expect(Token::SEMICOLON, "Expected ';' after 'continue'");
    return new ContinueStatement();
}

/* ########################################################## EXPRESSIONS ########################################################## */
Exp *Parser::parseExpression()
{
    return parseAssignmentExpression();
}

Exp *Parser::parseAssignmentExpression()
{
    Exp *expr = parseConditionalExpression();
    if (match(Token::ASSIGNMENT) ||
        match(Token::ADD_ASSIGN) ||
        match(Token::SUB_ASSIGN) ||
        match(Token::MULT_ASSIGN) ||
        match(Token::DIV_ASSIGN) ||
        match(Token::MOD_ASSIGN))
    {
        BinaryOp op;
        switch (previous->type)
        {
        case Token::ASSIGNMENT:
            op = BinaryOp::ASSIGN_OP;
            break;
        case Token::ADD_ASSIGN:
            op = BinaryOp::ADD_ASSIGN_OP;
            break;
        case Token::SUB_ASSIGN:
            op = BinaryOp::SUB_ASSIGN_OP;
            break;
        case Token::MULT_ASSIGN:
            op = BinaryOp::MUL_ASSIGN_OP;
            break;
        case Token::DIV_ASSIGN:
            op = BinaryOp::DIV_ASSIGN_OP;
            break;
        case Token::MOD_ASSIGN:
            op = BinaryOp::MOD_ASSIGN_OP;
            break;
        default:
            break;
        }
        IdentifierExpression *id = dynamic_cast<IdentifierExpression *>(expr);
        if (id == nullptr)
        {
            throw runtime_error("Invalid value in assignment");
        }
        Exp *right = parseAssignmentExpression();
        return new AssignmentExpression(id->getIdentifier(), right, op);
    }
    return expr;
}

Exp *Parser::parseConditionalExpression()
{
    Exp *expr = parseLogicalOrExpression();

    if (match(Token::QUESTION))
    {
        Exp *trueExpr = parseExpression();
        expect(Token::COLON, "Expected ':' in ternary expression");
        Exp *falseExpr = parseConditionalExpression();
        return new ConditionalExpression(expr, trueExpr, falseExpr);
    }

    return expr;
}

Exp *Parser::parseLogicalOrExpression()
{
    Exp *expr = parseLogicalAndExpression();

    while (match(Token::OR))
    {
        Exp *right = parseLogicalAndExpression();
        expr = new LogicalOrExpression(expr, right);
    }

    return expr;
}

Exp *Parser::parseLogicalAndExpression()
{
    Exp *expr = parseEqualityExpression();

    while (match(Token::AND))
    {
        Exp *right = parseEqualityExpression();
        expr = new LogicalAndExpression(expr, right);
    }

    return expr;
}

Exp *Parser::parseEqualityExpression()
{
    Exp *expr = parseRelationalExpression();

    while (match(Token::EQ) || match(Token::NEQ))
    {
        BinaryOp op;
        if (previous->type == Token::EQ)
        {
            op = BinaryOp::EQ_OP;
        }
        else if (previous->type == Token::NEQ)
        {
            op = BinaryOp::NEQ_OP;
        }
        Exp *right = parseRelationalExpression();
        expr = new EqualityExpression(expr, right, op);
    }

    return expr;
}

Exp *Parser::parseRelationalExpression()
{
    Exp *expr = parseAddExpression();

    while (match(Token::LT) || match(Token::LTE) || match(Token::GT) || match(Token::GTE))
    {
        BinaryOp op;
        switch (previous->type)
        {
        case Token::LT:
            op = BinaryOp::LT_OP;
            break;
        case Token::LTE:
            op = BinaryOp::LTE_OP;
            break;
        case Token::GT:
            op = BinaryOp::GT_OP;
            break;
        case Token::GTE:
            op = BinaryOp::GTE_OP;
            break;
        default:
            break;
        }
        Exp *right = parseAddExpression();
        expr = new RelationalExpression(expr, right, op);
    }

    return expr;
}

Exp *Parser::parseAddExpression()
{
    Exp *expr = parseMultExpression();

    while (match(Token::ADDITION) || match(Token::SUBTRACTION))
    {
        BinaryOp op;
        if (previous->type == Token::ADDITION)
        {
            op = BinaryOp::PLUS_OP;
        }
        else if (previous->type == Token::SUBTRACTION)
        {
            op = BinaryOp::MINUS_OP;
        }
        Exp *right = parseMultExpression();
        expr = new AdditiveExpression(expr, right, op);
    }

    return expr;
}

Exp *Parser::parseMultExpression()
{
    Exp *expr = parseUnaryExpression();

    while (match(Token::ASTERISK) || match(Token::DIVISION) || match(Token::MODULUS))
    {
        BinaryOp op;
        switch (previous->type)
        {
        case Token::ASTERISK:
            op = BinaryOp::MULT_OP;
            break;
        case Token::DIVISION:
            op = BinaryOp::DIV_OP;
            break;
        case Token::MODULUS:
            op = BinaryOp::MOD_OP;
            break;
        default:
            break;
        }
        Exp *right = parseUnaryExpression();
        expr = new MultiplicativeExpression(expr, right, op);
    }

    return expr;
}

Exp *Parser::parseUnaryExpression()
{
    BinaryOp op;
    if (match(Token::NOT))
    {
        Exp *expr = parseUnaryExpression();
        return new UnaryLogicalExpression(expr);
    }

    if (match(Token::ASTERISK) || match(Token::ADDRESS))
    {
        switch (previous->type)
        {
        case Token::ASTERISK:
            op = BinaryOp::ASTERIK_OP;
            break;
        case Token::ADDRESS:
            op = BinaryOp::ADDRESS_OP;
            break;
        default:
            break;
        }
        Exp *expr = parseUnaryExpression();
        return new PointerExpression(expr, op);
    }

    if (match(Token::INCREMENT) || match(Token::DECREMENT))
    {
        switch (previous->type)
        {
        case Token::INCREMENT:
            op = BinaryOp::INC_OP;
            break;
        case Token::DECREMENT:
            op = BinaryOp::DEC_OP;
            break;
        default:
            break;
        }
        Exp *expr = parseUnaryExpression();
        return new UnaryArithmeticExpression(expr, op, true);
    }

    return parsePostfixExpression();
}

Exp *Parser::parsePostfixExpression()
{
    Exp *expr = parsePrimaryExpression();

    while (true)
    {
        if (match(Token::BKL))
        {
            // arrays []
            Exp *index = parseExpression();
            expect(Token::BKR, "Expected ']' after array index");
            // expr = new ArrayAccessExpression(expr, index);
        }
        else if (match(Token::PL))
        {
            // Function call
            IdentifierExpression *id = dynamic_cast<IdentifierExpression *>(expr);
            if (id == nullptr)
            {
                throw runtime_error("Invalid function call");
            }

            FunctionCallExpression *funcCall = new FunctionCallExpression(id->getIdentifier());

            if (!check(Token::PR))
            {
                vector<Exp *> args = parseArgumentList();
                for (auto arg : args)
                {
                    funcCall->addArgument(arg);
                }
            }

            expect(Token::PR, "Expected ')' after function arguments");
            expr = funcCall;
        }
        else if (match(Token::DOT) || match(Token::ARROW))
        {
            // . or ->
            bool isPointer = (previous->type == Token::ARROW);
            expect(Token::IDENTIFIER, "Expected member name");
            string member = previous->text;
            expr = new MemberAccessExpression(expr, member, isPointer);
        }
        else if (match(Token::INCREMENT) || match(Token::DECREMENT))
        {
            // ++ or --
            BinaryOp op;
            switch (previous->type)
            {
            case Token::INCREMENT:
                op = BinaryOp::INC_OP;
                break;
            case Token::DECREMENT:
                op = BinaryOp::DEC_OP;
                break;
            default:
                break;
            }
            expr = new UnaryArithmeticExpression(expr, op, false);
        }
        else
        {
            break;
        }
    }

    return expr;
}

Exp *Parser::parsePrimaryExpression()
{
    if (match(Token::IDENTIFIER))
    {
        return new IdentifierExpression(previous->text);
    }

    if (match(Token::NUMBER))
    {
        return new LiteralExpression(previous->text, "int");
    }

    if (match(Token::FLOAT_NUM))
    {
        return new LiteralExpression(previous->text, "float");
    }

    if (match(Token::CHAR_LIT))
    {
        return new LiteralExpression(previous->text, "char");
    }

    if (match(Token::STRING))
    {
        return new LiteralExpression(previous->text, "string");
    }

    if (match(Token::PL))
    {
        Exp *expr = parseExpression();
        expect(Token::PR, "Expected ')' after expression");
        return new ParenthesizedExpression(expr);
    }

    throw runtime_error("Expected expression");
}

vector<Exp *> Parser::parseArgumentList()
{
    vector<Exp *> arguments;

    arguments.push_back(parseAssignmentExpression());

    while (match(Token::COMMA))
    {
        arguments.push_back(parseAssignmentExpression());
    }

    return arguments;
}

Parser::Parser(Scanner *sc) : scanner(sc)
{
    previous = NULL;
    current = scanner->nextToken();
    if (current->type == Token::ERROR)
    {
        cout << "Error in the first token: " << current->text << endl;
        exit(1);
    }
}