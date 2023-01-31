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
        if (Level == 1) {
            printf("Warning: %s , %s on line %d\n", 
                    Error.c_str(), Identifier.c_str(), Line);
        }
    }
};
