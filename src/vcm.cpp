#include "Headers/error_log.h"
#include "Headers/exec.h"
#include "Headers/vcm.h" 
#include <unordered_map>

// +++++++++++++++++
// ++++ GLOBALS ++++
// +++++++++++++++++

// Stack that stores variables and constants for execution
Calculus ExecStack;

// Map of Instructions to String
std::unordered_map<Instruction, std::string> inst_to_str = { 
    {ndoubl, "ndoubl"},
    {cstr, "cstr"},
    {bolen, "bolen"},
    {none, "none"},
    {varst, "varst"},
    {varrt, "vart"},
    {addD, "addD"},
    {subD, "subD"},
    {divD, "divD"},
    {mulD, "mulD"},
    {eqD, "eqD"},
    {ineqD, "ineqD"},
    {grD, "grD"},
    {lsD, "lsD"},
    {negte, "negte"},
    {invsig, "invsig"},
    {stio, "stio"},
};

// Map of Instructions to Methods
typedef void (Calculus::*calc_method)();
std::unordered_map<Instruction, calc_method> inst_to_func = {
    {addD, &Calculus::addData},
    {subD, &Calculus::subData},
    {divD, &Calculus::divData},
    {mulD, &Calculus::mulData},
    {eqD, &Calculus::eqData},
    {ineqD, &Calculus::ineqData},
    {grD, &Calculus::grData},
    {lsD, &Calculus::lsData},
    {greqD, &Calculus::greqData},
    {lseqD, &Calculus::lseqData},
    {negte, &Calculus::negData},
    {invsig, &Calculus::invsigData},
    {stio, &Calculus::printData},
};

// Stack and pointer of instructions
std::vector<Bytecode> CobaluStack; // Stack of instructions 

// Function for stack of instructions
void PushStack(Bytecode byte) {
    CobaluStack.push_back(byte);
    if (CobaluStack.size() == 0) {
        return;
    }
}

int SizeStack() {
    return CobaluStack.size();
}

void InsertVal (Value data, int offset) {
    Bytecode byte = CobaluStack[offset];
    byte.data = data;
    CobaluStack[byte.offset] = byte;
    return;
}

Bytecode RetStack (int offset) {
    return CobaluStack[offset];
}

///////////////////////////////////////////////////////////////////////////////
////////////                    VM EXECUTION                       ////////////
///////////////////////////////////////////////////////////////////////////////

void Interpreter(Bytecode byte, int offset) {
    switch (byte.inst) {
        case ndoubl:
        case bolen:
        case cstr:
        case none: {
            #ifdef DEBUG 
                printf("[%s]:\n", inst_to_str[byte.inst].c_str());
            #endif

            ExecStack.PushCalc(byte.data);
            break;
        }
        case addD:
        case subD:
        case mulD:
        case divD:
        case eqD:
        case ineqD:
        case grD:
        case lsD:
        case greqD:
        case lseqD:
        case negte:
        case invsig:
        case stio: {
            #ifdef DEBUG 
                printf("[%s]:\n", inst_to_str[byte.inst].c_str());
            #endif

            // Cool Hack to call methods that don't need args
            calc_method method = inst_to_func[byte.inst];
            (ExecStack.*method)();
            break;
        } 
        case varst: {
            #ifdef DEBUG
                printf("[varst]:\n");
            #endif
         
           ExecStack.stvarData(offset);
           break;
        }
        case varrt: {
            #ifdef DEBUG 
                printf("[varrt]:\n");
            #endif

            ExecStack.retvarData(offset);
            break;
        }
        default: {
            std::string Error = "Instruction was not reconized";
            std::string Id = ".";
            ErLogs.PushError(Id, Error, 2);
            break; // the show must go on
        }
    }
}

void CodeExec() {
    // Execute instruction line by line
    for (int i = 0; i < CobaluStack.size(); i++) {
        Interpreter(CobaluStack[i], i);
    }
    
    // If there is any error show all of them
    if (ErLogs.NumErrors()) {
       ErLogs.ShowErrors();
       return;
    }
   
    #ifdef STACK
    std::cout << std::left << std::setw(30) << "================ COBALUS STACK ================" << std::endl;
    std::cout << std::left << std::setw(6) << "x" << "|";
    std::cout << std::left << std::setw(20) << "data";
    std::cout << std::left << std::setw(15) << "instruction";
    std::cout << std::left << std::setw(10) << "offset" << std::endl;
    for (int i=0; i< CobaluStack.size(); i++) {
        Bytecode byte = CobaluStack[i];
    std::cout << std::left << std::setw(6) << i << "|";
            switch(byte.data.index()) {
            case doub: {
                std::cout << std::left << std::setw(20) << 
                    std::get<double>(byte.data);
                break;
            }
            case str: {
                std::cout << std::left << std::setw(20) << 
                    std::get<std::string>(byte.data);
                break;
            }
            case boo: {
                std::cout << std::left << std::setw(20) << 
                    std::get<bool>(byte.data);
                break;
            }
            case nil: {
                std::cout << std::left << std::setw(20) << "null";
                break;
            }
            default: {
                    std::cout << std::left << std::setw(20) << "[ERROR]";
                    break;
                }
        }
        std::cout << std::left << std::setw(15) << inst_to_str[byte.inst];
        std::cout << std::left << std::setw(10) << byte.offset << std::endl;
    }
    std::cout << std::left << std::setw(30) << "================ END OF STACK =================" << std::endl;
    #endif
}

void InitVM() {
    // Generate the code and fill the stack
    Compile();
    
    CodeExec();
}
