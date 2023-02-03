#include "Headers/error_log.h"
#include "Headers/lexer.h"

// Buffers
std::string Identifier;
std::string StringBuffer;
double DoubleBuffer;

// Reads file char by char
char Buffer = ' ';

void WhiteSpaceRM() {
    while(isspace(Buffer)) {
        FileInput.get(Buffer);

        // Add lines
        if (Buffer == '\n') {
            ErLogs.AddLine();
        }

        if (FileInput.eof()) { 
            Buffer = -1; 
            break; 
        }
    }
}

// Compare strings
Token checkId(int lenght, char Comp[], Token type) 
{
    char Peek[lenght];
    for (int i=0; i <= lenght; i++) {
        FileInput.get(Buffer);
        if (!isalpha(Buffer)) {
            break;
        }
        Peek[i] = Buffer;
        Identifier += Buffer;
    }

    if(memcmp(Comp, Peek, lenght) == 0) {
        return type;
    }

    // If fails saves the identifier
    while (isalnum(Buffer) || Buffer ==  '_') {
            Identifier += Buffer;
            FileInput.get(Buffer);
    }
    return TOKEN_ID;
}

int Tokenizer() {     
    // Remove Whitespaces 
    WhiteSpaceRM();
    FileInput.eof();

    // Ignore Comments
    // #.*
    while (Buffer == '#') {
        FileInput.get(Buffer);

        while (Buffer != '\r' &&  Buffer != '\n' && !FileInput.eof()) {
            FileInput.get(Buffer);
        }
        
        WhiteSpaceRM();
        FileInput.eof();
    }

    // Numbers
    // [0-9]+[.][0-9]*
    if (isdigit(Buffer)) {
        std::string NumStr;
        NumStr += Buffer;
        FileInput.get(Buffer);

        while (isdigit(Buffer)) {
            NumStr += Buffer;
            FileInput.get(Buffer);
        }
        if (Buffer == '.') {
            NumStr += Buffer;
            FileInput.get(Buffer);
            while(isdigit(Buffer)) {
                NumStr += Buffer;
                FileInput.get(Buffer);
            }

            DoubleBuffer = strtod(NumStr.c_str(), nullptr);
            return TOKEN_DOUBLE;
        }

        // Modify to int
        DoubleBuffer = strtod(NumStr.c_str(), nullptr);
        return TOKEN_DOUBLE;
    }
    
    // Strings
    // ".*"
    if (Buffer == '"') {
        StringBuffer.clear();
        FileInput.get(Buffer);
        while (isprint(Buffer)) {
            if (Buffer == '"') {
                FileInput.get(Buffer);
                return TOKEN_STRING;
            }
            StringBuffer += Buffer;
            FileInput.get(Buffer);
        }
    }

    // Operations
    // [+-/*]
    if (Buffer == '+') {
        FileInput.get(Buffer);
        return TOKEN_PLUS;
    }
    if (Buffer == '-') {
        FileInput.get(Buffer);
        return TOKEN_MINUS;
    }
    if (Buffer == '/') {
        FileInput.get(Buffer);
        return TOKEN_DIV;
    }
    if (Buffer == '*') {
        FileInput.get(Buffer);
        return TOKEN_MUL;
    }

    // Atribution
    // =
    if (Buffer == '=') {
        FileInput.get(Buffer);
        if (Buffer != '=') {
            return TOKEN_ATR;
        }
    }

    // Comparasion
    // [><=](?=)
    if (Buffer == '=') {
        FileInput.get(Buffer);
        return TOKEN_EQUAL; // '=='
    }
    if (Buffer == '<') {
        FileInput.get(Buffer);
        if (Buffer == '=') {
            FileInput.get(Buffer);
            return TOKEN_LESSEQ; // '<='
        }
        return TOKEN_LESS; // '<'
    }
    if (Buffer == '>') {
        FileInput.get(Buffer);
        if (Buffer == '=') { 
            FileInput.get(Buffer);
            return TOKEN_GREATEQ; // '>='
        }
        return TOKEN_GREATER; // '>'
    }

    // Unary
    // !(=)?
    if (Buffer == '!') {
        FileInput.get(Buffer);
        if (Buffer == '=') {
            // A wild comparasion appears!
            FileInput.get(Buffer);
            return TOKEN_INEQUAL; // '!='
        }
        return TOKEN_NOT; 
    }
    
    // Logical
    // (&&) | (||)
    if (Buffer == '&') {
        FileInput.get(Buffer);
        if (Buffer == '&') {
            FileInput.get(Buffer);
            return TOKEN_AND;
        }
        
        // For now there is no use for a single &
        int Literal = Buffer;
        FileInput.get(Buffer);
        return Literal;
    }
    if (Buffer == '|') {
        FileInput.get(Buffer);
        if (Buffer == '|') {
            FileInput.get(Buffer);
            return TOKEN_OR;
        }
        // For now there is no use for a single |
        int Literal = Buffer;
        FileInput.get(Buffer);
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
                return checkId(1, Comp, TOKEN_IF);
            }
            case 'p': {
                Identifier += Buffer;
                char Comp[] = "rint";
                return checkId(4, Comp, TOKEN_PRINT);
            }
            case 'e': {
                Identifier += Buffer;
                char Comp[] = "lse";
                return checkId(3, Comp, TOKEN_ELSE);
            }
            case 'v': {
                Identifier += Buffer;
                char Comp[] = "ar";
                return checkId(2, Comp, TOKEN_VAR);
            }
            case 'b': {
                Identifier += Buffer;
                char Comp[] = "reak";
                return checkId(4, Comp, TOKEN_BREAK);
            }
            case 'n': {
                Identifier += Buffer;
                char Comp[] = "ull";
                return checkId(3, Comp, TOKEN_NULL);
            }
            case 'w': {
                Identifier += Buffer;
                char Comp[] = "hile";
                return checkId(4, Comp, TOKEN_WHILE);
            }
            case 'c': {
                Identifier += Buffer;
                char Comp[] = "lass";
                return checkId(4, Comp, TOKEN_CLASS);
            }
            case 's': {
                Identifier += Buffer;
                char Comp[] = "uper";
                return checkId(4, Comp, TOKEN_SUPER);
            }
            case 'r': {
                Identifier += Buffer;
                char Comp[] = "eturn";
                return checkId(5, Comp, TOKEN_RET);
            }
            case 't': {
                Identifier += Buffer;
                FileInput.get(Buffer);
                switch(Buffer) {
                    case 'r': {
                        Identifier += Buffer;
                        char Comp[] = "ue";
                        return checkId(2, Comp, TOKEN_TRUE);
                    }
                    case 'h': {
                        Identifier += Buffer;
                        char Comp[] = "is";
                        return checkId(2, Comp, TOKEN_THIS);
                    }
                }
            }
            case 'f': {
                Identifier += Buffer;
                FileInput.get(Buffer);
                switch(Buffer) {
                    case 'a': {
                        Identifier += Buffer;
                        char Comp[] = "lse";
                        return checkId(3, Comp, TOKEN_FALSE);
                    }
                    case 'u': {
                        Identifier += Buffer;
                        char Comp[] = "nc";
                        return checkId(2, Comp, TOKEN_FUNC);
                    }
                    case 'o': {
                        Identifier += Buffer;
                        char Comp[] = "r";
                        return checkId(1, Comp, TOKEN_FOR);
                    }
                }
            }
        }
        while (isalnum(Buffer) || Buffer == '_') {
            Identifier += Buffer;
            FileInput.get(Buffer);
        }
        return TOKEN_ID;
    }

    // If nothing else worked, return a literal
    int Literal = Buffer;
    FileInput.get(Buffer);
    return Literal;
}
