// Standard
#include<fstream> 
#include <memory>

// Local
#include "error_log.h"
#include "vcm.h" 
#include "compiler.h"


void InitVM(std::shared_ptr<std::fstream> FileInput) {
    Compile(FileInput);
}
