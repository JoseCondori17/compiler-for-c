#include <iostream>
#include "token.h"

Token::Token(Type type) : type(type) { text = ""; }

Token::Token(Type type, char c) : type(type) { text = string(1, c); }

Token::Token(Type type, const string &source, int first, int last) : type(type)
{
    text = source.substr(first, last);
}

ostream &operator<<(ostream &outs, const Token &tok)
{
    switch (tok.type)
    {
    case Token::NUMBER:
        outs << "TOKEN(NUMBER)";
        break;
    case Token::STRING:
        outs << "TOKEN(STRING)";
        break;
    case Token::INT:
        outs << "TOKEN(INT)";
        break;
    case Token::FLOAT:
        outs << "TOKEN(FLOAT)";
        break;
    case Token::CHAR:
        outs << "TOKEN(CHAR)";
        break;
    case Token::VOID:
        outs << "TOKEN(VOID)";
        break;
    case Token::FLOAT_NUM:
        outs << "TOKEN(FLOAT NUMBER)";
        break;
    case Token::CHAR_LIT:
        outs << "TOKEN(CHAR LITERAL)";
        break;

    case Token::BKL:
        outs << "TOKEN(BRACKET LEFT)";
        break;
    case Token::BKR:
        outs << "TOKEN(BRACKET RIGHT)";
        break;
    case Token::BCL:
        outs << "TOKEN(BRACES LEFT)";
        break;
    case Token::BCR:
        outs << "TOKEN(BRACES RIGHT)";
        break;
    case Token::PL:
        outs << "TOKEN(PARENTHESES LEFT)";
        break;
    case Token::PR:
        outs << "TOKEN(PARENTHESES RIGHT)";
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
    case Token::QUESTION:
        outs << "TOKEN(QUESTION)";
        break;
    case Token::ASTERISK:
        outs << "TOKEN(ASTERISK)";
        break;
    case Token::ADDRESS:
        outs << "TOKEN(ADDRESS)";
        break;
    case Token::ASSIGNMENT:
        outs << "TOKEN(ASSIGNMENT)";
        break;
    case Token::DOT:
        outs << "TOKEN(DOT)";
        break;
    case Token::ARROW:
        outs << "TOKEN(ARROW)";
        break;
    case Token::IDENTIFIER:
        outs << "TOKEN(IDENTIFIER)";
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

    case Token::ADD_ASSIGN:
        outs << "TOKEN(ADDITION & ASSIGN)";
        break;
    case Token::SUB_ASSIGN:
        outs << "TOKEN(SUBTRACTION & ASSIGN)";
        break;
    case Token::MULT_ASSIGN:
        outs << "TOKEN(MULTIPLICATION & ASSIGN)";
        break;
    case Token::DIV_ASSIGN:
        outs << "TOKEN(DIVISION & ASSIGN)";
        break;
    case Token::MOD_ASSIGN:
        outs << "TOKEN(MODULUS & ASSIGN)";
        break;

    case Token::BREAK:
        outs << "TOKEN(BREAK)";
        break;
    case Token::CONTINUE:
        outs << "TOKEN(CONTINUE)";
        break;

    case Token::RETURN:
        outs << "TOKEN(RETURN)";
        break;

    case Token::STRUCT:
        outs << "TOKEN(STRUCT)";
        break;
    case Token::CONST:
        outs << "TOKEN(CONST)";
        break;

    case Token::ADDITION:
        outs << "TOKEN(ADDITION)";
        break;
    case Token::SUBTRACTION:
        outs << "TOKEN(SUBSTRACTION)";
        break;
    case Token::DIVISION:
        outs << "TOKEN(DIVISION)";
        break;
    case Token::MULTIPLICATION:
        outs << "TOKEN(MULTIPLICATION)";
        break;

    case Token::EQ:
        outs << "TOKEN(EQUAL)";
        break;
    case Token::NEQ:
        outs << "TOKEN(NOT EQUAL)";
        break;
    case Token::GT:
        outs << "TOKEN(GREATHER THAN)";
        break;
    case Token::LT:
        outs << "TOKEN(LESS THAN)";
        break;
    case Token::GTE:
        outs << "TOKEN(GREATHER THAN OR EQUAL)";
        break;
    case Token::LTE:
        outs << "TOKEN(LESS THAN OR EQUAL)";
        break;

    case Token::TRUE:
        outs << "TOKEN(TRUE)";
        break;
    case Token::FALSE:
        outs << "TOKEN(FALSE)";
        break;

    case Token::AND:
        outs << "TOKEN(LOGIC AND)";
        break;
    case Token::OR:
        outs << "TOKEN(LOGIC OR)";
        break;
    case Token::NOT:
        outs << "TOKEN(LOGIC NOT)";
        break;

    case Token::INCREMENT:
        outs << "TOKEN(INCREMENT)";
        break;
    case Token::DECREMENT:
        outs << "TOKEN(DECREMENT)";
        break;

    case Token::PRINT:
        outs << "TOKEN(PRINT)";
        break;
    case Token::END:
        outs << "TOKEN(END)";
        break;

    case Token::ERROR:
        outs << "TOKEN(ERROR)";
        break;
    default:
        outs << "TOKEN(UNKNOWN)";
        break;
    }
    return outs;
}

ostream &operator<<(ostream &outs, const Token *tok)
{
    return outs << *tok;
}