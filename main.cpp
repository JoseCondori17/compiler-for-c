#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "parser.h"
#include "visitor.h"

using namespace std;

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        cout << "Numero incorrecto de argumentos. Uso: " << argv[0] << " <archivo_de_entrada>" << endl;
        exit(1);
    }

    ifstream infile(argv[1]);
    if (!infile.is_open())
    {
        cout << "No se pudo abrir el archivo: " << argv[1] << endl;
        exit(1);
    }

    string input;
    string line;
    while (getline(infile, line))
    {
        input += line + '\n';
    }
    infile.close();

    Scanner scanner(input.c_str());

    string input_copy = input;
    Scanner scanner_test(input_copy.c_str());
    cout << "SCANNER RUN:" << endl;
    test_scanner(&scanner_test);
    cout << "SCANNER CORRECT" << endl;
    cout << endl;
    Parser parser(&scanner);
    try {
        cout << "===========================" << endl;
        cout << "PARSING RUN:" << endl;
        unique_ptr<Program> program = parser.parse();
        cout << "PARSING CORRECT" << endl;
        cout << endl;

        PrintVisitor printVisitor;
        TypeVisitor typeVisitor;
        //ImpCODE interpreter;
        cout << "===========================" << endl;
        cout << endl;
        cout << "PRINTER RUN:" << endl;
        printVisitor.imprimir(program.get());
        cout << "PRINT VISITOR CORRECT" << endl;
        cout  << endl;
        cout << "===========================" << endl;
        cout << "TYPE CHECKER RUN:" << endl;
        typeVisitor.visit(program.get());
        cout << "TYPE CHECKER VISITOR CORRECT" << endl;
        cout  << endl;
        cout  << "===========================" << endl;
        cout << "GENCODE RUN:" << endl;
        ofstream output("output.s");
        GenCodeVisitor generator(&output);
        generator.generate(program.get());
        output.close();
        cout << "GENCODE VISITOR CORRECT" << endl;
        cout  << endl;
    
        /* cout << endl << "Run program:" << endl;
        interpreter.interpret(program); */
        cout << "End of program execution" << endl;
        //delete program;
    } catch (const exception& e) {
        cout << "Error durante la ejecución: " << e.what() << endl;
        return 1;
    }

    return 0;
}
//Para recompilar:
//g++ *.cpp -o program
//./program.exe "test.txt"
//Para ejecutar los tests(luego de recompilar para aplicar cambios)
//python make_test.py