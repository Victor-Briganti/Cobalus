#include "global.h"

// All the program is wrapper by thin layer of Block
class BlockAST {
    // Variable that stores the state of the block
    int State;
    
    // It's possible to have nested blocks, and each block has it's own 
    // variable map. 
    std::shared_ptr<BlockAST> ParentBlock;

    // This will be a map that stores the offset of variables and functions
    std::unordered_map<std::string, int> OffsetMap;
    
    public:
        BlockAST(std::shared_ptr<BlockAST> ParentBlock ,int State) 
            : State(State), ParentBlock(ParentBlock)  {}

         int getOffset(std::string Variable);
         int setOffset(std::string Variable);
};
