#include "error_log.h"
#include "exec.h" 
#include "vcm.h"

int Calculus::EmptyStack() {
    if (Calc.empty()) {
        PushError("", "illegal instruction stack of execution is empty", 2);        
        return 1;
    }
    return 0;
}

void Calculus::PushCalc(Value byte) {
    #ifdef DEBUG
        switch(byte.index()) {
            case 0:{
                printf("  %g\n", std::get<double>(byte));
                break;
            }
            case 1: {
                printf("  %d\n", std::get<bool>(byte));
                break;
            }
            case 2: {
                printf("  %s\n", std::get<std::string>(byte).c_str());
                break;
            }
            case 4:{
                printf("  null\n");
                break;
            }
            default: {
                printf("ERROR\n");
                break;
            }
        }
    #endif
    Calc.push_back(byte);
}

// double + double
void Calculus::addDouble() {
    if (EmptyStack()) {
        return;
    }

    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();

    if (Right.index() != Left.index()) {
        PushError("", "types don't match", 2);        
        return;
    } 

    #ifdef DEBUG
    if(!Right.index()){
        printf("  %g\n", std::get<double>(Left));
        printf("  +\n");
        printf("  %g\n", std::get<double>(Right));
    }
    if (Right.index() == 2) {
        printf("  %s\n", std::get<std::string>(Left).c_str);
        printf("  +\n");
        printf("  %s\n", std::get<std::string>(Right).c_str);
    }
    #endif

    if (!Left.index()) {
        Right = std::get<double>(Left) + std::get<double>(Right);
    }
    if (!Left.index()) {
        Right = std::get<std::string>(Left) + std::get<std::string>(Right);
    }
    Calc.push_back(Right);
}

// double - double
void Calculus::subDouble() {
    if (EmptyStack()) {
        return;
    }

    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();

    if (Right.index() != Left.index()) {
        PushError("", "types don't match", 2);        
        return;
    }
    if (Right.index() == 2 || Left.index() == 2) {
        PushError("", "illegal instruction in strings", 2);        
        return;
    }

    #ifdef DEBUG 
        printf("  %g\n", std::get<double>(Left));
        printf("  -\n");
        printf("  %g\n", std::get<double>(Right));
    #endif
    
    Right = std::get<double>(Left) - std::get<double>(Right);
    Calc.push_back(Right);
}

// double * double
void Calculus::mulDouble() {
    if (EmptyStack()) {
        return;
    }

    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();
    
    if (Right.index() != Left.index()) {
        PushError("", "types don't match", 2);        
        return;
    }
    if (Right.index() == 2 || Left.index() == 2) {
        PushError("", "illegal instruction in strings", 2);        
        return;
    }

    #ifdef DEBUG 
        printf("  %g\n", std::get<double>(Left));
        printf("  *\n");
        printf("  %g\n", std::get<double>(Right));
    #endif
    
    Right = std::get<double>(Left) * std::get<double>(Right);
    Calc.push_back(Right);
}

// double / double
void Calculus::divDouble() {
    if (EmptyStack()) {
        return;
    }

    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();
    
    if (Right.index() != Left.index()) {
        PushError("", "types don't match", 2);        
        return;
    }
    if (Right.index() == 2 || Left.index() == 2) {
        PushError("", "illegal instruction in strings", 2);        
        return;
    }

    #ifdef DEBUG 
        printf("  %g\n", std::get<double>(Left));
        printf("  /\n");
        printf("  %g\n", std::get<double>(Right));
    #endif
    
    Right = std::get<double>(Left) / std::get<double>(Right);
    Calc.push_back(Right);
}

// - double
void Calculus::invsigDouble() {
    if (EmptyStack()) {
        return;
    }

    Value Expr = Calc.back();
    Calc.pop_back();
    
    if (Expr.index() == 2) {
        PushError("", "illegal instruction in strings", 2);        
        return;
    }
    
    #ifdef DEBUG 
        printf("  -\n");
        printf("  %g\n", std::get<double>(Expr));
    #endif

    Calc.push_back(-std::get<double>(Expr));
}

// ! double
void Calculus::negDouble() {
    if (EmptyStack()) {
        return;
    }

    Value Expr = Calc.back();
    Calc.pop_back();
    
    if (Expr.index() == 2) {
        PushError("", "illegal instruction in strings", 2);        
        return;
    }

    #ifdef DEBUG 
        printf("  !\n");
        printf("  %g\n", std::get<double>(Expr));
    #endif
    
    if(!std::get<double>(Expr)) {
        Value tmp = 1.0;
        Calc.push_back(tmp);
        return;
    } else {
        Value tmp = 0.0;
        Calc.push_back(tmp);
        return;
    }
}

void Calculus::PrintTop() {
    if (EmptyStack()) {
        return;
    }

    Value tmp = Calc.back();
    Calc.pop_back();
    
    switch(tmp.index()) {
        case 0: {
            printf("%g\n", std::get<double>(tmp));
            break;
        }
        case 1: {
             printf("%d\n", std::get<bool>(tmp));
             break;
        }
        case 2: {
            printf("'%s'\n", std::get<std::string>(tmp).c_str());
            break;
        }
        default: {
            printf("null\n");
            break;
        }
    }
}

