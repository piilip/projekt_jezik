#pragma once

#include <memory>
#include <vector>

class FuncDef;
using FuncDefPtr = std::unique_ptr<FuncDef>;

// Vector that includes all function definitions
class Program {
public:
  [[nodiscard]] const std::vector<FuncDefPtr> &GetFunctions() const {
    return m_functions;
  }

  std::vector<FuncDefPtr> &GetFunctions() { return m_functions; }

private:
  std::vector<FuncDefPtr> m_functions;
};

using ProgramPtr = std::unique_ptr<Program>;
