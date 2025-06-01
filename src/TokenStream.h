#pragma once

#include "Scanner.h"
#include "Token.h"

class TokenStream {
public:
  explicit TokenStream(const char *source)
      : scanner(source), currentToken(scanner.nextToken()) {}

  const Token &operator*() const { return currentToken; }

  TokenStream &operator++() {
    currentToken = scanner.nextToken();
    return *this;
  }

  // Postfix, used when we want to get current token and advance the stream
  TokenStream operator++(int) {
    TokenStream temp(*this);
    this->operator++();
    return temp;
  }
  void printCurrentToken() const {
    std::cout << currentToken.ToString() << '\n';
  }

  void printCurrentTokenWithLocation() const {
    std::cout << "Token: " << currentToken.ToString() << " "
              << "'" << Token::typeToString(currentToken.getType()) << "'"
              << " at line " << currentToken.getLine() << ", column "
              << currentToken.getColumn() << '\n';
  }

  // Consumes the stream
  void printAllTokens() {
    while (currentToken.getType() != kTokenEOF) {
      printCurrentTokenWithLocation();
      ++(*this);
    }
    // print EOF too
    printCurrentTokenWithLocation();
  }

private:
  Scanner scanner;
  Token currentToken;
};