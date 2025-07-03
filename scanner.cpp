#include <iostream>
#include <cstring>
#include <cctype>
#include "token.h"
#include "scanner.h"

using std::cout;
using std::endl;

Scanner::Scanner(const char *s) : input(s), first(0), current(0) {}

bool Scanner::is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

void Scanner::clear_spaces() {
    while (current < input.length()) {
        // spaces
        if (is_white_space(input[current])) {
            current++;
            continue;
        }
        // comments
        if (input[current] == '/' && current + 1 < input.length() && input[current + 1] == '/') {
            current += 2;
            while (current < input.length() && input[current] != '\n') {
                current++;
            }
            if (current < input.length() && input[current] == '\n') {
                current++;
            }
            continue;
        }
        // header/preprocessor directives
        if (input[current] == '#') {
            while (current < input.length() && input[current] != '\n') {
                current++; 
            }
            if (current < input.length() && input[current] == '\n') {
                current++;
            }
            continue;
        }
        break;
    }
}

Token* Scanner::nextToken() {
    Token* token;
    clear_spaces();

    if (current >= input.length()) {
        token = new Token(Token::END);
        return token;
    }

    char c = input[current];
    first = current;

    // numbers
    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current]))
            current++;
        
        // float numbers
        if (current < input.length() && input[current] == '.') {
            current++;
            while (current < input.length() && isdigit(input[current]))
                current++;
        }
        
        token = new Token(Token::NUMBER, input, first, current - first);
    } 
    // identifiers and keywords
    else if (isalpha(c) || c == '_') {
        current++;
        while (current < input.length() && (isalnum(input[current]) || input[current] == '_'))
            current++;
        string word = input.substr(first, current - first);

        // Keywords
        if (word == "printf") token = new Token(Token::PRINTF, word, 0, word.length());
        else if (word == "if") token = new Token(Token::IF, word, 0, word.length());
        else if (word == "else") token = new Token(Token::ELSE, word, 0, word.length());
        else if (word == "for") token = new Token(Token::FOR, word, 0, word.length());
        else if (word == "while") token = new Token(Token::WHILE, word, 0, word.length());
        else if (word == "continue") token = new Token(Token::CONTINUE, word, 0, word.length());
        else if (word == "break") token = new Token(Token::BREAK, word, 0, word.length());
        else if (word == "return") token = new Token(Token::RETURN, word, 0, word.length());
        else if (word == "void") token = new Token(Token::VOID, word, 0, word.length());
        else if (word == "do") token = new Token(Token::DO, word, 0, word.length());
        else if (word == "struct") token = new Token(Token::STRUCT, word, 0, word.length());
        else if (word == "int") token = new Token(Token::INT, word, 0, word.length());
        else if (word == "float") token = new Token(Token::FLOAT, word, 0, word.length());
        else if (word == "double") token = new Token(Token::DOUBLE, word, 0, word.length());
        else if (word == "char") token = new Token(Token::CHAR, word, 0, word.length());
        else if (word == "bool") token = new Token(Token::BOOL, word, 0, word.length());
        else if (word == "true") token = new Token(Token::TRUE, word, 0, word.length());
        else if (word == "false") token = new Token(Token::FALSE, word, 0, word.length());
        else token = new Token(Token::IDENTIFIER, word, 0, word.length());
    } 
    // String literals
    else if (c == '"') { 
        current++;
        while (current < input.length() && input[current] != '"') {
            if (input[current] == '\\' && current + 1 < input.length()) {
                current += 2; // skip escape sequences
            } else {
                current++;
            }
        }
        if (current >= input.length()) {
            token = new Token(Token::ERROR, "Unterminated string", 0, 18);
        } else {
            token = new Token(Token::STRING, input, first + 1, current - first - 1);
            current++;
        }
    } 
    // literals
    else if (c == '\'') {
        current++;
        if (current < input.length()) {
            if (input[current] == '\\' && current + 1 < input.length()) {
                current += 2; // skip escape sequences
            } else {
                current++;
            }
        }
        if (current >= input.length() || input[current] != '\'') {
            token = new Token(Token::ERROR, "Unterminated character", 0, 21);
        } else {
            token = new Token(Token::CHAR, input, first + 1, current - first - 1);
            current++;
        }
    }
    // Operators and symbols
    else {
        switch (c) {
            case '+': 
                if (current + 1 < input.length() && input[current + 1] == '+') {
                    token = new Token(Token::INCREMENT, "++", 0, 2);
                    current += 2;
                } else {
                    token = new Token(Token::PLUS, c);
                    current++;
                }
                break;
            case '-': 
                if (current + 1 < input.length() && input[current + 1] == '-') {
                    token = new Token(Token::DECREMENT, "--", 0, 2);
                    current += 2;
                } else if (current + 1 < input.length() && input[current + 1] == '>') {
                    token = new Token(Token::ARROW, "->", 0, 2);
                    current += 2;
                } else {
                    token = new Token(Token::MINUS, c);
                    current++;
                }
                break;
            case '*': 
                token = new Token(Token::ASTERISK, c);
                current++;
                break;
            case '/': 
                token = new Token(Token::DIVISION, c);
                current++;
                break;
            case '%': 
                token = new Token(Token::MODULUS, c);
                current++;
                break;
            case '=':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::EQ, "==", 0, 2);
                    current += 2;
                } else {
                    token = new Token(Token::ASSIGN, c);
                    current++;
                }
                break;
            case '<':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::LTE, "<=", 0, 2);
                    current += 2;
                } else {
                    token = new Token(Token::LT, c);
                    current++;
                }
                break;
            case '>':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::GTE, ">=", 0, 2);
                    current += 2;
                } else {
                    token = new Token(Token::GT, c);
                    current++;
                }
                break;
            case '!':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::NEQ, "!=", 0, 2);
                    current += 2;
                } else {
                    token = new Token(Token::NOT, c);
                    current++;
                }
                break;
            case '&':
                if (current + 1 < input.length() && input[current + 1] == '&') {
                    token = new Token(Token::AND, "&&", 0, 2);
                    current += 2;
                } else {
                    token = new Token(Token::ADDRESS, c);
                    current++;
                }
                break;
            case '|':
                if (current + 1 < input.length() && input[current + 1] == '|') {
                    token = new Token(Token::OR, "||", 0, 2);
                    current += 2;
                } else {
                    token = new Token(Token::ERROR, c);
                    current++;
                }
                break;
            case '(': 
                token = new Token(Token::PL, c);
                current++;
                break;
            case ')': 
                token = new Token(Token::PR, c);
                current++;
                break;
            case '{': 
                token = new Token(Token::BRACESL, c);
                current++;
                break;
            case '}': 
                token = new Token(Token::BRACESR, c);
                current++;
                break;
            case '[': 
                token = new Token(Token::BRACKETL, c);
                current++;
                break;
            case ']': 
                token = new Token(Token::BRACKETR, c);
                current++;
                break;
            case ',': 
                token = new Token(Token::COMMA, c);
                current++;
                break;
            case ';': 
                token = new Token(Token::SEMICOLON, c);
                current++;
                break;
            case ':': 
                token = new Token(Token::COLON, c);
                current++;
                break;
            case '.': 
                token = new Token(Token::DOT, c);
                current++;
                break;
            default:
                cout << "Error: Carácter inválido: " << c << endl;
                token = new Token(Token::ERROR, c);
                current++;
        }
    }
    return token;
}

void Scanner::reset() {
    first = 0;
    current = 0;
}

Scanner::~Scanner() {}

void test_scanner(Scanner* scanner) {
    Token* current;
    cout << "-------------------" << endl;
    cout << "Iniciando Scanner:" << endl;
    do {
        current = scanner->nextToken();
        cout << *current << endl;
        delete current;
    } while (current->type != Token::END);
    cout << "Fin del escaneo." << endl;
    cout << "----------------" << endl;
}