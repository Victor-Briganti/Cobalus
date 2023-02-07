#include "Headers/error_log.h"
#include "Headers/exec.h"
#include "Headers/vcm.h" 

// +++++++++++++++++
// ++++ GLOBALS ++++
// +++++++++++++++++

// Stack that stores variables and constants for execution
Calculus ExecStack;

// Stack of instructions
InstructionStack CobaluStack;

#ifdef DEBUG
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
    {setto, "setto"},
    {funcsta, "funcsta"},
    {funcend, "funcend"},
    {callfunc, "callfunc"},
    {endstk, "endstk"},
};
#endif

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
    {setto, &Calculus::evalCondition},
};

///////////////////////////////////////////////////////////////////////////////
////////////                COBALUSTACK METHODS                    ////////////
///////////////////////////////////////////////////////////////////////////////

void InstructionStack::Push(Bytecode byte) {
    Stack.push_back(byte);
}

int InstructionStack::Size() {
    return Stack.size();
}

void InstructionStack::SetEOS() {
    eos = Stack.size() - 1;
}

int InstructionStack::EOS() {
    return eos;
}

void InstructionStack::ChangeValue(Value data, int offset) {
    Bytecode byte = Stack[offset];
    byte.data = data;
    Stack[byte.offset] = byte;
    return;
}

void InstructionStack::Insert(Bytecode byte, int offset) {
    Stack[offset] = byte;
    return;
}

Bytecode InstructionStack::Return(int offset = -1) {
    if (offset == -1) {
        return Stack[sp];
    }
    return Stack[offset];
}

void InstructionStack::Advance() {
    sp++;
    if (sp > Stack.size()) {
        ErLogs.PushError("", "Stack overflow.", 2);
        ErLogs.ShowErrors();
        exit(1);
    }
}

int InstructionStack::SP() {
    return sp;
}

void InstructionStack::Goto(int offset) {
    sp = offset;
}

void InstructionStack::SetBreaks(int Start, int End) {
    for (;Start < End; Start++) {
        if (Stack[Start].inst == setto) {
            if (std::get<double>(Stack[Start].data) == -1) {
                Stack[Start].data = 0.0;
                Stack[Start].offset = -(Stack[Start].offset - End);
            }
        }
    }
}

#ifdef DEBUG
void InstructionStack::StackReset() {
    sp = 0;
}
#endif


///////////////////////////////////////////////////////////////////////////////
////////////                    VM EXECUTION                       ////////////
///////////////////////////////////////////////////////////////////////////////

void Interpreter(Bytecode byte, int offset) {
    switch (byte.inst) {
        case ndoubl:
        case bolen:
        case cstr:
        case none: {
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
        case stio:
        case setto: {
            // Cool Hack to call methods that don't need args
            calc_method method = inst_to_func[byte.inst];
            (ExecStack.*method)();
            break;
        } 
        case varst: {
           ExecStack.stvarData(offset);
           break;
        }
        case varrt: {
            ExecStack.retvarData(offset);
            break;
        }
        case funcsta: {
            ExecStack.funcGen(offset);
            break;
        }
        case callfunc: {
            ExecStack.callFunc(offset);
            break;
        }
        case stop: {
            break;
        }
        default: {
            ErLogs.PushError("", "Instuction was not reconized", 2);
            break; // the show must go on
        }
    }
}

void CodeExec(int EOS) {
    // Execute instruction line by line
    while (true) {
        if (CobaluStack.SP() != EOS) {
            Interpreter(CobaluStack.Return(), CobaluStack.SP());
            CobaluStack.Advance();
        } else {
            break;
        }
    }
    
    // If there is any error show all of them
    if (ErLogs.NumErrors()) {
       ErLogs.ShowErrors();
       return;
    }
}

void InitVM() {
    // Generate the code and fill the stack
    Compile();

    // Set the End of Stack
    Bytecode byte;
    byte.inst = endstk;
    CobaluStack.Push(byte);
    CobaluStack.SetEOS();

    CodeExec(CobaluStack.Size() - 1);
    #ifdef DEBUG
        std::cout << std::left << std::setw(30) << "================ COBALUS STACK ================" << std::endl;
        std::cout << std::left << std::setw(6) << "x" << "|";
        std::cout << std::left << std::setw(20) << "data";
        std::cout << std::left << std::setw(15) << "instruction";
        std::cout << std::left << std::setw(10) << "offset" << std::endl;
        CobaluStack.StackReset();
        while (CobaluStack.SP() < CobaluStack.Size()) {
            Bytecode byte = CobaluStack.Return();
            std::cout << std::left << std::setw(6) << CobaluStack.SP() << "|";
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
            CobaluStack.Advance();
        }
        std::cout << std::left << std::setw(30) << "================ END OF STACK =================" << std::endl;
        #endif
}
