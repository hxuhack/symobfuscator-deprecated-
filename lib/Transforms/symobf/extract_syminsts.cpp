#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include <list> 
using namespace llvm;

#define DEBUG_TYPE "Extract_SymInsts"


const std::string default_symvar = "symvar";

static cl::opt<std::string> symvar("symvar", cl::desc("choose a symbolic variable to obfuscate"), cl::value_desc("variable name"), cl::init(default_symvar), cl::Optional);

namespace {
  struct ExtSymInsts : public FunctionPass {
    static char ID; 
    ExtSymInsts() : FunctionPass(ID) {}
    virtual bool runOnFunction(Function &F){
      //errs()<<"Function " << F.getName()<<"\n";
      std::list<BasicBlock *> basicBlocks;
      for (Function::iterator i=F.begin();i!=F.end();++i) {
        basicBlocks.push_back((BasicBlock *)i);
      }
      while(!basicBlocks.empty()){
        BasicBlock* basicBlock = basicBlocks.front();
        basicBlock->print(errs());
        errs()<<"--------------------\n";
        for (BasicBlock::iterator it = basicBlock->begin(); it!=basicBlock->end(); ++it){
          Instruction *inst = dyn_cast<Instruction>(it); 
          //errs()<<inst->getName()<<"\n";
	  if(AllocaInst *allocaInst = dyn_cast<AllocaInst>(inst)){
            errs()<<"+ALLOCA INST:"<<allocaInst->getName()<<"="<<allocaInst->getValueName()<<"\n";
 	  }
	  if(LoadInst *loadInst = dyn_cast<LoadInst>(inst)){
            errs()<<"+LOAD INST:"<<"="<<loadInst->getValueName()<<loadInst->getName()<<"\n";
	  }
	  if(BinaryOperator *bOp = dyn_cast<BinaryOperator>(inst)){
            errs()<<"BIN_OP:"<<bOp->getOpcode()<<"\n";
 	  }
        }
        basicBlocks.pop_front();
      }
      return false;
    }
  };
}

char ExtSymInsts::ID = 0;
static RegisterPass<ExtSymInsts> X("syminst", "extract symbolic instructions");
