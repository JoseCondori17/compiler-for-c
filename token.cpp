#include "token.h"
#include <iostream>
using std::cout;
using std::endl;

Token::Token(Type type) : type(type), text("") {}

Token::Token(Type type, char c) : type(type), text(1, c) {}

Token::Token(Type type, const string& source, int first, int last):type(type) {
    text = source.substr(first, last);
}

std::ostream& operator<<(std::ostream& outs, const Token& tok) {
    switch(tok.type) {
        case Token::IDENTIFIER: outs << "IDENTIFIER(" << tok.text << ")"; break;
        case Token::NUMBER: outs << "NUMBER(" << tok.text << ")"; break;
        case Token::STRING: outs << "STRING(" << tok.text << ")"; break;
        case Token::PRINTF: outs << "PRINTF"; break;
        case Token::IF: outs << "IF"; break;
        case Token::ELSE: outs << "ELSE"; break;
        case Token::FOR: outs << "FOR"; break;
        case Token::WHILE: outs << "WHILE"; break;
        case Token::CONTINUE: outs << "CONTINUE"; break;
        case Token::BREAK: outs << "BREAK"; break;
        case Token::RETURN: outs << "RETURN"; break;
        case Token::VOID: outs << "VOID"; break;
        case Token::DO: outs << "DO"; break;
        case Token::STRUCT: outs << "STRUCT"; break;
        case Token::INT: outs << "INT"; break;
        case Token::FLOAT: outs << "FLOAT"; break;
        case Token::DOUBLE: outs << "DOUBLE"; break;
        case Token::CHAR: outs << "CHAR"; break;
        case Token::BOOL: outs << "BOOL"; break;
        case Token::TRUE: outs << "TRUE"; break;
        case Token::FALSE: outs << "FALSE"; break;
        case Token::PL: outs << "PL"; break;
        case Token::PR: outs << "PR"; break;
        case Token::BRACESL: outs << "BRACESL"; break;
        case Token::BRACESR: outs << "BRACESR"; break;
        case Token::BRACKETL: outs << "BRACKETL"; break;
        case Token::BRACKETR: outs << "BRACKETR"; break;
        case Token::PLUS: outs << "PLUS"; break;
        case Token::MINUS: outs << "MINUS"; break;
        case Token::MULTIPLICATION: outs << "MULTIPLICATION"; break;
        case Token::DIVISION: outs << "DIVISION"; break;
        case Token::MODULUS: outs << "MODULUS"; break;
        case Token::ASSIGN: outs << "ASSIGN"; break;
        case Token::EQ: outs << "EQ"; break;
        case Token::NEQ: outs << "NEQ"; break;
        case Token::LT: outs << "LT"; break;
        case Token::LTE: outs << "LTE"; break;
        case Token::GT: outs << "GT"; break;
        case Token::GTE: outs << "GTE"; break;
        case Token::AND: outs << "AND"; break;
        case Token::OR: outs << "OR"; break;
        case Token::NOT: outs << "NOT"; break;
        case Token::INCREMENT: outs << "INCREMENT"; break;
        case Token::DECREMENT: outs << "DECREMENT"; break;
        case Token::ASTERISK: outs << "ASTERISK"; break;
        case Token::ADDRESS: outs << "ADDRESS"; break;
        case Token::ARROW: outs << "ARROW"; break;
        case Token::COMMA: outs << "COMMA"; break;
        case Token::SEMICOLON: outs << "SEMICOLON"; break;
        case Token::COLON: outs << "COLON"; break;
        case Token::DOT: outs << "DOT"; break;
        case Token::END: outs << "END"; break;
        case Token::ERROR: outs << "ERROR(" << tok.text << ")"; break;
        default: outs << "UNKNOWN"; break;
    }
    return outs;
}

std::ostream& operator<<(std::ostream& outs, const Token* tok) {
    return outs << *tok;
}

string Token::symbolToString(Token::Type type) {
    switch(type) {
        // BASIC
        case IDENTIFIER: return "identifier";
        case ASSIGN: return "=";
        case TRUE: return "true";
        case FALSE: return "false";
        case PRINTF: return "printf";
        
        // TYPES
        case INT: return "int";
        case FLOAT: return "float";
        case DOUBLE: return "double";
        case CHAR: return "char";
        case VOID: return "void";
        case STRING: return "string";
        case NUMBER: return "number";
        case BOOL: return "bool";
        
        // CLOSURES
        case BRACKETL: return "[";
        case BRACKETR: return "]";
        case BRACESL: return "{";
        case BRACESR: return "}";
        case PL: return "(";
        case PR: return ")";
        
        // SYMBOLS
        case COMMA: return ",";
        case SEMICOLON: return ";";
        case COLON: return ":";
        case QUESTION: return "?";
        case ASTERISK: return "*";
        case ADDRESS: return "&";
        case ARROW: return "->";
        
        // SENTENCES
        case IF: return "if";
        case ELSE: return "else";
        case FOR: return "for";
        case WHILE: return "while";
        case BREAK: return "break";
        case CONTINUE: return "continue";
        case DO: return "do";
        
        // SPECIAL
        case STRUCT: return "struct";
        case RETURN: return "return";
        
        // OPERATIONS
        case PLUS: return "+";
        case MINUS: return "-";
        case MULTIPLICATION: return "*";
        case DIVISION: return "/";
        case MODULUS: return "%";
        
        // RELATIONAL
        case LT: return "<";
        case LTE: return "<=";
        case GT: return ">";
        case GTE: return ">=";
        case EQ: return "==";
        case NEQ: return "!=";
        
        // LOGICAL
        case AND: return "&&";
        case OR: return "||";
        case NOT: return "!";
        case DOT: return ".";
        
        // INCREMENT/DECREMENT
        case INCREMENT: return "++";
        case DECREMENT: return "--";
        
        // SPECIAL
        case END: return "end";
        case ERROR: return "error";
        case HASH: return "#";
        case INCLUDE: return "include";
        
        default: return "unknown";
    }
}