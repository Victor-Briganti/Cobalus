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
        void addData();
        void subData();
        void mulData();
        void divData();

        // Unary Operations on Doubles
        void negData();
        void invsigData();
        
        // Built-in
        void PrintTop(); // print
};
    
