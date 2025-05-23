#pragma once

#include <cassert>
#include <iosfwd>
#include <string>
#include <utility>

enum TokenType
{

    kTokenIntNum,
    kTokenFPNum,
    kTokenId,
    kTokenStrLit,


    kTokenString,
    kTokenBool,
    kTokenTrue,
    kTokenFalse,
    INT,
    kTokenFloat,
    kTokenIf,
    kTokenElse,
    kTokenReturn,
    kTokenWhile,
    kTokenFor,
    kTokenOperator,

    kTokenPlus,
    kTokenMinus,
    kTokenTimes,
    kTokenDiv,
    kTokenMod,
    kTokenEQ,
    kTokenNE,
    kTokenLT,
    kTokenLE,
    kTokenGT,
    kTokenGE,
    kTokenAnd,
    kTokenOr,
    kTokenNot,

    kTokenLbrace,
    kTokenRbrace,
    kTokenLparen,
    kTokenRparen,
    kTokenComma,
    kTokenAssign,
    kTokenSemicolon,
    kTokenLbracket,
    kTokenRbracket,
    kTokenUnknown,
    kTokenEOF
};

class Token
{
  public:
     Token(int value, int line, int column)
        : m_type(kTokenIntNum), m_int(value), m_float(0), line(line), column(column) {}

     Token(float value, int line, int column)
        : m_type(kTokenFPNum), m_int(0), m_float(value), line(line), column(column) {}


     Token(std::string  id, int line, int column)
            : m_type(kTokenId), m_int(0), m_float(0), line(line), column(column), m_id(std::move(id)) {}

    Token(TokenType tag, int line, int column)
            : m_type(tag), m_int(0), m_float(0), line(line), column(column) {
        assert(tag != kTokenIntNum && tag != kTokenId && "Value required for integer and id tokens");
    }


    TokenType getType() const { return m_type; }

    /// Get the value of a numeric token.
    template <typename T>
    T getNum() const
    {
        assert((getType() == kTokenIntNum || getType() == kTokenFPNum) && "Expected numeric token");

        if (getType() == kTokenFPNum) {
            return static_cast<T>(m_float);
        }
        return static_cast<T>(m_int);
    }

    const std::string& getId() const
    {
        assert(getType() == kTokenId && "Expected identifier token" );
        return m_id;
    }

    std::string ToString() const;


    bool operator==( const TokenType& other ) const
    {
        if(getType() != other)
            return false;
        return true;
    }


    bool operator!=( const TokenType& other ) const { return !( *this == other ); }


    bool isOperator() const
    {
        switch(getType() )
        {
            case kTokenPlus:
            case kTokenMinus:
            case kTokenTimes:
            case kTokenDiv:
            case kTokenMod:
            case kTokenEQ:
            case kTokenNE:
            case kTokenLT:
            case kTokenLE:
            case kTokenGT:
            case kTokenGE:
            case kTokenAnd:
            case kTokenOr:
            case kTokenNot:
            case kTokenBool:
            case INT:
            case kTokenFloat:
                return true;
            default:
                return false;
        }
    }

    int getLine() const;

    void setLine(int line);

    int getColumn() const;

    void setColumn(int column);

    static std::string typeToString(TokenType type) {
        switch (type) {
            case kTokenFloat:      return "float";
            case kTokenBool:      return "bool";
            case INT:       return "int";
            case kTokenTrue:      return "true";
            case kTokenFalse:     return "false";
            case kTokenIf:        return "if";
            case kTokenElse:      return "else";
            case kTokenReturn:    return "return";
            case kTokenWhile:     return "while";
            case kTokenOperator:  return "operator";
            case kTokenFor:       return "for";
            case kTokenPlus:      return "+";
            case kTokenMinus:     return "-";
            case kTokenTimes:     return "*";
            case kTokenDiv:       return "/";
            case kTokenMod:       return "%";
            case kTokenEQ:        return "==";
            case kTokenNE:        return "!=";
            case kTokenLT:        return "<";
            case kTokenLE:        return "<=";
            case kTokenGT:        return ">";
            case kTokenGE:        return ">=";
            case kTokenAnd:       return "&&";
            case kTokenOr:        return "||";
            case kTokenNot:       return "!";
            case kTokenLbrace:    return "{";
            case kTokenRbrace:    return "}";
            case kTokenLbracket:    return "[";
            case kTokenRbracket:    return "]";
            case kTokenLparen:    return "(";
            case kTokenRparen:    return ")";
            case kTokenComma:     return ",";
            case kTokenAssign:    return "=";
            case kTokenSemicolon: return ";";
            case kTokenEOF:       return "<EOF>";
            case kTokenUnknown:    return "Unknown character";
            default: return "unknown";
        }
    }


private:
    TokenType    m_type;
    int         m_int;
    float        m_float;
    int line;
    int column;
    std::string m_id;
};





