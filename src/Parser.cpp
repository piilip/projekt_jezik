#include "Parser.h"
#include "Exp.h"
#include "FuncDef.h"
#include "Program.h"
#include "Stmt.h"
#include "TokenStream.h"

namespace {


    class ParseError : public std::runtime_error {
    public:
        explicit ParseError(const std::string &msg, int line, int column)
                : std::runtime_error(msg + " at line " + std::to_string(line) + ", column " + std::to_string(column)) {
        }
    };


    // Forward declarations
    ExpPtr parseExp(TokenStream &tokens);

    std::vector<ExpPtr> parseArgs(TokenStream &tokens);

    SeqStmtPtr parseSeq(TokenStream &tokens);

    ExpPtr parseRemainingExp(ExpPtr leftExp, int leftPrecedence, TokenStream &tokens);

    StmtPtr parseStmt(TokenStream &tokens, bool inForLoop = false);

    int getPrecedence(const Token &token);


// Skip the specified token, throwing ParseError if it's not present.
    void skipToken(TokenType expected, TokenStream &tokens) {
        Token token(*tokens++);
        if (token != expected)
            throw ParseError(std::string("Expected '") + Token::typeToString(expected) + "'", token.getLine(),
                             token.getColumn());
    }


    ExpPtr parsePrimaryExp(TokenStream &tokens) {
        // Fetch the next token, advancing the token stream.  (Note that this
        // dereferences, then increments the TokenStream.)
        Token token(*tokens++);
        switch (token.getType()) {
            // Boolean constant?
            case kTokenTrue:
                return std::make_unique<BoolExp>(true);
            case kTokenFalse:
                return std::make_unique<BoolExp>(false);
                // Integer constant?
            case kTokenIntNum:
                return std::make_unique<IntExp>(token.getNum<int>());
            case kTokenFPNum:
                return std::make_unique<FloatExp>(token.getNum<float>());
                // An identifier might be a variable or the start of a function call.
            case kTokenId: {
                if (*tokens == kTokenLbracket) {
                    // This is an array access
                    ++tokens; // move past '['
                    ExpPtr indexExp = parseExp(tokens); // parse the index expression
                    skipToken(kTokenRbracket, tokens); // expect and skip ']'
                    return std::make_unique<ArrayAccessExp>(token.getId(), std::move(indexExp));
                }

                // If the next token is a left paren, it's a function call.
                if (*tokens == kTokenLparen) {
                    // Parse argument expressions and construct CallExp.
                    return std::make_unique<CallExp>(token.getId(), parseArgs(tokens));
                }

                return std::make_unique<VarExp>(token.getId());
            }
            case kTokenBool:
            case kTokenInt:
            case kTokenFloat: {
                return std::make_unique<CallExp>(token.ToString(), parseArgs(tokens));
            }
                // Parenthesized expression
            case kTokenLparen: {
                skipToken(kTokenLparen, tokens);
                ExpPtr exp(parseExp(tokens));
                skipToken(kTokenRparen, tokens);
                return exp;
            }
                // Prefix minus
            case kTokenMinus: {

                ExpPtr exp(parsePrimaryExp(tokens));
                return std::make_unique<CallExp>(token.ToString(), std::move(exp));
            }
            default:
                throw ParseError(std::string("Unexpected token: ") + token.ToString(), token.getLine(),
                                 token.getColumn());
        }
    }

    std::vector<ExpPtr> parseArgs(TokenStream &tokens) {
        skipToken(kTokenLparen, tokens);
        std::vector<ExpPtr> exps;
        if ((*tokens).getType() != kTokenRparen) {
            exps.push_back(parseExp(tokens));
            while (*tokens == kTokenComma) {
                exps.push_back(parseExp(++tokens));
            }
        }
        skipToken(kTokenRparen, tokens);

        return std::move(exps);
    }


// Parse an expression with infix operators.
    ExpPtr parseExp(TokenStream &tokens) {


        // First, parse a primary expression, which contains no infix operators.
        ExpPtr leftExp(parsePrimaryExp(tokens));

        // The next token might be an operator.  Call a helper routine
        // to parse the remainder of the expression.
        return parseRemainingExp(std::move(leftExp), 0 /*initial precedence*/, tokens);
    }

    // operator precedence expression parser.
    ExpPtr parseRemainingExp(ExpPtr leftExp, int leftPrecedence, TokenStream &tokens) {
        while (true) {
            // If the previous operator has higher precedence than the current one,
            // it claims the previously parsed expression.
            int precedence = getPrecedence(*tokens);
            if (leftPrecedence > precedence)
                return leftExp;

            // Parse the current operator and the current primary expression.
            auto opToken = *tokens;
            ++tokens;
            ExpPtr rightExp = parsePrimaryExp(tokens);

            // If the next operator has higher precedence, it claims the current expression.
            int rightPrecedence = getPrecedence(*tokens);
            if (rightPrecedence > precedence) {
                rightExp = parseRemainingExp(std::move(rightExp), precedence + 1, tokens);
            }

            // Construct a call expression with the left and right expressions.
            leftExp = std::make_unique<CallExp>(opToken.ToString(),
                                                std::move(leftExp), std::move(rightExp));
        }
    }

    int getPrecedence(const Token &token) {
        switch (token.getType()) {
            case kTokenTimes:
            case kTokenDiv:
                return 5;
            case kTokenMod:
            case kTokenPlus:
            case kTokenMinus:
                return 4;
            case kTokenLT:
            case kTokenLE:
            case kTokenGT:
            case kTokenGE:
                return 3;
            case kTokenEQ:
            case kTokenNE:
                return 2;
            case kTokenAnd:
                return 1;
            case kTokenOr:
                return 0;
            default:
                return -1;
        }
    }


    Type parseType(TokenStream &tokens) {
        Token typeName = *tokens++;
        switch (typeName.getType()) {
            case kTokenBool:
                return kTypeBool;
            case kTokenInt:
                return kTypeInt;
            case kTokenFloat:
                return kTypeFloat;
            default:
                throw ParseError("Expected type name", typeName.getLine(), typeName.getColumn());
        }
    }

    std::string parseId(TokenStream &tokens) {
        Token id = *tokens++;
        if (id.getType() != kTokenId)
            throw ParseError("Invalid declaration (expected identifier)", id.getLine(), id.getColumn());
        return id.getId();
    }

    int parseArray(TokenStream &tokens) {

        Token token(*(++tokens));
        if (token.getType() != kTokenIntNum) {
            throw ParseError("Array size must be an integer number", token.getLine(), token.getColumn());
        }
        int arraySize = token.getNum<int>();
        tokens++;
        // Consume the size token and the closing ']'
        skipToken(kTokenRbracket, tokens);
        // Construct and return an ArrayType
        return arraySize;
    }

    StmtPtr parseForStmt(TokenStream &tokens) {
        skipToken(kTokenFor, tokens);
        skipToken(kTokenLparen, tokens);

        // Parse initialization part
        StmtPtr initStmt = parseStmt(tokens, true);
        skipToken(kTokenSemicolon, tokens);
        // Parse condition expression
        ExpPtr condExp = parseExp(tokens);

        skipToken(kTokenSemicolon, tokens);
        // Parse update expression
        StmtPtr updateStmt = parseStmt(tokens, true);
        skipToken(kTokenRparen, tokens);

        // Parse body of the loop
        StmtPtr bodyStmt = parseStmt(tokens);

        return std::make_unique<ForStmt>(std::move(initStmt), std::move(condExp), std::move(updateStmt),
                                         std::move(bodyStmt));
    }

    VarDeclPtr parseVarDecl(VarDecl::Kind kind, TokenStream &tokens) {
        Type type(parseType(tokens));
        std::string id(parseId(tokens));
        if (*tokens == kTokenLbracket) {
            auto arraySize = parseArray(tokens);
            VarType arrType(type, arraySize);
            return std::make_unique<VarDecl>(kind, arrType, id);
        }
        return std::make_unique<VarDecl>(kind, type, id);
    }


    StmtPtr parseStmt(TokenStream &tokens, bool inForLoop) {
        Token token(*tokens);
        switch (token.getType()) {
            case kTokenId: {
                Token id(*tokens++);
                if (*tokens == kTokenLbracket) {
                    // This is an array assignment
                    ++tokens; // move past '['
                    ExpPtr indexExp = parseExp(tokens); // parse the index expression
                    skipToken(kTokenRbracket, tokens); // expect and skip ']'

                    skipToken(kTokenAssign, tokens); // expect and skip '='
                    ExpPtr rvalue = parseExp(tokens); // parse the right-hand side expression

                    skipToken(kTokenSemicolon, tokens); // expect and skip ';'
                    return std::make_unique<ArrayAssignStmt>(id.getId(), std::move(indexExp), std::move(rvalue));
                }

                if (*tokens == kTokenAssign) {
                    // Assignment
                    ExpPtr rvalue(parseExp(++tokens));
                    if (!inForLoop) {
                        skipToken(kTokenSemicolon, tokens);
                    }
                    return std::make_unique<AssignStmt>(id.getId(), std::move(rvalue));
                } else {
                    // Call
                    std::vector<ExpPtr> args(parseArgs(tokens));
                    CallExpPtr callExp(std::make_unique<CallExp>(id.getId(), std::move(args)));
                    skipToken(kTokenSemicolon, tokens);
                    return std::make_unique<CallStmt>(std::move(callExp));
                }
            }
            case kTokenInt:
            case kTokenBool:
            case kTokenFloat: {
                // Declaration
                VarDeclPtr varDecl(parseVarDecl(VarDecl::kLocal, tokens));
                ExpPtr initExp;
                if (*tokens == kTokenAssign) {
                    initExp = parseExp(++tokens);
                }
                if (!inForLoop) {
                    skipToken(kTokenSemicolon, tokens);
                }
                return std::make_unique<DeclStmt>(std::move(varDecl), std::move(initExp));
            }
            case kTokenLbrace: {
                // Sequence
                return parseSeq(tokens);
            }
            case kTokenReturn: {
                ++tokens;  // skip "return"
                ExpPtr returnExp(parseExp(tokens));
                skipToken(kTokenSemicolon, tokens);
                return std::make_unique<ReturnStmt>(std::move(returnExp));
            }
            case kTokenIf: {
                ++tokens;  // skip "if"
                skipToken(kTokenLparen, tokens);
                ExpPtr condExp(parseExp(tokens));
                skipToken(kTokenRparen, tokens);

                StmtPtr thenStmt(parseStmt(tokens));
                StmtPtr elseStmt;
                if (*tokens == kTokenElse) {
                    ++tokens;  // skip "else"
                    elseStmt = parseStmt(tokens);
                }
                return std::make_unique<IfStmt>(std::move(condExp), std::move(thenStmt), std::move(elseStmt));
            }
            case kTokenWhile: {
                ++tokens;  // skip "while"
                skipToken(kTokenLparen, tokens);
                ExpPtr condExp(parseExp(tokens));
                skipToken(kTokenRparen, tokens);

                StmtPtr bodyStmt(parseStmt(tokens));
                return std::make_unique<WhileStmt>(std::move(condExp), std::move(bodyStmt));
            }
            case kTokenFor:
                return parseForStmt(tokens);
            default:
                throw ParseError(std::string("Unexpected token: ") + token.ToString(), token.getLine(),
                                 token.getColumn());
        }
    }


    SeqStmtPtr parseSeq(TokenStream &tokens) {
        skipToken(kTokenLbrace, tokens);
        std::vector<StmtPtr> stmts;
        while (*tokens != kTokenRbrace) {
            stmts.push_back(parseStmt(tokens));
        }
        skipToken(kTokenRbrace, tokens);
        return std::make_unique<SeqStmt>(std::move(stmts));
    }

    std::string parseFuncId(TokenStream &tokens) {
        if (*tokens == kTokenOperator) {
            ++tokens;
            Token op(*tokens++);
            if (!op.isOperator())
                throw ParseError("Invalid operator", op.getLine(), op.getColumn());
            return op.ToString();
        } else
            return parseId(tokens);
    }

    FuncDefPtr parseFuncDef(TokenStream &tokens) {

        // Parse return type and function id.
        Type returnType(parseType(tokens));
        std::string id(parseFuncId(tokens));

        // Parse parameter declarations
        skipToken(kTokenLparen, tokens);
        std::vector<VarDeclPtr> params;
        if (*tokens != kTokenRparen) {
            params.push_back(parseVarDecl(VarDecl::kParam, tokens));
            while (*tokens != kTokenRparen) {
                skipToken(kTokenComma, tokens);
                params.push_back(parseVarDecl(VarDecl::kParam, tokens));
            }
        }
        skipToken(kTokenRparen, tokens);

        // Parse function body
        SeqStmtPtr body;
        if (*tokens == kTokenLbrace)
            body = parseSeq(tokens);
        else
            skipToken(kTokenSemicolon, tokens);

        return std::make_unique<FuncDef>(returnType, id, std::move(params), std::move(body));
    }

}

// Adding function definitions to the program
int ParseProgram(TokenStream &tokens, Program *program) {
    try {
        do {
            FuncDefPtr function(parseFuncDef(tokens));
            program->GetFunctions().push_back(std::move(function));
        } while (*tokens != kTokenEOF);
        return 0;
    }
    catch (const ParseError &error) {
        std::cerr << "Error: " << error.what() << std::endl;
        return -1;
    }
}

