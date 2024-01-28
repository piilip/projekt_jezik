#pragma once

#include "Exp.h"
#include "VarDecl.h"



class Stmt
{
  public:
    virtual ~Stmt() {}

    virtual void Dispatch( StmtVisitor& visitor ) = 0;
};

using StmtPtr = std::unique_ptr<Stmt>;




class CallStmt : public Stmt
{
  public:
    CallStmt( CallExpPtr&& callExp )
        : m_callExp( std::move( callExp ) )
    {
    }

    const CallExp& GetCallExp() const { return *m_callExp; }

    void Dispatch( StmtVisitor& visitor ) override { visitor.Visit( *this ); }

  private:
    CallExpPtr m_callExp;
};


// Assignment statement.
class AssignStmt : public Stmt
{
  public:
    // rvalue is some arbitrary expression
    AssignStmt( const std::string& varName, ExpPtr&& rvalue )
        : m_varName( varName )
        , m_rvalue( std::move( rvalue ) )
    {
    }

    const std::string& GetVarName() const { return m_varName; }

    const Exp& GetRvalue() const { return *m_rvalue; }

    const VarDecl* GetVarDecl() const { return m_varDecl; }

    // Typechecker uses this to link assignment to the variable
    void SetVarDecl( const VarDecl* varDecl ) { m_varDecl = varDecl; }

    void Dispatch( StmtVisitor& visitor ) override { visitor.Visit( *this ); }

protected:
    std::string    m_varName;
    ExpPtr         m_rvalue;
    const VarDecl* m_varDecl;
};

class ArrayAssignStmt : public AssignStmt {
private:
    ExpPtr indexExp;

public:
    const ExpPtr &getIndexExp() const {
        return indexExp;
    }

    ArrayAssignStmt(const std::string& arrayId, ExpPtr indexExp, ExpPtr rValue)
            : AssignStmt(arrayId, std::move(rValue)), // Temporary representation
              indexExp(std::move(indexExp)) {}

    void Dispatch( StmtVisitor& visitor ) override { visitor.Visit(*this); }
};


/// A declaration statement (e.g. "int x = 0;") declares a local variable with
/// an optional initializer.
class DeclStmt : public Stmt
{
  public:
    /// Construct a declaration statement from the specified variable declaration
    /// and optional initializer expression.
    DeclStmt( VarDeclPtr&& varDecl, ExpPtr&& initExp = ExpPtr() )
        : m_varDecl( std::move( varDecl ) )
        , m_initExp( std::move( initExp ) )
    {
    }

    /// Get pointer to variable declaration, which is stored at use sites by the typechecker.
    const VarDecl* GetVarDecl() const { return m_varDecl.get(); }

    /// Check whether this declaration has an initializer expression.
    bool HasInitExp() const { return bool( m_initExp ); }

    /// Get the initializer expression.  Check HasInitExp() before calling.
    const Exp& GetInitExp() const
    {
        assert( HasInitExp() && "Expected initializer expression in variable declaration" );
        return *m_initExp;
    }

    /// Dispatch to a visitor.
    void Dispatch( StmtVisitor& visitor ) override { visitor.Visit( *this ); }

  private:
    VarDeclPtr m_varDecl;
    ExpPtr     m_initExp;
};


/// Return statement.
class ReturnStmt : public Stmt
{
  public:
    // For now return is required because there's no void
    ReturnStmt( ExpPtr&& exp )
        : m_exp( std::move( exp ) )
    {
    }

    const Exp& GetExp() const { return *m_exp; }

    void Dispatch( StmtVisitor& visitor ) override { visitor.Visit( *this ); }

  private:
    ExpPtr m_exp;
};


class SeqStmt : public Stmt
{
  public:

    SeqStmt( std::vector<StmtPtr>&& stmts )
        : m_stmts( std::move( stmts ) )
    {
    }

    const std::vector<StmtPtr>& Get() const { return m_stmts; }

    void Dispatch( StmtVisitor& visitor ) override { visitor.Visit( *this ); }

  private:
    std::vector<StmtPtr> m_stmts;
};


using SeqStmtPtr = std::unique_ptr<SeqStmt>;

class IfStmt : public Stmt
{
  public:

    IfStmt( ExpPtr condExp, StmtPtr thenStmt, StmtPtr elseStmt = StmtPtr() )
        : m_condExp( std::move( condExp ) )
        , m_thenStmt( std::move( thenStmt ) )
        , m_elseStmt( std::move( elseStmt ) )
    {
    }

    /// Get the conditional expression.
    const Exp& getCondExp() const { return *m_condExp; }

    /// Get the "then" statement, which might be a sequence.
    const Stmt& getThenStmt() const { return *m_thenStmt; }

    /// Check whether this "if" statement has an "else" statement.
    bool hasElseStmt() const { return bool(m_elseStmt ); }

    /// Get the "else" statement.
    const Stmt& getElseStmt() const
    {
        assert(hasElseStmt() && "Expected else statement" );
        return *m_elseStmt;
    }

    /// Dispatch to a visitor.
    void Dispatch( StmtVisitor& visitor ) override { visitor.Visit( *this ); }

  private:
    ExpPtr  m_condExp;
    StmtPtr m_thenStmt;
    StmtPtr m_elseStmt;
};


/// While statement.
class WhileStmt : public Stmt
{
  public:
    /// Construct while statement from a conditional expression and the loop body
    /// statement (which might be a sequence).
    WhileStmt( ExpPtr condExp, StmtPtr bodyStmt )
        : m_condExp( std::move( condExp ) )
        , m_bodyStmt( std::move( bodyStmt ) )
    {
    }

    /// Get the conditional expression.
    const Exp& GetCondExp() const { return *m_condExp; }

    /// Get the loop body statement (which might be a sequence).
    const Stmt& GetBodyStmt() const { return *m_bodyStmt; }

    /// Dispatch to a visitor.
    void Dispatch( StmtVisitor& visitor ) override { visitor.Visit( *this ); }
    
  private:
    ExpPtr  m_condExp;
    StmtPtr m_bodyStmt;
};

class ForStmt : public Stmt {
public:
    ForStmt(StmtPtr initStmt, ExpPtr condExp, StmtPtr updateStmt, StmtPtr bodyStmt)
            : m_initStmt(std::move(initStmt)), m_condExp(std::move(condExp)), m_updateStmt(std::move(updateStmt)), m_bodyStmt(std::move(bodyStmt)) {}

    const Stmt& GetInitStmt() const { return *m_initStmt; }
    const Exp& GetCondExp() const { return *m_condExp; }
    const Stmt& GetUpdateStmt() const { return *m_updateStmt; }
    const Stmt& GetBodyStmt() const { return *m_bodyStmt; }
    bool HasCondExp() const { return static_cast<bool>(m_condExp);}
    bool HasUpdateStmt() const {   return static_cast<bool>(m_updateStmt);}
    bool HasInitStmt() const {  return static_cast<bool>(m_initStmt);}
    bool HasBodyStmt() const {  return static_cast<bool>(m_bodyStmt);}
    void Dispatch(StmtVisitor& visitor) override { visitor.Visit(*this); }

private:
    StmtPtr m_initStmt;
    ExpPtr m_condExp;
    StmtPtr m_updateStmt;
    StmtPtr m_bodyStmt;
};

