// File only for macros
#include <string>
#include <variant>

// Define "union"
#define Value std::variant<double, bool, std::string, int*>
