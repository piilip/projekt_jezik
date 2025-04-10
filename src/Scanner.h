

#include "Token.h"
#include <string>
#include <cctype>
#include <iostream>
#include <utility>
class Scanner {
public:
    explicit Scanner(const char* source) : source(source), current(0), line(1), column(0) {}

    Token nextToken() {
        skipWhitespace();

        if (isAtEnd()) {
            return makeToken(TokenType::kTokenEOF, "", line, column);
        }

        char c = advance();

        if (isAlpha(c)) return identifier();
        if (isdigit(c) || (c == '-' && isDigit(peek()))) {
            return number();
        }

        switch (c) {
            case '+': return makeToken(TokenType::kTokenPlus, "+", line, column - 1);
            case '-': return makeToken(TokenType::kTokenMinus, "-", line, column - 1);
            case '*': return makeToken(TokenType::kTokenTimes, "*", line, column - 1);
            case '/': return makeToken(TokenType::kTokenDiv, "/", line, column - 1);
            case '%': return makeToken(TokenType::kTokenMod, "%", line, column - 1);
            case '!':
                return peek() == '=' ? makeToken(TokenType::kTokenNE, "!=", line, column - 1, true)
                                     : makeToken(TokenType::kTokenNot, "!", line, column - 1);
            case '=':
                return peek() == '=' ? makeToken(TokenType::kTokenEQ, "==", line, column - 1, true)
                                     : makeToken(TokenType::kTokenAssign, "=", line, column - 1);  // Single '=' is usually an assignment, not an operator
            case '<':
                return peek() == '=' ? makeToken(TokenType::kTokenLE, "<=", line, column - 1, true)
                                     : makeToken(TokenType::kTokenLT, "<", line, column - 1);
            case '>':
                return peek() == '=' ? makeToken(TokenType::kTokenGE, ">=", line, column - 1, true)
                                     : makeToken(TokenType::kTokenGT, ">", line, column - 1);
            case '&':
                return peek() == '&' ? makeToken(TokenType::kTokenAnd, "&&", line, column - 1, true)
                                     : makeToken(TokenType::kTokenUnknown, "&", line, column - 1);  // Single '&' might be a bitwise AND
            case '|':
                return peek() == '|' ? makeToken(TokenType::kTokenOr, "||", line, column - 1, true)
                                     : makeToken(TokenType::kTokenUnknown, "|", line, column - 1);  // Single '|' might be a bitwise OR
            case '{': return makeToken(TokenType::kTokenLbrace, "{", line, column - 1);
            case '}': return makeToken(TokenType::kTokenRbrace, "}", line, column - 1);
            case '[': return makeToken(TokenType::kTokenLbracket, "[", line, column - 1);
            case ']': return makeToken(TokenType::kTokenRbracket, "]", line, column - 1);
            case '(': return makeToken(TokenType::kTokenLparen, "(", line, column - 1);
            case ')': return makeToken(TokenType::kTokenRparen, ")", line, column - 1);
            case ',': return makeToken(TokenType::kTokenComma, ",", line, column - 1);
            case ';': return makeToken(TokenType::kTokenSemicolon, ";", line, column - 1);

            default:
                return unknownCharacter(c);
        }
    }

private:
    std::string source;
    size_t current;
    int line;
    int column;

    char advance() {
        char c = source[current++];
        if (c == '\n') { line++; column = 1; } else { column++; }
        return c;
    }

    bool isAtEnd() const { return current >= source.size(); }

    void skipWhitespace() {
        while (!isAtEnd() && std::isspace(peek())) {
            if (peek() == '\n') {
                line++;
                column = 0;
            }
            advance();
        }
    }
    bool isAlpha(char c) const {
        return std::isalpha(c) || c == '_';
    }

    bool isDigit(char c) const {
        return std::isdigit(c);
    }

    char peek() const {
        if (isAtEnd()) return '\0';
        return source[current];
    }

    char peekNext() const {
        if (current + 1 >= source.size()) return '\0';
        return source[current + 1];
    }

    Token identifier() {
        size_t start = current - 1;
        while (isAlpha(peek()) || isDigit(peek())) advance();

        std::string lexeme = source.substr(start, current - start);


        // Keyword check
        if (lexeme == "float") return makeToken(TokenType::kTokenFloat, lexeme, line, column);
        else if (lexeme == "bool") return makeToken(TokenType::kTokenBool, lexeme, line, column);
        else if (lexeme == "true") return makeToken(TokenType::kTokenTrue, lexeme, line, column);
        else if (lexeme == "false") return makeToken(TokenType::kTokenFalse, lexeme, line, column);
        else if (lexeme == "int") return makeToken(TokenType::INT, lexeme, line, column);
        else if (lexeme == "if") return makeToken(TokenType::kTokenIf, lexeme, line, column);
        else if (lexeme == "else") return makeToken(TokenType::kTokenElse, lexeme, line, column);
        else if (lexeme == "return") return makeToken(TokenType::kTokenReturn, lexeme, line, column);
        else if (lexeme == "while") return makeToken(TokenType::kTokenWhile, lexeme, line, column);
        else if (lexeme == "operator") return makeToken(TokenType::kTokenOperator, lexeme, line, column);
        else if (lexeme == "for") return makeToken(TokenType::kTokenFor, lexeme, line, column);

        return makeToken(TokenType::kTokenId, lexeme, line, column);
    }

    Token number() {
        size_t start = current - 1;

        // Check whether it's negative number
        bool isNegative = false;
        if (source[start-1] == '-') {
            isNegative = true;
            advance();
        }

        while (isDigit(peek())) advance();

        // Check whether it's a float
        if (peek() == '.' && isDigit(peekNext())) {
            advance();
            while (isDigit(peek())) advance();
        }

        std::string value = source.substr(start, current - start);


        if (isNegative) {
            value = "-" + value;
        }

        TokenType type = value.find('.') != std::string::npos ? TokenType::kTokenFPNum : TokenType::kTokenIntNum;
        return makeToken(type, value, line, column - value.size());
    }


    Token makeToken(TokenType type, const std::string& value, int line, int column, char nextChar = '\0') {
        if (nextChar != '\0') {
            advance();
        }
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

    Token unknownCharacter(char c) {
        //std::cerr << "Error on line " << line << ", column " << column << ": unidentified character '" << c << "'" << std::endl;
        return makeToken(TokenType::kTokenUnknown, std::string(1, c), line, column - 1);
    }

};

