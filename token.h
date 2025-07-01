#ifndef TOKEN_H
#define TOKEN_H
#include <string>
#include <iostream>
using std::string;

class Token {
public:
    enum Type {
        // BASIC
        IDENTIFIER, ASSIGN, TRUE, FALSE, PRINTF,
        // TYPES
        INT, FLOAT, DOUBLE, CHAR, VOID, STRING, NUMBER, BOOL,
        // CLOSURES
        BRACKETL, BRACKETR, BRACESL, BRACESR, PL, PR,
        // SYMBOLS
        COMMA, SEMICOLON, COLON, QUESTION, ASTERISK, ADDRESS, ARROW,
        // SENTENCES
        IF, ELSE, FOR, WHILE, BREAK, CONTINUE, DO,
        // SPECIAL
        STRUCT, RETURN,
        // OPERATIONS
        PLUS, MINUS, MULTIPLICATION, DIVISION, MODULUS,
        // RELATIONAL
        LT, LTE, GT, GTE, EQ, NEQ,
        // LOGICAL
        AND, OR, NOT, DOT,
        // INCREMENT/DECREMENT
        INCREMENT, DECREMENT,
        // SPECIAL
        END, ERROR, HASH, INCLUDE
    };

    Type type;
    string text;

    static string symbolToString(Type type);

    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const string& source, int first, int last);
    
    friend std::ostream& operator<<(std::ostream& outs, const Token& tok);
    friend std::ostream& operator<<(std::ostream& outs, const Token* tok);
};

#endif // TOKEN_H