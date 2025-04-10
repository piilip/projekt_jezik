#include "Printer.h"
#include "Exp.h"
#include "FuncDef.h"
#include "Program.h"
#include "Stmt.h"
#include "Visitor.h"

class ExpPrinter : public ExpVisitor {
public:
  ExpPrinter(std::ostream &out) : m_out(out) {}

  void Print(const Exp &exp) { const_cast<Exp &>(exp).Dispatch(*this); }

  void *Visit(ArrayAccessExp &exp) override {
    m_out << exp.getName() << '[' << *(exp.getIndexExp()) << "]";
    return nullptr;
  }

  void *Visit(BoolExp &exp) override {
    m_out << (exp.getValue() ? "true" : "false");
    return nullptr;
  }

  void *Visit(IntExp &exp) override {
    m_out << exp.getValue();
    return nullptr;
  }

  void *Visit(FloatExp &exp) override {
    m_out << exp.getValue();
    return nullptr;
  }

  void *Visit(VarExp &exp) override {
    m_out << exp.getName();
    return nullptr;
  }

  void *Visit(CallExp &exp) override {
    m_out << exp.getFuncName() << '(';
    for (size_t i = 0; i < exp.getArgs().size(); ++i) {
      if (i > 0)
        m_out << ", ";
      exp.getArgs()[i]->Dispatch(*this);
    }
    m_out << ')';
    return nullptr;
  }

private:
  std::ostream &m_out;
};

class StmtPrinter : public StmtVisitor {
public:
  explicit StmtPrinter(std::ostream &out) : m_out(out) {}
  void printForLoopAssignStmt(const Stmt &stmt) {
    if (typeid(stmt) == typeid(AssignStmt)) {
      const auto &assignStmt = dynamic_cast<const AssignStmt &>(stmt);
      m_out << assignStmt.GetVarName() << " = " << assignStmt.GetRvalue();
    } else if (typeid(stmt) == typeid(DeclStmt)) {
      const auto &declStmt = dynamic_cast<const DeclStmt &>(stmt);
      m_out << *declStmt.GetVarDecl();
      if (declStmt.HasInitExp()) {
        m_out << " = " << declStmt.GetInitExp();
      }
    } else {
      Print(stmt);
    }
  }

  void Visit(ArrayAssignStmt &stmt) override {
    m_out << stmt.GetVarName() << '[' << *(stmt.getIndexExp())
          << "] = " << stmt.GetRvalue() << ';';
  }

  void Print(const Stmt &stmt) { const_cast<Stmt &>(stmt).Dispatch(*this); }

  void Visit(CallStmt &stmt) override { m_out << stmt.GetCallExp() << ';'; }

  void Visit(AssignStmt &stmt) override {
    m_out << stmt.GetVarName() << " = " << stmt.GetRvalue() << ';';
  }

  void Visit(DeclStmt &stmt) override {
    m_out << *stmt.GetVarDecl();
    if (stmt.HasInitExp())
      m_out << " = " << stmt.GetInitExp();
    m_out << ';';
  }

  void Visit(ReturnStmt &stmt) override {
    m_out << "return " << stmt.GetExp() << ';';
  }

  void Visit(SeqStmt &seq) override {
    m_out << "{" << std::endl;
    for (const StmtPtr &stmt : seq.Get()) {
      Print(*stmt);
      m_out << std::endl;
    }
    m_out << "}";
  }

  void Visit(IfStmt &stmt) override {
    m_out << "if (" << stmt.getCondExp() << ")" << std::endl;
    Print(stmt.getThenStmt());
    if (stmt.hasElseStmt()) {
      m_out << std::endl << "else" << std::endl;
      Print(stmt.getElseStmt());
    }
  }

  void Visit(WhileStmt &stmt) override {
    m_out << "while (" << stmt.GetCondExp() << ")" << std::endl;
    Print(stmt.GetBodyStmt());
  }

  void Visit(ForStmt &stmt) override {
    m_out << "for (";

    // Print initialization statement if present
    if (stmt.HasInitStmt()) {
      printForLoopAssignStmt(stmt.GetInitStmt());
    }
    m_out << "; ";

    // Print condition expression if present
    if (stmt.HasCondExp()) {
      m_out << stmt.GetCondExp();
    }
    m_out << "; ";

    // Print update expression if present
    if (stmt.HasUpdateStmt()) {
      printForLoopAssignStmt(stmt.GetUpdateStmt());
    }
    m_out << ")" << std::endl;
    m_out << ")" << std::endl;

    Print(stmt.GetBodyStmt());
  }

private:
  std::ostream &m_out;
};

std::ostream &operator<<(std::ostream &out, const Exp &exp) {
  ExpPrinter(out).Print(exp);
  return out;
}

std::ostream &operator<<(std::ostream &out, const Stmt &stmt) {
  StmtPrinter(out).Print(stmt);
  return out;
}

std::ostream &operator<<(std::ostream &out, const FuncDef &def) {
  out << toString(def.getReturnType()) << ' ' << def.getName() << '(';
  for (size_t i = 0; i < def.getParams().size(); ++i) {
    if (i > 0)
      out << ", ";
    out << *def.getParams()[i];
  }
  out << ')' << std::endl;
  if (def.hasBody())
    out << def.GetBody();
  return out;
}

std::ostream &operator<<(std::ostream &out, const Program &program) {
  for (const FuncDefPtr &funcDef : program.GetFunctions()) {
    if (funcDef->hasBody())
      out << *funcDef << std::endl;
  }
  return out;
}
