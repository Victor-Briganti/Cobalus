#include "global.h"

// TODO: use a unique define for the variant in all files

// Definition of the class 
class Calculus {
    std::vector<Value> Calc;
    
    public:
        // Operations on stack
        void PushCalc(Value byte);

        // Binary Operations on Doubles
        // Arithmetic 
        void addDouble();
        void subDouble();
        void mulDouble();
        void divDouble();

        // Unary Operations on Doubles
        void negDouble();
        void invsigDouble();
        
        // Built-in
        void PrintTop(); // print
};
    
