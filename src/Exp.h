#pragma once

#include "Type.h"
#include "Visitor.h"
#include <iostream>
#include <memory>
#include <vector>
#include "Printer.h"
/// Base class for an expression, which holds its type.
class Exp
{
  public:
    explicit Exp( Type type = kTypeUnknown )
        : m_type( type )
    {
    }

    virtual ~Exp() {}

    Type getType() const { return m_type; }

    void setType(Type type ) { m_type = type; }

    virtual void* Dispatch(ExpVisitor& visitor ) = 0;

  private:
    Type m_type;
};
using ExpPtr = std::unique_ptr<Exp>;

class BoolExp : public Exp
{
  public:
    BoolExp( bool value )
        : Exp( kTypeBool )
        , m_value( value )
    {
    }

    bool getValue() const { return m_value; }

    void* Dispatch( ExpVisitor& visitor ) override { return visitor.Visit( *this ); }

  private:
    bool m_value;
};


class FloatExp : public Exp
{
public:
    FloatExp( float value )
            : Exp( kTypeFloat )
            , m_value( value )
    {
    }

    float getValue() const {
        return m_value;
    }


    void* Dispatch( ExpVisitor& visitor ) override { return visitor.Visit( *this ); }

private:
    float m_value;
};


class IntExp : public Exp
{
  public:
    IntExp( int value )
        : Exp( kTypeInt )
        , m_value( value )
    {
    }

    int getValue() const { return m_value; }

    void* Dispatch( ExpVisitor& visitor ) override { return visitor.Visit( *this ); }

  private:
    int m_value;
};


class VarExp : public Exp
{
  public:
    VarExp( const std::string& name )
        : m_name( name )
    {
    }
    const std::string& getName() const { return m_name; }

    // (null if not yet typechecked).
    const VarDecl* getVarDecl() const { return m_varDecl; }

    // Typechecker calls this to link the variable to it's declaration
    void setVarDecl(const VarDecl* varDecl ) { m_varDecl = varDecl; }

    void* Dispatch( ExpVisitor& visitor ) override { return visitor.Visit( *this ); }

  private:
    std::string    m_name;
    const VarDecl* m_varDecl;  // assigned by the typechecker.
};

class ArrayAccessExp : public VarExp {
private:
    std::string arrayId;
    ExpPtr indexExp;

public:
    ArrayAccessExp(const std::string& arrayId, ExpPtr indexExp)
            : VarExp(arrayId), indexExp(std::move(indexExp)) {}

    void* Dispatch( ExpVisitor& visitor ) override { return visitor.Visit( *this ); }

    const std::string &getArrayId() const {
        return arrayId;
    }

    const ExpPtr &getIndexExp() const {
        return indexExp;
    }
};


/// Function call expression.
class CallExp : public Exp
{
  public:
    // varargs function call
    CallExp( const std::string& funcName, std::vector<ExpPtr>&& args )
        : m_funcName( funcName )
        , m_args( std::move( args ) )
        , m_funcDef( nullptr )
    {
    }

    // unary function call
    CallExp( const std::string& funcName, ExpPtr exp )
        : m_funcName( funcName )
        , m_args( 1 )
        , m_funcDef( nullptr )
    {
        m_args[0] = std::move( exp );
    }

    // binary function call
    CallExp( const std::string& funcName, ExpPtr leftExp, ExpPtr rightExp )
        : m_funcName( funcName )
        , m_args( 2 )
        , m_funcDef( nullptr )
    {
        m_args[0] = std::move( leftExp );
        m_args[1] = std::move( rightExp ); 
    }


    const std::string& getFuncName() const { return m_funcName; }


    const std::vector<ExpPtr>& getArgs() const { return m_args; }


    const FuncDef* getFuncDef() const { return m_funcDef; }

    void setFuncDef(const FuncDef* funcDef ) { m_funcDef = funcDef; }

    void* Dispatch( ExpVisitor& visitor ) override { return visitor.Visit( *this ); }

  private:
    std::string         m_funcName;
    std::vector<ExpPtr> m_args;
    const FuncDef*      m_funcDef;  // set by typechecker.
};

// Unique pointer to function call expression
using CallExpPtr = std::unique_ptr<CallExp>;

