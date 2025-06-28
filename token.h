#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
public:
    enum Type {
        NUMBER,
        STRING,

        // COMMENTS


        // KEYWORDS
        KEYWORD,
        IF,
        ELSE,
        FOR,
        WHILE,
        DO,
        RETURN,
        BREAK,
        CONTINUE,
        STRUCT,

        IDENTIFIER,

        // ARIMETICS OPERATORS
        PLUS,
        MINUS,
        MULTIPLICATION,
        DIVISION,
        MODULE,

        // RELATIONAL OPERATORS
        EQ,
        NEQ,
        LT,
        GT,
        LTE,
        GTE,

        // LOGICAL OPERATORS
        AND,
        OR,
        NOT,

        // ASIGNMENT OPERATORS
        ASSIGN,
        PLUS_ASSIGN,
        MINUS_ASSIGN,
        MULT_ASSIGN,
        DIV_ASSIGN,
        MOD_ASSIGN,

        // SEPARATORS
        PR,
        PL,
        BCR,        // [
        BCL,        // ]
        BKR,        // {
        BKL,        // }
        COMMA,      // ,
        SEMICOLON,  // ;
        COLON,      // :
        PERIOD,     // .
        ASTERISK,   // *

        // ERRORS
        ERROR,
        UNKNOWN
    };

    Type type;
    std::string text;

    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const std::string& source, int first, int last);

    friend std::ostream& operator<<(std::ostream& outs, const Token& tok);
    friend std::ostream& operator<<(std::ostream& outs, const Token* tok);
};

#endif // TOKEN_H