#include "parser.h"
#include <iostream>
#include <stdexcept>

using std::cerr;
using std::endl;
using std::make_unique;

Parser::Parser(Scanner* scanner) : scanner(scanner), current(nullptr), previous(nullptr) {
    previous = NULL;
    current = scanner->nextToken();
}

Parser::~Parser() {
    if (current) delete current;
    if (previous) delete previous;
}

bool Parser::match(Token::Type type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type t) { 
    return !isAtEnd() && current->type == t; 
}

bool Parser::isAtEnd() { 
    return current->type == Token::END; 
}

bool Parser::advance() {
    if (isAtEnd()) {
        return false;
    }
    Token* tmp = current;
    current = scanner->nextToken();
    if (previous) {
        delete previous;
    }
    previous = tmp;
    std::cout << "Avanzando token: " << current->text << " (" << current->type << ")" << std::endl;
    if (check(Token::ERROR)){
        cerr << "Scanner error: " << current->text << endl;
        exit(1);
    }
    return true;
}

void Parser::expect(Token::Type type, const string& message) {
    if (check(type)) {
        advance();
        return;
    }
    throw std::runtime_error("Parse error: " + message + ". Expected token type, got " + current->text);
}

unique_ptr<Program> Parser::parse() {
    return parseProgram();
}

unique_ptr<Program> Parser::parseProgram() {
    vector<unique_ptr<Stm>> declarations;
    while (!isAtEnd()) {
        auto decl = parseDeclaration();
        if (decl) {
            declarations.push_back(std::move(decl));
        }
    }
    
    return make_unique<Program>(std::move(declarations));
}

unique_ptr<Stm> Parser::parseDeclaration() {
    try {
        // Check if it's a struct declaration
        if (check(Token::STRUCT)) {
            // Save current position in a simple way
            size_t structPos = 0; // We'll use a different approach
            
            // Look at next tokens to determine what this is
            advance(); // consume STRUCT
            
            if (check(Token::IDENTIFIER)) {
                advance(); // consume identifier (struct name)
                
                if (check(Token::BRACESL)) {
                    // Pattern: struct Name { 
                    // This is a struct definition - backtrack and parse properly
                    // We need to go back 2 tokens
                    // Since we can't easily backtrack, let's handle it differently
                    
                    // Get the struct name from previous token
                    string structName = previous->text;
                    
                    // Continue parsing as struct definition
                    expect(Token::BRACESL, "Expected '{' after struct name");
                    
                    vector<std::pair<string, string>> members;
                    while (!check(Token::BRACESR) && !isAtEnd()) {
                        string memberType = parseFullType();
                        expect(Token::IDENTIFIER, "Expected member name");
                        string memberName = previous->text;
                        expect(Token::SEMICOLON, "Expected ';' after struct member");
                        
                        members.emplace_back(memberType, memberName);
                    }
                    
                    expect(Token::BRACESR, "Expected '}' after struct members");
                    expect(Token::SEMICOLON, "Expected ';' after struct declaration");
                    
                    return make_unique<StructDeclaration>(structName, std::move(members));
                } else {
                    // Pattern: struct Name something_else
                    // This is a variable declaration using struct type
                    // We have: struct consumed, Name consumed, now at something_else
                    
                    string structType = "struct " + previous->text;
                    
                    // Handle pointers
                    while (match(Token::MULTIPLICATION)) {
                        structType += "*";
                    }
                    
                    // Now expect variable name
                    expect(Token::IDENTIFIER, "Expected variable name");
                    string varName = previous->text;
                    
                    // Handle optional initialization
                    unique_ptr<Exp> init = nullptr;
                    if (match(Token::ASSIGN)) {
                        init = parseExpression();
                    }
                    
                    expect(Token::SEMICOLON, "Expected ';' after variable declaration");
                    
                    return make_unique<VarDeclaration>(structType, varName, std::move(init));
                }
            } else if (check(Token::BRACESL)) {
                // Anonymous struct: struct { ... }
                return parseStructDeclaration();
            }
        }
        
        // Check if it's a type keyword (potential variable or function declaration)
        if (check(Token::INT) || check(Token::FLOAT) || check(Token::DOUBLE) || 
            check(Token::CHAR) || check(Token::VOID) || check(Token::BOOL) || check(Token::STRUCT)) {
            
            // Parse the full type (including pointers)
            string fullType = parseFullType();
            
            if (check(Token::IDENTIFIER)) {
                advance(); // consume identifier
                string name = previous->text;
                
                if (check(Token::PL)) {
                    // It's a function declaration
                    expect(Token::PL, "Expected '(' after function name");
                    vector<std::pair<string, string>> params = parseParameters();
                    expect(Token::PR, "Expected ')' after parameters");
                    
                    unique_ptr<BlockStm> body = parseBlock();
                    
                    return make_unique<FunctionDeclaration>(fullType, name, std::move(params), std::move(body));
                } else {
                    // It's a variable declaration (could be a pointer)
                    unique_ptr<Exp> init = nullptr;
                    if (match(Token::ASSIGN)) {
                        init = parseExpression();
                    }
                    
                    expect(Token::SEMICOLON, "Expected ';' after variable declaration");
                    
                    return make_unique<VarDeclaration>(fullType, name, std::move(init));
                }
            } else {
                throw std::runtime_error("Expected identifier after type");
            }
        }
        
        return parseStatement();
    } catch (const std::exception& e) {
        cerr << "Parse error: " << e.what() << endl;
        // Error recovery: skip to next semicolon or brace
        while (!isAtEnd() && current->type != Token::SEMICOLON && 
               current->type != Token::BRACESR && current->type != Token::BRACESL) {
            advance();
        }
        if (match(Token::SEMICOLON) || match(Token::BRACESR)) {
            // Continue parsing
        }
        return nullptr;
    }
}

unique_ptr<FunctionDeclaration> Parser::parseFunctionDeclaration() {
    string returnType = parseType();
    
    expect(Token::IDENTIFIER, "Expected function name");
    string name = previous->text;
    
    expect(Token::PL, "Expected '(' after function name");
    vector<std::pair<string, string>> params = parseParameters();
    expect(Token::PR, "Expected ')' after parameters");
    
    unique_ptr<BlockStm> body = parseBlock();
    
    return make_unique<FunctionDeclaration>(returnType, name, std::move(params), std::move(body));
}

unique_ptr<VarDeclaration> Parser::parseVarDeclaration() {
    string type = parseType();
    
    expect(Token::IDENTIFIER, "Expected variable name");
    string name = previous->text;
    
    unique_ptr<Exp> init = nullptr;
    if (match(Token::ASSIGN)) {
        init = parseExpression();
    }
    
    expect(Token::SEMICOLON, "Expected ';' after variable declaration");
    
    return make_unique<VarDeclaration>(type, name, std::move(init));
}

unique_ptr<Stm> Parser::parseStatement() {
    if (match(Token::PRINTF)) {
        expect(Token::PL, "Expected '(' after 'printf'");
        auto exp = parseExpression();
        expect(Token::PR, "Expected ')' after printf argument");
        expect(Token::SEMICOLON, "Expected ';' after printf");
        return make_unique<PrintStm>(std::move(exp));
    }
    if (match(Token::IF)) return parseIfStatement();
    if (match(Token::WHILE)) return parseWhileStatement();
    if (match(Token::FOR)) return parseForStatement();
    if (match(Token::DO)) return parseDoWhileStatement();
    if (match(Token::RETURN)) return parseReturnStatement();
    if (match(Token::BREAK)) {
        expect(Token::SEMICOLON, "Expected ';' after 'break'");
        return make_unique<BreakStm>();
    }
    if (match(Token::CONTINUE)) {
        expect(Token::SEMICOLON, "Expected ';' after 'continue'");
        return make_unique<ContinueStm>();
    }
    if (match(Token::BRACESL)) {
        vector<unique_ptr<Stm>> statements;
        while (!check(Token::BRACESR) && !isAtEnd()) {
            auto stmt = parseDeclaration(); // Cambio aquí: usar parseDeclaration en lugar de parseStatement
            if (stmt) statements.push_back(std::move(stmt));
        }
        expect(Token::BRACESR, "Expected '}' after block");
        return make_unique<BlockStm>(std::move(statements));
    }
    
    return parseExpressionStatement();
}

unique_ptr<BlockStm> Parser::parseBlock() {
    expect(Token::BRACESL, "Expected '{' to start block");
    
    vector<unique_ptr<Stm>> statements;
    while (!check(Token::BRACESR) && !isAtEnd()) {
        auto stmt = parseDeclaration(); // Cambio aquí también
        if (stmt) statements.push_back(std::move(stmt));
    }
    
    expect(Token::BRACESR, "Expected '}' to end block");
    return make_unique<BlockStm>(std::move(statements));
}

unique_ptr<IfStm> Parser::parseIfStatement() {
    expect(Token::PL, "Expected '(' after 'if'");
    auto condition = parseExpression();
    expect(Token::PR, "Expected ')' after if condition");
    
    auto thenStm = parseStatement();
    unique_ptr<Stm> elseStm = nullptr;
    
    if (match(Token::ELSE)) {
        elseStm = parseStatement();
    }
    
    return make_unique<IfStm>(std::move(condition), std::move(thenStm), std::move(elseStm));
}

unique_ptr<WhileStm> Parser::parseWhileStatement() {
    expect(Token::PL, "Expected '(' after 'while'");
    auto condition = parseExpression();
    expect(Token::PR, "Expected ')' after while condition");
    
    auto body = parseStatement();
    
    return make_unique<WhileStm>(std::move(condition), std::move(body));
}

unique_ptr<ForStm> Parser::parseForStatement() {
    expect(Token::PL, "Expected '(' after 'for'");
    
    unique_ptr<Exp> init = nullptr;
    if (!check(Token::SEMICOLON)) {
        init = parseExpression();
    }
    expect(Token::SEMICOLON, "Expected ';' after for loop initializer");
    
    unique_ptr<Exp> condition = nullptr;
    if (!check(Token::SEMICOLON)) {
        condition = parseExpression();
    }
    expect(Token::SEMICOLON, "Expected ';' after for loop condition");
    
    unique_ptr<Exp> increment = nullptr;
    if (!check(Token::PR)) {
        increment = parseExpression();
    }
    expect(Token::PR, "Expected ')' after for clauses");
    
    auto body = parseStatement();
    
    return make_unique<ForStm>(std::move(init), std::move(condition), std::move(increment), std::move(body));
}

unique_ptr<DoWhileStm> Parser::parseDoWhileStatement() {
    auto body = parseStatement();
    
    expect(Token::WHILE, "Expected 'while' after do body");
    expect(Token::PL, "Expected '(' after 'while'");
    auto condition = parseExpression();
    expect(Token::PR, "Expected ')' after while condition");
    expect(Token::SEMICOLON, "Expected ';' after do-while statement");
    
    return make_unique<DoWhileStm>(std::move(body), std::move(condition));
}

unique_ptr<ReturnStm> Parser::parseReturnStatement() {
    unique_ptr<Exp> value = nullptr;
    if (!check(Token::SEMICOLON)) {
        value = parseExpression();
    }
    expect(Token::SEMICOLON, "Expected ';' after return value");
    
    return make_unique<ReturnStm>(std::move(value));
}

unique_ptr<ExpressionStm> Parser::parseExpressionStatement() {
    auto expr = parseExpression();
    expect(Token::SEMICOLON, "Expected ';' after expression");
    return make_unique<ExpressionStm>(std::move(expr));
}

unique_ptr<Exp> Parser::parseExpression() {
    return parseAssignment();
}

unique_ptr<Exp> Parser::parseAssignment() {
    auto expr = parseConditional();
    
    if (match(Token::ASSIGN)) {
        auto value = parseAssignment();
        if (auto* identifier = dynamic_cast<IdentifierExp*>(expr.get())) {
            string name = identifier->name;
            expr.release(); // Release ownership before creating AssignExp
            return make_unique<AssignExp>(name, std::move(value));
        } else if (auto* memberAccess = dynamic_cast<MemberAccessExp*>(expr.get())) {
            // Member assignment: obj.member = value or ptr->member = value
            auto memberAssign = make_unique<MemberAssignExp>(std::move(expr), std::move(value));
            return std::move(memberAssign);
        } else if (auto* arrayAccess = dynamic_cast<ArrayAccessExp*>(expr.get())) {
            // Array assignment: arr[index] = value
            auto arrayAssign = make_unique<ArrayAssignExp>(std::move(expr), std::move(value));
            return std::move(arrayAssign);
        } else {
            throw std::runtime_error("Invalid assignment target");
        }
    }
    
    return expr;
}

unique_ptr<Exp> Parser::parseConditional() {
    auto expr = parseLogicalOr();
    
    if (match(Token::QUESTION)) {
        auto thenExpr = parseExpression();
        expect(Token::COLON, "Expected ':' after '?' in conditional expression");
        auto elseExpr = parseConditional();
        return make_unique<ConditionalExp>(std::move(expr), std::move(thenExpr), std::move(elseExpr));
    }
    
    return expr;
}

unique_ptr<Exp> Parser::parseLogicalOr() {
    auto expr = parseLogicalAnd();
    
    while (match(Token::OR)) {
        Token::Type op = previous->type;
        auto right = parseLogicalAnd();
        expr = make_unique<BinaryExp>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

unique_ptr<Exp> Parser::parseLogicalAnd() {
    auto expr = parseEquality();
    
    while (match(Token::AND)) {
        Token::Type op = previous->type;
        auto right = parseEquality();
        expr = make_unique<BinaryExp>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

unique_ptr<Exp> Parser::parseEquality() {
    auto expr = parseRelational();
    
    while (match(Token::NEQ) || match(Token::EQ)) {
        Token::Type op = previous->type;
        auto right = parseRelational();
        expr = make_unique<BinaryExp>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

unique_ptr<Exp> Parser::parseRelational() {
    auto expr = parseAdditive();
    
    while (match(Token::GT) || match(Token::GTE) || match(Token::LT) || match(Token::LTE)) {
        Token::Type op = previous->type;
        auto right = parseAdditive();
        expr = make_unique<BinaryExp>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

unique_ptr<Exp> Parser::parseAdditive() {
    auto expr = parseMultiplicative();
    
    while (match(Token::MINUS) || match(Token::PLUS)) {
        Token::Type op = previous->type;
        auto right = parseMultiplicative();
        expr = make_unique<BinaryExp>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

unique_ptr<Exp> Parser::parseMultiplicative() {
    auto expr = parseUnary();
    
    while (match(Token::DIVISION) || match(Token::MULTIPLICATION) || match(Token::MODULUS)) {
        Token::Type op = previous->type;
        auto right = parseUnary();
        expr = make_unique<BinaryExp>(std::move(expr), op, std::move(right));
    }
    
    return expr;
}

unique_ptr<Exp> Parser::parseUnary() {
    if (match(Token::NOT) || match(Token::MINUS) || match(Token::PLUS) || match(Token::ADDRESS) || match(Token::MULTIPLICATION)) {
        Token::Type op = previous->type;
        auto right = parseUnary();
        return make_unique<UnaryExp>(op, std::move(right));
    }
    
    // Handle pre-increment and pre-decrement
    if (match(Token::INCREMENT) || match(Token::DECREMENT)) {
        Token::Type op = previous->type;
        auto operand = parseUnary();
        return make_unique<PreIncrementExp>(std::move(operand), op == Token::INCREMENT);
    }
    
    return parsePostfix();
}

unique_ptr<Exp> Parser::parsePostfix() {
    auto expr = parsePrimary();
    
    while (true) {
        if (match(Token::PL)) {
            expr = finishCall(std::move(expr));
        } else if (match(Token::BRACKETL)) {
            auto index = parseExpression();
            expect(Token::BRACKETR, "Expected ']' after array index");
            expr = make_unique<ArrayAccessExp>(std::move(expr), std::move(index));
        } else if (match(Token::DOT)) {
            expect(Token::IDENTIFIER, "Expected property name after '.'");
            string name = previous->text;
            expr = make_unique<MemberAccessExp>(std::move(expr), name, false);
        } else if (match(Token::ARROW)) {
            expect(Token::IDENTIFIER, "Expected property name after '->'");
            string name = previous->text;
            expr = make_unique<MemberAccessExp>(std::move(expr), name, true);
        } else if (match(Token::INCREMENT)) {
            expr = make_unique<PostIncrementExp>(std::move(expr), true);
        } else if (match(Token::DECREMENT)) {
            expr = make_unique<PostIncrementExp>(std::move(expr), false);
        } else {
            break;
        }
    }
    
    return expr;
}

unique_ptr<Exp> Parser::parsePrimary() {
    if (match(Token::TRUE)) return make_unique<BoolExp>(true);
    if (match(Token::FALSE)) return make_unique<BoolExp>(false);
    
    if (match(Token::NUMBER)) {
        return make_unique<NumberExp>(previous->text);
    }
    
    if (match(Token::STRING)) {
        return make_unique<StringExp>(previous->text);
    }
    
    if (match(Token::IDENTIFIER)) {
        return make_unique<IdentifierExp>(previous->text);
    }
    
    if (match(Token::PL)) {
        auto expr = parseExpression();
        expect(Token::PR, "Expected ')' after expression");
        return expr;
    }
    
    // Si llegamos aquí y el token es '}', probablemente estemos al final de un bloque
    if (check(Token::BRACESR)) {
        throw std::runtime_error("Unexpected end of block - missing expression");
    }
    
    throw std::runtime_error("Unexpected token in expression: " + current->text);
}

unique_ptr<Exp> Parser::finishCall(unique_ptr<Exp> callee) {
    vector<unique_ptr<Exp>> args = parseArguments();
    expect(Token::PR, "Expected ')' after function arguments");
    
    if (auto* identifier = dynamic_cast<IdentifierExp*>(callee.get())) {
        string name = identifier->name;
        callee.release();
        return make_unique<FunctionCallExp>(name, std::move(args));
    }
    
    throw std::runtime_error("Invalid function call");
}

vector<unique_ptr<Exp>> Parser::parseArguments() {
    vector<unique_ptr<Exp>> args;
    
    if (!check(Token::PR)) {
        do {
            args.push_back(parseExpression());
        } while (match(Token::COMMA));
    }
    
    return args;
}

string Parser::parseType() {
    if (match(Token::INT)) return "int";
    if (match(Token::FLOAT)) return "float";
    if (match(Token::DOUBLE)) return "double";
    if (match(Token::CHAR)) return "char";
    if (match(Token::VOID)) return "void";
    if (match(Token::BOOL)) return "bool";
    if (match(Token::STRUCT)) {
        if (check(Token::IDENTIFIER)) {
            advance(); // consume el identificador
            return "struct " + previous->text;
        } 
    }
    
    throw std::runtime_error("Expected type specifier");
}

string Parser::parseFullType() {
    string baseType = parseType();
    
    // Handle pointers
    while (match(Token::MULTIPLICATION)) {
        baseType += "*";
    }
    
    return baseType;
}

unique_ptr<StructDeclaration> Parser::parseStructDeclaration() {
    expect(Token::STRUCT, "Expected 'struct'");
    expect(Token::IDENTIFIER, "Expected struct name");
    string name = previous->text;
    
    expect(Token::BRACESL, "Expected '{' after struct name");
    
    vector<std::pair<string, string>> members;
    while (!check(Token::BRACESR) && !isAtEnd()) {
        string memberType = parseFullType();
        expect(Token::IDENTIFIER, "Expected member name");
        string memberName = previous->text;
        expect(Token::SEMICOLON, "Expected ';' after struct member");
        
        members.emplace_back(memberType, memberName);
    }
    
    expect(Token::BRACESR, "Expected '}' after struct members");
    expect(Token::SEMICOLON, "Expected ';' after struct declaration");
    
    return make_unique<StructDeclaration>(name, std::move(members));
}

vector<std::pair<string, string>> Parser::parseParameters() {
    vector<std::pair<string, string>> params;
    
    if (!check(Token::PR)) {
        do {
            string type = parseFullType(); // Cambio aquí para soportar punteros en parámetros
            expect(Token::IDENTIFIER, "Expected parameter name");
            string name = previous->text;
            params.emplace_back(type, name);
        } while (match(Token::COMMA));
    }
    
    return params;
}