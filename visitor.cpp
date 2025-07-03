#include "visitor.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
string PrintVisitor::getIndent() {
    return string(nivel_indentacion * 4, ' ');
}

// Expressions implementations
void PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    output << " " << Token::symbolToString(exp->op) << " ";
    exp->right->accept(this);
}

void PrintVisitor::visit(UnaryExp* exp) {
    output << Token::symbolToString(exp->op);
    exp->exp->accept(this);
}

void PrintVisitor::visit(NumberExp* exp) {
    output << exp->value;
}

void PrintVisitor::visit(StringExp* exp) {
    output << "\"" << exp->value << "\"";
}

void PrintVisitor::visit(BoolExp* exp) {
    output << (exp->value ? "true" : "false");
}

void PrintVisitor::visit(IdentifierExp* exp) {
    output << exp->name;
}

void PrintVisitor::visit(AssignExp* exp) {
    output << exp->var << " = ";
    exp->exp->accept(this);
}

void PrintVisitor::visit(FunctionCallExp* exp) {
    output << exp->name << "(";
    for (size_t i = 0; i < exp->args.size(); ++i) {
        exp->args[i]->accept(this);
        if (i != exp->args.size() - 1) {
            output << ", ";
        }
    }
    output << ")";
}

void PrintVisitor::visit(ArrayAccessExp* exp) {
    exp->array->accept(this);
    output << "[";
    exp->index->accept(this);
    output << "]";
}

void PrintVisitor::visit(MemberAccessExp* exp) {
    exp->object->accept(this);
    output << (exp->isPointer ? "->" : ".") << exp->member;
}

void PrintVisitor::visit(ConditionalExp* exp) {
    exp->condition->accept(this);
    output << " ? ";
    exp->trueExp->accept(this);
    output << " : ";
    exp->falseExp->accept(this);
}

void PrintVisitor::visit(MemberAssignExp* exp) {
    exp->object->accept(this);
    output << " = ";
    exp->value->accept(this);
}

void PrintVisitor::visit(ArrayAssignExp* exp) {
    exp->array->accept(this);
    output << " = ";
    exp->value->accept(this);
}

void PrintVisitor::visit(PostIncrementExp* exp) {
    exp->exp->accept(this);
    output << (exp->isIncrement ? "++" : "--");
}

void PrintVisitor::visit(PreIncrementExp* exp) {
    output << (exp->isIncrement ? "++" : "--");
    exp->exp->accept(this);
}

// Statements implementations
void PrintVisitor::visit(ExpressionStm* stm) {
    output << getIndent();
    stm->exp->accept(this);
    output << ";\n";
}

void PrintVisitor::visit(PrintStm* stm) {
    output << getIndent() << "printf(";
    stm->exp->accept(this);
    output << ");\n";
}

void PrintVisitor::visit(IfStm* stm) {
    output << getIndent() << "if (";
    stm->condition->accept(this);
    output << ") {\n";
    
    nivel_indentacion++;
    stm->thenStm->accept(this);
    nivel_indentacion--;
    
    if (stm->elseStm) {
        output << getIndent() << "} else {\n";
        nivel_indentacion++;
        stm->elseStm->accept(this);
        nivel_indentacion--;
    }
    
    output << getIndent() << "}\n";
}

void PrintVisitor::visit(WhileStm* stm) {
    output << getIndent() << "while (";
    stm->condition->accept(this);
    output << ") {\n";
    
    nivel_indentacion++;
    stm->body->accept(this);
    nivel_indentacion--;
    
    output << getIndent() << "}\n";
}

void PrintVisitor::visit(ForStm* stm) {
    output << getIndent() << "for (";
    if (stm->init) stm->init->accept(this);
    output << "; ";
    if (stm->condition) stm->condition->accept(this);
    output << "; ";
    if (stm->increment) stm->increment->accept(this);
    output << ") {\n";
    
    nivel_indentacion++;
    stm->body->accept(this);
    nivel_indentacion--;
    
    output << getIndent() << "}\n";
}

void PrintVisitor::visit(DoWhileStm* stm) {
    output << getIndent() << "do {\n";
    
    nivel_indentacion++;
    stm->body->accept(this);
    nivel_indentacion--;
    
    output << getIndent() << "} while (";
    stm->condition->accept(this);
    output << ");\n";
}

void PrintVisitor::visit(BlockStm* stm) {
    for (auto& s : stm->statements) {
        s->accept(this);
    }
}

void PrintVisitor::visit(ReturnStm* stm) {
    output << getIndent() << "return";
    if (stm->exp) {
        output << " ";
        stm->exp->accept(this);
    }
    output << ";\n";
}

void PrintVisitor::visit(BreakStm* stm) {
    output << getIndent() << "break;\n";
}

void PrintVisitor::visit(ContinueStm* stm) {
    output << getIndent() << "continue;\n";
}

void PrintVisitor::visit(VarDeclaration* stm) {
    output << getIndent() << stm->type << " " << stm->name;
    if (stm->init) {
        output << " = ";
        stm->init->accept(this);
    }
    output << ";\n";
}

void PrintVisitor::visit(FunctionDeclaration* stm) {
    output << endl;
    output << getIndent() << stm->returnType << " " << stm->name << "(";
    for (size_t i = 0; i < stm->params.size(); ++i) {
        output << stm->params[i].first << " " << stm->params[i].second;
        if (i != stm->params.size() - 1) {
            output << ", ";
        }
    }
    output << ") {\n";
    
    nivel_indentacion++;
    stm->body->accept(this);
    nivel_indentacion--;
    
    output << getIndent() << "}\n";
}

void PrintVisitor::visit(StructDeclaration* stm) {
    output << endl;
    output << getIndent() << "struct " << stm->name << " {\n";
    nivel_indentacion++;
    for (auto& member : stm->members) {
        output << getIndent() << member.first << " " << member.second << ";\n";
    }
    nivel_indentacion--;
    output << getIndent() << "};\n";
}

void PrintVisitor::visit(Program* stm) {
    for (auto& decl : stm->declarations) {
        decl->accept(this);
    }
}

string PrintVisitor::getOutput() {
    return output.str();
}

void PrintVisitor::imprimir(Program* program) {
    cout << "#include <stdio.h>" << endl;
    program->accept(this);
    cout << output.str();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
void TypeVisitor::setExpType(Exp* e, const string& type) {
    expTypes[e] = type;
}

string TypeVisitor::getExpType(Exp* e) {
    auto it = expTypes.find(e);
    if (it != expTypes.end()) {
        return it->second;
    }
    return "unknown";
}

bool TypeVisitor::isNumericType(const string& type) {
    return type == "int" || type == "float" || type == "double" || 
           type == "char" || type == "short" || type == "long";
}

bool TypeVisitor::isIntegerType(const string& type) {
    return type == "int" || type == "char" || type == "short" || type == "long";
}

string TypeVisitor::extractStructName(const string& type) {
    if (type.substr(0, 7) == "struct ") {
        return type.substr(7);
    }
    return type;
}

bool TypeVisitor::isStructType(const string& type) {
    string structName = extractStructName(type);
    return struct_fields.find(structName) != struct_fields.end();
}

bool TypeVisitor::isAssignmentCompatible(const string& target, const string& source) {
    if (target == source) return true;
    
    if (isNumericType(target) && isNumericType(source)) {
        return true;
    }
    
    if (target == "void*" && source.back() == '*') {
        return true;
    }
    
    string targetStruct = extractStructName(target);
    string sourceStruct = extractStructName(source);
    if (targetStruct == sourceStruct && isStructType(target)) {
        return true;
    }
    
    return false;
}

void TypeVisitor::visit(BinaryExp* e) {
    e->left->accept(this);
    e->right->accept(this);
    
    string t1 = getExpType(e->left.get());
    string t2 = getExpType(e->right.get());
    
    switch (e->op) {
        case Token::PLUS:
        case Token::MINUS:
        case Token::ASTERISK:
        case Token::DIVISION:
            if (isNumericType(t1) && isNumericType(t2)) {
                if (t1 == "float" || t2 == "float") {
                    setExpType(e, "float");
                } else if (t1 == "double" || t2 == "double") {
                    setExpType(e, "double");
                } else {
                    setExpType(e, "int");
                }
            } else {
                cerr << "[TYPE ERROR] Operacion aritmetica invalida entre '" << t1 << "' y '" << t2 << "'\n";
                exit(1);
            }
            break;
            
        case Token::LT:
        case Token::LTE:
        case Token::EQ:
        case Token::GT:
        case Token::GTE:
        case Token::NEQ:
            if (isNumericType(t1) && isNumericType(t2)) {
                setExpType(e, "int");
            } else if (t1 == t2) {
                setExpType(e, "int");
            } else {
                cerr << "[TYPE ERROR] Comparacion invalida entre '" << t1 << "' y '" << t2 << "'\n";
                exit(1);
            }
            break;
            
        case Token::AND:
        case Token::OR:
            setExpType(e, "int");
            break;
            
        default:
            cerr << "[TYPE ERROR] Operador desconocido en BinaryExp\n";
            exit(1);
    }
}

void TypeVisitor::visit(UnaryExp* e) {
    e->exp->accept(this);
    string t = getExpType(e->exp.get());
    
    switch (e->op) {
        case Token::MINUS:
        case Token::PLUS:
            if (isNumericType(t)) {
                setExpType(e, t);
            } else {
                cerr << "[TYPE ERROR] Operador unario aritmetico aplicado a tipo no numerico: " << t << "\n";
                exit(1);
            }
            break;
            
        case Token::NOT:
            setExpType(e, "int");
            break;
            
        case Token::ASTERISK: // Dereference
            if (t.back() == '*') {
                setExpType(e, t.substr(0, t.length() - 1));
            } else {
                cerr << "[TYPE ERROR] Intento de dereferenciar tipo no puntero: " << t << "\n";
                exit(1);
            }
            break;
            
        case Token::ADDRESS: // Address
            setExpType(e, t + "*");
            break;
            
        default:
            cerr << "[TYPE ERROR] Operador unario desconocido\n";
            exit(1);
    }
}

void TypeVisitor::visit(NumberExp* e) {
    setExpType(e, "int");
}

void TypeVisitor::visit(StringExp* e) {
    setExpType(e, "char*");
}

void TypeVisitor::visit(BoolExp* e) {
    setExpType(e, "int");
}

void TypeVisitor::visit(IdentifierExp* e) {
    if (!env.check(e->name)) {
        cerr << "[TYPE ERROR] Variable no declarada: " << e->name << "\n";
        exit(1);
    }
    setExpType(e, env.lookup_type(e->name));
}

void TypeVisitor::visit(AssignExp* e) {
    if (!env.check(e->var)) {
        cerr << "[TYPE ERROR] Variable no declarada: " << e->var << "\n";
        exit(1);
    }
    
    e->exp->accept(this);
    string varType = env.lookup_type(e->var);
    string expType = getExpType(e->exp.get());
    
    if (!isAssignmentCompatible(varType, expType)) {
        cerr << "[TYPE ERROR] Tipos incompatibles en asignacion a " << e->var 
             << ": " << varType << " = " << expType << "\n";
        exit(1);
    }
    
    setExpType(e, varType);
}

void TypeVisitor::visit(FunctionCallExp* e) {
    if (fdecs.find(e->name) == fdecs.end()) {
        cerr << "[TYPE ERROR] Funcion no declarada: " << e->name << "\n";
        exit(1);
    }
    
    FunctionDeclaration* func = fdecs[e->name];
    
    if (func->params.size() != e->args.size()) {
        cerr << "[TYPE ERROR] Numero incorrecto de argumentos en llamada a " << e->name 
             << ": esperados " << func->params.size() << ", recibidos " << e->args.size() << "\n";
        exit(1);
    }
    
    for (size_t i = 0; i < e->args.size(); i++) {
        e->args[i]->accept(this);
        string paramType = func->params[i].first;
        string argType = getExpType(e->args[i].get());
        
        if (!isAssignmentCompatible(paramType, argType)) {
            cerr << "[TYPE ERROR] Tipo incorrecto en argumento " << (i+1) 
                 << " de llamada a " << e->name << ": esperado " << paramType 
                 << ", recibido " << argType << "\n";
            exit(1);
        }
    }
    
    setExpType(e, func->returnType);
}

void TypeVisitor::visit(ArrayAccessExp* e) {
    e->array->accept(this);
    e->index->accept(this);
    
    string arrayType = getExpType(e->array.get());
    string indexType = getExpType(e->index.get());
    
    if (!isIntegerType(indexType)) {
        cerr << "[TYPE ERROR] Indice de arreglo debe ser tipo entero, recibido: " << indexType << "\n";
        exit(1);
    }
    
    if (arrayType.back() == '*') {
        setExpType(e, arrayType.substr(0, arrayType.length() - 1));
    } else {
        cerr << "[TYPE ERROR] Intento de indexar tipo no puntero/arreglo: " << arrayType << "\n";
        exit(1);
    }
}

void TypeVisitor::visit(MemberAccessExp* e) {
    e->object->accept(this);
    string objType = getExpType(e->object.get());
    
    if (e->isPointer) {
        if (objType.back() != '*') {
            cerr << "[TYPE ERROR] Operador -> aplicado a tipo no puntero: " << objType << "\n";
            exit(1);
        }
        objType = objType.substr(0, objType.length() - 1);
    }
    
    // obtener el nombre del struct
    string structName = extractStructName(objType);
    
    if (struct_fields.find(structName) == struct_fields.end()) {
        cerr << "[TYPE ERROR] Acceso a miembro de tipo no struct: " << objType << "\n";
        exit(1);
    }
    
    if (struct_fields[structName].find(e->member) == struct_fields[structName].end()) {
        cerr << "[TYPE ERROR] Miembro no encontrado: " << e->member << " en struct " << structName << "\n";
        exit(1);
    }
    
    setExpType(e, struct_fields[structName][e->member]);
}

void TypeVisitor::visit(ConditionalExp* e) {
    e->condition->accept(this);
    e->trueExp->accept(this);
    e->falseExp->accept(this);
    
    string trueType = getExpType(e->trueExp.get());
    string falseType = getExpType(e->falseExp.get());
    
    if (trueType != falseType && !isAssignmentCompatible(trueType, falseType)) {
        cerr << "[TYPE ERROR] Tipos incompatibles en operador ternario: " 
             << trueType << " y " << falseType << "\n";
        exit(1);
    }
    
    setExpType(e, trueType);
}

void TypeVisitor::visit(PostIncrementExp* e) {
    e->exp->accept(this);
    string type = getExpType(e->exp.get());
    
    if (!isNumericType(type) && type.back() != '*') {
        cerr << "[TYPE ERROR] Operador de incremento/decremento aplicado a tipo incompatible: " << type << "\n";
        exit(1);
    }
    
    setExpType(e, type);
}

void TypeVisitor::visit(PreIncrementExp* e) {
    e->exp->accept(this);
    string type = getExpType(e->exp.get());
    
    if (!isNumericType(type) && type.back() != '*') {
        cerr << "[TYPE ERROR] Operador de incremento/decremento aplicado a tipo incompatible: " << type << "\n";
        exit(1);
    }
    
    setExpType(e, type);
}

void TypeVisitor::visit(MemberAssignExp* e) {
    e->object->accept(this);
    e->value->accept(this);
    
    string objType = getExpType(e->object.get());
    string valType = getExpType(e->value.get());
    
    if (!isAssignmentCompatible(objType, valType)) {
        cerr << "[TYPE ERROR] Tipos incompatibles en asignacion de miembro\n";
        exit(1);
    }
    
    setExpType(e, objType);
}

void TypeVisitor::visit(ArrayAssignExp* e) {
    e->array->accept(this);
    e->value->accept(this);
    
    string arrType = getExpType(e->array.get());
    string valType = getExpType(e->value.get());
    
    if (!isAssignmentCompatible(arrType, valType)) {
        cerr << "[TYPE ERROR] Tipos incompatibles en asignacion de arreglo\n";
        exit(1);
    }
    
    setExpType(e, arrType);
}

void TypeVisitor::visit(ExpressionStm* s) {
    s->exp->accept(this);
}

void TypeVisitor::visit(PrintStm* s) {
    s->exp->accept(this);
}

void TypeVisitor::visit(IfStm* s) {
    s->condition->accept(this);
    s->thenStm->accept(this);
    if (s->elseStm) {
        s->elseStm->accept(this);
    }
}

void TypeVisitor::visit(WhileStm* s) {
    s->condition->accept(this);
    s->body->accept(this);
}

void TypeVisitor::visit(ForStm* s) {
    env.add_level();
    if (s->init) s->init->accept(this);
    if (s->condition) s->condition->accept(this);
    if (s->increment) s->increment->accept(this);
    s->body->accept(this);
    env.remove_level();
}

void TypeVisitor::visit(DoWhileStm* s) {
    s->body->accept(this);
    s->condition->accept(this);
}

void TypeVisitor::visit(BlockStm* s) {
    env.add_level();
    for (auto& stmt : s->statements) {
        stmt->accept(this);
    }
    env.remove_level();
}

void TypeVisitor::visit(ReturnStm* s) {
    if (currFun.empty()) {
        cerr << "[TYPE ERROR] return fuera de funcion\n";
        exit(1);
    }
    
    string expectedType = fdecs[currFun]->returnType;
    
    if (s->exp) {
        s->exp->accept(this);
        string actualType = getExpType(s->exp.get());
        
        if (!isAssignmentCompatible(expectedType, actualType)) {
            cerr << "[TYPE ERROR] Tipo de return incompatible en funcion " << currFun
                 << ": esperado " << expectedType << ", recibido " << actualType << "\n";
            exit(1);
        }
    } else if (expectedType != "void") {
        cerr << "[TYPE ERROR] Funcion " << currFun << " debe retornar " << expectedType << "\n";
        exit(1);
    }
}

void TypeVisitor::visit(BreakStm* s) {}

void TypeVisitor::visit(ContinueStm* s) {}

void TypeVisitor::visit(VarDeclaration* s) {
    if (s->init) {
        s->init->accept(this);
        string initType = getExpType(s->init.get());
        
        if (!isAssignmentCompatible(s->type, initType)) {
            cerr << "[TYPE ERROR] Inicializacion incompatible de variable " << s->name
                 << ": " << s->type << " = " << initType << "\n";
            exit(1);
        }
    }
    
    env.add_var(s->name, s->type);
}

void TypeVisitor::visit(FunctionDeclaration* s) {
    fdecs[s->name] = s;
    
    env.add_level();
    currFun = s->name;
    
    for (auto& param : s->params) {
        env.add_var(param.second, param.first);
    }
    
    s->body->accept(this);
    
    currFun = "";
    env.remove_level();
}

void TypeVisitor::visit(StructDeclaration* s) {
    struct_registry[s->name] = s;
    
    unordered_map<string, string> fields;
    for (auto& member : s->members) {
        if (fields.find(member.second) != fields.end()) {
            cerr << "[TYPE ERROR] Campo duplicado en struct " << s->name << ": " << member.second << "\n";
            exit(1);
        }
        fields[member.second] = member.first;
    }
    struct_fields[s->name] = fields;
}

void TypeVisitor::visit(Program* p) {
    env.add_level();
    
    // registrar structs y funciones
    for (auto& decl : p->declarations) {
        if (auto* funcDecl = dynamic_cast<FunctionDeclaration*>(decl.get())) {
            if (fdecs.find(funcDecl->name) != fdecs.end()) {
                cerr << "[TYPE ERROR] Funcion redeclarada: " << funcDecl->name << "\n";
                exit(1);
            }
            fdecs[funcDecl->name] = funcDecl;
        } else if (auto* structDecl = dynamic_cast<StructDeclaration*>(decl.get())) {
            structDecl->accept(this);
        }
    }
    
    for (auto& decl : p->declarations) {
        decl->accept(this);
    }
    
    env.remove_level();
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
GenCodeVisitor::GenCodeVisitor(ostream* output) : output(output), label_counter(0), stack_offset(0) {
    env.add_level(); // global level
    loop_contexts = stack<LoopContext>(); // break y continue
}

void GenCodeVisitor::generate(Program* p) {
    *output << ".section .rodata\n";
    *output << "str_format: .string \"%s\\n\"\n";
    *output << "int_format: .string \"%d\\n\"\n";
    *output << "float_format: .string \"%.2f\\n\"\n";
    *output << "\n.text\n";
    *output << ".global main\n\n";
    p->accept(this);
}

string GenCodeVisitor::next_label() {
    return "L" + to_string(label_counter++);
}

void GenCodeVisitor::push_loop_context(const string& break_label, const string& continue_label) {
    loop_contexts.push({break_label, continue_label});
}

void GenCodeVisitor::pop_loop_context() {
    if (!loop_contexts.empty()) {
        loop_contexts.pop();
    }
}

string GenCodeVisitor::get_break_label() {
    if (loop_contexts.empty()) {
        cerr << "Error: break statement outside of loop" << endl;
        exit(1);
    }
    return loop_contexts.top().break_label;
}

string GenCodeVisitor::get_continue_label() {
    if (loop_contexts.empty()) {
        cerr << "Error: continue statement outside of loop" << endl;
        exit(1);
    }
    return loop_contexts.top().continue_label;
}

// Expression visitors
void GenCodeVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    *output << "    pushq %rax\n";
    
    exp->right->accept(this);
    *output << "    popq %rbx\n";
    
    switch (exp->op) {
        case Token::PLUS:
            *output << "    addq %rbx, %rax\n";
            break;
        case Token::MINUS:
            *output << "    subq %rax, %rbx\n";
            *output << "    movq %rbx, %rax\n";
            break;
        case Token::MULTIPLICATION:
            *output << "    imulq %rbx, %rax\n";
            break;
        // Corrected Division Operation
case Token::DIVISION:
    *output << "    movq %rbx, %rax\n";  // Dividend in rax
    *output << "    cqto\n";             // Sign extend
    *output << "    idivq %rax, %rbx\n";  // Divide by divisor in rbx
    break;

// Corrected Modulus Operation
case Token::MODULUS:
    *output << "    movq %rbx, %rax\n";
    *output << "    cqto\n";
    *output << "    idivq %rax, %rbx\n";
    *output << "    movq %rdx, %rax\n";  // Remainder in rdx
    break;

        case Token::LT:
            *output << "    cmpq %rax, %rbx\n";
            *output << "    setl %al\n";
            *output << "    movzbq %al, %rax\n";
            break;
        case Token::LTE:
            *output << "    cmpq %rax, %rbx\n";
            *output << "    setle %al\n";
            *output << "    movzbq %al, %rax\n";
            break;
        case Token::GT:
            *output << "    cmpq %rax, %rbx\n";
            *output << "    setg %al\n";
            *output << "    movzbq %al, %rax\n";
            break;
        case Token::GTE:
            *output << "    cmpq %rax, %rbx\n";
            *output << "    setge %al\n";
            *output << "    movzbq %al, %rax\n";
            break;
        case Token::EQ:
            *output << "    cmpq %rax, %rbx\n";
            *output << "    sete %al\n";
            *output << "    movzbq %al, %rax\n";
            break;
        case Token::NEQ:
            *output << "    cmpq %rax, %rbx\n";
            *output << "    setne %al\n";
            *output << "    movzbq %al, %rax\n";
            break;
        case Token::AND:
            *output << "    andq %rbx, %rax\n";
            break;
        case Token::OR:
            *output << "    orq %rbx, %rax\n";
            break;
    }
}

void GenCodeVisitor::visit(UnaryExp* exp) {
    switch (exp->op) {
        case Token::MINUS:
            exp->exp->accept(this);
            *output << "    negq %rax\n";
            break;
        case Token::PLUS:
            exp->exp->accept(this);
            break;
        case Token::NOT:
            exp->exp->accept(this);
            *output << "    testq %rax, %rax\n";
            *output << "    setz %al\n";
            *output << "    movzbq %al, %rax\n";
            break;
        case Token::ADDRESS:
            if (auto* id = dynamic_cast<IdentifierExp*>(exp->exp.get())) {
                int offset = var_offsets[id->name];
                *output << "    leaq " << offset << "(%rbp), %rax\n";
            }
            break;
        case Token::MULTIPLICATION: // Dereference
            exp->exp->accept(this);
            *output << "    movq (%rax), %rax\n";
            break;
    }
}

void GenCodeVisitor::visit(NumberExp* exp) {
    *output << "    movq $" << exp->value << ", %rax\n";
}

void GenCodeVisitor::visit(StringExp* exp) {
    string label = "str_" + to_string(label_counter++);
    // Cambiar a sección .rodata temporalmente
    *output << ".section .rodata\n";
    *output << label << ": .string \"" << exp->value << "\"\n";  // Agregar comillas
    *output << ".text\n";
    *output << "    leaq " << label << "(%rip), %rax\n";
}

void GenCodeVisitor::visit(BoolExp* exp) {
    *output << "    movq $" << (exp->value ? 1 : 0) << ", %rax\n";
}

void GenCodeVisitor::visit(IdentifierExp* exp) {
    if (var_offsets.find(exp->name) != var_offsets.end()) {
        int offset = var_offsets[exp->name];
        *output << "    movq " << offset << "(%rbp), %rax\n";
    } else {
        cerr << "Error: Variable no declarada: " << exp->name << endl;
        exit(1);
    }
}

void GenCodeVisitor::visit(AssignExp* exp) {
    exp->exp->accept(this);
    
    if (var_offsets.find(exp->var) != var_offsets.end()) {
        int offset = var_offsets[exp->var];
        *output << "    movq %rax, " << offset << "(%rbp)\n";
        env.update(exp->var, 0); // Actualizar environment (valor simbólico)
    } else {
        cerr << "Error: Variable no declarada: " << exp->var << endl;
        exit(1);
    }
}

void GenCodeVisitor::visit(FunctionCallExp* exp) {

    int stack_args_count = (exp->args.size() > 6) ? exp->args.size() - 6 : 0;
    if (stack_args_count > 0) {
        *output << "    subq $" << (stack_args_count * 8) << ", %rsp\n";
    }

    vector<string> arg_registers = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    for (int i = exp->args.size() - 1; i >= 0; i--) {
        exp->args[i]->accept(this);
        
        if (i < 6) {
            *output << "    movq %rax, " << arg_registers[i] << "\n"; // args register
        } else {
            *output << "    pushq %rax\n"; // arg in stack
        }
    }
    
    *output << "    movq %rsp, %r11\n";  // Guardar rsp actual
    if ((stack_args_count % 2) == 0) {   // Si la cantidad de pushq es impar
        *output << "    subq $8, %rsp\n";
    }
    
    // Llamar función
    *output << "    call " << exp->name << "\n";

    // clear args
    if (stack_args_count > 0) {
        *output << "    addq $" << (stack_args_count * 8) << ", %rsp\n";
    }
}

// Corrected Array Access
void GenCodeVisitor::visit(ArrayAccessExp* exp) {
    exp->array->accept(this);
    *output << "    pushq %rax\n";
    exp->index->accept(this);
    *output << "    popq %rbx\n";
    *output << "    imulq $8, %rax\n"; // Assuming 8-byte elements
    *output << "    addq %rbx, %rax\n"; // Corrected: base + offset*8
    *output << "    movq (%rax), %rax\n";
}

void GenCodeVisitor::visit(MemberAccessExp* exp) {
     if (auto* id = dynamic_cast<IdentifierExp*>(exp->object.get())) {
        string var_name = id->name;
        string var_type = env.lookup_type(var_name);

        if (var_type.find("struct ") == 0) {
            string struct_name = var_type.substr(7);
            string field_name = exp->member;
            int field_offset = env.get_field_offset(struct_name, field_name);
            int base_offset = var_offsets[var_name];

            if (exp->isPointer) {
                *output << "    movq " << base_offset << "(%rbp), %rax\n";
                *output << "    addq $" << field_offset << ", %rax\n";
            } else {
                *output << "    movq " << base_offset + field_offset << "(%rbp), %rax\n";
            }
            return;
        }
    }
}

void GenCodeVisitor::visit(ConditionalExp* exp) {
    string false_label = next_label();
    string end_label = next_label();
    
    exp->condition->accept(this);
    *output << "    testq %rax, %rax\n";
    *output << "    jz " << false_label << "\n";
    
    exp->trueExp->accept(this);
    *output << "    jmp " << end_label << "\n";
    
    *output << false_label << ":\n";
    exp->falseExp->accept(this);
    
    *output << end_label << ":\n";
}

void GenCodeVisitor::visit(MemberAssignExp* exp) {
    exp->value->accept(this);
    
    if (auto* memberAccess = dynamic_cast<MemberAccessExp*>(exp->object.get())) {
        if (auto* id = dynamic_cast<IdentifierExp*>(memberAccess->object.get())) {
            string var_name = id->name;
            string var_type = env.lookup_type(var_name);
            
            if (var_type.find("struct ") == 0) {
                string struct_name = var_type.substr(7);
                string field_name = memberAccess->member;
                
                int field_offset = env.get_field_offset(struct_name, field_name);
                int var_offset = var_offsets[var_name];
                
                *output << "    movq %rax, " << (var_offset + field_offset) << "(%rbp)\n";
                return;
            }
        }
    }
}

void GenCodeVisitor::visit(ArrayAssignExp* exp) {
    exp->value->accept(this);
    *output << "    pushq %rax\n";
    
    if (auto* arrayAccess = dynamic_cast<ArrayAccessExp*>(exp->array.get())) {
        arrayAccess->array->accept(this);
        *output << "    pushq %rax\n";
        arrayAccess->index->accept(this);
        *output << "    popq %rbx\n";
        *output << "    imulq $8, %rax\n";
        *output << "    addq %rbx, %rax\n";
        *output << "    popq %rbx\n";
        *output << "    movq %rbx, (%rax)\n";
    }
}

void GenCodeVisitor::visit(PostIncrementExp* exp) {
    if (auto* id = dynamic_cast<IdentifierExp*>(exp->exp.get())) {
        int offset = var_offsets[id->name];
        *output << "    movq " << offset << "(%rbp), %rax\n";
        *output << "    pushq %rax\n";
        if (exp->isIncrement) {
            *output << "    incq " << offset << "(%rbp)\n";
        } else {
            *output << "    decq " << offset << "(%rbp)\n";
        }
        *output << "    popq %rax\n";
    }
}

void GenCodeVisitor::visit(PreIncrementExp* exp) {
    if (auto* id = dynamic_cast<IdentifierExp*>(exp->exp.get())) {
        int offset = var_offsets[id->name];
        if (exp->isIncrement) {
            *output << "    incq " << offset << "(%rbp)\n";
        } else {
            *output << "    decq " << offset << "(%rbp)\n";
        }
        *output << "    movq " << offset << "(%rbp), %rax\n";
    }
}

// Statement visitors
void GenCodeVisitor::visit(ExpressionStm* stm) {
    stm->exp->accept(this);
}

void GenCodeVisitor::visit(PrintStm* stm) {
    if (auto* memberAccess = dynamic_cast<MemberAccessExp*>(stm->exp.get())) {
        if (auto* id = dynamic_cast<IdentifierExp*>(memberAccess->object.get())) {
            string var_name = id->name;
            string var_type = env.lookup_type(var_name);
            string format_label;
            if (var_type == "int") {
                format_label = "int_format";
            } else if (var_type == "float" || var_type == "double") {
                format_label = "float_format";
            } else if (var_type == "char") {
                format_label = "char_format";
            } else {
                format_label = "int_format"; // Default
            }
            
            if (var_type.find("struct ") == 0) {
                string struct_name = var_type.substr(7);
                string field_name = memberAccess->member;
                int field_offset = env.get_field_offset(struct_name, field_name);
                int var_offset = var_offsets[var_name];
                
                *output << "    movq " << (var_offset + field_offset) << "(%rbp), %rax\n";
                *output << "    movq %rax, %rsi\n";
                *output << "    leaq " << format_label << "(%rip), %rdi\n";
                *output << "    call printf\n";
                return;
            }
        }
    }
    
    stm->exp->accept(this);
    *output << "    movq %rax, %rsi\n";
    *output << "    leaq int_format(%rip), %rdi\n";
    *output << "    movq $0, %rax\n";
    *output << "    call printf\n";
}

void GenCodeVisitor::visit(IfStm* stm) {
    string false_label = next_label();
    string end_label = next_label();
    
    stm->condition->accept(this);
    *output << "    testq %rax, %rax\n";
    *output << "    jz " << false_label << "\n";
    
    stm->thenStm->accept(this);
    
    if (stm->elseStm) {
        *output << "    jmp " << end_label << "\n";
        *output << false_label << ":\n";
        stm->elseStm->accept(this);
        *output << end_label << ":\n";
    } else {
        *output << false_label << ":\n";
    }
}

void GenCodeVisitor::visit(WhileStm* stm) {
    string start_label = next_label();
    string end_label = next_label();
    
    push_loop_context(end_label, start_label);
    
    *output << start_label << ":\n";
    stm->condition->accept(this);
    *output << "    testq %rax, %rax\n";
    *output << "    jz " << end_label << "\n";
    
    stm->body->accept(this);
    *output << "    jmp " << start_label << "\n";
    *output << end_label << ":\n";
    
    pop_loop_context();
}

void GenCodeVisitor::visit(ForStm* stm) {
    string start_lbl = next_label();
    string continue_lbl = next_label();
    string end_lbl = next_label();

    push_loop_context(end_lbl, continue_lbl);

    if (stm->init) {
        stm->init->accept(this);
    }

    *output << start_lbl << ":\n";

    if (stm->condition) {
        stm->condition->accept(this);
        *output << "    testq %rax, %rax\n";
        *output << "    jz " << end_lbl << "\n";
    }

    stm->body->accept(this);

    *output << continue_lbl << ":\n";
    if (stm->increment) {
        stm->increment->accept(this);
    }

    *output << "    jmp " << start_lbl << "\n";
    *output << end_lbl << ":\n";

    pop_loop_context();
}

void GenCodeVisitor::visit(DoWhileStm* stm) {
    string start_label = next_label();
    string continue_label = next_label();
    string end_label = next_label();
    
    push_loop_context(end_label, continue_label);
    
    *output << start_label << ":\n";
    stm->body->accept(this);
    
    *output << continue_label << ":\n";
    stm->condition->accept(this);
    *output << "    testq %rax, %rax\n";
    *output << "    jnz " << start_label << "\n";
    *output << end_label << ":\n";
    
    pop_loop_context();
}

void GenCodeVisitor::visit(BlockStm* stm) {
    env.add_level();
    
    for (auto& stmt : stm->statements) {
        stmt->accept(this);
    }
    
    env.remove_level();
}

void GenCodeVisitor::visit(ReturnStm* stm) {
    if (stm->exp) {
        stm->exp->accept(this);
    } else {
        *output << "    movq $0, %rax\n";
    }
    
    *output << "    movq %rbp, %rsp\n";
    *output << "    popq %rbp\n";
    *output << "    ret\n";
}

void GenCodeVisitor::visit(BreakStm* stm) {
    string break_label = get_break_label();
    *output << "    jmp " << break_label << "\n";
}

void GenCodeVisitor::visit(ContinueStm* stm) {
    string continue_label = get_continue_label();
    *output << "    jmp " << continue_label << "\n";
}

void GenCodeVisitor::visit(VarDeclaration* stm) {
    if (stm->type.find("struct") == 0) {
        string struct_name = stm->type.substr(7);
        int size = env.get_type_size(stm->type);
        stack_offset -= size;
        var_offsets[stm->name] = stack_offset;
        env.add_var(stm->name, stm->type);
    } else {
        stack_offset -= 8;
        var_offsets[stm->name] = stack_offset;
        env.add_var(stm->name, stm->type);
    }

    if (stm->init) {
        stm->init->accept(this);
        *output << "    movq %rax, " << var_offsets[stm->name] << "(%rbp)\n";
    }
}

// review
void GenCodeVisitor::visit(FunctionDeclaration* stm) {
    // Function prologue
    *output << "\n# Función: " << stm->name << "(";
    for (size_t i = 0; i < stm->params.size(); ++i) {
        if (i != 0) *output << ", ";
        *output << stm->params[i].first << " " << stm->params[i].second;
    }
    *output << ")\n";
    
    *output << stm->name << ":\n";
    *output << "    pushq %rbp\n";
    *output << "    movq %rsp, %rbp\n";
    
    // Calculate needed space for local variables
    int local_vars_size = 0;
    unordered_map<string, int> saved_offsets = var_offsets;
    var_offsets.clear();
    env.add_level();
    
    // Process parameters (System V AMD64 calling convention)
    vector<string> param_registers = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int param_offset = 16; // Skip saved RBP (8) and return address (8)
    
    // Save parameters to stack
    for (size_t i = 0; i < stm->params.size(); i++) {
        if (i < 6) {
            // Parameters passed in registers - save to stack
            *output << "    movq " << param_registers[i] << ", -" << (i+1)*8 << "(%rbp) # guarda " 
                   << stm->params[i].second << "\n";
            var_offsets[stm->params[i].second] = -(i+1)*8;
        } else {
            // Parameters passed on stack (already at correct position)
            var_offsets[stm->params[i].second] = param_offset + 8 * (i - 6);
        }
        env.add_var(stm->params[i].second, stm->params[i].first);
    }
    
    // Calculate space needed for local variables
    stack_offset = -8 - 8*stm->params.size(); // Start below saved params
    
    // First pass to calculate total size needed
    for (auto& stmt : stm->body->statements) {
        if (auto varDecl = dynamic_cast<VarDeclaration*>(stmt.get())) {
            int size = 8;  // Default 8 bytes for most types
            if (varDecl->type.find("struct ") == 0) {
                size = env.get_type_size(varDecl->type);
            }
            stack_offset -= size;
            local_vars_size += size;
        }
    }
    
    // Align stack to 16 bytes if needed
    if ((local_vars_size % 16) != 0) {
        local_vars_size += (16 - (local_vars_size % 16));
    }
    
    if (local_vars_size > 0) {
        *output << "    subq $" << local_vars_size << ", %rsp # espacio para variables locales\n";
    }
    
    // Second pass to generate initialization code
    stack_offset = -8 - 8*stm->params.size();
    for (auto& stmt : stm->body->statements) {
        if (auto varDecl = dynamic_cast<VarDeclaration*>(stmt.get())) {
            int size = 8;
            if (varDecl->type.find("struct ") == 0) {
                size = env.get_type_size(varDecl->type);
            }
            stack_offset -= size;
            var_offsets[varDecl->name] = stack_offset;
            env.add_var(varDecl->name, varDecl->type);
            
            if (varDecl->init) {
                varDecl->init->accept(this);
                *output << "    movq %rax, " << stack_offset << "(%rbp) # inicializa " 
                       << varDecl->name << "\n";
            }
        }
    }
    
    // Generate function body
    for (auto& stmt : stm->body->statements) {
        if (!dynamic_cast<VarDeclaration*>(stmt.get())) {
            stmt->accept(this);
        }
    }
    
    // Default return if no explicit return statement
    if (!dynamic_cast<ReturnStm*>(stm->body->statements.back().get())) {
        *output << "    movq $0, %rax\n";
        *output << "    popq %rbp\n";
        *output << "    ret\n";
    }
    
    // Clean up
    env.remove_level();
    var_offsets = saved_offsets;
}

void GenCodeVisitor::visit(StructDeclaration* stm) {
    env.add_struct(stm->name, stm->members);
}

void GenCodeVisitor::visit(Program* stm) {
    for (auto& decl : stm->declarations) {
        decl->accept(this);
    }
}