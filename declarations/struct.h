#ifndef STRUCT_DECLARATION_H
#define STRUCT_DECLARATION_H

#include "declaration.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

class VariableDeclaration;

class StructDeclaration : public Declaration
{
private:
    string identifier;
    vector<VariableDeclaration *> members;

public:
    StructDeclaration(const string &id) : identifier(id) {}
    ~StructDeclaration()
    {
        for (auto member : members)
        {
            delete member;
        }
    }

    void addMember(VariableDeclaration *member) { members.push_back(member); }
    string getIdentifier() const { return identifier; }
    vector<VariableDeclaration *> &getMembers() { return members; }
};

#endif // STRUCT_DECLARATION_H
