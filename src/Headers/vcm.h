#include "global.h"

enum Instruction {
    // Types
    ndoubl, // double
    cstr, // string
    bolen, // bool
    none, // null
    
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
    
    // Variables
    varst, // store
    varrt, // return

    // Goto
    setto,
};

struct Bytecode {
    Instruction inst;
    Value data;
    int offset;
};

class InstructionStack {
    std::vector<Bytecode> Stack;
    int sp; // stack pointer

    public:
        InstructionStack() {}

        // Stack Operations
        void Push(Bytecode);
        int Size();
        void ChangeValue(Value, int);
        void Insert(Bytecode, int);
        Bytecode Return(int);
        void Advance();
        int SP();
        void Goto(int);
        #ifdef DEBUG
        void StackReset();
        #endif
};

extern InstructionStack CobaluStack;

// VM Operation
void InitVM();

// Declaration for codegeneration
void Compile();
