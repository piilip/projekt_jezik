#include "Token.h"
#include <cctype>
#include <iostream>
#include <string>
#include <utility>

class Scanner {
public:
  explicit Scanner(const char *source)
      : source(source), current(0), line(1), column(1) {}

  Token nextToken() {
    skipWhitespace();

    if (isAtEnd()) {
      return makeToken(TokenType::kTokenEOF, "", line, column);
    }

    // Store position before advancing for the token
    int token_line = line;
    int token_column = column;

    char c = advance();

    if (isAlpha(c))
      return identifier(token_line, token_column);
    if (isdigit(c) || (c == '-' && isDigit(peek()))) {
      return number(token_line, token_column);
    }

    switch (c) {
    case '+':
      return makeToken(TokenType::kTokenPlus, "+", token_line, token_column);
    case '-':
      return makeToken(TokenType::kTokenMinus, "-", token_line, token_column);
    case '*':
      return makeToken(TokenType::kTokenTimes, "*", token_line, token_column);
    case '/':
      return makeToken(TokenType::kTokenDiv, "/", token_line, token_column);
    case '%':
      return makeToken(TokenType::kTokenMod, "%", token_line, token_column);
    case '!':
      if (peek() == '=') {
        advance(); // consume the '='
        return makeToken(TokenType::kTokenNE, "!=", token_line, token_column);
      } else {
        return makeToken(TokenType::kTokenNot, "!", token_line, token_column);
      }
    case '=':
      if (peek() == '=') {
        advance(); // consume the '='
        return makeToken(TokenType::kTokenEQ, "==", token_line, token_column);
      } else {
        return makeToken(TokenType::kTokenAssign, "=", token_line,
                         token_column);
      }
    case '<':
      if (peek() == '=') {
        advance(); // consume the '='
        return makeToken(TokenType::kTokenLE, "<=", token_line, token_column);
      } else {
        return makeToken(TokenType::kTokenLT, "<", token_line, token_column);
      }
    case '>':
      if (peek() == '=') {
        advance(); // consume the '='
        return makeToken(TokenType::kTokenGE, ">=", token_line, token_column);
      } else {
        return makeToken(TokenType::kTokenGT, ">", token_line, token_column);
      }
    case '&':
      if (peek() == '&') {
        advance(); // consume the '&'
        return makeToken(TokenType::kTokenAnd, "&&", token_line, token_column);
      } else {
        return makeToken(TokenType::kTokenUnknown, "&", token_line,
                         token_column);
      }
    case '|':
      if (peek() == '|') {
        advance(); // consume the '|'
        return makeToken(TokenType::kTokenOr, "||", token_line, token_column);
      } else {
        return makeToken(TokenType::kTokenUnknown, "|", token_line,
                         token_column);
      }
    case '{':
      return makeToken(TokenType::kTokenLbrace, "{", token_line, token_column);
    case '}':
      return makeToken(TokenType::kTokenRbrace, "}", token_line, token_column);
    case '[':
      return makeToken(TokenType::kTokenLbracket, "[", token_line,
                       token_column);
    case ']':
      return makeToken(TokenType::kTokenRbracket, "]", token_line,
                       token_column);
    case '(':
      return makeToken(TokenType::kTokenLparen, "(", token_line, token_column);
    case ')':
      return makeToken(TokenType::kTokenRparen, ")", token_line, token_column);
    case ',':
      return makeToken(TokenType::kTokenComma, ",", token_line, token_column);
    case ';':
      return makeToken(TokenType::kTokenSemicolon, ";", token_line,
                       token_column);

    default:
      return unknownCharacter(c, token_line, token_column);
    }
  }

private:
  std::string source;
  size_t current;
  int line;
  int column;

  char advance() {
    char c = source[current++];
    if (c == '\n') {
      line++;
      column = 1;
    } else {
      column++;
    }
    return c;
  }

  [[nodiscard]] bool isAtEnd() const { return current >= source.size(); }

  void skipWhitespace() {
    while (!isAtEnd() && std::isspace(peek())) {
      advance();
    }
  }

  bool isAlpha(char c) const { return std::isalpha(c) || c == '_'; }

  bool isDigit(char c) const { return std::isdigit(c); }

  char peek() const {
    if (isAtEnd())
      return '\0';
    return source[current];
  }

  char peekNext() const {
    if (current + 1 >= source.size())
      return '\0';
    return source[current + 1];
  }

  Token identifier(int tokenLine, int tokenColumn) {
    size_t start = current - 1;
    while (isAlpha(peek()) || isDigit(peek()))
      advance();

    std::string lexeme = source.substr(start, current - start);

    // Keyword check
    if (lexeme == "float")
      return makeToken(TokenType::kTokenFloat, lexeme, tokenLine, tokenColumn);
    else if (lexeme == "bool")
      return makeToken(TokenType::kTokenBool, lexeme, tokenLine, tokenColumn);
    else if (lexeme == "true")
      return makeToken(TokenType::kTokenTrue, lexeme, tokenLine, tokenColumn);
    else if (lexeme == "false")
      return makeToken(TokenType::kTokenFalse, lexeme, tokenLine, tokenColumn);
    else if (lexeme == "int")
      return makeToken(TokenType::INT, lexeme, tokenLine, tokenColumn);
    else if (lexeme == "if")
      return makeToken(TokenType::kTokenIf, lexeme, tokenLine, tokenColumn);
    else if (lexeme == "else")
      return makeToken(TokenType::kTokenElse, lexeme, tokenLine, tokenColumn);
    else if (lexeme == "return")
      return makeToken(TokenType::kTokenReturn, lexeme, tokenLine, tokenColumn);
    else if (lexeme == "while")
      return makeToken(TokenType::kTokenWhile, lexeme, tokenLine, tokenColumn);
    else if (lexeme == "operator")
      return makeToken(TokenType::kTokenOperator, lexeme, tokenLine,
                       tokenColumn);
    else if (lexeme == "for")
      return makeToken(TokenType::kTokenFor, lexeme, tokenLine, tokenColumn);

    return makeToken(TokenType::kTokenId, lexeme, tokenLine, tokenColumn);
  }

  Token number(int tokenLine, int tokenColumn) {
    size_t start = current - 1;

    // Check whether it's negative number
    bool isNegative = false;
    if (start > 0 && source[start] == '-') {
      isNegative = true;
    }

    while (isDigit(peek()))
      advance();

    // Check whether it's a float
    if (peek() == '.' && isDigit(peekNext())) {
      advance(); // consume '.'
      while (isDigit(peek()))
        advance();
    }

    std::string value = source.substr(start, current - start);

    TokenType type = value.find('.') != std::string::npos
                         ? TokenType::kTokenFPNum
                         : TokenType::kTokenIntNum;
    return makeToken(type, value, tokenLine, tokenColumn);
  }

  Token makeToken(TokenType type, const std::string &value, int line,
                  int column) {
    if (type == TokenType::kTokenIntNum) {
      return Token(std::stoi(value), line, column);
    } else if (type == TokenType::kTokenFPNum) {
      return Token(std::stof(value), line, column);
    } else if (type == TokenType::kTokenId) {
      return Token(value, line, column);
    } else {
      return Token(type, line, column);
    }
  }

  Token unknownCharacter(char c, int tokenLine, int tokenColumn) {
    return makeToken(TokenType::kTokenUnknown, std::string(1, c), tokenLine,
                     tokenColumn);
  }
};