#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <string>
using std::string;

class Visitor;

enum BinaryOp
{
    PLUS_OP, MINUS_OP, MULT_OP, DIV_OP, MOD_OP,

    LT_OP, LTE_OP, GT_OP, GTE_OP, EQ_OP, NEQ_OP,

    AND_OP, OR_OP,

    ASTERIK_OP, ADDRESS_OP,

    INC_OP, DEC_OP,

    ASSIGN_OP, ADD_ASSIGN_OP, SUB_ASSIGN_OP, 
    MUL_ASSIGN_OP, DIV_ASSIGN_OP, MOD_ASSIGN_OP
};

class Exp
{
public:
    virtual ~Exp() = 0;
    virtual int accept(Visitor *visitor) = 0;
    static string binopToChar(BinaryOp op);
};

inline Exp::~Exp() {}

inline string Exp::binopToChar(BinaryOp op)
{
    switch (op)
    {
    case PLUS_OP:
        return "+";
    case MINUS_OP:
        return "-";
    case MULT_OP:
        return "*";
    case DIV_OP:
        return "/";
    case MOD_OP:
        return "%";
    case LT_OP:
        return "<";
    case LTE_OP:
        return "<=";
    case GT_OP:
        return ">";
    case GTE_OP:
        return ">=";
    case EQ_OP:
        return "==";
    case NEQ_OP:
        return "!=";
    case AND_OP:
        return "&&";
    case OR_OP:
        return "||";
    case ASSIGN_OP:
        return "=";
    case ADD_ASSIGN_OP:
        return "+=";
    case SUB_ASSIGN_OP:
        return "-=";
    case MUL_ASSIGN_OP:
        return "*=";
    case DIV_ASSIGN_OP:
        return "/=";
    case MOD_ASSIGN_OP:
        return "%=";
    default:
        return "??";
    }
}

#endif // EXPRESSION_H