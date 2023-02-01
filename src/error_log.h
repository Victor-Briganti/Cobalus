#include <string>
#include <stdio.h>

class Logging {
    std::string Identifier;
    std::string Error;
    int Level;
    int Line;

public:
    Logging(std::string Identifier, std::string Error, int Level, int Line) : 
        Identifier(Identifier), Error(Error), Level(Level), Line(Line) {}
    
    void PrintError() {
        // Used for errors during the parser
        if (Level == 1) {
            printf("Warning: %s , %s on line %d\n", 
                    Error.c_str(), Identifier.c_str(), Line);
            return;
        }
        // Used for error in execution
        if (Level == 2) {
            printf("Error: %s , %s during execution", 
                    Error.c_str(), Identifier.c_str());
            return;
        }
    
    }
};

// Saves the number of lines
extern int LineNumber;

// Error Functions
void PushError(std::string Identifier, std::string Error, int Level);
int Errors(); // return the number of errors
void ShowErrors();
