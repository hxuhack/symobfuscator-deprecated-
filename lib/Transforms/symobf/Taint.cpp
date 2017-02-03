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
using namespace taint;


//STATISTIC(TaintedInst, "Number of tainted llvm instructions");
loglevel_e loglevel = L2_DEBUG;

const std::string default_symvar = "symvar";

static cl::opt<std::string> symvar("symvar", cl::desc("choose a symbolic variable to obfuscate"), cl::value_desc("variable name"), cl::init(default_symvar), cl::Optional);

namespace {

  class TaintEngine : public InstVisitor<TaintEngine>{

    private:
    SmallVector<Value*, 32> taintSourceVec;
    SmallPtrSet<BasicBlock*, 8> bbSet;

    public:
    void SetSource(Value *val) {
      taintSourceVec.push_back(val);
    }

    void Propagate(){
      while(!taintSourceVec.empty()){
        Value *val = taintSourceVec.pop_back_val();
        LOG(L_DEBUG) << val->getName().str();
        for (Value::use_iterator useIt = val->use_begin(); useIt != val->use_end(); ++useIt){
	  if(Instruction *inst = dyn_cast<Instruction>(*useIt)){
            LOG(L_DEBUG) << inst->getOpcodeName();
	    OperandChangedState(inst);
          }
        }
      }
    }
    void OperandChangedState(Instruction *inst) {
      if (bbSet.count(inst->getParent())){   // Instruction is executable?
        LOG(L_DEBUG) << "bbSet";
        //visit(*inst);
      }
    }
  };
}

namespace {

  struct TaintPass : public FunctionPass {
    static char ID; 
    TaintPass() : FunctionPass(ID) {}

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

char TaintPass::ID = 0;
static RegisterPass<TaintPass> X("taint", "taint llvm instructions");
