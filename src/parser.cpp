#include "Headers/error_log.h"
#include "Headers/lexer.h"
#include "Headers/parser.h"

// +++++++++++++++++++++++
// +-----+ GLOBALS +-----+
// +++++++++++++++++++++++

// Buffer for tokens and function to get then 
int CurToken;
void getNextToken() {
    CurToken = Tokenizer();
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
std::unique_ptr<DeclarationAST> ExpressionParser \
    (std::shared_ptr<BlockAST> CurBlock);
std::unique_ptr<DeclarationAST> StatementParser \
    (std::shared_ptr<BlockAST> CurBlock);

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
std::unique_ptr<DeclarationAST> ParenParser(std::shared_ptr<BlockAST> CurBlock) 
{
    getNextToken(); // consume '('
    
    auto Expr = ExpressionParser(CurBlock);
    if (!Expr) {
        return nullptr;
    }

    if (CurToken != ')') {
       ErLogs.PushError(Identifier, "expected a '('", 1); 
       return nullptr;
    }
    getNextToken(); // consume ')'
    return Expr;
}

// variable -> any
std::unique_ptr<DeclarationAST> \
    VariableParser(std::shared_ptr<BlockAST> CurBlock) 
{
    getNextToken(); // consume id
    std::string VarName = Identifier;

    return std::make_unique<VarValAST>(Identifier, CurBlock);
}

// primary -> number
//         |  bool
//         |  string
//         |  null
//         |  parenexpr
//         |  variable
std::unique_ptr<DeclarationAST> PrimaryParser(std::shared_ptr<BlockAST> CurBlock) 
{
    switch(CurToken) {
        default:{
            ErLogs.PushError(Identifier, "expression not identified", 1); 
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
            return ParenParser(CurBlock);
        case TOKEN_ID:
            return VariableParser(CurBlock);
    }
}

// unaryexpr -> '!'|'-' unary
//           |  primary
std::unique_ptr<DeclarationAST> UnaryParser(std::shared_ptr<BlockAST> CurBlock) 
{
    // If the current token is not a operator, it must be a primary
    if (!isUnary() || CurToken == '(' || CurToken == ',') {
        return PrimaryParser(CurBlock);
    }
    
    // If is a unary operator read it
    int Op = CurToken;
    getNextToken(); // consume '!'|'-'
    
    auto Operand = UnaryParser(CurBlock);
    if (!Operand) {
        return nullptr;
    }
    return std::make_unique<UnaryAST>(std::move(Operand), Op);
}

// operation -> number 
//           |  number '+' operation
std::unique_ptr<DeclarationAST> OperationParser(int PrecLHS, 
                                        std::unique_ptr<DeclarationAST> LHS,
                                        std::shared_ptr<BlockAST> CurBlock) 
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
        auto RHS = UnaryParser(CurBlock);
        if (!RHS) {
            return nullptr;
        }

        // Gets the next operation
        int NextPrec = getPrecedence();

        // If the precedence of the next operation is higher than the current
        // one parses the RHS
        if (PrecRHS < NextPrec) {
            // Removing PrecRHS+1 if error is that
            RHS = OperationParser(PrecRHS+1, std::move(RHS), CurBlock);
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
std::unique_ptr<DeclarationAST> \
    ExpressionParser(std::shared_ptr<BlockAST> CurBlock) 
{
    auto LHS = UnaryParser(CurBlock);
    if (!LHS) {
        return nullptr;
    }

    return OperationParser(0, std::move(LHS), CurBlock);
}

// printstmt -> print parenexpr
std::unique_ptr<DeclarationAST> PrintParser(std::shared_ptr<BlockAST> CurBlock) 
{
    getNextToken(); // consume print
    
    if (CurToken != '(') {
       ErLogs.PushError(Identifier, "expected a '('", 1); 
       return nullptr;
    }
    
    auto Expr = ParenParser(CurBlock);
    if (!Expr) {
        return nullptr;
    }

    return std::make_unique<PrintAST>(std::move(Expr));
}

// vardecl -> var id '(' = expression ')'?
std::unique_ptr<DeclarationAST> \
    VarAssignParser(std::shared_ptr<BlockAST> CurBlock) 
{
    int Decl = 0;
    if (CurToken == TOKEN_VAR) {
        Decl = 1; // its a first declaration
        getNextToken(); // consume var
    }

    std::string VarName = Identifier;
    getNextToken(); // consume identifier

    if (CurToken != TOKEN_ATR) {
        return std::make_unique<VarDeclAST>(VarName, Decl, nullptr, 
                                            CurBlock);
    }
    
    getNextToken(); // consume '='
    auto Expr = ExpressionParser(CurBlock);
    if (!Expr) {
       ErLogs.PushError(Identifier, "expression was not reconized", 1); 
       return nullptr;
    }
    return std::make_unique<VarDeclAST>(VarName, Decl, std::move(Expr), 
                                        CurBlock);
}

// inside -> statement
std::unique_ptr<DeclarationAST> InsideParser(std::shared_ptr<BlockAST> CurBlock) {
    if (CurToken == '}') {
        return std::make_unique<NullAST>();
    }

    auto Stmt = StatementParser(CurBlock);
    if (CurToken == ';') {
        getNextToken(); // consume ';'
    }
    if (!Stmt) {
        return nullptr;
    }

    return std::make_unique<InsideAST>(InsideParser(CurBlock), 
                                       std::move(Stmt));
}

// block -> '{' inside '}'
std::unique_ptr<DeclarationAST> BlockParser(std::shared_ptr<BlockAST> CurBlock)
{
    getNextToken(); // consume '{'

    std::shared_ptr<BlockAST> CodeBlock = 
        std::make_shared<BlockAST>(CurBlock, COMMON);

    auto Inside = InsideParser(CodeBlock);

    if (CurToken != '}') {
       ErLogs.PushError("", "expected a '}'", 1); 
       return nullptr;
    }
    getNextToken(); // consume '}'
    return Inside;
}

// statement -> printstmt
//           |  vardecl
//           |  block
std::unique_ptr<DeclarationAST> \
    StatementParser(std::shared_ptr<BlockAST> CurBlock) 
{
    switch(CurToken) {
        case TOKEN_PRINT:
            return PrintParser(CurBlock);
        case TOKEN_VAR:
            return VarAssignParser(CurBlock);
        case TOKEN_ID:
            return VarAssignParser(CurBlock); // Change this when function added
        case '{':
            return BlockParser(CurBlock); 
        default: {
            ErLogs.PushError(Identifier, "statement not identified", 1); 
            return nullptr;
        }
    }
}

// declaration -> statement
//             |  expression
std::unique_ptr<DeclarationAST> \
    DeclarationParser(std::shared_ptr<BlockAST> CurBlock) 
{
    switch(CurToken) {
        case TOKEN_DOUBLE:
            return ExpressionParser(CurBlock);
        case TOKEN_NULL:
            return ExpressionParser(CurBlock);
        case TOKEN_STRING:
            return ExpressionParser(CurBlock);
        case TOKEN_TRUE:
            return ExpressionParser(CurBlock);
        case TOKEN_FALSE:
            return ExpressionParser(CurBlock);
        case '(':
            return ExpressionParser(CurBlock);
        case '{':
            return StatementParser(CurBlock);
        default:
            return StatementParser(CurBlock);
    }
}

// program -> declaration
std::unique_ptr<DeclarationAST> Parser(std::shared_ptr<BlockAST> Global) 
{
    if (CurToken == 0 || CurToken == ';'){
        getNextToken(); // Get the first token
    }

    if (CurToken == TOKEN_EOF) {
        return nullptr;
    }
    
    auto Program = DeclarationParser(Global);
    if (!Program) {
        return nullptr;
    }
    return Program;
}
