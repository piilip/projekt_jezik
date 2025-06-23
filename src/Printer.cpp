#include "Printer.h"
#include "Exp.h"
#include "FuncDef.h"
#include "Program.h"
#include "Stmt.h"
#include "Type.h"
#include "Visitor.h"
#include <iomanip>

class ASTExpPrinter : public ExpVisitor {
public:
    ASTExpPrinter(std::ostream &out, int indent = 0)
            : m_out(out), m_indent(indent) {}

    void Print(const Exp &exp) { const_cast<Exp &>(exp).Dispatch(*this); }

    void *Visit(ArrayAccessExp &exp) override {
        printIndent();
        m_out << "ArrayAccessExp:";

        // Print type information if available
        if (exp.getType() != kTypeUnknown) {
            m_out << " [type: " << toString(exp.getType()) << "]";
        }

        // Print variable declaration info if linked
        if (exp.getVarDecl()) {
            m_out << " [decl: " << exp.getVarDecl()->GetName();
            if (exp.getVarDecl()->GetIsArray()) {
                m_out << " (array)";
            }
            m_out << "]";
        }
        m_out << std::endl;

        printIndent();
        m_out << "├─ array: " << exp.getName() << std::endl;

        printIndent();
        m_out << "└─ index:" << std::endl;
        ASTExpPrinter(m_out, m_indent + 1).Print(*(exp.getIndexExp()));

        return nullptr;
    }

    void *Visit(BoolExp &exp) override {
        printIndent();
        m_out << "BoolExp: " << (exp.getValue() ? "true" : "false");

        // Type should always be kTypeBool, but show it for consistency
        if (exp.getType() != kTypeUnknown) {
            m_out << " [type: " << toString(exp.getType()) << "]";
        }
        m_out << std::endl;
        return nullptr;
    }

    void *Visit(IntExp &exp) override {
        printIndent();
        m_out << "IntExp: " << exp.getValue();

        if (exp.getType() != kTypeUnknown) {
            m_out << " [type: " << toString(exp.getType()) << "]";
        }
        m_out << std::endl;
        return nullptr;
    }

    void *Visit(FloatExp &exp) override {
        printIndent();
        m_out << "FloatExp: " << exp.getValue();

        if (exp.getType() != kTypeUnknown) {
            m_out << " [type: " << toString(exp.getType()) << "]";
        }
        m_out << std::endl;
        return nullptr;
    }

    void *Visit(VarExp &exp) override {
        printIndent();
        m_out << "VarExp: " << exp.getName();

        // Print type information if available
        if (exp.getType() != kTypeUnknown) {
            m_out << " [type: " << toString(exp.getType()) << "]";
        }

        // Print variable declaration info if linked by typechecker
        if (exp.getVarDecl()) {
            m_out << " [decl: ";
            switch (exp.getVarDecl()->GetKind()) {
                case VarDecl::kLocal:
                    m_out << "local";
                    break;
                case VarDecl::kParam:
                    m_out << "param";
                    break;
            }
            if (exp.getVarDecl()->GetIsArray()) {
                m_out << " array";
            }
            m_out << "]";
        }
        m_out << std::endl;
        return nullptr;
    }

    void *Visit(CallExp &exp) override {
        printIndent();
        m_out << "CallExp:";

        // Print return type if available
        if (exp.getType() != kTypeUnknown) {
            m_out << " [return_type: " << toString(exp.getType()) << "]";
        }

        // Print function definition info if linked by typechecker
        if (exp.getFuncDef()) {
            m_out << " [resolved: " << exp.getFuncDef()->getName();
            m_out << "(";
            const auto& params = exp.getFuncDef()->getParams();
            for (size_t i = 0; i < params.size(); ++i) {
                if (i > 0) m_out << ", ";
                m_out << toString(params[i]->GetType());
                if (params[i]->GetIsArray()) {
                    m_out << "[]";
                }
            }
            m_out << ") -> " << toString(exp.getFuncDef()->getReturnType()) << "]";
        }
        m_out << std::endl;

        printIndent();
        m_out << "├─ function: " << exp.getFuncName() << std::endl;

        if (!exp.getArgs().empty()) {
            printIndent();
            m_out << "└─ arguments:" << std::endl;

            for (size_t i = 0; i < exp.getArgs().size(); ++i) {
                printIndent();
                m_out << "   ├─ arg[" << i << "]:" << std::endl;
                ASTExpPrinter(m_out, m_indent + 2).Print(*(exp.getArgs()[i]));
            }
        }

        return nullptr;
    }

private:
    void printIndent() {
        for (int i = 0; i < m_indent; ++i) {
            m_out << "  ";
        }
    }

    std::ostream &m_out;
    int m_indent;
};

class ASTStmtPrinter : public StmtVisitor {
public:
    explicit ASTStmtPrinter(std::ostream &out, int indent = 0)
            : m_out(out), m_indent(indent) {}

    void Print(const Stmt &stmt) { const_cast<Stmt &>(stmt).Dispatch(*this); }

    void Visit(ArrayAssignStmt &stmt) override {
        printIndent();
        m_out << "ArrayAssignStmt:";

        // Print variable declaration info if linked
        if (stmt.GetVarDecl()) {
            m_out << " [target_type: " << toString(stmt.GetVarDecl()->GetType()) << "]";
        }
        m_out << std::endl;

        printIndent();
        m_out << "├─ array: " << stmt.GetVarName() << std::endl;

        printIndent();
        m_out << "├─ index:" << std::endl;
        ASTExpPrinter(m_out, m_indent + 1).Print(*(stmt.getIndexExp()));

        printIndent();
        m_out << "└─ value:" << std::endl;
        ASTExpPrinter(m_out, m_indent + 1).Print(stmt.GetRvalue());
    }

    void Visit(CallStmt &stmt) override {
        printIndent();
        m_out << "CallStmt:" << std::endl;

        printIndent();
        m_out << "└─ expression:" << std::endl;
        ASTExpPrinter(m_out, m_indent + 1).Print(stmt.GetCallExp());
    }

    void Visit(AssignStmt &stmt) override {
        printIndent();
        m_out << "AssignStmt:";

        // Print variable declaration info if linked
        if (stmt.GetVarDecl()) {
            m_out << " [target_type: " << toString(stmt.GetVarDecl()->GetType());
            switch (stmt.GetVarDecl()->GetKind()) {
                case VarDecl::kLocal:
                    m_out << ", local";
                    break;
                case VarDecl::kParam:
                    m_out << ", param";
                    break;
            }
            m_out << "]";
        }
        m_out << std::endl;

        printIndent();
        m_out << "├─ variable: " << stmt.GetVarName() << std::endl;

        printIndent();
        m_out << "└─ value:" << std::endl;
        ASTExpPrinter(m_out, m_indent + 1).Print(stmt.GetRvalue());
    }

    void Visit(DeclStmt &stmt) override {
        printIndent();
        m_out << "DeclStmt:" << '\n';

        const auto *var_decl = stmt.GetVarDecl();
        printIndent();
        m_out << "├─ type:" << toString(var_decl->GetType());
        if (var_decl->GetIsArray()) {
            m_out << " (array)" << '\n';
        } else {
            m_out << '\n';
        }

        printIndent();
        m_out << "├─ name: " << var_decl->GetName();

        // Show variable kind
        m_out << " [kind: ";
        switch (var_decl->GetKind()) {
            case VarDecl::kLocal:
                m_out << "local";
                break;
            case VarDecl::kParam:
                m_out << "param";
                break;
        }
        m_out << "]" << '\n';

        // Handle array declarations with size expression
        if (var_decl->GetIsArray()) {
            printIndent();
            m_out << "├─ array_size:" << std::endl;
            ASTExpPrinter(m_out, m_indent + 1)
                    .Print(var_decl->getVariable().getArraySizeExp());
        }

        if (stmt.HasInitExp()) {
            printIndent();
            m_out << "└─ initializer:" << std::endl;
            ASTExpPrinter(m_out, m_indent + 1).Print(stmt.GetInitExp());
        }
    }

    void Visit(ReturnStmt &stmt) override {
        printIndent();
        m_out << "ReturnStmt:" << std::endl;

        printIndent();
        m_out << "└─ expression:" << std::endl;
        ASTExpPrinter(m_out, m_indent + 1).Print(stmt.GetExp());
    }

    void Visit(SeqStmt &seq) override {
        printIndent();
        m_out << "SeqStmt:" << std::endl;

        const auto &stmts = seq.Get();
        for (size_t i = 0; i < stmts.size(); ++i) {
            printIndent();
            if (i == stmts.size() - 1) {
                m_out << "└─ stmt[" << i << "]:" << std::endl;
            } else {
                m_out << "├─ stmt[" << i << "]:" << std::endl;
            }
            ASTStmtPrinter(m_out, m_indent + 1).Print(*(stmts[i]));
        }
    }

    void Visit(IfStmt &stmt) override {
        printIndent();
        m_out << "IfStmt:" << std::endl;

        printIndent();
        m_out << "├─ condition:" << std::endl;
        ASTExpPrinter(m_out, m_indent + 1).Print(stmt.getCondExp());

        printIndent();
        m_out << "├─ then:" << std::endl;
        ASTStmtPrinter(m_out, m_indent + 1).Print(stmt.getThenStmt());

        if (stmt.hasElseStmt()) {
            printIndent();
            m_out << "└─ else:" << std::endl;
            ASTStmtPrinter(m_out, m_indent + 1).Print(stmt.getElseStmt());
        }
    }

    void Visit(WhileStmt &stmt) override {
        printIndent();
        m_out << "WhileStmt:" << std::endl;

        printIndent();
        m_out << "├─ condition:" << std::endl;
        ASTExpPrinter(m_out, m_indent + 1).Print(stmt.GetCondExp());

        printIndent();
        m_out << "└─ body:" << std::endl;
        ASTStmtPrinter(m_out, m_indent + 1).Print(stmt.GetBodyStmt());
    }

    void Visit(ForStmt &stmt) override {
        printIndent();
        m_out << "ForStmt:" << std::endl;

        if (stmt.HasInitStmt()) {
            printIndent();
            m_out << "├─ init:" << std::endl;
            ASTStmtPrinter(m_out, m_indent + 1).Print(stmt.GetInitStmt());
        }

        if (stmt.HasCondExp()) {
            printIndent();
            m_out << "├─ condition:" << std::endl;
            ASTExpPrinter(m_out, m_indent + 1).Print(stmt.GetCondExp());
        }

        if (stmt.HasUpdateStmt()) {
            printIndent();
            m_out << "├─ update:" << std::endl;
            ASTStmtPrinter(m_out, m_indent + 1).Print(stmt.GetUpdateStmt());
        }

        printIndent();
        m_out << "└─ body:" << std::endl;
        ASTStmtPrinter(m_out, m_indent + 1).Print(stmt.GetBodyStmt());
    }

private:
    void printIndent() {
        for (int i = 0; i < m_indent; ++i) {
            m_out << "  ";
        }
    }

    std::ostream &m_out;
    int m_indent;
};

// AST-style output operators
std::ostream &printAST(std::ostream &out, const Exp &exp) {
    ASTExpPrinter(out).Print(exp);
    return out;
}

std::ostream &printAST(std::ostream &out, const Stmt &stmt) {
    ASTStmtPrinter(out).Print(stmt);
    return out;
}

std::ostream &printAST(std::ostream &out, const FuncDef &def) {
    out << "FuncDef:" << std::endl;
    out << "├─ return_type: " << toString(def.getReturnType()) << std::endl;
    out << "├─ name: " << def.getName() << std::endl;

    if (!def.getParams().empty()) {
        out << "├─ parameters:" << std::endl;
        for (size_t i = 0; i < def.getParams().size(); ++i) {
            out << "│  ├─ param[" << i << "]: " << *(def.getParams()[i]);

            // Add parameter kind info
            out << " [kind: ";
            switch (def.getParams()[i]->GetKind()) {
                case VarDecl::kLocal:
                    out << "local";
                    break;
                case VarDecl::kParam:
                    out << "param";
                    break;
            }
            out << "]" << std::endl;
        }
    }

    if (def.hasBody()) {
        out << "└─ body:" << std::endl;
        ASTStmtPrinter(out, 1).Print(def.GetBody());
    }

    return out;
}

std::ostream &printAST(std::ostream &out, const Program &program) {
    out << "Program:" << std::endl;
    constexpr int builtin_num = 55;
    const auto &functions = program.GetFunctions();

    // Count user-defined functions (those with bodies)
    int userFuncCount = 0;
    for (const auto& func : functions) {
        if (func->hasBody()) {
            userFuncCount++;
        }
    }

    out << "├─ total_functions: " << functions.size() << " (builtin: " << builtin_num
        << ", user-defined: " << userFuncCount << ")" << std::endl;

    for (size_t i = 0; i < functions.size(); ++i) {
        if (functions[i]->hasBody()) {
            if (i == functions.size() - 1) {
                out << "└─ function[" << i - builtin_num << "]:" << std::endl;
            } else {
                out << "├─ function[" << i - builtin_num << "]:" << std::endl;
            }

            // Indent the function definition
            std::stringstream funcStream;
            printAST(funcStream, *(functions[i]));

            std::string line;
            bool first = true;
            while (std::getline(funcStream, line)) {
                if (!first) {
                    out << "  ";
                }
                out << line << std::endl;
                first = false;
            }
        }
    }

    return out;
}