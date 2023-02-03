#include "Headers/global.h"
#include "Headers/error_log.h"
#include "Headers/vcm.h"

// Definition of the global class for errors
Logging ErLogs;

// Global File
std::fstream FileInput;

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("No file was provided\nExiting...\n");
        exit(1);
    }

    FileInput.open(argv[1]);

    if (!FileInput.is_open()) {
        printf("Could not load file\nExiting...\n");
        exit(1);
    }

    InitVM();
}
