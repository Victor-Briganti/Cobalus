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
        // Comparasion
        void eqData();
        void ineqData();
        void grData();
        void lsData();
        void greqData();
        void lseqData();

        // Unary Operations on Doubles
        void negData();
        void invsigData();
        
        // Built-in
        void PrintTop(); // print
};
    
