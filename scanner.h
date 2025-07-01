#ifndef SCANNER_H
#define SCANNER_H
#include "token.h"
#include <string>
using std::string;

class Scanner {
private:
    string input;
    int first, current;
    bool is_white_space(char c);
    
public:
    Scanner(const char* s);
    ~Scanner();
    Token* nextToken();
    void clear_spaces();
    void reset();
};

void test_scanner(Scanner* scanner);

#endif // SCANNER_H