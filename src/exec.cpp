#include "error_log.h"
#include "exec.h" 

void Calculus::PushCalc(Data byte) {
    Calc.push_back(byte);
}

// double + double
void Calculus::addDouble() {
    Data Right = Calc.back();
    Calc.pop_back();

    Data Left = Calc.back();
    Calc.pop_back();
   
    #ifdef DEBUG 
        printf("Funciona\n");
    #endif

    Right = std::get<double>(Left) + std::get<double>(Right);
    Calc.push_back(Right);
}

// double - double
void Calculus::subDouble() {
    Data Right = Calc.back();
    Calc.pop_back();

    Data Left = Calc.back();
    Calc.pop_back();

    Right = std::get<double>(Left) - std::get<double>(Right);
    Calc.push_back(Right);
}

// double * double
void Calculus::mulDouble() {
    Data Right = Calc.back();
    Calc.pop_back();

    Data Left = Calc.back();
    Calc.pop_back();

    Right = std::get<double>(Left) * std::get<double>(Right);
    Calc.push_back(Right);
}

// double / double
void Calculus::divDouble() {
    Data Right = Calc.back();
    Calc.pop_back();

    Data Left = Calc.back();
    Calc.pop_back();

    Right = std::get<double>(Left) / std::get<double>(Right);
    Calc.push_back(Right);
}

void Calculus::PrintTop() {
    Data tmp = Calc.back();
    Calc.pop_back();
    printf("%g\n", std::get<double>(tmp));
}

