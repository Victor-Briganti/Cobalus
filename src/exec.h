#include "global.h"

// Definition of the class 
class Calculus {
    std::vector<Value> Calc;


    public:
        // Verify the Stack
        int EmptyStack();    

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
    
