#include "global.h"

class Logging {
    struct Log {
        std::string Id;
        std::string Error;
        int Level;
        int Line;
    };

    // Stack of Errors
    std::vector<Log> StackError;

    int LineNumber;

public:
    // Log for Errors. This happen during the execution
    Logging() {}
        
    
    void ShowErrors();
    void PushError(std::string, std::string, int);
    int NumErrors();
    void AddLine();
};

extern Logging ErLogs;