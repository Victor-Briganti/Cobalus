#include "global.h"

// Instructions
enum Instruction {
    // Types
    ndoubl, // double
    cstr, // string
    bolen, // bool

    // Binary:
    // arithmetic
    addD, // D = Double
    subD,
    mulD,
    divD,
    
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

// VM Operation
void InitVM(std::shared_ptr<std::fstream> FileInput);
