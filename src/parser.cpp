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

///////////////////////////////////////////////////////////////////////////////
/////////                           PARSER                            /////////
///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<DeclarationAST> DoubleParser() {
    getNextToken(); // consume double
    return std::make_unique<DoubleAST>(DoubleBuffer);
}

// number -> double
std::unique_ptr<DeclarationAST> NumberParser() {
    if (CurToken == TOKEN_DOUBLE) {
        return DoubleParser();
    }
}

// expression -> number
std::unique_ptr<DeclarationAST> ExpressionParser() {
    switch(CurToken) {
        default:
            return NumberParser();
    }
}

// statement -> expression
std::unique_ptr<DeclarationAST> StatementParser() {
    switch(CurToken) {
        default:
            return ExpressionParser();
    }
}

// declaration -> statement
std::unique_ptr<DeclarationAST> DeclarationParser() { 
    switch(CurToken) {
        default:
            return StatementParser();
    }
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
