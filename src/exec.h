// Standard
#include <string>
#include <variant> 
#include <vector> 

// Local

// TODO: use a unique define for the variant in all files
#define Data std::variant<double, bool, std::string, int*>

// Definition of the class 
class Calculus {
    std::vector<Data> Calc;
    
    public:
        // Operations on stack
        void PushCalc(Data byte);

        // Operation for Double Values
        // Arithmetic 
        void addDouble();
        void subDouble();
        void mulDouble();
        void divDouble();
        
        // Built-in
        void PrintTop(); // print
};
    
