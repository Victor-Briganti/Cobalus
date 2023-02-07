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

    // Function
    funcsta,
    funcend,
    stop, // used to separated expressions in args
    callfunc,

    // Goto
    setto,
    endstk, // End Of Stack
};

struct Bytecode {
    Instruction inst;
    Value data;
    int offset;
};

class InstructionStack {
    std::vector<Bytecode> Stack;
    int sp; // stack pointer
    int eos; // end of stack

    public:
        InstructionStack() {}

        // Stack Operations
        void Push(Bytecode);
        int Size();
        void SetEOS();
        int EOS();
        void ChangeValue(Value, int);
        void Insert(Bytecode, int);
        Bytecode Return(int);
        void Advance();
        int SP();
        void Goto(int);
        void SetBreaks(int, int);
        #ifdef DEBUG
        void StackReset();
        #endif
};

extern InstructionStack CobaluStack;

// VM Operation
void InitVM();

// Declaration for codegeneration
void Compile();

// Declaration for execution of code
void CodeExec(int);
