#pragma once
#include <ostream>

class Exp;
class Stmt;
class FuncDef;
class Program;

std::ostream &operator<<(std::ostream &out, const Exp &exp);
std::ostream &operator<<(std::ostream &out, const Stmt &stmt);
std::ostream &operator<<(std::ostream &out, const FuncDef &def);
std::ostream &operator<<(std::ostream &out, const Program &program);

// AST tree-style printing functions
std::ostream &printAST(std::ostream &out, const Exp &exp);
std::ostream &printAST(std::ostream &out, const Stmt &stmt);
std::ostream &printAST(std::ostream &out, const FuncDef &def);
std::ostream &printAST(std::ostream &out, const Program &program);

class ExpPrinter;
class StmtPrinter;
class ASTExpPrinter;
class ASTStmtPrinter;