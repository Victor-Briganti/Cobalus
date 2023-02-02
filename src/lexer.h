#include "global.h"

// First stage of the parser TOKENS!!!
int Tokenizer(std::shared_ptr<std::fstream> FileInput);

// Buffers.
// Used to save the current value of a constant
extern std::string Identifier;
extern std::string StringBuffer; 
extern double DoubleBuffer;

enum Token {
    // The following will be treated as literals
    // ';', '"', '.', '{', '}', '#', ...

    // End of File
    TOKEN_EOF = -1,

    // Identifier
    TOKEN_VAR = -2,
    TOKEN_ATR = -3, // =
    TOKEN_ID = -4,

    // Function 
    TOKEN_FUNC = -5,
    TOKEN_RET = -6,

    // Values 
    TOKEN_DOUBLE = -7,
    //token_int = -8
    TOKEN_STRING = -9,
    TOKEN_TRUE = -10,
    TOKEN_FALSE = -11,
    TOKEN_NULL = -12,

    // Control Flow
    TOKEN_WHILE = -13,
    TOKEN_FOR = -14,
    TOKEN_BREAK = -15,
    TOKEN_IF = -16,
    TOKEN_ELSE = -17,

    // Class
    TOKEN_CLASS = -17,
    TOKEN_SUPER = -18,
    TOKEN_THIS = -19,

    // Operators:
    // comparasion
    TOKEN_EQUAL = -20,
    TOKEN_INEQUAL = -21,
    TOKEN_GREATER = -22,
    TOKEN_LESS = -23,
    TOKEN_LESSEQ = -24,
    TOKEN_GREATEQ = -25,
    // arithmetic 
    TOKEN_PLUS = -26,
    TOKEN_MINUS = -27,
    TOKEN_MUL = -28,
    TOKEN_DIV = -29,
    // logical
    TOKEN_AND = -30,
    TOKEN_OR = -31,
    // unary
    TOKEN_NOT = -32,
    
    // Built-in
    TOKEN_PRINT = -33,
    
    TOKEN_INSIG = -34,
};
