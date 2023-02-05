#include "Headers/block.h"
#include "Headers/vcm.h"

///////////////////////////////////////////////////////////////////////////////
////////////                    BLOCK METHODS                      ////////////
///////////////////////////////////////////////////////////////////////////////

int BlockAST::setOffset(std::string Variable) {
    OffsetMap[Variable] = CobaluStack.Size() - 1;
    return CobaluStack.Size() - 1;
}

int BlockAST::getOffset(std::string Variable) {
   if(!OffsetMap.count(Variable)) {
        if (!ParentBlock) {
            return -1;
        }
        ParentBlock->getOffset(Variable);
    } 
    return OffsetMap[Variable];
}

void BlockAST::ChangeState(int NewState) {
    State = NewState;
}

int BlockAST::ReturnState() {
    return State;
}
