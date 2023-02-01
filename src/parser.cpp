// Standard
#include <fstream>
#include <memory>
// Local
#include "error_log.h"
#include "lexer.h"
#include "parser.h"

// +++++++++++++++++++++++
// +-----+ GLOBALS +-----+
// +++++++++++++++++++++++
// File to be parser.
std::shared_ptr<std::fstream> FileParser;

// Buffer for tokens and function to get then 
int CurToken;
void getNextToken() {
    CurToken = Tokenizer(FileParser);
}

// Returns the precedence of operations.
// 1 is the lowest
int getPrecedence() {
    switch(CurToken) {
        default: return -1;
        case TOKEN_ATR: return 2;
        case TOKEN_AND: return 3;
        case TOKEN_OR: return 3;
        case TOKEN_EQUAL: return 5;
        case TOKEN_INEQUAL: return 5;
        case TOKEN_GREATER: return 5;
        case TOKEN_LESS: return 5;
        case TOKEN_GREATEQ: return 5;
        case TOKEN_LESSEQ: return 5;
        case TOKEN_PLUS: return 10;
        case TOKEN_MINUS: return 10;
        case TOKEN_MUL: return 20;
        case TOKEN_DIV: return 20; // highest
    }
}

///////////////////////////////////////////////////////////////////////////////
/////////                           PARSER                            /////////
///////////////////////////////////////////////////////////////////////////////

// Forward definition
std::unique_ptr<DeclarationAST> ExpressionAST();

// number -> double
std::unique_ptr<DeclarationAST> DoubleParser() {
    getNextToken(); // consume double
    return std::make_unique<DoubleAST>(DoubleBuffer);
}

// primary -> number
std::unique_ptr<DeclarationAST> PrimaryParser() {
    switch(CurToken) {
        default:
            return nullptr; // needs to return a error
        case TOKEN_DOUBLE:
            return DoubleParser();
    }
}

// operation -> number | number '+' operation
std::unique_ptr<DeclarationAST> OperationParser(int PrecLHS, 
                                        std::unique_ptr<DeclarationAST> LHS) 
{
    // Mounts the operation precedence in reverse polish
    while (true) {
        int PrecRHS = getPrecedence();

        // If the precedence is higher on the operation passed(LHS) return it.
        if (PrecRHS < PrecLHS) {
            return std::move(LHS);
        }

        // Precedence is higher on the right side. Save current operator
        int Op = CurToken;
        getNextToken(); // consume operator

        // Parse the RHS of the expression 
        auto RHS = PrimaryParser();
        if (!RHS) {
            return nullptr;
        }

        // Gets the next operation
        int NextPrec = getPrecedence();

        // If the precedence of the next operation is higher than the current
        // one parses the RHS
        if (PrecRHS < NextPrec) {
            // Removing PrecRHS+1 if error is that
            RHS = OperationParser(PrecRHS, std::move(RHS));
            if (!RHS) {
                return nullptr;
            }
        }

        // Merge LHS/RHS
        LHS = std::make_unique<OperationAST>(std::move(LHS), std::move(RHS), 
                                            Op);
    }
}

// expression -> operation
std::unique_ptr<DeclarationAST> ExpressionParser() {
    auto LHS = PrimaryParser();
    if (!LHS) {
        return nullptr;
    }

    return OperationParser(0, std::move(LHS));
}

// statement -> expression
std::unique_ptr<DeclarationAST> StatementParser() {
    return ExpressionParser();
}

// declaration -> statement
std::unique_ptr<DeclarationAST> DeclarationParser() { 
    return StatementParser();
}

// program -> declaration
std::unique_ptr<DeclarationAST> Parser(std::shared_ptr<std::fstream> FileInput) 
{
    // Set file as global
    FileParser = FileInput;
    
    getNextToken(); // Get the first token

    auto Program = DeclarationParser();
    if (!Program) {
        return nullptr;
    }
    return std::move(Program);
}
