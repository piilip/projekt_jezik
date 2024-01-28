#pragma once

#include "Stmt.h"
#include "Nodes.h"
#include "Type.h"

#include <memory>
#include <string>
#include <vector>


class FuncDef
{
  public:
    FuncDef( const Type& returnType, const std::string& name, std::vector<VarDeclPtr>&& params, SeqStmtPtr body )
        : m_returnType( returnType )
        , m_name( name )
        , m_params( std::move( params ) )
        , m_body( std::move( body ) )
    {
    }


    const Type& getReturnType() const { return m_returnType; }


    const std::string& getName() const { return m_name; }


    const std::vector<VarDeclPtr>& getParams() const { return m_params; }

    bool hasBody() const { return bool(m_body ); }

    const SeqStmt& GetBody() const
    {
        assert(hasBody() && "Expected function body" );
        return *m_body;
    }

  private:
    Type                    m_returnType;
    std::string             m_name;
    std::vector<VarDeclPtr> m_params;
    SeqStmtPtr              m_body;
};

using FuncDefPtr = std::unique_ptr<FuncDef>;

