#include "Headers/block.h"
#include "Headers/vcm.h"

///////////////////////////////////////////////////////////////////////////////
////////////                    BLOCK METHODS                      ////////////
///////////////////////////////////////////////////////////////////////////////

///   VARIABLES   ///
int BlockAST::varSetOffset(std::string Variable) {
    VarMap[Variable] = CobaluStack.Size() - 1;
    return CobaluStack.Size() - 1;
}

int BlockAST::varGetOffset(std::string Variable) {
   if(!VarMap.count(Variable)) {
        if (!ParentBlock) {
            return -1;
        }
        return ParentBlock->varGetOffset(Variable);
    }
    return VarMap[Variable];
}

///   FUNCTIONS   ///
int BlockAST::funcSetOffset(std::string Variable) {
    FuncMap[Variable] = CobaluStack.Size() - 1;
    return CobaluStack.Size() - 1;
}

int BlockAST::funcGetOffset(std::string Variable) {
   if(!FuncMap.count(Variable)) {
        if (!ParentBlock) {
            return -1;
        }
        return ParentBlock->funcGetOffset(Variable);
    }
    return FuncMap[Variable];
}
void BlockAST::ChangeState(int NewState) {
    State = NewState;
}

int BlockAST::ReturnState() {
    return State;
}
