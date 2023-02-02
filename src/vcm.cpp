#include "error_log.h"
#include "exec.h"
#include "vcm.h" 
#include "compiler.h"

// +++++++++++++++++
// ++++ GLOBALS ++++
// +++++++++++++++++

// Stack and pointer of instructions
std::vector<Bytecode> CobaluStack; // Stack of instructions
int ps; // pointer for instructions 

// Function for stack of instructions
void PushStack(Bytecode byte) {
    CobaluStack.push_back(byte);
    if (CobaluStack.size() == 0) {
        return;
    }
}

// Stack that stores variables and constants for execution
Calculus ExecStack;

///////////////////////////////////////////////////////////////////////////////
////////////                    VM EXECUTION                       ////////////
///////////////////////////////////////////////////////////////////////////////

void Interpreter(Bytecode byte) {
    switch (byte.inst) {
        case ndoubl: {
            #ifdef DEBUG 
                printf("[ndouble]:\n");
            #endif

            ExecStack.PushCalc(byte.data);
            break;
        }
        case addD: {
            #ifdef DEBUG 
                printf("[addD]:\n");
            #endif
            
            ExecStack.addDouble();
            break;
        }
        case subD: {
            #ifdef DEBUG 
                printf("[subD]:\n");
            #endif
           
            ExecStack.subDouble();
            break;
        }
        case divD: {
            #ifdef DEBUG 
                printf("[divD]:\n");
            #endif
           
            ExecStack.divDouble();
            break;
        }
        case mulD: {
            #ifdef DEBUG 
                printf("[mulD]:\n");
            #endif
           
            ExecStack.mulDouble();
            break;
        }
        case stio: {
            #ifdef DEBUG 
                printf("[stdio]:\n");
            #endif
           
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
    // Execute instruction line by line
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
