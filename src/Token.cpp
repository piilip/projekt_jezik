#include "Token.h"
#include <sstream>
#include <string>
std::string Token::ToString() const {
  switch (getType()) {
  case kTokenIntNum: {
    std::stringstream stream;
    stream << getNum<int>();
    return stream.str();
  }
  case kTokenFPNum: {
    std::stringstream stream;
    stream << getNum<float>();
    return stream.str();
  }

  case kTokenId:
    return getId();
  case kTokenFloat:
    return "float";
  case kTokenBool:
    return "bool";
  case kTokenTrue:
    return "true";
  case kTokenFalse:
    return "false";
  case INT:
    return "int";
  case kTokenIf:
    return "if";
  case kTokenElse:
    return "else";
  case kTokenReturn:
    return "return";
  case kTokenWhile:
    return "while";
  case kTokenOperator:
    return "operator";
  case kTokenFor:
    return "for";
  case kTokenPlus:
    return "+";
  case kTokenMinus:
    return "-";
  case kTokenTimes:
    return "*";
  case kTokenDiv:
    return "/";
  case kTokenMod:
    return "%";
  case kTokenEQ:
    return "==";
  case kTokenNE:
    return "!=";
  case kTokenLT:
    return "<";
  case kTokenLE:
    return "<=";
  case kTokenGT:
    return ">";
  case kTokenGE:
    return ">=";
  case kTokenAnd:
    return "&&";
  case kTokenOr:
    return "||";
  case kTokenNot:
    return "!";
  case kTokenLbrace:
    return "{";
  case kTokenRbrace:
    return "}";
  case kTokenLbracket:
    return "[";
  case kTokenRbracket:
    return "]";
  case kTokenLparen:
    return "(";
  case kTokenRparen:
    return ")";
  case kTokenComma:
    return ",";
  case kTokenAssign:
    return "=";
  case kTokenSemicolon:
    return ";";
  case kTokenEOF:
    return "<EOF>";
  case kTokenUnknown:
    return "Unknown character";
  }
  assert(false && "Unhandled token kind");
  return "";
}

int Token::getLine() const { return line; }

void Token::setLine(int line) { Token::line = line; }

int Token::getColumn() const { return column; }

void Token::setColumn(int column) { Token::column = column; }
