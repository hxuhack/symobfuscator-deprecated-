/*
Author: Hui Xu @ CUHK
Feature: Taint LLVM IR instructions
*/
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/InstVisitor.h"
#include "logger.hpp"
#include "TaintEngine.hpp"
#include <list> 

using namespace llvm;


//STATISTIC(TaintedInst, "Number of tainted llvm instructions");
loglevel_e loglevel = L2_DEBUG;

const std::string default_symvar = "symvar";

static cl::opt<std::string> symvar("symvar", cl::desc("choose a symbolic variable to obfuscate"), cl::value_desc("variable name"), cl::init(default_symvar), cl::Optional);

namespace Taint{

  struct Taint : public FunctionPass {
    static char ID; 
    Taint() : FunctionPass(ID) {}

    //Main function
    virtual bool runOnFunction(Function &F){
      LOG(L_DEBUG) << "Function: " << F.getName().str();

      TaintEngine taintEngine;
      
      // Set taint source: all arguments
      for (Function::arg_iterator argIt = F.arg_begin(); argIt != F.arg_end(); ++argIt){
        Value *argValue = &*argIt;
        LOG(L_DEBUG) << "Argument: " << argValue->getName().str();
	taintEngine.SetSource(argValue);
	taintEngine.Propagate();
      }
/*
      std::list<BasicBlock *> basicBlocks;
      for (Function::iterator i=F.begin();i!=F.end();++i) {
        basicBlocks.push_back((BasicBlock *)i);
      }
      while(!basicBlocks.empty()){
        BasicBlock* basicBlock = basicBlocks.front();
        basicBlock->print(errs());
        LOG(L_DEBUG)<<"--------------------\n";
        for (BasicBlock::iterator it = basicBlock->begin(); it!=basicBlock->end(); ++it){
          Instruction *inst = dyn_cast<Instruction>(it); 
          //errs()<<inst->getName()<<"\n";
	  if(AllocaInst *allocaInst = dyn_cast<AllocaInst>(inst)){
            LOG(L_DEBUG) << "+ALLOCA INST:" << allocaInst->getName().str() << "=" << allocaInst->getValueName() << "\n";
 	  }
	  if(LoadInst *loadInst = dyn_cast<LoadInst>(inst)){
            LOG(L_DEBUG)<<"+LOAD INST:" << "=" << loadInst->getValueName() << loadInst->getName().str() << "\n";
	  }
	  if(BinaryOperator *bOp = dyn_cast<BinaryOperator>(inst)){
            LOG(L_DEBUG)<<"BIN_OP:" << bOp->getOperand(0) << bOp->getOpcode() << bOp->getOperand(1) << "\n";
 	  }
        }
        basicBlocks.pop_front();
      }
*/
      return false;
    }
  };
}

char Taint::Taint::ID = 0;
static RegisterPass<Taint::Taint> X("taint", "taint llvm instructions");
