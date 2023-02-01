#include <memory>
#include <string>
#include <variant>
#include <unordered_map>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
/////////                           DEFINITIONS                       /////////
///////////////////////////////////////////////////////////////////////////////

// The last int* represents the null
#define Value std::variant<double, bool,std::string, int*>

enum BlockState {
    GLOBAL,
    FUNC,
    LOOP,
    COMMON,
};

///////////////////////////////////////////////////////////////////////////////
/////////                           AST CLASS                         /////////
///////////////////////////////////////////////////////////////////////////////

class DeclarationAST {
    public:
        DeclarationAST() = default;
        virtual ~DeclarationAST() = default;
        virtual void codegen() = 0;
};

class StatementAST : public DeclarationAST {
    public:
        virtual void codegen() = 0;
};

class ExpressionAST : public StatementAST {
    public:
        virtual void codegen() = 0;
};

class DoubleAST : public ExpressionAST {
    double DoubleValue;
    public:
        DoubleAST(double DoubleValue) : DoubleValue(DoubleValue) {}
        
        void codegen() override;
};

///////////////////////////////////////////////////////////////////////////////
/////////                           FUNCTIONS                         /////////
///////////////////////////////////////////////////////////////////////////////

std::unique_ptr<DeclarationAST> Parser(std::shared_ptr<std::fstream> FileInput);

