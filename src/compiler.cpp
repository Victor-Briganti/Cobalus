#include "global.h"
#include "error_log.h"
#include "parser.h"
#include "compiler.h"
#include "lexer.h"
#include "vcm.h"

// +++++++++++++++++
// ++++ HELPERS ++++
// +++++++++++++++++

Instruction getInstruction(int Op) {
    switch(Op) {
        case TOKEN_PLUS: {
            return addD;
        }
        case TOKEN_MINUS: {
            return subD;
        }
        case TOKEN_MUL: {
            return mulD;
        }
        case TOKEN_DIV: {
            return divD;
        }
        case TOKEN_EQUAL: {
            return eqD;
        }
        case TOKEN_INEQUAL: {
            return ineqD;
        }
        case TOKEN_GREATER: {
            return grD;
        }
        case TOKEN_LESS: {
            return lsD;
        }
        case TOKEN_GREATEQ: {
            return greqD;
        }
        case TOKEN_LESSEQ: {
            return lseqD;
        }
        default: {
            PushError("", "illegal instruction", 1); 
            ShowErrors();
            exit(0);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
////////////                    CODE GENERATION                    ////////////
///////////////////////////////////////////////////////////////////////////////

void DoubleAST::codegen() {
    Bytecode byte;
    byte.inst = ndoubl;
    byte.data = DoubleValue;
    PushStack(byte);
    return;
}

void StringAST::codegen() {
    Bytecode byte;
    byte.inst = cstr;
    byte.data = StringValue;
    PushStack(byte);
    return;
}

void BoolAST::codegen() {
    Bytecode byte;
    byte.inst = bolen;
    byte.data = BoolValue;
    PushStack(byte);
    return;
}

void NullAST::codegen() {
    Bytecode byte;
    byte.inst = none;
    byte.data = nullptr;
    PushStack(byte);
    return;
}

void OperationAST::codegen() {
    LHS->codegen();
    RHS->codegen();
    
    Bytecode byte;
    byte.inst = getInstruction(Op);
    PushStack(byte);
    return;
}

void UnaryAST::codegen() {
    Expr->codegen();
    
    Bytecode byte;
    if (Op == TOKEN_MINUS) {
        byte.inst = invsig;
    }
    if (Op == TOKEN_NOT) {
        byte.inst = negte;
    }
    PushStack(byte);
    return;
}

void PrintAST::codegen() {
    Expr->codegen();

    Bytecode byte;
    byte.inst = stio;
    PushStack(byte);
    return;
}

void VarValAST::codegen() {
    Bytecode byte;
    byte.inst = varrt;
    byte.offset = ParentBlock->getOffset(Variable);
    if (byte.offset == -1) {
        PushError(Variable, "not identified", 2);        
        Bytecode byte;
        byte.inst = none;
        byte.data = nullptr;
        PushStack(byte);
        return;
    }
    PushStack(byte);
    return;
}

void VarDeclAST::codegen() {
    Bytecode byte;
    byte.inst = varst;
   
    if (!Expr) {
        Bytecode byte;
        byte.inst = none;
        byte.data = nullptr;
        PushStack(byte);
    } else {
        Expr->codegen();
    }

    if (Decl == 1) {
        byte.offset = ParentBlock->setOffset(Variable);
        PushStack(byte);
        return;
    }
    byte.offset = ParentBlock->getOffset(Variable);
    PushStack(byte);
    return;
}

void InsideAST::codegen() {
    if (!Chain) {
        if (!Exec) {
            return;
        }
        Exec->codegen();
        return;
    }
    Exec->codegen();
    Chain->codegen();
    return;
}

///////////////////////////////////////////////////////////////////////////////
////////////                    FRONT COMPILER                     ////////////
///////////////////////////////////////////////////////////////////////////////

void Compile(std::shared_ptr<std::fstream> FileComp) {
    // Generate the global block 
    std::shared_ptr<BlockAST> Global = 
        std::make_shared<BlockAST>(nullptr, GLOBAL);

    std::unique_ptr<DeclarationAST> Decl = Parser(FileComp, Global);

    while (Decl) {
        Decl->codegen();
        Decl = Parser(FileComp, Global);
    }
}
