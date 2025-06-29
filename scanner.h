#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include "token.h"

using std::string;

class Scanner
{
private:
    string input;
    int first, current;

public:
    Scanner(const char *in_s);
    auto nextToken() -> Token*;
    auto reset() -> void;
    ~Scanner();
};

void test_scanner(Scanner *scanner);

#endif // SCANNER_H