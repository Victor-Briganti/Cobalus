// Definition of all librarys
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <memory>
#include <string> 
#include <unordered_map>
#include <variant>
#include <vector>

// Define "union"
#define Value std::variant<double, bool, std::string, int*>

extern std::fstream FileInput;

// Definition for DEBUGs
//#define DEBUG
//#define STACK
