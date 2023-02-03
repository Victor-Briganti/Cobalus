#include "Headers/error_log.h"

void Logging::ShowErrors() {
    for (int i=0; i < StackError.size(); i++) {
        // 1 is a warning
        if (StackError[i].Level == 1){
            printf("Warning: %s on %s. Line %d.", StackError[i].Id.c_str(), 
                StackError[i].Error.c_str(), StackError[i].Line);
        }
        // 2 is a execution error
        if (StackError[i].Level == 2) {
            printf("Error: %s on %s\n", StackError[i].Error.c_str(),
                StackError[i].Id.c_str());
        }
    }
}

// Insert a new error into the stack
void Logging::PushError(std::string Identifier, std::string Error, int Level) {
    Log Message;
    Message.Id = Identifier;
    Message.Error = Error;
    Message.Level = Level;

    // 1 is for errors during parser
    // 2 is for errors during execution
    if (Level == 1){
        Message.Line = LineNumber;
    }
    StackError.push_back(Message);
}

// Return the number of errors
int Logging::NumErrors() {
    return StackError.size();
}

// Keep track of line number
void Logging::AddLine() {
    LineNumber++;
}