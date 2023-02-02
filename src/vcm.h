// Standard
#include <fstream> 
#include <memory>
#include <variant>
#include <vector>

// Local
#include "definition.h"

// Instructions
enum Instruction {
    ndoubl,

    // Arithmetic
    addD, // D = Double
    subD,
    mulD,
    divD,
    
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
