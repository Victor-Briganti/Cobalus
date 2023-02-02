#include "error_log.h"
#include "exec.h"
#include "vcm.h" 
#include "compiler.h"

// +++++++++++++++++
// ++++ GLOBALS ++++
// +++++++++++++++++

// Stack and pointer of instructions
std::vector<Bytecode> CobaluStack; // Stack of instructions
int ps = 0; // pointer for instructions 

// Function for stack of instructions
void PushStack(Bytecode byte) {
    CobaluStack.push_back(byte);
    if (CobaluStack.size() == 0) {
        return;
    }
    ps++;
}

Calculus ExecStack;

///////////////////////////////////////////////////////////////////////////////
////////////                    VM EXECUTION                       ////////////
///////////////////////////////////////////////////////////////////////////////

void Interpreter(Bytecode byte) {
    switch (byte.inst) {
        case ndoubl: {
            ExecStack.PushCalc(byte.data);
            break;
        }
        case addD: {
            ExecStack.addDouble();
            break;
        }
        case subD: {
            ExecStack.subDouble();
            break;
        }
        case divD: {
            ExecStack.divDouble();
            break;
        }
        case mulD: {
            ExecStack.mulDouble();
            break;
        }
        case stio: {
            ExecStack.PrintTop();
            break;
        }
        default: {
            std::string Error = "Instruction was not reconized";
            std::string Id = ".";
            PushError(Id, Error, 2);
            break; // the show must go on
        }
    }
}

void CodeExec() {
    for (int i = 0; i < CobaluStack.size(); i++) {
        Interpreter(CobaluStack[i]);
    }
    // If there is any error show all of them
    if (NumErrors()) {
       ShowErrors();
       return;
    }
}

void InitVM(std::shared_ptr<std::fstream> FileInput) {
    // Generate the code and fill the stack
    Compile(FileInput);
    
    CodeExec();
}
