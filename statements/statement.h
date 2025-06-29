#ifndef STATEMENT_H
#define STATEMENT_H
class Visitor;

class Stmt
{
public:
    virtual ~Stmt() = 0;
    virtual int accept(Visitor *visitor) = 0;
};

inline Stmt::~Stmt() {}
#endif // STATEMENT_H