#include <stack>

#include "error_log.h"

std::stack<Logging> Logs;

// Used to save LineNumber where the error occur
int LineNumber;

// Insert a new error into the stack
void PushError(std::string Identifier, std::string Error, int Level) {
    Logging ErrorLog = Logging(Identifier, Error, Level, LineNumber);
    Logs.push(ErrorLog);
}

// Return the number of errors
int Errors() {
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
