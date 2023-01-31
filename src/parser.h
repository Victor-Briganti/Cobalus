// Library 
#include <fstream>
#include <memory>

// First stage of the parser TOKENS!!!
int Tokenizer(std::shared_ptr<std::fstream> FileInput);

enum Token {
    // The following will be treated as literals
    // ';', '"', '.', '{', '}', '#', ...

    // End of File
    token_eof = -1,

    // Identifier
    token_var = -2,
    token_atr = -3, // =
    token_id = -4,

    // Function 
    token_func = -5,
    token_ret = -6,

    // Values 
    token_double = -7,
    //token_int = -8
    token_string = -9,
    token_true = -10,
    token_false = -11,
    token_null = -12,

    // Control Flow
    token_while = -13,
    token_for = -14,
    token_break = -15,
    token_if = -16,
    token_else = -17,

    // Class
    token_class = -17,
    token_super = -18,
    token_this = -19,

    // Operators:
    // comparasion
    token_equal = -20,
    token_inequal = -21,
    token_greater = -22,
    token_less = -23,
    token_lesseq = -24,
    token_greateq = -25,
    // arithmetic 
    token_plus = -26,
    token_minus = -27,
    token_mul = -28,
    token_div = -29,
    // logical
    token_and = -30,
    token_or = -31,
    // unary
    token_not = -32,
};
