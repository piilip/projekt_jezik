#pragma once

#include <iostream>

class Exp;
class Stmt;
class FuncDef;
class Program;


std::ostream& operator<<( std::ostream& out, const Exp& exp );

std::ostream& operator<<( std::ostream& out, const Stmt& stmt );

std::ostream& operator<<( std::ostream& out, const FuncDef& def );

std::ostream& operator<<( std::ostream& out, const Program& program );

class ExpPrinter;
class StmtPrinter;
