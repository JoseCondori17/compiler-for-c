#include <iostream>
#include <cstring>
#include "token.h"
#include "scanner.h"

using namespace std;

Scanner::Scanner(const char* s):input(s),first(0), current(0) { }


bool is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

Token* Scanner::nextToken() {
    Token* token;
    while (current < input.length() &&  is_white_space(input[current]) ) current++;
    if (current >= input.length()) return new Token(Token::END);
    char c  = input[current];
    first = current;
    if (isdigit(c)) {
        current++;
        while (current < input.length() && isdigit(input[current]))
            current++;
        token = new Token(Token::NUMBER, input, first, current - first);
    }

    else if (isalpha(c)) {
        current++;
        while (current < input.length() && isalnum(input[current]))
            current++;
        string word = input.substr(first, current - first);
        if (word == "if") {
            token = new Token(Token::IF, word, 0, word.length());
        }
        else if (word == "else") {
            token = new Token(Token::ELSE, word, 0, word.length());
        }
        else if (word == "for") {
            token = new Token(Token::FOR, word, 0, word.length());
        }
        else if (word == "while") {
            token = new Token(Token::WHILE, word, 0, word.length());
        }
        else if (word == "do") {
            token = new Token(Token::DO, word, 0, word.length());
        }
        else if (word == "return") {
            token = new Token(Token::RETURN, word, 0, word.length());
        }
        else if (word == "break") {
            token = new Token(Token::BREAK, word, 0, word.length());
        }
        else if (word == "continue") {
            token = new Token(Token::CONTINUE, word, 0, word.length());
        }
        else if (word == "struct") {
            token = new Token(Token::struct, word, 0, word.length());
        }
        else {
            token = new Token(Token::IDENTIFIER, word, 0, word.length());
        }
    }

    else if (strchr("+-*/()=;,<{}[]>:", c)) {
        switch(c) {
            case '+':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::PLUS_ASSING, "=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::PLUS, c);
                }
                break;
            case '-':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::MINUS_ASSIGN, "=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::MINUS, c);
                }
                break;
            case '*':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::MULT_ASSIGN, "=", 0, 2);
                    current++;
                } else if (current + 1 < input.length() && isalpha(input[current + 1])) {
                    token = new Token(Token::ASTERISK, c);
                    current++;
                } else {
                    token = new Token(Token::MULTIPLICATION, c);
                }
                break;
            case '/':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::DIV_ASSIGN, "=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::DIVISION, c);
                }
                break;
            case ',': token = new Token(Token::COMMA, c); break;
            case '(': token = new Token(Token::PL, c); break;
            case ')': token = new Token(Token::PR, c); break;
            case '[': token = new Token(Token::BCL, c); break;
            case ']': token = new Token(Token::BCR, c); break;
            case '{': token = new Token(Token::BKL, c); break;
            case '}': token = new Token(Token::BKR, c); break;
            case '=':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::EQ, "==", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::ASSIGN, c);
                }
                break;
            case '<':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::LTE, "<=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::LT, c);
                }
                break;
            case '>':
                if (current + 1 < input.length() && input[current + 1] == '=') {
                    token = new Token(Token::GTE, "<=", 0, 2);
                    current++;
                } else {
                    token = new Token(Token::GT, c);
                }
                break;
            case ';': token = new Token(Token::SEMICOLON, c); break;
            case ':': token = new Token(Token::COLON, c); break;
            default:
                cout << "No debería llegar acá" << endl;
                token = new Token(Token::ERROR, c);
        }
        current++;
    }
    else {
        token = new Token(Token::ERROR, c);
        current++;
    }
    return token;
}

void Scanner::reset() {
    first = 0;
    current = 0;
}

Scanner::~Scanner() { }

void test_scanner(Scanner* scanner) {
    Token* current;
    cout << "Iniciando Scanner:" << endl<< endl;
    while ((current = scanner->nextToken())->type != Token::END) {
        if (current->type == Token::ERROR) {
            cout << "Error en scanner - carácter inválido: " << current->text << endl;
            break;
        } else {
            cout << *current << endl;
        }
        delete current;
    }
    cout << "TOKEN(END)" << endl;
    delete current;
}