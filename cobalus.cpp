/******************************************************************************
 * Cobalus Interpreter
 *
 * Name: cobalus
 * Author: John Mag0
 * Date: 2023-01-08
 * Version: 0.1 
 *******************************************************************************
 * MIT Lincense 
 * 
 * Copyright 2023 John Mag0
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 ******************************************************************************/

// Standard C++ Library
#include <algorithm>
#include <cctype> 
#include <cstdlib> 
#include <fstream> 
#include <memory> 
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector> 
#include <iostream> 

// MACRO for variables 
#define dynamic std::variant<double, bool, std::string, char>

std::ifstream FileInput;

//int LineNum = 0; // Counter for line numbers 

// ============================================================================
// ============                          LEXER                     ============
// ============================================================================

// +++++++++++++++++++++++
// +-----+ GLOBALS +-----+
// +++++++++++++++++++++++

std::string Identifier; // Saves the identifiers
std::string StrBuff; // Saves the strings in the variables
double NumBuff; // Saves the numbers(every number will be considered a double)
char Buffer = ' '; // Buffer to read the file char by char

// Saves the tokens of each keyword
std::unordered_map<std::string, int> Keyword;

// Token identifier 
enum Token {
    // The following will be treated as literals
    // ';', '"', '.', '{', '}', '#', ...

    // End of File 
    token_eof = -1,

    // Identifiers 
    token_var = -2,
    token_atr = -3, // =
    token_id = -4,
    
    // Function
    token_func = -5,
    token_ret = -6,

    // Values 
    token_num = -7,
    token_str = -8,
    token_true = -9,
    token_false = -10,
    token_null = -11,

    // Control Flow 
    token_while = -12,
    token_for = -13,
    token_if = -14,
    token_else = -15,
    token_break = -16,

    // Class 
    token_class = -17,
    token_super = -18,
    token_this = -19,

    // Operators:
    // Comparasion
    token_equal = -20,    // ==
    token_inequal = -21,  // != 
    token_greater = -22,  // >
    token_less = -23,     // <
    token_greateq = -24,  // >=
    token_lesseq = -25,   // <=
    // Arithmetic 
    token_minus = -26,    // -
    token_plus = -27,     // +
    token_mul = -28,      // *
    token_div = -29,      // / 
    //token_mod = -30,      // %
    // Logical 
    token_and = -31,      // &&
    token_or = -32,       // ||
    token_not = -33,      // !

    // Standard Lib 
    token_print = -34,

    // #TODO
    // Unary
    //token_uplus = ,    // ++
    //token_uminus = ,   // --
    // BitWise 
    // Binary AND
    // toke_band  = ,   // & 
    // Binary OR
    // token_bor // |
    // Binary Exclusive OR
    // token_xor // ^
    // Binary left shift 
    // token_lshift // <<
    // Binary right shift
    // token_rshift // >>
    // Binary complement
    // token_comp // ~
};


// ++++++++++++++++++++++++
// +-----+ ANALYSIS +-----+
// ++++++++++++++++++++++++

int Tokenizer() {
    // Ignores whitespaces
    while (isspace(Buffer)) {
        FileInput.get(Buffer);
        if (Buffer == '\n') {
           //LineNum++;
        }
        
        // End of File
        if (FileInput.eof()) {
            return token_eof;
        }
    }
    
    // Ignore comments 
    if (Buffer == '#') {
        FileInput.get(Buffer);
        while (Buffer != '\n' && Buffer != '\r' && !FileInput.eof()) {
            FileInput.get(Buffer);
        }

        if (Buffer == '\n') {
            //LineNum++;
            FileInput.get(Buffer);
        }

        if (FileInput.eof()) {
            return token_eof;
        }
    }

    // VALUES
    // Digit: [0-9]+[.][0-9]
    while(isdigit(Buffer)) {
        std::string NumStr;
        NumStr += Buffer;
        FileInput.get(Buffer);
        
        while(isdigit(Buffer)) {
            NumStr += Buffer;
            FileInput.get(Buffer);
        }
        if (Buffer == '.') {
            NumStr += Buffer;
            FileInput.get(Buffer);
            
            while (isdigit(Buffer)) {
                NumStr += Buffer;
                FileInput.get(Buffer);
            }
        }
        
        NumBuff = strtod(NumStr.c_str(), nullptr);
        return token_num;
    }
    
    // String: ".*"
    if (Buffer == '"') {
        FileInput.get(Buffer);
        while(isprint(Buffer)) {
            if (Buffer == '"') {
                FileInput.get(Buffer);
                return token_str;
            }
            StrBuff += Buffer;
            FileInput.get(Buffer);
        }
    }

    // OPERATIONS
    // Arithmetic:
    if (Buffer == '-') {
        FileInput.get(Buffer);
        return token_minus;
    }
    if (Buffer == '+') {
        FileInput.get(Buffer);
        return token_plus;
    }
    if (Buffer == '*') {
        FileInput.get(Buffer);
        return token_mul;
    }
    if (Buffer == '/') {
        FileInput.get(Buffer);
        return token_div;
    }
    /*
    if (Buffer == '%') {
        FileInput.get(Buffer);
        return token_mod;
    }
    */
    // Comparasion, Atribution(=) and Logical(NOT)
    if (Buffer == '=') {
        FileInput.get(Buffer);
        if (Buffer == '=') {
            FileInput.get(Buffer);
            return token_equal;
        }
        return token_atr;
    }
    if (Buffer == '!') {
        FileInput.get(Buffer);
        if (Buffer == '=') {
            FileInput.get(Buffer);
            return token_inequal;
        }
        return token_not;
    }
    if (Buffer == '<') {
        FileInput.get(Buffer);
        if (Buffer == '=') {
            FileInput.get(Buffer);
            return token_lesseq;
        }
        return token_less;
    }
    if (Buffer == '>') {
        FileInput.get(Buffer);
        if (Buffer == '=') {
            FileInput.get(Buffer);
            return token_greateq;
        }
        return token_greater;
    }
    // Logical 
    if (Buffer == '&') {
        FileInput.get(Buffer);
        if (Buffer == '&') {
            FileInput.get(Buffer);
            return token_and;
        }
    }
    if (Buffer == '|') {
        FileInput.get(Buffer);
        if (Buffer == '|') {
            FileInput.get(Buffer);
            return token_or;
        }
    }

    // Verify if is a identifier 
    // [a-zA-Z]+[0-9a-zA-Z_]* 
    if (isalpha(Buffer)) {
        std::string IdTmp;
        IdTmp += Buffer; 
        FileInput.get(Buffer);

        while(isalnum(Buffer) || Buffer == '_') {
            IdTmp += Buffer;
            FileInput.get(Buffer);
        }
        
        if (Keyword[IdTmp]) {
            return Keyword[IdTmp];
        }
        
        Identifier = IdTmp;
        return token_id;
    }

    // If everything fails returns as a literal
    int Literal = Buffer;
    FileInput.get(Buffer);
    return Literal;
}

// ============================================================================
// ============                    AST CLASSES                     ============
// ============================================================================

// +++++++++++++++++++++
// +-----+ TYPES +-----+
// +++++++++++++++++++++

// Enum for type of variable
enum TypeValue {
    type_num,
    type_bool,
    type_str,
    type_null,
};

// +++++++++++++++++++++++
// +-----+ CLASSES +-----+
// +++++++++++++++++++++++

// Base class for all expressions 
class ExprAST {
    public:
        // Default destructor 
        virtual ~ExprAST() = default;
        virtual dynamic Res() = 0;
};

// Base class for all statements 
class StmtAST {
    public:
        // Default destructor 
        virtual ~StmtAST() = default;
        virtual void Res() = 0;
};

// Classes for blocks
// Class for inside blocks 
class InsideAST : public StmtAST {
    std::unique_ptr<StmtAST> Next;
    std::unique_ptr<StmtAST> Exec;

    public:
        InsideAST(std::unique_ptr<StmtAST> Next, 
                std::unique_ptr<StmtAST> Exec)
            : Next(std::move(Next)), Exec(std::move(Exec)) {}
        
        void PointTo (std::unique_ptr<StmtAST> Point) {
            Next = std::move(Point);
        }

        void Res() override;
};
// Class for blocks 
class BlockAST : public StmtAST {
    std::unordered_map<std::string, dynamic> ValMap;
    std::shared_ptr<BlockAST> Block;

    void DeepSetVar(std::string IdName, dynamic Value) {
        if (!ValMap.count(IdName)) {
            if (!Block) {
                return;
            }
            Block->DeepSetVar(IdName, Value);
        }
        if (ValMap.count(IdName)) {
            ValMap[IdName] = Value;
            return;
        }
        return;
    }

    public:
        BlockAST(std::shared_ptr<BlockAST> Block) : Block(std::move(Block)) {}

        void SetVar(std::string IdName, dynamic Value, int In) {
            if (!In) {
                if (!ValMap.count(IdName)){
                    Block->DeepSetVar(IdName, Value);
                    return;
                }
                ValMap[IdName] = Value;
            }
            ValMap[IdName] = Value;
            return;
        }

        dynamic GetVar(std::string IdName) {
            if (!ValMap.count(IdName)) {
                if (!Block) {
                    dynamic tmp = 'n';
                    return std::move(tmp);
                }
                return Block->GetVar(IdName);
            }
            return ValMap[IdName];
        }
        
        void Res() override;
};

// Class for numbers
class NumAST : public ExprAST {
    double Value;

    public:
        NumAST (double Value) : Value(Value) {}

        dynamic Res() override;
};

// Class for booleans 
class BoolAST : public ExprAST {
    bool Value;
    
    public:
        BoolAST (bool Value) : Value(Value) {}

        dynamic Res() override;
};

// Class for strings
class StrAST : public ExprAST {
    std::string Value;

    public :
        StrAST (std::string Value) : Value(Value) {}

        dynamic Res() override;
};

// Class for Unary 
class UnaryAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> RHS;

    public:
        UnaryAST(char Op, std::unique_ptr<ExprAST> RHS)
        : Op(Op), RHS(std::move(RHS)) {}
    
        dynamic Res() override; 
};

// Class for Operations 
class OperationAST : public ExprAST {
    char Op;
    std::unique_ptr<ExprAST> RHS, LHS;

    public:
        OperationAST(char Op, 
                    std::unique_ptr<ExprAST> LHS,
                    std::unique_ptr<ExprAST> RHS) 
            : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        
        dynamic Res() override;
};

// Class for print 
class PrintAST : public StmtAST {
    std::unique_ptr<ExprAST> Expr;

    public:
        PrintAST(std::unique_ptr<ExprAST> Expr) : Expr(std::move(Expr)) {}

        void Res() override;
};

// Class for variables 
// Define variable  
class VarDeclAST : public StmtAST {
    std::string Name;
    std::unique_ptr<ExprAST> Value;
    std::shared_ptr<BlockAST> Block;

    public:
        VarDeclAST(std::string Name, std::unique_ptr<ExprAST> Value,
                   std::shared_ptr<BlockAST> Block) 
            : Name(std::move(Name)), Value(std::move(Value)), 
              Block(std::move(Block)) {}
        
        void Res() override;
};
// Reassignig variable 
class VarReasAST : public StmtAST {
    std::string Name;
    std::unique_ptr<ExprAST> Value;
    std::shared_ptr<BlockAST> Block;

    public:
        VarReasAST(std::string Name, std::unique_ptr<ExprAST> Value, 
                   std::shared_ptr<BlockAST> Block) 
            : Name(Name), Value(std::move(Value)), Block(std::move(Block)) {}
        
        void Res() override;
};
// Return variable value 
class VarValAST : public ExprAST {
    std::string Name;
    std::shared_ptr<BlockAST> Block;

    public:
        VarValAST(std::string Name, std::shared_ptr<BlockAST> Block)
            : Name(std::move(Name)), Block(std::move(Block)) {}

        dynamic Res() override;
};

// Class for if..else statments 
class IfAST : public StmtAST {
    std::unique_ptr<ExprAST> Cond;
    std::unique_ptr<StmtAST> IfBlock;
    std::unique_ptr<StmtAST> ElseBlock;

    public:
        IfAST(std::unique_ptr<ExprAST> Cond, 
              std::unique_ptr<StmtAST> IfBlock,
              std::unique_ptr<StmtAST> ElseBlock)
            : Cond(std::move(Cond)), 
              IfBlock(std::move(IfBlock)), 
              ElseBlock(std::move(ElseBlock)) {}

        void Res() override;
};

// Class for while statments
class WhileAST : public StmtAST {
    std::unique_ptr<ExprAST> Cond;
    std::unique_ptr<StmtAST> Loop;
    
    public:
        WhileAST(std::unique_ptr<ExprAST> Cond, 
                 std::unique_ptr<StmtAST> Loop)
            : Cond(std::move(Cond)), Loop(std::move(Loop)) {}

        void Res() override;
};

// Class for for statmens 
class ForAST : public StmtAST {
    std::unique_ptr<StmtAST> VarIt;
    std::unique_ptr<ExprAST> Cond;
    std::unique_ptr<StmtAST> ExprIt;
    std::unique_ptr<StmtAST> Loop;

    public:
        ForAST(std::unique_ptr<StmtAST> VarIt, std::unique_ptr<ExprAST> Cond,
               std::unique_ptr<StmtAST> ExprIt, std::unique_ptr<StmtAST> Loop)
            : VarIt(std::move(VarIt)), Cond(std::move(Cond)), 
            ExprIt(std::move(ExprIt)), Loop(std::move(Loop)) {}

        void Res() override;
};

// ============================================================================
// ============                       PARSER                       ============
// ============================================================================

// +++++++++++++++++++++++++
// +-----+ LOG ERROR +-----+
// +++++++++++++++++++++++++

// Log Error for expression 
std::unique_ptr<ExprAST> LogErrorE(std::string Err)
{
    std::cout << "Error: " << Err << '\n';
    return nullptr;
}

// Log error for statements
std::unique_ptr<StmtAST> LogErrorS(std::string Err) {
    LogErrorE(Err);
    return nullptr;
}

// +++++++++++++++++++++++
// +-----+ GLOBALS +-----+
// +++++++++++++++++++++++

// Buffer for the Current Token
int CurToken;
// Each time getNextToken is called the Current Token is consumed and a we 
// update Buffer with a new one.
void getNextToken() { CurToken = Tokenizer(); }

// Map for the precedence of operators
std::unordered_map<int, int>Precedence;

// Function to return the precedence 
int getPrecedence() {
    if (!Precedence[CurToken]) {
        return -1;
    }
    return Precedence[CurToken];
}

// +++++++++++++++++++++++
// +-----+ GRAMMAR +-----+
// +++++++++++++++++++++++

// Definition of expression parser 
std::unique_ptr<ExprAST> ExprParser(std::shared_ptr<BlockAST> CodeBlock);

// Definition for statments parser 
std::unique_ptr<StmtAST> StmtParser(std::shared_ptr<BlockAST> CodeBlock);
std::unique_ptr<StmtAST> BlockParser(std::shared_ptr<BlockAST> CodeBlock);
std::unique_ptr<StmtAST> IfParser(std::shared_ptr<BlockAST> CodeBlock);

// numexpr -> num
std::unique_ptr<ExprAST> NumParser() {
   getNextToken(); // consume number
   return std::make_unique<NumAST>(NumBuff);
}

// boolexpr -> true | false 
std::unique_ptr<ExprAST> BoolParser() {
    if (CurToken == token_true) {
        getNextToken(); // consume 'true'
        return std::make_unique<BoolAST>(true);
    }
    getNextToken(); // consume 'false' 
    return std::make_unique<BoolAST>(false);
}

// strexpr -> "string"
std::unique_ptr<ExprAST> StrParser() {
    getNextToken(); // consume string
    std::string tmp = StrBuff;
    StrBuff.clear();
    return std::make_unique<StrAST>(tmp);
}

// varexpr -> id
std::unique_ptr<ExprAST> VarParser(std::string IdName, 
                                   std::shared_ptr<BlockAST> CodeBlock) 
{ 
    return std::make_unique<VarValAST>(IdName, CodeBlock);
}

// idexpr -> varexpr
std::unique_ptr<ExprAST> IdExprParser(std::shared_ptr<BlockAST> CodeBlock) {
    getNextToken(); // consume id 
    std::string Name = Identifier;

    return VarParser(Name, CodeBlock);
}

// parenexpr -> '('expr')'
std::unique_ptr<ExprAST> ParenParser(std::shared_ptr<BlockAST> CodeBlock) {
    getNextToken(); // consume '('
    auto Expr = ExprParser(CodeBlock);
    if (!Expr) {
        return nullptr;
    }

    if (CurToken != ')') {
        return LogErrorE("expected ')'");
    }
    getNextToken(); // consume ')'
    return Expr;
}

// unaryexpr -> '!'|'-' expr
std::unique_ptr<ExprAST> UnaryParser(std::shared_ptr<BlockAST> CodeBlock) {
    int Op = CurToken;
    getNextToken(); // consume '!'
    auto Expr = ExprParser(CodeBlock);
    
    return std::make_unique<UnaryAST>(Op, std::move(Expr));
}

// primaryexpr -> parenexpr 
//                | numexpr 
//                | boolexpr 
//                | unaryexpr 
//                | strexpr 
//                | idexpr
std::unique_ptr<ExprAST> PrimaryParser(std::shared_ptr<BlockAST> CodeBlock) {
    switch (CurToken) {
        default:
            return LogErrorE("expecting a expression");
        case '(':
            return ParenParser(CodeBlock);
        case token_num:
            return NumParser();
        case token_true:
            return BoolParser();
        case token_false:
            return BoolParser();
        case token_minus:
            return UnaryParser(CodeBlock);
        case token_not:
            return UnaryParser(CodeBlock);
        case token_str:
            return StrParser();
        case token_id:
            return IdExprParser(CodeBlock);
        case ';':
            return nullptr;
    }
}

// opexpr -> num | num '+' opexpr | parenexpr 
std::unique_ptr<ExprAST> OperationParser(int PrecLHS, 
                                         std::unique_ptr<ExprAST> LHS,
                                         std::shared_ptr<BlockAST> CodeBlock) 
{
    // Mounts the parser tree 
    while(true) {
        int PrecRHS = getPrecedence();

        // If the Precedence is higher on the operation passed(LHS) returns it.
        if (PrecRHS < PrecLHS) {
            return LHS;
        }

        // Precedence is higher on the right side. Save current operator 
        int Op = CurToken;
        getNextToken(); // consume operator 

        // Parse the RHS of the expression 
        auto RHS = PrimaryParser(CodeBlock);
        if (!RHS) {
            return nullptr;
        }

        // Gets the next operation 
        int NextPrec = getPrecedence();

        // If the precedence of the next operation is higher than the current 
        // one parses the RHS.
        if (PrecRHS < NextPrec) {
            RHS = OperationParser(PrecRHS+1, std::move(RHS), CodeBlock);
            if (!RHS) {
                return nullptr;
            }
        }

        // Merge LHS/RHS 
        LHS = std::make_unique<OperationAST>(Op, 
                                             std::move(LHS), 
                                             std::move(RHS));
    }
}

// expr -> opexpr
std::unique_ptr<ExprAST> ExprParser(std::shared_ptr<BlockAST> CodeBlock) {
    auto LHS = PrimaryParser(CodeBlock);
    if (!LHS) {
        return nullptr;
    }

    return OperationParser(0, std::move(LHS), CodeBlock);
}

// printstmt -> 'print' parenexpr
std::unique_ptr<StmtAST> PrintParser(std::shared_ptr<BlockAST> CodeBlock) {
    getNextToken(); // consume print 

    if (CurToken != '(') {
        return LogErrorS("expected a '(' after print");
    }

    auto E = ParenParser(CodeBlock);
    
    if (!E) {
        return nullptr;
    }

    return std::make_unique<PrintAST>(std::move(E));
}

// varstmt -> 'var' id '=' expr 
std::unique_ptr<StmtAST> VarAssignParser(std::shared_ptr<BlockAST> CodeBlock) {
    getNextToken(); // consume var 
    getNextToken(); // consume id 
    std::string IdName = Identifier;

    if (CurToken == token_atr) {
        getNextToken(); // consume '=' 
        auto E = ExprParser(CodeBlock);
        if (!E) {
            return nullptr;
        }
        return std::make_unique<VarDeclAST>(IdName, std::move(E), CodeBlock);
    }

    return std::make_unique<VarDeclAST>(IdName, nullptr, CodeBlock);
}

// reasstmt -> id '=' expr 
std::unique_ptr<StmtAST> ReassignParser(std::string IdName, 
                                        std::shared_ptr<BlockAST> CodeBlock) 
{
    getNextToken(); // consume '=' 
    auto E = ExprParser(CodeBlock);
    if (!E) {
        return nullptr;
    }

    return std::make_unique<VarReasAST>(IdName, std::move(E), CodeBlock);
}

// idstmt -> reasstmt
std::unique_ptr<StmtAST> IdParser(std::shared_ptr<BlockAST> CodeBlock) {
    getNextToken(); // consume id 
    std::string IdName = Identifier;

    switch (CurToken) {
        case token_atr:
            return ReassignParser(IdName, CodeBlock);
        default:
            LogErrorS("unexpected expression after identifier");
    }
    return nullptr;
}

// insidestmt -> stmt
std::unique_ptr<StmtAST> InsideParser(std::shared_ptr<BlockAST> CodeBlock) {
    if (CurToken == '}') {
        return nullptr;
    }
    
    auto S = StmtParser(CodeBlock);
    if (CurToken == ';') {
        getNextToken(); // consume ';'
    }
    if (!S) {
        return nullptr;
    }

    return std::make_unique<InsideAST>(InsideParser(CodeBlock), std::move(S));
}

// blockstmt -> '{' insidestmt '}'
std::unique_ptr<StmtAST> BlockParser(std::shared_ptr<BlockAST> CodeBlock) {
    getNextToken(); // consume '{'
    
    std::shared_ptr<BlockAST> CurBlock = std::make_shared<BlockAST>(CodeBlock);
    
    auto Inside = InsideParser(CurBlock);
    
    if (CurToken != '}') {
        return LogErrorS("expected a '}'");
    }
    getNextToken(); // consume '}'

    return std::move(Inside);
}

// ifstmt -> 'if' parenexpr stmt ('else' stmt)?
std::unique_ptr<StmtAST> IfParser(std::shared_ptr<BlockAST> CodeBlock) {
    getNextToken(); // consume 'if'

    auto Cond = ParenParser(CodeBlock);
    if (!Cond) {
        return LogErrorS("expected a expression after if");
    }

    auto IfBlock = StmtParser(CodeBlock);
    if (!IfBlock) {
        return nullptr;
    }
    
    if (CurToken == ';') {
        getNextToken(); // consume ';'
    }

    if (CurToken == token_else) {
        getNextToken(); // consume else
        auto ElseBlock = StmtParser(CodeBlock);
        if (!ElseBlock) {
            return nullptr;
        }

        return std::make_unique<IfAST>(std::move(Cond), std::move(IfBlock), 
                                       std::move(ElseBlock));
    }

    return std::make_unique<IfAST>(std::move(Cond), std::move(IfBlock), 
                                    nullptr);
}

// whilestmt -> 'while' parenexpr stmt
std::unique_ptr<StmtAST> WhileParser(std::shared_ptr<BlockAST> CodeBlock) {
    getNextToken(); // consume while 

    auto Cond = ParenParser(CodeBlock);
    if (!Cond) {
        return LogErrorS("expected a expression after while");
    }

    auto Loop = StmtParser(CodeBlock);

    return std::make_unique<WhileAST>(std::move(Cond), std::move(Loop));
}

// forstmt -> 'for' '(' stmt ',' expr ',' expr ')' stmt 
std::unique_ptr<StmtAST> ForParser(std::shared_ptr<BlockAST> CodeBlock) {
    getNextToken(); // consume for 

    if (CurToken != '(') {
        return LogErrorS("expected a '(' after the for");
    }
    getNextToken(); // consume '('

    // Variables declared on the for loop exists only in itself
    std::shared_ptr<BlockAST> CurBlock = std::make_shared<BlockAST>(CodeBlock);

    auto Var = StmtParser(CurBlock);

    if (CurToken != ';') {
        return LogErrorS("expected a ';' in for");
    }
    getNextToken(); // consume ','

    auto Cond = ExprParser(CurBlock);

    if (CurToken != ';') {
        return LogErrorS("expected a ';' in for");
    }
    getNextToken(); // consume ',' 
    
    auto Interator = StmtParser(CurBlock);

    if (CurToken != ')') {
        return LogErrorS("expected a ')' in for");
    }
    getNextToken(); // consume ')'

    auto Loop = StmtParser(CurBlock);

    return std::make_unique<ForAST>(std::move(Var), std::move(Cond), 
                                    std::move(Interator), std::move(Loop));
}

// declstmt -> printstmt
//             | varstmt
//             | idstmt 
//             | blockstmt
//             | ifstmt
//             | whilestmt
//             | forstmt
std::unique_ptr<StmtAST> DeclarationParser(std::shared_ptr<BlockAST> CodeBlock) 
{
    switch(CurToken) {
        case token_print:
            return PrintParser(CodeBlock);
        case token_var:
            return VarAssignParser(CodeBlock);
        case token_id:
            return IdParser(CodeBlock);
        case '{':
            return BlockParser(CodeBlock);
        case token_if:
            return IfParser(CodeBlock);
        case token_while:
            return WhileParser(CodeBlock);
        case token_for:
            return ForParser(CodeBlock);
        case ';':
            return nullptr;
        default:
            return LogErrorS("declaration not found");
    }
}

// stmt -> declstmt
std::unique_ptr<StmtAST> StmtParser(std::shared_ptr<BlockAST> CodeBlock) {
    auto Stmt = DeclarationParser(CodeBlock);
    if (!Stmt) {
        return nullptr;
    }
    return std::move(Stmt);
}

// ============================================================================
// ============                   TREE  EXECUTION                  ============
// ============================================================================

// +++++++++++++++++++++++++
// +-----+ LOG ERROR +-----+
// +++++++++++++++++++++++++

dynamic LogErrorD(std::string Err) {
    std::cout << "Error: " << Err << '\n';
    exit(70);
}

// ++++++++++++++++++++
// +-----+ EXEC +-----+
// ++++++++++++++++++++

void BlockAST::Res() {
    return;
}

void InsideAST::Res() {
    if(!Next) {
        if (!Exec) {
            return;
        }
        Exec->Res();
        return;
    }
    Exec->Res();
    Next->Res();
    return;
}

dynamic VarValAST::Res() {
    dynamic res = Block->GetVar(Name);
    return res;
}

dynamic NumAST::Res() {
    dynamic res = (double)Value;
    return res;
}

dynamic BoolAST::Res() {
    dynamic res = (bool)Value;
    return res;
}

dynamic StrAST::Res() {
    dynamic res = Value;
    return res;
}

dynamic UnaryAST::Res() {
    dynamic Operand = RHS->Res();
    
    if (Operand.index() == type_null) {
        return LogErrorD("operation on not initialize variable");
    }

    if (Operand.index() == type_num) {
        if (Op == token_not) {
            if (!std::get<double>(Operand)) {
                Operand = (double)1;
                return Operand;
            }
            if (std::get<double>(Operand)) {
                Operand = (double)0;
                return Operand;
            }
        }
        if (Op == token_minus) {
            Operand = -(double)(std::get<double>(Operand));
            return Operand;
        }
    }

    if (Operand.index() == type_bool) {
        if (Op == token_not) {
            if (!std::get<bool>(Operand)) {
                Operand = (bool)true;
                return Operand;
            }
            if (std::get<bool>(Operand)) {
                Operand = (bool)false;
                return Operand;
            }
        }
        if (Op == token_minus) {
            Operand = -(double)(std::get<bool>(Operand));
            return Operand;
        }
    }

    if (Operand.index() == type_str) {
        return LogErrorD("invalid syntax");
    }

    // Error Handling
    std::string Err;
    return LogErrorD("operand not reconized");
}

dynamic OperationAST::Res() {
    dynamic Left = LHS->Res();
    dynamic Right = RHS->Res();
    
    if (Left.index() == type_null || Right.index() == type_null) {
        return LogErrorD("operation on not initialize variable");
    }

    // bool '+' bool
    if (Left.index() == type_bool && Right.index() == type_bool) {
        
        // bool && bool
        if (Op == token_and) {
            if (std::get<bool>(Left) && std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }

        // bool || bool 
        if (Op == token_or) {
            if (std::get<bool>(Left) || std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }

        // bool == bool
        if (Op == token_equal) {
            if (std::get<bool>(Left) == std::get<bool>(Right)) { 
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // bool != bool
        if (Op == token_inequal) {
            if (std::get<bool>(Left) != std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // bool > bool 
        if (Op == token_greater) {
            if (std::get<bool>(Left) > std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // bool < bool
        if (Op == token_less) {
            if (std::get<bool>(Left) < std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // bool >= bool 
        if (Op == token_greateq) {
            if (std::get<bool>(Left) >= std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // bool <= bool
        if (Op == token_lesseq) {
            if (std::get<bool>(Left) <= std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // bool - bool 
        if (Op == token_minus) {
            Left = (double)(std::get<bool>(Left) - std::get<bool>(Right));
            return Left;
        }
        // bool + bool 
        if (Op == token_plus) {
            Left = (double)(std::get<bool>(Left) + std::get<bool>(Right));
            return Left;
        }
        // bool * bool
        if (Op == token_plus) {
            Left = (double)(std::get<bool>(Left) * std::get<bool>(Right));
            return Left;
        }
        // bool / bool 
        if (Op == token_div) {
            if (!std::get<bool>(Right)) {
                LogErrorD("cannot divide by zero");
            }
            Left = (double)(std::get<bool>(Left) / std::get<bool>(Right));
            return Left;
        }
    }
    
    // double '+' bool
    if (Left.index() == type_num && Right.index() == type_bool) {
        // double && bool 
        if (Op == token_and) {
            if (std::get<double>(Left) && std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }

        //double || bool 
        if (Op == token_or) {
            if (std::get<double>(Left) || std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }

        // double == bool
        if (Op == token_equal) {
            if (std::get<double>(Left) == std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // double != bool
        if (Op == token_inequal) {
            if (std::get<double>(Left) != std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // double > bool 
        if (Op == token_greater) {
            if (std::get<double>(Left) > std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // double < bool
        if (Op == token_less) {
            if (std::get<double>(Left) < std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // double >= bool 
        if (Op == token_greateq) {
            if(std::get<double>(Left) >= std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // double <= bool
        if (Op == token_lesseq) {
            if (std::get<double>(Left) <= std::get<bool>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // double - bool 
        if (Op == token_minus) {
            Left = (double)(std::get<double>(Left) - std::get<bool>(Right));
            return Left;
        }
        // double + bool 
        if (Op == token_plus) {
            Left = (double)(std::get<double>(Left) + std::get<bool>(Right));
            return Left;
        }
        // double * bool
        if (Op == token_mul) {
            Left = (double)(std::get<double>(Left) * std::get<bool>(Right));
            return Left;
        }
        // double / bool 
        if (Op == token_div) {
            if (!std::get<bool>(Right)) {
                LogErrorD("cannot divide by zero");
            }
            Left = (double)(std::get<double>(Left) / std::get<bool>(Right));
        }
    }
    
    // bool '+' double
    if (Left.index() == type_bool && Right.index() == type_num) {
        // bool && bool 
        if (Op == token_and) {
            if (std::get<bool>(Left) && std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }

        // bool || bool 
        if (Op == token_or) {
            if (std::get<bool>(Left) || std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }

        // bool == double
        if (Op == token_equal) {
            if (std::get<bool>(Left) == std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // bool != double
        if (Op == token_inequal) {
            if (std::get<bool>(Left) != std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // bool > double 
        if (Op == token_greater) {
            if (std::get<bool>(Left) > std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // bool < double
        if (Op == token_less) {
            if (std::get<bool>(Left) < std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // bool >= double 
        if (Op == token_greateq) {
            if(std::get<bool>(Left) >= std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // bool <= double
        if (Op == token_lesseq) {
            if (std::get<bool>(Left) <= std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // bool - double 
        if (Op == token_minus) {
            Left = (double)(std::get<bool>(Left) - std::get<double>(Right));
            return Left;
        }
        // bool + double 
        if (Op == token_plus) {
            Left = (double)(std::get<bool>(Left) + std::get<double>(Right));
            return Left;
        }
        // bool * double
        if (Op == token_mul) {
            Left = (double)(std::get<bool>(Left) * std::get<double>(Right));
            return Left;
        }
        // bool / double 
        if (Op == token_div) {
            if (!std::get<double>(Right)) {
                LogErrorD("cannot divide by zero");
            }
            Left = (double)(std::get<bool>(Left) / std::get<double>(Right));
        }
    }
    
    // double '+' double
    if (Left.index() == type_num && Right.index() == type_num) {
        // double && double 
        if (Op == token_and) {
            if (std::get<double>(Left) && std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        };

        // double || double 
        if (Op == token_or) {
            if (std::get<double>(Left) || std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }

        // double == double
        if (Op == token_equal) {
            if (std::get<double>(Left) == std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // double != double
        if (Op == token_inequal) {
            if (std::get<double>(Left) != std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // double > double 
        if (Op == token_greater) {
            if (std::get<double>(Left) > std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // double < double
        if (Op == token_less) {
            if (std::get<double>(Left) < std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // double >= double 
        if (Op == token_greateq) {
            if(std::get<double>(Left) >= std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // double <= double
        if (Op == token_lesseq) {
            if (std::get<double>(Left) <= std::get<double>(Right)) {
                Left = (bool)true;
                return Left;
            }
            Left = (bool)false;
            return Left;
        }
        // double - double 
        if (Op == token_minus) {
            Left = (double)(std::get<double>(Left) - std::get<double>(Right));
            return Left;
        }
        // double + double 
        if (Op == token_plus) {
            Left = (double)(std::get<double>(Left) + std::get<double>(Right));
            return Left;
        }
        // double * double
        if (Op == token_mul) {
            Left = (double)(std::get<double>(Left) * std::get<double>(Right));
            return Left;
        }
        // double / double 
        if (Op == token_div) {
            if (!std::get<double>(Right)) {
                LogErrorD("cannot divide by zero");
            }
            Left = (double)(std::get<double>(Left) / std::get<double>(Right));
        }
    }

    // string + num|bool 
    if (Left.index() == type_str && (Right.index() == type_num 
                                    || Right.index() == type_bool)) 
    {
        // string + num|bool   
        if (Op == token_plus) {
            if (Right.index() == type_num){
                Right = std::to_string(std::get<double>(Right));
                Left = std::get<std::string>(Left) 
                        + std::get<std::string>(Right);
                return Left;
            }
            if (Right.index() == type_bool) {
                Right = std::to_string(std::get<bool>(Right));
                Left = std::get<std::string>(Left) 
                    + std::get<std::string>(Right);
                return Left;
            }
        }

        // string '*' num|bool  
        if (Precedence[Op]) {
            LogErrorD("operation on string not supported");
        }
    }

    // num|bool + string
    if ((Left.index() == type_num || Left.index() == type_bool) 
            && Right.index() == type_str) 
    {
        // num|bool + string 
        if (Op == token_plus) {
            if (Left.index() == type_num){
                Left = std::to_string(std::get<double>(Left));
                Left = std::get<std::string>(Left) 
                        + std::get<std::string>(Right);
                return Left;
            }
            if (Left.index() == type_bool) {
                Left = std::to_string(std::get<bool>(Left));
                Left = std::get<std::string>(Left) 
                    + std::get<std::string>(Right);
                return Left;
            }
        }

        // string '*' num|bool  
        if (Precedence[Op]) {
            LogErrorD("operation on string not supported");
        }
    }
    
    // string '+' string 
    if (Left.index() == type_str && Right.index() == type_str) {
        // string == string 
        if (Op == token_equal) {
            if (std::get<std::string>(Left) == std::get<std::string>(Right)) {
                Left = (bool)true;
            } else {
                Left = (bool)false;
            }
            return Left;
        }
        
        // string != string 
        if (Op == token_inequal) {
            if (std::get<std::string>(Left) != std::get<std::string>(Right)) {
                Left = (bool)true;
            } else {
                Left = (bool)false;
            }
            return Left;
        }

        // string > string 
        if (Op == token_greater) {
            if (std::get<std::string>(Left) > std::get<std::string>(Right)) {
                Left = (bool)true;
            } else {
                Left = (bool)false;
            }
            return Left;
        }

        // string < string 
        if (Op == token_less) {
            if (std::get<std::string>(Left) < std::get<std::string>(Right)) {
                Left = (bool)true;
            } else {
                Left = (bool)false;
            }
            return Left;
        }

        // string >= string 
        if (Op == token_greateq) {
            if (std::get<std::string>(Left) >= std::get<std::string>(Right)) {
                Left = (bool)true;
            } else {
                Left = (bool)false;
            }
            return Left;
        }

        // string <= string 
        if (Op == token_lesseq) {
            if (std::get<std::string>(Left) <= std::get<std::string>(Right)) {
                Left = (bool)true;
            } else {
                Left = (bool)false;
            }
            return Left;
        }

        // string + string 
        if (Op == token_plus) {
            Left = std::get<std::string>(Left) + std::get<std::string>(Right);
            return Left;
        }

        // string (* | / | -) string 
        if (Op == token_mul || Op == token_div || Op == token_minus) {
            LogErrorD("operation on strings not supported");
        }
    }

    // Error Handling
    return LogErrorD("operation not reconized");
}

void VarDeclAST::Res() {
    if (!Value) {
        dynamic tmp = 'n';
        Block->SetVar(Name, std::move(tmp), 1);
        return;
    }
    Block->SetVar(Name, Value->Res(), 1);
    return;
}

void VarReasAST::Res() {
    Block->SetVar(Name, Value->Res(), 0);
    return;
}

void PrintAST::Res() {
    dynamic show = Expr->Res();

    if (show.index() == type_bool) {
        if (std::get<bool>(show)) {
            std::cout << "True\n";
            return;
        }
        std::cout << "False\n";
        return;
    }

    if (show.index() == type_str) {
        std::cout << "'" << std::get<std::string>(show) << "'\n";
        return;
    }

    if (show.index() == type_null) {
        std::cout << "NULL\n";
        return;
    }

    std::cout << std::get<double>(show) << '\n';
    return;
}

void IfAST::Res() {
    dynamic result = Cond->Res();

    if (result.index() == type_null) {
        if (!ElseBlock) {
            return;
        }
        ElseBlock->Res();
        return;
    }
    
    if (result.index() == type_num) {
        if (!std::get<double>(result)) {
            if (!ElseBlock) {
                return;
            }
            ElseBlock->Res();
            return;
        }
        IfBlock->Res();
        return;
    }

    if (result.index() == type_bool) {
        if (!std::get<bool>(result)) {
            if (!ElseBlock) {
                return;
            }
            ElseBlock->Res();
            return;
        }
        IfBlock->Res();
        return;
    }

    if (result.index() == type_str) {
        IfBlock->Res();
        return;
    }
    return;
}

void WhileAST::Res() {
    dynamic res = Cond->Res();
    
    if (res.index() == type_bool){
        while (std::get<bool>(res) != 0) {
            Loop->Res();
            res = Cond->Res();
        }
    }

    if (res.index() == type_num) {
        while (std::get<double>(res) != 0) {
            Loop->Res();
            res = Cond->Res();
        }
    }

    if (res.index() == type_str) {
        while(true) {
            Loop->Res();
        }
    }

    return;
}

void ForAST::Res() {
   if (VarIt) {
        VarIt->Res();
   } 
   
   dynamic ResCond = Cond->Res();

   if (ExprIt) {
       if (ResCond.index() == type_num) {
            while(std::get<double>(ResCond) != 0) {
                Loop->Res();
                ExprIt->Res();
                ResCond = Cond->Res();
            }
            return;
       }

       if(ResCond.index() == type_bool) {
            while (std::get<bool>(ResCond) != 0) {
                Loop->Res();
                ExprIt->Res();
                ResCond = Cond->Res();
            }
            return;
       }

       if (ResCond.index() == type_str) {
            while (true) {
                Loop->Res();
                ExprIt->Res();
            }
            return;
       }

       return;
   }

   if (ResCond.index() == type_num) {
        while(std::get<double>(ResCond) != 0) {
            Loop->Res();
            ResCond = Cond->Res();
        }
        return;
   }

   if (ResCond.index() == type_bool) {
        while(std::get<bool>(ResCond) != 0) {
            Loop->Res();
            ResCond = Cond->Res();
        }
        return;
   }

   if (ResCond.index() == type_str) {
        while(true) {
            Loop->Res();
        }
        return;
   }

   return;
}

// ============================================================================
// ============                 FIRST LEVEL PARSER                 ============
// ============================================================================

// Global Block init
std::shared_ptr<BlockAST> Global = std::make_shared<BlockAST>(nullptr);

void HandleStatement() {
    if (auto E = StmtParser(Global)) {
        E->Res();
    } else {
        getNextToken();
    }
}

void HandleExpr() {
    if (auto E = ExprParser(Global)) {} 
    else {
        getNextToken();
    }
}

void Interpreter() {
    while(true) {
        switch(CurToken) {
            case token_eof:
                return;
            case ';':
                getNextToken();
                break;
            case token_num:
                HandleExpr();
                break;
            case token_true:
                HandleExpr();
                break;
            case token_false:
                HandleExpr();
                break;
            case token_str:
                HandleExpr();
                break;
            default:
                HandleStatement();
                break;
        }
    }
}

// ============================================================================
int main() {
    FileInput.open("test.cb");
   
    // Definition of keywods 
    Keyword["var"] = token_var;
    Keyword["func"] = token_func;
    Keyword["return"] = token_ret;
    Keyword["true"] = token_true;
    Keyword["false"] = token_false;
    Keyword["null"] = token_null;
    Keyword["while"] = token_while;
    Keyword["for"] = token_for;
    Keyword["if"] = token_if;
    Keyword["else"] = token_else;
    Keyword["class"] = token_class;
    Keyword["super"] = token_super;
    Keyword["this"] = token_this;
    Keyword["print"] = token_print;
    Keyword["break"] = token_break;

    // Definition of precedence of operators 
    // 1 is the lowest 
    Precedence[token_atr] = 2;
    Precedence[token_and] = 3;
    Precedence[token_or] = 3;
    Precedence[token_equal] = 5;
    Precedence[token_inequal] = 5;
    Precedence[token_greater] = 5;
    Precedence[token_less] = 5;
    Precedence[token_greateq] = 5;
    Precedence[token_lesseq] = 5;
    Precedence[token_plus] = 10;
    Precedence[token_minus] = 10;
    //Precedence[token_mod] = 20;
    Precedence[token_mul] = 20;
    Precedence[token_div] = 20;
    
    getNextToken(); // get the first token 

    Interpreter();

    return 0;
}
