// Standard
#include <fstream> 
#include <memory>

// Local

enum Bytecode {
    // Arithmetic
    addD, // D = Double
    subD,
    mulD,
    divD,
};

void InitVM(std::shared_ptr<std::fstream> FileInput);
