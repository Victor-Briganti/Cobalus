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
    // Generates the data
    Expr->codegen();

    // Creates the print instruction
    Bytecode byte;
    byte.inst = stio;
    CobaluStack.Push(byte);
    return;
}

void VarValAST::codegen() {
    // Generates the instruction to return the variable from the 
    // CobaluStack
    Bytecode byte;
    byte.inst = varrt;
    byte.offset = ParentBlock->varGetOffset(Variable) + 1;
    
    // If not found push a null value
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
    // Generates the instruction to insert a variable in the stack
    Bytecode byte;
    byte.inst = varst;
   
   // Verify if the variable is initialized, if not insert a null
    if (!Expr) {
        Bytecode byte;
        byte.inst = none;
        byte.data = nullptr;
        CobaluStack.Push(byte);
    } else {
        Expr->codegen();
    }

    // Verify if is a declaration or is a reassign of a value.
    // If is a declaration insert its offset on the table
    if (Decl == 1) {
        byte.offset = ParentBlock->varSetOffset(Variable) + 1;
        CobaluStack.Push(byte);
        return;
    }

    // Is a reassign so get the offset value
    byte.offset = ParentBlock->varGetOffset(Variable) + 1;
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


    if (!ElseBlock) {
        // Gets position of the stack where it needs to go if the condition fails
        byte.offset = CobaluStack.Size() - 1; 

        // Reinsert the value into the stack
        CobaluStack.Insert(byte, tmp);
        return;
    } else {
        // Gets position of the stack where it needs to go if the condition fails
        // it needs to go 2 more because of the none of the block and the setto 
        byte.offset = CobaluStack.Size() + 1;  

        // Reinsert the value into the stack
        CobaluStack.Insert(byte, tmp);
    }
    
    // Not very proud of this hack, but for now will do
    // Generates a always false
    Bytecode alwfalse;
    alwfalse.inst = bolen;
    alwfalse.data = false;
    CobaluStack.Push(alwfalse);

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

void WhileAST::codegen() {
    // Saves the current position of the stack, to return later
    int start = CobaluStack.Size() - 1;

    // Generates the code of the condition
    Cond->codegen();

    // Creates the setto to that points to the end of the loop
    Bytecode endloop;
    endloop.inst = setto;
    CobaluStack.Push(endloop);

    // Saves the position of the endloop
    int endpos = CobaluStack.Size() - 1;

    // Generates the loop code
    Loop->codegen();

    // Generates the byte code to return to the start of the loop
    // Doesn't need to generate always false because block already does it
    // Generates the setto
    Bytecode startloop;
    startloop.inst = setto;
    startloop.offset = start;
    CobaluStack.Push(startloop);

    // Generates the null so the condition has where to go if fails
    Bytecode endpoint;
    endpoint.inst = none;
    CobaluStack.Push(endpoint);

    // Saves the current position of the stack so we can escape
    // the loop
    int end = CobaluStack.Size() - 1;
    endloop.offset = end;
    CobaluStack.Insert(endloop, endpos);

    // Set breakpoints if any. (End - 2) we don't need to verify the end anyway
    CobaluStack.SetBreaks(start, end-2, end);
}

void ForAST::codegen() {
    // First generates the variable
    Var->codegen();

    // Saves the current position of the stack, to return later
    int start = CobaluStack.Size() - 1;

    // Generates the code of the condition
    Cond->codegen();

    // Creates the setto to that points to the end of the loop
    Bytecode endloop;
    endloop.inst = setto;
    CobaluStack.Push(endloop);

    // Saves the position of the endloop
    int endpos = CobaluStack.Size() - 1;

    // Generates the loop code
    Loop->codegen();

    // Generates the code of the iterator
    Iterator->codegen();

    // Generates a always false
    Bytecode alwfalse;
    alwfalse.inst = bolen;
    alwfalse.data = false;
    CobaluStack.Push(alwfalse);

    // Generates the byte code to return to the start of the loop
    // Doesn't need to generate always false because block already does it
    // Generates the setto
    Bytecode startloop;
    startloop.inst = setto;
    startloop.offset = start;
    CobaluStack.Push(startloop);

    // Generates the null so the condition has where to go if fails
    Bytecode endpoint;
    endpoint.inst = none;
    CobaluStack.Push(endpoint);

    // Saves the current position of the stack so we can escape
    // the loop
    int end = CobaluStack.Size() - 1;
    endloop.offset = end;
    CobaluStack.Insert(endloop, endpos);

    // Set breakpoints if any. (End - 2) we don't need to verify the end anyway
    CobaluStack.SetBreaks(start, end-2, end);
    return;
}

void BreakAST::codegen() {
    // Generates a always false
    Bytecode alwfalse;
    alwfalse.inst = bolen;
    alwfalse.data = false;
    CobaluStack.Push(alwfalse);

    // Generates the break, it's offset wiil be -1 so we can search it in while
    Bytecode bytebreak;
    bytebreak.inst = setto;
    bytebreak.offset = -1;
    CobaluStack.Push(bytebreak);

    return;
}

void FunctionAST::codegen() {
    return;
}

void CallFuncAST::codegen() {
    return;
}

void ReturnAST::codegen() {
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
