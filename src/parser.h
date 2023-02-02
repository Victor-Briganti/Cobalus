#include "global.h"

// Enum for definition of Blocks
enum BlockState {
    GLOBAL,
    FUNC,
    LOOP,
    COMMON,
};

///////////////////////////////////////////////////////////////////////////////
/////////                           AST CLASS                         /////////
///////////////////////////////////////////////////////////////////////////////

// Declarations are the top of the grammar, everything falls in a declaration
class DeclarationAST {
    public:
        DeclarationAST() = default;
        virtual ~DeclarationAST() = default;
        virtual void codegen() = 0;
};

// Statements are the second class. Consider that every line will be a 
// statement
class StatementAST : public DeclarationAST {
    public:
        virtual void codegen() = 0;
};

// Expression will define numberic expressions and calls
class ExpressionAST : public StatementAST {
    public:
        virtual void codegen() = 0;
};

// Values
class DoubleAST : public ExpressionAST {
    double DoubleValue;
    public:
        DoubleAST(double DoubleValue) : DoubleValue(DoubleValue) {}
        
        void codegen() override;
};

class StringAST : public ExpressionAST {
    std::string StringValue;
    public:
        StringAST(std::string StringValue) : StringValue(StringValue) {}

        void codegen() override;
};

class BoolAST : public ExpressionAST {
    bool BoolValue;
    public:
        BoolAST(bool BoolValue) : BoolValue(BoolValue) {}

        void codegen() override;
};

class NullAST : public ExpressionAST {
    public:
        NullAST() {}

        void codegen() override;
};

// Define Binary operation
class OperationAST : public ExpressionAST {
    std::unique_ptr<DeclarationAST> LHS;
    std::unique_ptr<DeclarationAST> RHS;
    int Op;

    public:
        OperationAST(std::unique_ptr<DeclarationAST> LHS,
                     std::unique_ptr<DeclarationAST> RHS,
                     int Op) 
        : LHS(std::move(LHS)), RHS(std::move(RHS)), Op(Op) {}
    
        void codegen() override;
};

// Define Unary operation
class UnaryAST : public ExpressionAST {
    std::unique_ptr<DeclarationAST> Expr;
    int Op;

    public:
        UnaryAST(std::unique_ptr<DeclarationAST> Expr, int Op)
            : Expr(std::move(Expr)), Op(Op) {}

        void codegen() override;
};

// Built-in Function
class PrintAST : public StatementAST {
    std::unique_ptr<DeclarationAST> Expr;

    public:
        PrintAST(std::unique_ptr<DeclarationAST> Expr) 
            : Expr(std::move(Expr)) {}
        
        void codegen() override;
};

///////////////////////////////////////////////////////////////////////////////
/////////                           FUNCTIONS                         /////////
///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<DeclarationAST> Parser(std::shared_ptr<std::fstream> FileInput);

