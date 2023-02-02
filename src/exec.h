#include "global.h"

// TODO: use a unique define for the variant in all files

// Definition of the class 
class Calculus {
    std::vector<Value> Calc;
    
    public:
        // Operations on stack
        void PushCalc(Value byte);

        // Operation for Double Values
        // Arithmetic 
        void addDouble();
        void subDouble();
        void mulDouble();
        void divDouble();
        
        // Built-in
        void PrintTop(); // print
};
    
