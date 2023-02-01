// Standard
#include <fstream>
#include <memory>
#include <variant>

#include <stdio.h> // remove

// Local
#include "parser.h"
#include "compiler.h"

///////////////////////////////////////////////////////////////////////////////
////////////                    CODE GENERATION                    ////////////
///////////////////////////////////////////////////////////////////////////////

void DoubleAST::codegen() {
   printf("%g\n", DoubleValue);
   return;
}

void OperationAST::codegen() {
    if (LHS) { LHS->codegen(); }
    if (RHS) { RHS->codegen(); }

    if (Op != 0){
        printf("%d\n", Op);
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////
////////////                    FRONT COMPILER                     ////////////
///////////////////////////////////////////////////////////////////////////////

void Compile(std::shared_ptr<std::fstream> FileComp) {
    std::unique_ptr<DeclarationAST> Decl = Parser(FileComp);

    while (Decl) {
        Decl->codegen();
        Decl = Parser(FileComp);
    }
}
