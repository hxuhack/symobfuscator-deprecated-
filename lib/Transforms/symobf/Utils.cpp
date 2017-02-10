#include "Utils.hpp"

using namespace llvm;
using namespace std;

void PrintIR(Function* func){
//we print the whole IR for development
  LOG(L2_DEBUG) << "=====================Print Function IR==========================";

  list<BasicBlock *> basicBlocks;
  for (Function::iterator i=func->begin();i!=func->end();++i) {
    basicBlocks.push_back((BasicBlock *)i);
  }
  while(!basicBlocks.empty()){
    BasicBlock* basicBlock = basicBlocks.front();
    basicBlock->print(errs());
    basicBlocks.pop_front();
  }
}

void PrintIR(list<Instruction*> instList){
  LOG(L2_DEBUG) << "=====================Print List IR==========================";
  for(list<Instruction*>::iterator it = instList.begin(); it!=instList.end(); ++it){
    Instruction *inst = *it;
    inst->print(errs()); errs()<<"\n";
  }
}
