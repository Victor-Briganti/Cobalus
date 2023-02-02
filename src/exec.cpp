#include "error_log.h"
#include "exec.h" 
#include "vcm.h"

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
    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();
   
    #ifdef DEBUG 
        printf("  %g\n", std::get<double>(Left));
        printf("  +\n");
        printf("  %g\n", std::get<double>(Right));
    #endif

    Right = std::get<double>(Left) + std::get<double>(Right);
    Calc.push_back(Right);
}

// double - double
void Calculus::subDouble() {
    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();

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
    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();

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
    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();

    #ifdef DEBUG 
        printf("  %g\n", std::get<double>(Left));
        printf("  /\n");
        printf("  %g\n", std::get<double>(Right));
    #endif
    
    Right = std::get<double>(Left) / std::get<double>(Right);
    Calc.push_back(Right);
}

void Calculus::PrintTop() {
    Value tmp = Calc.back();
    Calc.pop_back();
    printf("%g\n", std::get<double>(tmp));
}

