#ifndef TYPE_H
#define TYPE_H

#include <string>
using std::string;

class Type
{
private:
    string baseType;  // int, float, char, void, struct name
    int pointerLevel; // 0 = not pointer, 1 = *, 2 = **, etc.
    bool isStruct;

public:
    Type(const string &baseType, int pointerLevel = 0, bool isStruct = false)
        : baseType(baseType), pointerLevel(pointerLevel), isStruct(isStruct) {}

    string getBaseType() const { return baseType; }
    int getPointerLevel() const { return pointerLevel; }
    bool getIsStruct() const { return isStruct; }
};

#endif // TYPE_H