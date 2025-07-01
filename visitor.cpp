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
