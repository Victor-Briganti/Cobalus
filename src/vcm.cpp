// Standard
#include<fstream> 
#include <memory>
#include <variant>
#include <vector>

#include <stdio.h>

// Local
#include "error_log.h"
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

// TODO: pass this class to another file before it gets to f@#$ big.

// Class for wrapping Calculation Stack 
class Calculus {
    std::vector<Value> Calc; // Stack for Calculus
    
    public:
        void PushStack(Value numb) {
            Calc.push_back(numb);
        }
        
        // double + double
        void addDouble() {
            Value Right = Calc.back();
            Calc.pop_back();
            
            Value Left = Calc.back();
            Calc.pop_back();
            
            Right = std::get<double>(Left) + std::get<double>(Right);
            Calc.push_back(Right);
        }
        
        // double - double
        void subDouble() {
            Value Right = Calc.back();
            Calc.pop_back();
            
            Value Left = Calc.back();
            Calc.pop_back();
            
            Right = std::get<double>(Left) - std::get<double>(Right);
            Calc.push_back(Right);
        }
        
        // double * double
        void mulDouble() {
            Value Right = Calc.back();
            Calc.pop_back();
            
            Value Left = Calc.back();
            Calc.pop_back();
            
            Right = std::get<double>(Left) * std::get<double>(Right);
            Calc.push_back(Right);
        }
        
        // double / double
        void divDouble() {
            Value Right = Calc.back();
            Calc.pop_back();
            
            Value Left = Calc.back();
            Calc.pop_back();
            
            Right = std::get<double>(Left) / std::get<double>(Right);
            Calc.push_back(Right);
        }

        void PrintTop() {
            Value tmp = Calc.back();
            Calc.pop_back();
            printf("%g\n", std::get<double>(tmp));
        }
};

Calculus ExecStack;

///////////////////////////////////////////////////////////////////////////////
////////////                    VM EXECUTION                       ////////////
///////////////////////////////////////////////////////////////////////////////


void Interpreter(Bytecode byte) {
    switch (byte.inst) {
        case ndoubl: {
            ExecStack.PushStack(byte.data);
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
    if (!NumErrors()) {
       ShowErrors();
        return;
    }
}

void InitVM(std::shared_ptr<std::fstream> FileInput) {
    // Generate the code and fill the stack
    Compile(FileInput);
    
    CodeExec();
}
