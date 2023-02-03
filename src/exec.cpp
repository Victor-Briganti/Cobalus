#include "Headers/error_log.h"
#include "Headers/exec.h" 
#include "Headers/vcm.h"

// Verification for Types
int TypesMatch(int R, int L) {
    if (R == doub && L == str) {
        return 0;
    }
    if (R == boo && L == doub) {
        return 0;
    }
    if (R == doub && L == boo) {
        return 0;
    }
    if (R == str && L == doub) {
        return 0;
    }
    if (R == boo && L == str) {
        return 0;
    }
    if (R == str && L == boo) {
        return 0;
    }
    if (R == nil || L == nil) {
        return 0;
    }
    return 1;
}

///////////////////////////////////////////////////////////////////////////////
////////////                    BYTECODE OPERATIONS                ////////////
///////////////////////////////////////////////////////////////////////////////

// If the stack of values is empty return a error
int Calculus::EmptyStack() {
    if (Calc.empty()) {
        ErLogs.PushError("", \
            "illegal instruction stack of execution is empty", 2);        
        return 1;
    }
    return 0;
}

// Insert Values on the stack of execution
void Calculus::PushCalc(Value byte) {
    #ifdef DEBUG
        switch(byte.index()) {
            case doub:{
                printf("  %g\n", std::get<double>(byte));
                break;
            }
            case boo: {
                printf("  %d\n", std::get<bool>(byte));
                break;
            }
            case str: {
                printf("  %s\n", std::get<std::string>(byte).c_str());
                break;
            }
            case nil:{
                printf("  null\n");
                break;
            }
            default: {
                printf("TYPE ERROR\n");
                break;
            }
        }
    #endif
    Calc.push_back(byte);
}

// double + double
// strint + string
void Calculus::addData() {
    if (EmptyStack()) {
        return;
    }

    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();
    
    // If is not a string or a a double give a error
    if ((Right.index()%2) || (Left.index()%2)) {
        ErLogs.PushError("", "operation on type not permited", 2);
        return;
    }
    // It the types are not the same give a error
    if (Right.index() != Left.index()) {
        ErLogs.PushError("", "types don't match", 2);        
        return;
    } 

    #ifdef DEBUG
    if(Right.index() == doub){
        printf("  %g\n", std::get<double>(Left));
        printf("  +\n");
        printf("  %g\n", std::get<double>(Right));
    }
    if (Right.index() == str) {
        printf("  %s\n", std::get<std::string>(Left).c_str());
        printf("  +\n");
        printf("  %s\n", std::get<std::string>(Right).c_str());
    }
    #endif

    if (!Left.index()) {
        Right = std::get<double>(Left) + std::get<double>(Right);
        Calc.push_back(Right);
        return;
    }
    if (Left.index() == str) {
        Right = std::get<std::string>(Left) + std::get<std::string>(Right);
        Calc.push_back(Right);
        return;
    }
}

// double - double
void Calculus::subData() {
    if (EmptyStack()) {
        return;
    }

    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();

    if (Right.index() != Left.index()) {
        ErLogs.PushError("", "types don't match", 2);        
        return;
    }
    // I already search to see if the types are equal, if they are equal but
    // are also a string give a error
    if (Right.index() == str) {
        ErLogs.PushError("", "illegal instruction in strings", 2);        
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
void Calculus::mulData() {
    if (EmptyStack()) {
        return;
    }

    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();
    
    if (Right.index() != Left.index()) {
        ErLogs.PushError("", "types don't match", 2);        
        return;
    }
    if (Right.index() == str) {
        ErLogs.PushError("", "illegal instruction in strings", 2);        
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
void Calculus::divData() {
    if (EmptyStack()) {
        return;
    }

    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();
    
    if (Right.index() != Left.index()) {
        ErLogs.PushError("", "types don't match", 2);        
        return;
    }
    if (Right.index() == str) {
        ErLogs.PushError("", "illegal instruction in strings", 2);        
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
void Calculus::invsigData() {
    if (EmptyStack()) {
        return;
    }

    Value Expr = Calc.back();
    Calc.pop_back();
   
    if (Expr.index() == boo) {
        ErLogs.PushError("", "illegal instruction on booleans", 2);        
        return;
    }
    if (Expr.index() == str) {
        ErLogs.PushError("", "illegal instruction in strings", 2);        
        return;
    }
    
    #ifdef DEBUG 
        printf("  -\n");
        printf("  %g\n", std::get<double>(Expr));
    #endif

    Calc.push_back(-std::get<double>(Expr));
}

// ! double
// ! bool
void Calculus::negData() {
    if (EmptyStack()) {
        return;
    }

    Value Expr = Calc.back();
    Calc.pop_back();
    
    if (Expr.index() == str) {
        ErLogs.PushError("", "illegal instruction in strings", 2);        
        return;
    }
    
    #ifdef DEBUG 
    if (!Expr.index()){
        printf("  !\n");
        printf("  %g\n", std::get<double>(Expr));
    }
    if (Expr.index() == 1) { 
        printf("  !\n");
        if (!std::get<bool>(Expr)){
            printf("  !\n");
            printf("  true\n");
        } else {
            printf("  !\n");
            printf("  false\n");
        }
    }
    #endif
    
    if (Expr.index() == doub){
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
    if (Expr.index() == boo) {
        if(!std::get<bool>(Expr)) {
            Value tmp = true;
            Calc.push_back(tmp);
            return;
        } else {
            Value tmp = false;
            Calc.push_back(tmp);
            return;
        }
    }
}

// double == double
// bool == bool
// string == string
void Calculus::eqData() {
    if (EmptyStack()) {
        return;
    }

    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();
   
    if (!TypesMatch(Left.index(), Right.index())) {
        ErLogs.PushError("", "types don't match", 2);        
        return;
    }

    #ifdef DEBUG 
     if (Right.index() == str) {
        printf("  %s\n", std::get<std::string>(Left).c_str());
        printf("  ==\n");
        printf("  %s\n", std::get<std::string>(Right).c_str());
     }
     if (Right.index() == boo && Left.index() == boo){
        printf("  %d\n", std::get<bool>(Left));
        printf("  ==\n");
        printf("  %d\n", std::get<bool>(Right));
     }
     if (Right.index() == doub && Left.index() == doub) {
        printf("  %g\n", std::get<double>(Left));
        printf("  ==\n");
        printf("  %g\n", std::get<double>(Right));
     }
    #endif
    
    if (Right.index() == doub && Left.index() == doub){
        Right = (std::get<double>(Left) == std::get<double>(Right));
        Calc.push_back(Right);
        return;
    }
    if (Right.index() == str){
        Right = std::get<std::string>(Left) == std::get<std::string>(Right);
        Calc.push_back(Right);
        return;
    }
    if (Right.index() == boo && Right.index() == boo){
        Right = std::get<bool>(Left) == std::get<bool>(Right);
        Calc.push_back(Right);
        return;
    }
    return;
}

// double != double
// bool != bool
// string != string
void Calculus::ineqData() {
    if (EmptyStack()) {
        return;
    }

    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();
    
    if (!TypesMatch(Left.index(), Right.index())) {
        ErLogs.PushError("", "types don't match", 2);        
        return;
    }

    #ifdef DEBUG 
     if (Right.index() == str) {
        printf("  %s\n", std::get<std::string>(Left).c_str());
        printf("  !=\n");
        printf("  %s\n", std::get<std::string>(Right).c_str());
     }
     if (Right.index() == boo && Left.index() == boo){
        printf("  %d\n", std::get<bool>(Left));
        printf("  !=\n");
        printf("  %d\n", std::get<bool>(Right));
     }
     if (Right.index() == doub && Left.index() == doub) {
        printf("  %g\n", std::get<double>(Left));
        printf("  !=\n");
        printf("  %g\n", std::get<double>(Right));
     }
    #endif
    
    if (Right.index() == doub && Left.index() == doub){
        Right = (std::get<double>(Left) != std::get<double>(Right));
        Calc.push_back(Right);
        return;
    }
    if (Right.index() == str){
        Right = std::get<std::string>(Left) != std::get<std::string>(Right);
        Calc.push_back(Right);
        return;
    }
    if (Right.index() == doub && Right.index() == boo){
        Right = std::get<double>(Left) != std::get<bool>(Right);
        Calc.push_back(Right);
        return;
    }
    if (Right.index() == boo && Right.index() == doub){
        Right = std::get<bool>(Left) != std::get<double>(Right);
        Calc.push_back(Right);
        return;
    }
    if (Right.index() == boo && Right.index() == boo){
        Right = std::get<bool>(Left) != std::get<bool>(Right);
        Calc.push_back(Right);
        return;
    }
    return;
}

// double > double
// bool > bool
void Calculus::grData() {
    if (EmptyStack()) {
        return;
    }

    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();
    
    if (!TypesMatch(Left.index(), Right.index())) {
        ErLogs.PushError("", "types don't match", 2);        
        return;
    }
    if(Right.index() == str) {
        ErLogs.PushError("", "operation not permited on strings", 2);        
        return;
    }

    #ifdef DEBUG 
     if (Right.index() == boo && Left.index() == boo){
        printf("  %d\n", std::get<bool>(Left));
        printf("  >\n");
        printf("  %d\n", std::get<bool>(Right));
     }
     if (Right.index() == doub && Left.index() == doub) {
        printf("  %g\n", std::get<double>(Left));
        printf("  >\n");
        printf("  %g\n", std::get<double>(Right));
     }
    #endif
    
    if (Right.index() == doub && Left.index() == doub){
        Right = (std::get<double>(Left) > std::get<double>(Right));
        Calc.push_back(Right);
        return;
    }
    if (Right.index() == boo && Right.index() == boo){
        Right = std::get<bool>(Left) > std::get<bool>(Right);
        Calc.push_back(Right);
        return;
    }
    return;
}

// double < double
// bool < bool
void Calculus::lsData() {
    if (EmptyStack()) {
        return;
    }

    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();
    
    if (!TypesMatch(Left.index(), Right.index())) {
        ErLogs.PushError("", "types don't match", 2);        
        return;
    }
    if(Right.index() == str) {
        ErLogs.PushError("", "operation not permited on strings", 2);        
        return;
    }

    #ifdef DEBUG 
     if (Right.index() == boo && Left.index() == boo){
        printf("  %d\n", std::get<bool>(Left));
        printf("  <\n");
        printf("  %d\n", std::get<bool>(Right));
     }
     if (Right.index() == doub && Left.index() == doub) {
        printf("  %g\n", std::get<double>(Left));
        printf("  <\n");
        printf("  %g\n", std::get<double>(Right));
     }
    #endif
    
    if (Right.index() == doub && Left.index() == doub){
        Right = (std::get<double>(Left) < std::get<double>(Right));
        Calc.push_back(Right);
        return;
    }
    if (Right.index() == boo && Right.index() == boo){
        Right = std::get<bool>(Left) < std::get<bool>(Right);
        Calc.push_back(Right);
        return;
    }
    return;
}

// double >= double
// bool >= bool
void Calculus::greqData() {
    if (EmptyStack()) {
        return;
    }

    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();
    
    if (!TypesMatch(Left.index(), Right.index()))
    {
        ErLogs.PushError("", "types don't match", 2);        
        return;
    }
    if(Right.index() == str) {
        ErLogs.PushError("", "operation not permited on strings", 2);        
        return;
    }

    #ifdef DEBUG 
     if (Right.index() == boo && Left.index() == boo){
        printf("  %d\n", std::get<bool>(Left));
        printf("  >=\n");
        printf("  %d\n", std::get<bool>(Right));
     }
     if (Right.index() == doub && Left.index() == doub) {
        printf("  %g\n", std::get<double>(Left));
        printf("  >=\n");
        printf("  %g\n", std::get<double>(Right));
     }
    #endif
    
    if (Right.index() == doub && Left.index() == doub){
        Right = (std::get<double>(Left) >= std::get<double>(Right));
        Calc.push_back(Right);
        return;
    }
    if (Right.index() == str){
        Right = std::get<std::string>(Left) >= std::get<std::string>(Right);
        Calc.push_back(Right);
        return;
    }
    if (Right.index() == boo && Right.index() == boo){
        Right = std::get<bool>(Left) >= std::get<bool>(Right);
        Calc.push_back(Right);
        return;
    }
    return;
}

// double <= double
// bool <= bool
void Calculus::lseqData() {
    if (EmptyStack()) {
        return;
    }

    Value Right = Calc.back();
    Calc.pop_back();

    Value Left = Calc.back();
    Calc.pop_back();
    
    if (!TypesMatch(Left.index(), Right.index())) 
    {
        ErLogs.PushError("", "types don't match", 2);        
        return;
    }
    if(Right.index() == str) {
        ErLogs.PushError("", "operation not permited on strings", 2);        
        return;
    }

    #ifdef DEBUG 
     if (Right.index() == str) {
        printf("  %s\n", std::get<std::string>(Left).c_str());
        printf("  <=\n");
        printf("  %s\n", std::get<std::string>(Right).c_str());
     }
     if (Right.index() == boo){
        printf("  %d\n", std::get<bool>(Left));
        printf("  <=\n");
        printf("  %d\n", std::get<bool>(Right));
     }
     if (Right.index() == doub) {
        printf("  %g\n", std::get<double>(Left));
        printf("  <=\n");
        printf("  %g\n", std::get<double>(Right));
     }
    #endif
    
    if (Right.index() == doub){
        Right = (std::get<double>(Left) <= std::get<double>(Right));
        Calc.push_back(Right);
        return;
    }
    if (Right.index() == boo){
        Right = std::get<bool>(Left) <= std::get<bool>(Right);
        Calc.push_back(Right);
        return;
    }
    return;
}

// Built-in Print
void Calculus::printData() {
    if (EmptyStack()) {
        return;
    }

    Value tmp = Calc.back();
    Calc.pop_back();
    
    switch(tmp.index()) {
        case doub: {
            printf("%g\n", std::get<double>(tmp));
            break;
        }
        case boo: {
            if(std::get<bool>(tmp)) {
                printf("true\n");
            } else {
                printf("false\n");
            }
            break;
        }
        case str: {
            printf("'%s'\n", std::get<std::string>(tmp).c_str());
            break;
        }
        default: {
            printf("null\n");
            break;
        }
    }
}

// Store Variable and it's offset
void Calculus::stvarData(int offset) {
    if (EmptyStack()) {
        return;
    }
    
    Value var = Calc.back();
    Calc.pop_back();

    #ifdef DEBUG
    printf("[offset]: %d\n", offset);
    printf("[stackval]: ");
    switch(var.index()) {
        case doub: {
            printf("%g\n", std::get<double>(var));
            break;
        }
        case boo: {
            if(std::get<bool>(var)) {
                printf("true\n");
            } else {
                printf("false\n");
            }
            break;
        }
        case str: {
            printf("'%s'\n", std::get<std::string>(var).c_str());
            break;
        }
        case nil: {
            printf("null\n");
            break;
        }
        default: {
            printf("error\n");
            break;
        }
    }
    #endif

    CobaluStack.Insert(var, offset);

    return;
}

// Return the value of the variable
void Calculus::retvarData(int offset) {
    #ifdef DEBUG
    Bytecode var = CobaluStack.Return(offset);
    printf("[offset]: %d\n", offset);
    printf("[getto]: ");
    var = CobaluStack.Return(var.offset);
    printf("%d\n", var.offset);
    printf("[stackval]:");
    switch(var.data.index()) {
        case doub: {
            printf("%g\n", std::get<double>(var.data));
            break;
        }
        case boo: {
            if(std::get<bool>(var.data)) {
                printf("true\n");
            } else {
                printf("false\n");
            }
            break;
        }
        case str: {
            printf("'%s'\n", std::get<std::string>(var.data).c_str());
            break;
        }
        case nil: {
            printf("null\n");
            break;
        }
        default: {
            printf("error\n");
            break;
        }
    }
    #endif

    Bytecode byte = CobaluStack.Return(offset);
    byte = CobaluStack.Return(byte.offset);
    Calc.push_back(byte.data);
}
