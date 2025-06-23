#pragma once

#include "Exp.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

// Variables, and function parameters information
class  VarDecl
{
  public:
    enum Kind
    {
        kLocal,
        kParam
    };


    VarDecl( Kind kind, VarType type, std::string  name )
        : m_kind( kind )
        , m_type( std::move(type) )
        , m_name(std::move( name ))
    {
    }

    VarDecl( Kind kind, Type type, std::string  name ):
            m_type(type), m_kind(kind), m_name(std::move(name))
            {}


    /// Get the kind of this variable declaration (kLocal vs. kParam).
    Kind GetKind() const { return m_kind; }

    /// Get the variable's type.
    const Type& GetType() const { return m_type.type; }
    const bool& GetIsArray() const { return m_type.isArray; }

    const VarType &getVariable() const {
        return m_type;
    }

    /// Get the variable name.
    const std::string& GetName() const { return m_name; }

  private:
    Kind         m_kind;
    VarType      m_type;
    std::string  m_name;
};

using VarDeclPtr = std::unique_ptr<VarDecl>;

inline std::ostream& operator<<( std::ostream& out, const VarDecl& varDecl )
{
    if(varDecl.GetIsArray()){
        return out << toString(varDecl.GetType()) << ' ' << varDecl.GetName() << "[";

    }else{
        return out << toString(varDecl.GetType()) << ' ' << varDecl.GetName();
    }

}


