#include "block.h"
#include "error_log.h"
#include "vcm.h"

int BlockAST::setOffset(std::string Variable) {
    OffsetMap[Variable] = SizeStack()+1;
    return SizeStack()+1;
}

int BlockAST::getOffset(std::string Variable) {
   if(!OffsetMap.count(Variable)) {
        if (!ParentBlock) {
            PushError(Variable, "not identified", 2);        
            return -1;
        }
        ParentBlock->getOffset(Variable);
    } 
    return OffsetMap[Variable];
}

