#pragma once

#include "VarDecl.h"
#include <string>
#include <unordered_map>

// Scope maps variable names to variable declarations
// Scope has a pointer to the parent scope which allows us to shadow variables
class Scope {
public:
  explicit Scope(const Scope *parent = nullptr) : m_map(), m_parent(parent) {}

  // We first search for the variable in the current scope, then if it's not
  // found we search in the parent scope thus creating recursion.
  const VarDecl *Find(const std::string &name) const {
    auto it = m_map.find(name);
    if (it != m_map.end())
      return it->second;
    else
      return m_parent ? m_parent->Find(name) : nullptr;
  }

  // If it's already defined in the current scope (map) it will return false
  bool Insert(const VarDecl *varDecl) {
    return m_map.insert(MapType::value_type(varDecl->GetName(), varDecl))
        .second;
  }

private:
  using MapType = std::unordered_map<std::string, const VarDecl *>;
  MapType m_map;
  const Scope *m_parent;
};
