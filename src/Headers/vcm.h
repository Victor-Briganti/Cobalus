#include "global.h"

// Instructions
enum Instruction {
    // Types
    ndoubl, // double
    cstr, // string
    bolen, // bool
    none, // null
    
    // Variables
    varst, // store
    varrt, // return

    // Binary:
    // arithmetic
    addD, // D = data
    subD,
    mulD,
    divD,
    // Comparasion
    eqD,
    ineqD,
    grD,
    lsD,
    greqD,
    lseqD,

    // Unary
    negte, // negate '!'
    invsig, // invert signal '-'

    // Built-in Function
    stio, // print
};

struct Bytecode {
    Instruction inst;
    Value data;
    int offset;
};

// Stack Operations
void PushStack(Bytecode);
int SizeStack();
void InsertVal(Value, int);
Bytecode RetStack(int);

// VM Operation
void InitVM();

// Declaration for codegeneration
void Compile();