/*
Author: Hui Xu @ CUHK
Feature: Taint LLVM IR instructions

The TaintEngine if referenced with llvm SCCP.cpp
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
#include "TaintEngine.hpp"
#include "Logger.hpp"
#include <list> 

using namespace llvm;
using namespace taint;


//STATISTIC(TaintedInst, "Number of tainted llvm instructions");
loglevel_e loglevel = L2_DEBUG;

//const std::string default_symvar = "argv";
//static cl::opt<std::string> symvar("symvar", cl::desc("choose a symbolic variable to obfuscate"), cl::value_desc("variable name"), cl::init(default_symvar), cl::Optional);

namespace {

  class TaintEngine : public InstVisitor<TaintEngine>{

    SmallVector<Value*, 32> taintSourceVec;
    SmallPtrSet<BasicBlock*, 8> bbSet;
    SmallVector<Value*, 64> instWorkVec;

  public:
    void SetSource(Value*);
    void Propagate();
    void VisitInst(Instruction*);
  private:
    friend class InstVisitor<TaintEngine>;
/*
    // visit implementations - Something changed in this instruction.  Either an
    // operand made a transition, or the instruction is newly executable.  Change
    // the value type of I to reflect these changes if appropriate.
    void visitPHINode(PHINode &I);

    // Terminators
    void visitReturnInst(ReturnInst &I);
    void visitTerminatorInst(TerminatorInst &TI);

    void visitCastInst(CastInst &I);
    void visitSelectInst(SelectInst &I);
    void visitBinaryOperator(Instruction &I);
    void visitCmpInst(CmpInst &I);
    void visitExtractElementInst(ExtractElementInst &I);
    void visitInsertElementInst(InsertElementInst &I);
    void visitShuffleVectorInst(ShuffleVectorInst &I);
    void visitExtractValueInst(ExtractValueInst &EVI);
    void visitInsertValueInst(InsertValueInst &IVI);
    void visitLandingPadInst(LandingPadInst &I) { markAnythingOverdefined(&I); }
    void visitFuncletPadInst(FuncletPadInst &FPI) { markAnythingOverdefined(&FPI);}
    void visitCatchSwitchInst(CatchSwitchInst &CPI) {
      //markAnythingOverdefined(&CPI);
      visitTerminatorInst(CPI);
    }
    // Instructions that cannot be folded away.
    void visitStoreInst     (StoreInst &I);
    void visitLoadInst      (LoadInst &I);
    void visitGetElementPtrInst(GetElementPtrInst &I);
    void visitCallInst      (CallInst &I) {visitCallSite(&I);}
    void visitInvokeInst    (InvokeInst &II) {
      visitCallSite(&II);
      visitTerminatorInst(II);
    }
    void visitCallSite      (CallSite CS);
    void visitResumeInst    (TerminatorInst &I) { //returns void }
    void visitUnreachableInst(TerminatorInst &I) { //returns void }
    void visitFenceInst     (FenceInst &I) { //returns void }
    void visitAtomicCmpXchgInst(AtomicCmpXchgInst &I) {markAnythingOverdefined(&I);}
    void visitAtomicRMWInst (AtomicRMWInst &I) { markOverdefined(&I); }
    void visitAllocaInst    (Instruction &I) { markOverdefined(&I); }
    void visitVAArgInst     (Instruction &I) { markAnythingOverdefined(&I); }
*/
    void visitInstruction(Instruction &I) {
      // If a new instruction is added to LLVM that we don't handle.
      LOG(L_WARNING) << "TaintEngine: Don't know how to handle:";
      I.print(errs());
     // markAnythingOverdefined(&I);   // Just in case
    }
  };
//End TaintEngine Definition

  void TaintEngine::SetSource(Value *val) {
    taintSourceVec.push_back(val);
  }

  void TaintEngine::Propagate(){
    LOG(L_DEBUG) << "Entering TaintEngine::Propagate...";
    while(!taintSourceVec.empty()){
      Value *val = taintSourceVec.pop_back_val();
      LOG(L_DEBUG) << val->getName().str();
      for (User *taintUser : val->users()){
        if (Instruction *inst = dyn_cast<Instruction>(taintUser)){
          inst->print(errs());
          VisitInst(inst);
        }
      }
/*
        for (Value::use_iterator useIt = val->use_begin(); useIt != val->use_end(); ++useIt){
          LOG(L_DEBUG) << "================USE=========";
	  if(Instruction *inst = dyn_cast<Instruction>(*useIt)){
            LOG(L_DEBUG) << inst->getName().str();
	    OperandChangedState(inst);
          }
        }
*/
    }
  }
  void TaintEngine::VisitInst(Instruction *inst) {
    if (bbSet.count(inst->getParent())){   // Instruction is executable?
      LOG(L_DEBUG) << "bbSet";
      visit(*inst);//Originally defined in native llvm/IR/InstVisitor.h
    }
  }
  //End TaintEngine Implementation
}
//End TaingEngine Namespace

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


      //we print the whole IR for development
      std::list<BasicBlock *> basicBlocks;
      for (Function::iterator i=F.begin();i!=F.end();++i) {
        basicBlocks.push_back((BasicBlock *)i);
      }
      while(!basicBlocks.empty()){
        BasicBlock* basicBlock = basicBlocks.front();
        basicBlock->print(errs());
/*
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
*/
        basicBlocks.pop_front();
      }
      return false;
    }
  };
}

char TaintPass::ID = 0;
static RegisterPass<TaintPass> X("taint", "taint llvm instructions");
