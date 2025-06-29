#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using std::ostream;
using std::string;

class Token
{
public:
    enum Type
    {
        NUMBER,
        STRING,

        INT,
        FLOAT,
        CHAR,
        VOID,

        FLOAT_NUM,
        CHAR_LIT,

        // special symbols
        BKL,        // [
        BKR,        // ]
        PL,         // (
        PR,         // )
        BCL,        // {
        BCR,        // }
        COMMA,      // ,
        SEMICOLON,  // ;
        COLON,      // :
        QUESTION,   // ?
        ASTERISK,   // *
        DEFERED,   // *
        ADDRESS,   // *
        ASSIGNMENT, // =
        IDENTIFIER,

        DOT,        // .
        ARROW,      // ->

        // control flow
        IF,
        ELSE,

        // looping
        FOR,
        WHILE,
        DO,

        // function
        RETURN,
        STRUCT,
        CONST,

        // special purpose
        BREAK,
        CONTINUE,

        // arimethics operators
        ADDITION,
        SUBTRACTION,
        MULTIPLICATION,
        DIVISION,
        MODULUS,

        // relational operators
        EQ,  // ==
        NEQ, // !=
        GT,  // >
        LT,  // <
        GTE, // >=
        LTE, // <=

        // booleans
        TRUE,
        FALSE,

        // logical operators
        AND,
        OR,
        NOT,

        // assign operators
        ADD_ASSIGN,  // +=
        SUB_ASSIGN,  // -=
        MULT_ASSIGN, // *=
        DIV_ASSIGN,  // /=
        MOD_ASSIGN,  // %=

        // unary operators
        INCREMENT, // ++
        DECREMENT, // --

        // ternary operator

        // printf
        PRINT,

        // endline
        END,

        // error
        ERROR,
    };

    Type type;
    string text;

    Token(Type type);
    Token(Type type, char c);
    Token(Type type, const string &source, int first, int last);

    friend ostream &operator<<(ostream &outs, const Token &tok);
    friend ostream &operator<<(ostream &outs, const Token *tok);
};

#endif // TOKEN_H