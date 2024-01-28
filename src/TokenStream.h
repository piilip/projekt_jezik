#pragma once

#include "Scanner.h"

class TokenStream {
public:

    explicit TokenStream(const char* source)
            : scanner(source), currentToken(scanner.nextToken()) {}


    const Token& operator*() const { return currentToken; }


    TokenStream& operator++() {
        currentToken = scanner.nextToken();
        return *this;
    }

    // Postfix, used when we want to get current token and advance the stream
    TokenStream operator++(int) {
        TokenStream temp(*this);
        this->operator++();
        return temp;
    }

private:
    Scanner scanner;
    Token currentToken;
};