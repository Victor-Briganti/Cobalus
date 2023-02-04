#include "Headers/error_log.h"
#include "Headers/lexer.h"
#include "Headers/parser.h"
#include "Headers/vcm.h"

// Helper for instructions
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
            ErLogs.PushError("", "illegal instruction", 1); 
            ErLogs.ShowErrors();
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
    CobaluStack.Push(byte);
    return;
}

void StringAST::codegen() {
    Bytecode byte;
    byte.inst = cstr;
    byte.data = StringValue;
    CobaluStack.Push(byte);
    return;
}

void BoolAST::codegen() {
    Bytecode byte;
    byte.inst = bolen;
    byte.data = BoolValue;
    CobaluStack.Push(byte);
    return;
}

void NullAST::codegen() {
    Bytecode byte;
    byte.inst = none;
    byte.data = nullptr;
    CobaluStack.Push(byte);
    return;
}

void OperationAST::codegen() {
    LHS->codegen();
    RHS->codegen();
    
    Bytecode byte;
    byte.inst = getInstruction(Op);
    CobaluStack.Push(byte);
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
    CobaluStack.Push(byte);
    return;
}

void PrintAST::codegen() {
    Expr->codegen();

    Bytecode byte;
    byte.inst = stio;
    CobaluStack.Push(byte);
    return;
}

void VarValAST::codegen() {
    Bytecode byte;
    byte.inst = varrt;
    byte.offset = ParentBlock->getOffset(Variable);
    if (byte.offset == -1) {
        ErLogs.PushError(Variable, "not identified", 2);        
        Bytecode byte;
        byte.inst = none;
        byte.data = nullptr;
        CobaluStack.Push(byte);
        return;
    }
    CobaluStack.Push(byte);
    return;
}

void VarDeclAST::codegen() {
    Bytecode byte;
    byte.inst = varst;
   
    if (!Expr) {
        Bytecode byte;
        byte.inst = none;
        byte.data = nullptr;
        CobaluStack.Push(byte);
    } else {
        Expr->codegen();
    }

    if (Decl == 1) {
        byte.offset = ParentBlock->setOffset(Variable);
        CobaluStack.Push(byte);
        return;
    }
    byte.offset = ParentBlock->getOffset(Variable);
    CobaluStack.Push(byte);
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

void IfAST::codegen() {
    // Generates the code of the condition
    Cond->codegen();

    Bytecode byte;
    byte.inst = setto; // goto equivalent
    CobaluStack.Push(byte);

    // Saves the current place of the instruction on the stack
    int tmp = CobaluStack.Size() - 1;

    // Generates the if block
    IfBlock->codegen();

    // Gets position of the stack where it needs to go if the condition fails
    byte.offset = CobaluStack.Size() - 1; 

    // Reinsert the value into the stack
    CobaluStack.Insert(byte, tmp);

    if (!ElseBlock) {
        return;
    }
    
    // Not very proud of this hack, but for now will do
    // Generates a always false
    Bytecode booltrue;
    booltrue.inst = bolen;
    booltrue.data = false;
    CobaluStack.Push(booltrue);

    // Saves the current place of the instruction on the stack
    // this place will be used so we can jump off else in the case
    // the condition is true.
    CobaluStack.Push(byte);
    tmp = CobaluStack.Size() - 1;

    // Generates the else block
    ElseBlock->codegen();

    // Gets position of the stack where it needs to go if condition 
    // succed
    byte.offset = CobaluStack.Size() - 1; 
    // Reinsert the value into the stack
    CobaluStack.Insert(byte, tmp);

    return;
}

///////////////////////////////////////////////////////////////////////////////
////////////                    FRONT COMPILER                     ////////////
///////////////////////////////////////////////////////////////////////////////

void Compile() {
    // Generate the global block 
    std::shared_ptr<BlockAST> Global = 
        std::make_shared<BlockAST>(nullptr, GLOBAL);

    std::unique_ptr<DeclarationAST> Decl = Parser(Global);

    while (Decl) {
        Decl->codegen();
        Decl = Parser(Global);
    }
}
