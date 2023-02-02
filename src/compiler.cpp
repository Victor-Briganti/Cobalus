// Standard
#include <fstream>
#include <memory>
#include <variant>

#include <stdio.h> // remove

// Local
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
        
    }
}

///////////////////////////////////////////////////////////////////////////////
////////////                    CODE GENERATION                    ////////////
///////////////////////////////////////////////////////////////////////////////

void BoolAST::codegen() {
    Bytecode byte;
    byte.inst = bolen;
    byte.data = BoolValue;
    PushStack(byte);
    return;
}

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

///////////////////////////////////////////////////////////////////////////////
////////////                    FRONT COMPILER                     ////////////
///////////////////////////////////////////////////////////////////////////////

void Compile(std::shared_ptr<std::fstream> FileComp) {
    std::unique_ptr<DeclarationAST> Decl = Parser(FileComp);

    while (Decl) {
        Decl->codegen();
        Decl = Parser(FileComp);
    }
}
