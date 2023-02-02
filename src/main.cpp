#include "global.h"
#include "vcm.h"

void Interpreter(std::fstream FileInput) {
    if (!FileInput.is_open()) {
        printf("Could not load file\nExiting...\n");
        exit(1);
    }
    
    std::unique_ptr<std::fstream> tmp = 
        std::make_unique<std::fstream>(std::move(FileInput));
    std::shared_ptr<std::fstream> FileMove = std::move(tmp);
    
    InitVM(FileMove);

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
