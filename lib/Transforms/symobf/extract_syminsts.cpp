#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"
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
      errs()<<"Function " << F.getName()<<"\n";
      std::list<BasicBlock *> basicBlocks;
      for (Function::iterator i=F.begin();i!=F.end();++i) {
        basicBlocks.push_back((BasicBlock *)i);
      }
      while(!basicBlocks.empty()){
        BasicBlock* basicBlock = basicBlocks.front();
        basicBlock->print(errs());
        for (BasicBlock::iterator it = basicBlock->begin(); it!=basicBlock->end(); ++it){
          Instruction *inst = dyn_cast<Instruction>(it); 
          errs()<<inst->getOpcodeName()<<"\n";
        }
        basicBlocks.pop_front();
      }
      return false;
    }
  };
/*
  struct extract_syminsts1 : public BasicBlockPass {
    static char ID; // Pass identification, replacement for typeid
    extract_syminsts1() : BasicBlockPass(ID) {}
    virtual bool runOnBasicBlock(BasicBlock &BB) {
      for (BasicBlock::iterator it = BB.begin(); it != BB.end(); ++it){
        //errs()<<it->getName() <<":"<<it->getOpcodeName()<<'\n';
        it->print(errs());
        errs()<<'\n';
      }
      return false;
    }
  };
*/
/*
  struct extract_syminsts : public ImmutablePass {
    static char ID; //
    extract_syminsts() : ImmutablePass(ID) {}
    virtual bool runOnModule(Module &MD) {
      errs()<<MD.getNamedValue("str")<<'\n';
      errs()<<symvar<<'\n';
      return false;
    }
  };
*/
}

char ExtSymInsts::ID = 0;
static RegisterPass<ExtSymInsts> X("syminst", "extract symbolic instructions");
