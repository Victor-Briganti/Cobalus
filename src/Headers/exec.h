#include "global.h"

enum ValueType {
    doub,
    boo,
    str,
    nil,
};

// Definition of the class 
class Calculus {
    std::vector<Value> Calc;

    public:
        // Verify the Stack
        int EmptyStack();    

        // Operations on stack
        void PushCalc(Value);

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
        
        // Variable
        void stvarData(int);
        void retvarData(int);

        // Built-in
        void printData(); // print

        // Condition
        void evalCondition();

        // Function
        void funcGen(int);
        void callFunc(int);
};
    
