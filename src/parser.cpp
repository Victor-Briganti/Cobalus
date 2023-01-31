// Standard
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <fstream>

// Local
#include "parser.h"

std::string Identifier; // Saves the current identifier
std::string StringBuffer; // Saves the current string
double DoubleBuffer; // Saves the current double

// Reads file char by char
char Buffer = ' ';

void WhiteSpaceRM(std::shared_ptr<std::fstream> FileInput) {
    while(isspace(Buffer)) {
        FileInput->get(Buffer);

        // Add lines
        // if (Buffer == '\n') {}

        if (FileInput->eof()) { 
            Buffer = -1; 
            break; 
        }
    }
}

// Compare strings
Token checkId(int lenght, char Comp[lenght], Token type, 
              std::shared_ptr<std::fstream> FileInput) 
{
    char Peek[lenght];
    for (int i=0; i <= lenght; i++) {
        FileInput->get(Buffer);
        Peek[i] = Buffer;
    }

    if(memcmp(Comp, Peek, lenght) == 0) {
        return type;
    }
    
    Identifier += Peek;

    // If fails saves the identifier
    while (isalnum(Buffer) || Buffer ==  '_') {
            Identifier += Buffer;
            FileInput->get(Buffer);
    }
    return token_id;
}

int Tokenizer(std::shared_ptr<std::fstream> FileInput) {     
    // Remove Whitespaces 
    WhiteSpaceRM(FileInput);
    FileInput->eof();

    // Ignore Comments
    // #.*
    while (Buffer == '#') {
        FileInput->get(Buffer);

        while (Buffer != '\r' &&  Buffer != '\n' && !FileInput->eof()) {
            FileInput->get(Buffer);
        }
        
        WhiteSpaceRM(FileInput);
        FileInput->eof();
    }

    // Numbers
    // [0-9]+[.][0-9]*
    if (isdigit(Buffer)) {
        std::string NumStr;
        NumStr += Buffer;
        FileInput->get(Buffer);

        while (isdigit(Buffer)) {
            NumStr += Buffer;
            FileInput->get(Buffer);
        }
        if (Buffer == '.') {
            NumStr += Buffer;
            FileInput->get(Buffer);
            while(isdigit(Buffer)) {
                NumStr += Buffer;
                FileInput->get(Buffer);
            }

            DoubleBuffer = strtod(NumStr.c_str(), nullptr);
            return token_double;
        }

        // Modify to int
        DoubleBuffer = strtod(NumStr.c_str(), nullptr);
        return token_double;
    }
    
    // Strings
    // ".*"
    if (Buffer == '"') {
        FileInput->get(Buffer);
        while (isprint(Buffer)) {
            if (Buffer == '"') {
                FileInput->get(Buffer);
                return token_string;
            }
            StringBuffer += Buffer;
            FileInput->get(Buffer);
        }
    }

    // Operations
    // [+-/*]
    if (Buffer == '+') {
        FileInput->get(Buffer);
        return token_plus;
    }
    if (Buffer == '-') {
        FileInput->get(Buffer);
        return token_minus;
    }
    if (Buffer == '/') {
        FileInput->get(Buffer);
        return token_div;
    }
    if (Buffer == '*') {
        FileInput->get(Buffer);
        return token_mul;
    }

    // Atribution
    // =
    if (Buffer == '=') {
        FileInput->get(Buffer);
        if (Buffer != '=') {
            return token_atr;
        }
    }

    // Comparasion
    // [><=](?=)
    if (Buffer == '=') {
        FileInput->get(Buffer);
        return token_equal; // '=='
    }
    if (Buffer == '<') {
        FileInput->get(Buffer);
        if (Buffer == '=') {
            return token_lesseq; // '<='
        }
        return token_less; // '<'
    }
    if (Buffer == '>') {
        FileInput->get(Buffer);
        if (Buffer == '=') { 
            return token_greateq; // '>='
        }
        return token_greater; // '>'
    }

    // Unary
    // !(=)?
    if (Buffer == '!') {
        FileInput->get(Buffer);
        if (Buffer == '=') {
            // A wild comparasion appears!
            return token_inequal; // '!='
        }
        return token_not; 
    }
    
    // Logical
    if (Buffer == '&') {
        FileInput->get(Buffer);
        if (Buffer == '&') {
            FileInput->get(Buffer);
            return token_and;
        }
        
        // For now there is no use for a single &
        int Literal = Buffer;
        FileInput->get(Buffer);
        return Literal;
    }
    if (Buffer == '|') {
        FileInput->get(Buffer);
        if (Buffer == '|') {
            FileInput->get(Buffer);
            return token_or;
        }
        // For now there is no use for a single |
        int Literal = Buffer;
        FileInput->get(Buffer);
        return Literal;
    }

    // Verify if is a identifier
    // [A-Za-z]+[A-Za-z0-9_]* 
    if (isalpha(Buffer)) {
        Identifier.clear();
        switch(Buffer) {
            case 'i': {
                Identifier += Buffer;
                char Comp[] = "f";
                return checkId(1, Comp, token_if, FileInput);
            }
            case 'e': {
                Identifier += Buffer;
                char Comp[] = "lse";
                return checkId(3, Comp, token_else, FileInput);
            }
            case 'v': {
                Identifier += Buffer;
                char Comp[] = "ar";
                return checkId(2, Comp, token_var, FileInput);
            }
            case 'b': {
                Identifier += Buffer;
                char Comp[] = "reak";
                return checkId(4, Comp, token_break, FileInput);
            }
            case 'n': {
                Identifier += Buffer;
                char Comp[] = "ull";
                return checkId(3, Comp, token_null, FileInput);
            }
            case 'w': {
                Identifier += Buffer;
                char Comp[] = "hile";
                return checkId(4, Comp, token_while, FileInput);
            }
            case 'c': {
                Identifier += Buffer;
                char Comp[] = "lass";
                return checkId(4, Comp, token_class, FileInput);
            }
            case 's': {
                Identifier += Buffer;
                char Comp[] = "uper";
                return checkId(4, Comp, token_super, FileInput);
            }
            case 'r': {
                Identifier += Buffer;
                char Comp[] = "eturn";
                return checkId(5, Comp, token_ret, FileInput);
            }
            case 't': {
                Identifier += Buffer;
                FileInput->get(Buffer);
                switch(Buffer) {
                    case 'r': {
                        Identifier += Buffer;
                        char Comp[] = "ue";
                        return checkId(2, Comp, token_true, FileInput);
                    }
                    case 'h': {
                        Identifier += Buffer;
                        char Comp[] = "is";
                        return checkId(2, Comp, token_this, FileInput);
                    }
                }
            }
            case 'f': {
                Identifier += Buffer;
                FileInput->get(Buffer);
                switch(Buffer) {
                    case 'a': {
                        Identifier += Buffer;
                        char Comp[] = "lse";
                        return checkId(3, Comp, token_false, FileInput);
                    }
                    case 'u': {
                        Identifier += Buffer;
                        char Comp[] = "nc";
                        return checkId(2, Comp, token_func, FileInput);
                    }
                    case 'o': {
                        Identifier += Buffer;
                        char Comp[] = "r";
                        return checkId(1, Comp, token_for, FileInput);
                    }
                }
            }
        }
        while (isalnum(Buffer) || Buffer == '_') {
            Identifier += Buffer;
            FileInput->get(Buffer);
        }
        return token_id;
    }

    // If nothing else worked, return a literal
    int Literal = Buffer;
    FileInput->get(Buffer);
    return Literal;
}
