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
        case TOKEN_INSIG: return 15;
        case TOKEN_MUL: return 20;
        case TOKEN_DIV: return 20; // highest
    }
}

int isUnary() {
    if (CurToken == TOKEN_MINUS || CurToken ==  TOKEN_NOT) {
        return 1;
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
/////////                           PARSER                            /////////
///////////////////////////////////////////////////////////////////////////////

// Forward definition
std::unique_ptr<DeclarationAST> ExpressionParser();

// number -> double
std::unique_ptr<DeclarationAST> DoubleParser() {
    getNextToken(); // consume double
    return std::make_unique<DoubleAST>(DoubleBuffer);
}

// string
std::unique_ptr<DeclarationAST> StringParser() {
    getNextToken(); // consume string
    return std::make_unique<StringAST>(StringBuffer);
}

// bool
std::unique_ptr<DeclarationAST> BoolParser() {
    if (CurToken == TOKEN_TRUE) {
        getNextToken(); // consume bool
        return std::make_unique<BoolAST>(true);
    }
    getNextToken(); // consume bool
    return std::make_unique<BoolAST>(false);
}

// null
std::unique_ptr<DeclarationAST> NullParser() {
    getNextToken(); // consume null
    return std::make_unique<NullAST>();
}

// parenexpr -> '(' expression ')'
std::unique_ptr<DeclarationAST> ParenParser() {
    getNextToken(); // consume '('
    
    auto Expr = ExpressionParser();
    if (!Expr) {
        return nullptr;
    }

    if (CurToken != ')') {
       PushError(Identifier, "expected a '('", 1); 
       return nullptr;
    }
    getNextToken(); // consume ')'
    return Expr;
}

// primary -> number
//         -> bool
//         -> string
//         -> null
//         -> parenexpr
std::unique_ptr<DeclarationAST> PrimaryParser() {
    switch(CurToken) {
        default:{
            PushError(Identifier, "expression not identified", 1); 
            return nullptr;
        }
        case TOKEN_DOUBLE:
            return DoubleParser();
        case TOKEN_STRING:
            return StringParser();
        case TOKEN_FALSE:
            return BoolParser();
        case TOKEN_TRUE:
            return BoolParser();
        case TOKEN_NULL:
            return NullParser();
        case '(':
            return ParenParser();
    }
}

// unaryexpr -> '!'|'-' unary
//           -> primary
std::unique_ptr<DeclarationAST> UnaryParser() {
    // If the current token is not a operator, it must be a primary
    if (!isUnary() || CurToken == '(' || CurToken == ',') {
        return PrimaryParser();
    }
    
    // If is a unary operator read it
    int Op = CurToken;
    getNextToken(); // consume '!'|'-'
    
    auto Operand = UnaryParser();
    if (!Operand) {
        return nullptr;
    }
    return std::make_unique<UnaryAST>(std::move(Operand), Op);
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
            return LHS;
        }

        // Precedence is higher on the right side. Save current operator
        int Op = CurToken;
        getNextToken(); // consume operator

        // Parse the RHS of the expression 
        auto RHS = UnaryParser();
        if (!RHS) {
            return nullptr;
        }

        // Gets the next operation
        int NextPrec = getPrecedence();

        // If the precedence of the next operation is higher than the current
        // one parses the RHS
        if (PrecRHS < NextPrec) {
            // Removing PrecRHS+1 if error is that
            RHS = OperationParser(PrecRHS+1, std::move(RHS));
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
    auto LHS = UnaryParser();
    if (!LHS) {
        return nullptr;
    }

    return OperationParser(0, std::move(LHS));
}

// printstmt -> print parenexpr
std::unique_ptr<DeclarationAST> PrintParser() {
    getNextToken(); // consume print
    
    if (CurToken != '(') {
       PushError(Identifier, "expected a '('", 1); 
       return nullptr;
    }
    
    auto Expr = ParenParser();
    if (!Expr) {
        return nullptr;
    }

    return std::make_unique<PrintAST>(std::move(Expr));
}

// statement -> expression
//              printstmt
std::unique_ptr<DeclarationAST> StatementParser() {
    switch(CurToken) {
        case TOKEN_PRINT:
            return PrintParser();
        case TOKEN_DOUBLE:
            return ExpressionParser();
        default:{
            PushError(Identifier, "statement not identified", 1); 
            return nullptr;
        }
    }
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
    
    if (CurToken == TOKEN_EOF) {
        return nullptr;
    }
    
    auto Program = DeclarationParser();
    if (!Program) {
        return nullptr;
    }
    return Program;
}
