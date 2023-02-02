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

void OperationAST::codegen() {
    LHS->codegen();
    RHS->codegen();
    
    Bytecode byte;
    byte.inst = getInstruction(Op);
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
