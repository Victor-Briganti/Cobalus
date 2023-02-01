#include <stack>

#include "error_log.h"

std::stack<Logging> Logs;

// Used to save LineNumber where the error occur
int LineNumber;

// Insert a new error into the stack
void PushError(std::string Identifier, std::string Error, int Level) {
    // I know it's a little dirty but for now it is what it is
    if (Level == 1){
        Logging ErrorLog = Logging(Identifier, Error, Level, LineNumber);
        Logs.push(ErrorLog);
    } 
    if (Level == 2) {
        Logging ErrorLog = Logging(Identifier, Error, Level);
        Logs.push(ErrorLog);
    }
}

// Return the number of errors
int NumErrors() {
    return Logs.size();
}

// Print the errors
void ShowErrors() {
    while(!Logs.empty()) {
        auto Show = Logs.top();
        Logs.pop();
        Show.PrintError();
    }
}
