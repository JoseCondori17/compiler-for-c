#ifndef BODY_STATEMENT_H
#define BODY_STATEMENT_H

#include "statement.h"
#include <vector>

using std::vector;

class BodyStatement : public Stmt
{
private:
    vector<void *> items;

public:
    BodyStatement() {}
    ~BodyStatement() {}

    int accept(Visitor *visitor) override;
    void addItem(void *item) { items.push_back(item); }
    vector<void *> &getItems() { return items; }
};

#endif // BODY_STATEMENT_H