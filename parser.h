#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"
#include "token.h"
#include "exp.h"
#include <memory>
#include <vector>

using std::unique_ptr;
using std::vector;

class Parser {
private:
    Scanner* scanner;
    Token* current;
    Token* previous;
    
    // Helper methods
    bool match(Token::Type type);
    bool check(Token::Type type);
    bool isAtEnd();
    bool advance();
    void expect(Token::Type type, const string& message);
    
    // Grammar rules
    unique_ptr<Program> parseProgram();
    unique_ptr<Stm> parseDeclaration();
    unique_ptr<FunctionDeclaration> parseFunctionDeclaration();
    unique_ptr<StructDeclaration> parseStructDeclaration();
    unique_ptr<VarDeclaration> parseVarDeclaration();
    unique_ptr<Stm> parseStatement();
    unique_ptr<BlockStm> parseBlock();
    unique_ptr<IfStm> parseIfStatement();
    unique_ptr<WhileStm> parseWhileStatement();
    unique_ptr<ForStm> parseForStatement();
    unique_ptr<DoWhileStm> parseDoWhileStatement();
    unique_ptr<ReturnStm> parseReturnStatement();
    unique_ptr<ExpressionStm> parseExpressionStatement();
    
    // Expression parsing
    unique_ptr<Exp> parseExpression();
    unique_ptr<Exp> parseAssignment();
    unique_ptr<Exp> parseConditional();
    unique_ptr<Exp> parseLogicalOr();
    unique_ptr<Exp> parseLogicalAnd();
    unique_ptr<Exp> parseEquality();
    unique_ptr<Exp> parseRelational();
    unique_ptr<Exp> parseAdditive();
    unique_ptr<Exp> parseMultiplicative();
    unique_ptr<Exp> parseUnary();
    unique_ptr<Exp> parsePostfix();
    unique_ptr<Exp> parsePrimary();
    
    // Helper for function calls and array access
    unique_ptr<Exp> finishCall(unique_ptr<Exp> callee);
    vector<unique_ptr<Exp>> parseArguments();
    
    // Type parsing
    string parseType();
    string parseFullType();
    vector<std::pair<string, string>> parseParameters();

public:
    Parser(Scanner* scanner);
    ~Parser();
    unique_ptr<Program> parse();
};

#endif // PARSER_H