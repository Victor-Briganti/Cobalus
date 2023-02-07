#include "global.h"

// All the program is wrapper by thin layer of Block
class BlockAST {
    // Variable that stores the state of the block
    int State;
    
    // It's possible to have nested blocks, and each block has it's own 
    // variable map. 
    std::shared_ptr<BlockAST> ParentBlock;

    // This will be a map that stores the offset of variables
    std::unordered_map<std::string, int> VarMap;

    // This will be a map that stores the offset of functions
    std::unordered_map<std::string, int> FuncMap;

    public:
        BlockAST(std::shared_ptr<BlockAST> ParentBlock ,int State) 
            : State(State), ParentBlock(ParentBlock)  {}

         int varGetOffset(std::string);
         int varSetOffset(std::string);
         int funcSetOffset(std::string);
         int funcGetOffset(std::string);
         void ChangeState(int);
         int ReturnState();
};
