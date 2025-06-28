#include <iostream>
#include "token.h"

using namespace std;

Token::Token(Type type):type(type) { text = ""; }

Token::Token(Type type, char c):type(type) { text = string(1, c); }

Token::Token(Type type, const string& source, int first, int last):type(type) {
    text = source.substr(first, last);
}

std::ostream& operator << ( std::ostream& outs, const Token & tok )
{
    switch (tok.type) {
        case Token::NUMBER:
            outs << "TOKEN(NUMBER, \"" << tok.text << "\")";
            break;
        case Token::STRING:
            outs << "TOKEN(STRING, \"" << tok.text << "\")";
            break;
            
        // KEYWORDS
        case Token::KEYWORD:
            outs << "TOKEN(KEYWORD, \"" << tok.text << "\")";
            break;
        case Token::IF:
            outs << "TOKEN(IF)";
            break;
        case Token::ELSE:
            outs << "TOKEN(ELSE)";
            break;
        case Token::FOR:
            outs << "TOKEN(FOR)";
            break;
        case Token::WHILE:
            outs << "TOKEN(WHILE)";
            break;
        case Token::DO:
            outs << "TOKEN(DO)";
            break;
        case Token::RETURN:
            outs << "TOKEN(RETURN)";
            break;
        case Token::BREAK:
            outs << "TOKEN(BREAK)";
            break;
        case Token::CONTINUE:
            outs << "TOKEN(CONTINUE)";
            break;
        case Token::SWITCH:
            outs << "TOKEN(SWITCH)";
            break;
        case Token::CASE:
            outs << "TOKEN(CASE)";
            break;
        case Token::DEFAULT:
            outs << "TOKEN(DEFAULT)";
            break;
        case Token::STRUCT:
            outs << "TOKEN(STRUCT)";
            break;
            
        case Token::IDENTIFIER:
            outs << "TOKEN(IDENTIFIER, \"" << tok.text << "\")";
            break;
            
        // ARITHMETIC OPERATORS
        case Token::PLUS:
            outs << "TOKEN(PLUS)";
            break;
        case Token::MINUS:
            outs << "TOKEN(MINUS)";
            break;
        case Token::MULTIPLICATION:
            outs << "TOKEN(MULTIPLICATION)";
            break;
        case Token::DIVISION:
            outs << "TOKEN(DIVISION)";
            break;
        case Token::MODULE:
            outs << "TOKEN(MODULE)";
            break;
            
        // RELATIONAL OPERATORS
        case Token::EQ:
            outs << "TOKEN(EQUAL TO)";
            break;
        case Token::NEQ:
            outs << "TOKEN(NOT EQUAL TO)";
            break;
        case Token::LT:
            outs << "TOKEN(LESS THAN)";
            break;
        case Token::GT:
            outs << "TOKEN(GREATER THAN)";
            break;
        case Token::LTE:
            outs << "TOKEN(LESS THAN OR EQUAL TO)";
            break;
        case Token::GTE:
            outs << "TOKEN(GREATER THAN OR EQUAL TO)";
            break;
            
        // LOGICAL OPERATORS
        case Token::AND:
            outs << "TOKEN(AND)";
            break;
        case Token::OR:
            outs << "TOKEN(OR)";
            break;
        case Token::NOT:
            outs << "TOKEN(NOT)";
            break;
            
        // ASSIGNMENT OPERATORS
        case Token::ASSIGN:
            outs << "TOKEN(ASSIGN)";
            break;
        case Token::PLUS_ASSIGN:
            outs << "TOKEN(PLUS ASSIGN)";
            break;
        case Token::MINUS_ASSIGN:
            outs << "TOKEN(MINUS ASSIGN)";
            break;
        case Token::MULT_ASSIGN:
            outs << "TOKEN(MULT ASSIGN)";
            break;
        case Token::DIV_ASSIGN:
            outs << "TOKEN(DIV ASSIGN)";
            break;
        case Token::MOD_ASSIGN:
            outs << "TOKEN(MOD ASSIGN)";
            break;
            
        // SEPARATORS
        case Token::PR:
            outs << "TOKEN(PARENTHESIS LEFT)";
            break;
        case Token::PL:
            outs << "TOKEN(PARENTHESIS RIGHT)";
            break;
        case Token::BCR:
            outs << "TOKEN(BRACES LEFT)";
            break;
        case Token::BCL:
            outs << "TOKEN(BRACES RIGHT)";
            break;
        case Token::BKL:
            outs << "TOKEN(BRACKET LEFT)";
            break;
        case Token::BKR:
            outs << "TOKEN(BRACKET RIGHT)";
            break;
        case Token::COMMA:
            outs << "TOKEN(COMMA)";
            break;
        case Token::SEMICOLON:
            outs << "TOKEN(SEMICOLON)";
            break;
        case Token::COLON:
            outs << "TOKEN(COLON)";
            break;
        case Token::PERIOD:
            outs << "TOKEN(PERIOD)";
            break;
        case Token::ASTERISK:
            outs << "TOKEN(ASTERISK)";
            break;
            
        // ERRORS
        case Token::ERROR:
            outs << "TOKEN(ERROR, \"" << tok.text << "\")";
            break;
            
        default: 
            outs << "TOKEN(UNKNOWN)"; 
            break;
    }
    return outs;
}

std::ostream& operator << ( std::ostream& outs, const Token* tok ) {
    return outs << *tok;
}