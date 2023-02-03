#include "Headers/block.h"
#include "Headers/vcm.h"

///////////////////////////////////////////////////////////////////////////////
////////////                    BLOCK METHODS                      ////////////
///////////////////////////////////////////////////////////////////////////////

int BlockAST::setOffset(std::string Variable) {
    OffsetMap[Variable] = SizeStack();
    return SizeStack();
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

