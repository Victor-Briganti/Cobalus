// Standard
#include <fstream>
#include <stdio.h>
#include <string>
#include <memory>

// Local
#include "parser.h"


void Interpreter(std::fstream FileInput) {
    if (!FileInput.is_open()) {
        printf("Could not load file\nExiting...\n");
        exit(1);
    }
    
    std::shared_ptr<std::fstream> Buffer = 
        std::make_unique<std::fstream>(std::move(FileInput));

    while(true) {
        int Result = Tokenizer(Buffer);
        if (Result == -1) {
            printf("EOF");
            break;
        }

        if (Result > 0) {
            if (Result == ';') {
                printf("%c\n", Result);
            } else {
                printf("%c", Result);
            }
        } else {
            printf("%d", Result);
        }
        
    }
}

int main(int argc, char** argv) {
    std::fstream FileInput; 
    
    if (argc < 2) {
        printf("No file was provided\nExiting...\n");
        exit(1);
    }

    FileInput.open(argv[1]);

    Interpreter(std::move(FileInput));
}
