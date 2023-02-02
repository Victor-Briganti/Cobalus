#include "global.h"

class Logging {
    std::string Identifier;
    std::string Error;
    int Level;
    int Line;

public:
    // Log for Errors. This happen during the execution
    Logging(std::string Identifier, std::string Error, int Level) :
        Identifier(Identifier), Error(Error), Level(Level) {}

    // Log for Warning. This happen on the parser
    Logging(std::string Identifier, std::string Error, int Level, int Line) : 
        Identifier(Identifier), Error(Error), Level(Level), Line(Line) {}
    
    void PrintError() {
        // 1 is a warning
        if (Level == 1){
            printf("Warning: %s, %s on line %d\n", Error.c_str(), 
                                            Identifier.c_str(), Line);
        }
        // 2 is a execution error
        if (Level == 2) {
            printf("Error: %s %s\n", Error.c_str(), Identifier.c_str());
        }
    }   
};

// Saves the number of lines
extern int LineNumber;

// Error Functions
void PushError(std::string Identifier, std::string Error, int Level);
int NumErrors(); // return the number of errors
void ShowErrors();
