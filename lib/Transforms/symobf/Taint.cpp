/*
Author: Hui Xu @ CUHK
Feature: Taint LLVM IR instructions

The TaintEngine if referenced with llvm SCCP.cpp
*/
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Analysis/ConstantFolding.h"
#include "Logger.hpp"
#include <list> 

using namespace llvm;
using namespace std;

//STATISTIC(TaintedInst, "Number of tainted llvm instructions");
loglevel_e loglevel = L3_DEBUG;

//This is to set parameters passing to opt
//const std::string default_symvar = "argv";
//static cl::opt<std::string> symvar("symvar", cl::desc("choose a symbolic variable to obfuscate"), cl::value_desc("variable name"), cl::init(default_symvar), cl::Optional);

namespace{

class TaintEngine : public InstVisitor<TaintEngine>{

  const DataLayout &DL;
  //const TargetLibraryInfo *TLI;

  list<Value*> workList;


public:
  list<Instruction*> taintedInstList;
  list<Value*> taintedValList;
  //TaintEngine(const DataLayout &dataLayout, const TargetLibraryInfo *targetLib): dataLayout(dataLayout), targetLib(targetLib) {}
  TaintEngine(const DataLayout &DL) : DL(DL) {}
  void SetSource(Value*);
  void Propagate();
  void VisitInst(Instruction*);


private:
  friend class InstVisitor<TaintEngine>;
  void visitStoreInst     (StoreInst &);
  void visitLoadInst      (LoadInst &);
  void visitPHINode(PHINode &);
  void visitReturnInst(ReturnInst &);
  void visitTerminatorInst(TerminatorInst &);
  void visitCastInst(CastInst &);
  void visitSelectInst(SelectInst &);
  void visitBinaryOperator(Instruction &);
  void visitCmpInst(CmpInst &);
  void visitExtractElementInst(ExtractElementInst &);
  void visitInsertElementInst(InsertElementInst &);
  void visitShuffleVectorInst(ShuffleVectorInst &);
  void visitExtractValueInst(ExtractValueInst &);
  void visitInsertValueInst(InsertValueInst &);
  void visitLandingPadInst(LandingPadInst &) {  }
  void visitFuncletPadInst(FuncletPadInst &) { }
  void visitCatchSwitchInst(CatchSwitchInst &);
  void visitGetElementPtrInst(GetElementPtrInst &);
  void visitCallInst      (CallInst &callInst) {visitCallSite(&callInst);}
  void visitInvokeInst    (InvokeInst &);
  void visitCallSite      (CallSite);
  void visitResumeInst    (TerminatorInst &) {  }
  void visitUnreachableInst(TerminatorInst &) { }
  void visitFenceInst     (FenceInst &) { }
  void visitAtomicCmpXchgInst(AtomicCmpXchgInst &) { }
  void visitAtomicRMWInst (AtomicRMWInst &) {  }
  void visitAllocaInst    (Instruction &) {  }
  void visitVAArgInst     (Instruction &) { }
  void visitInstruction(Instruction &); 

  void MarkTainted(Value *, Instruction *); 
};
//End TaintEngine Definition

void TaintEngine::MarkTainted(Value *val, Instruction *inst) {
  workList.push_back(val);
  taintedValList.push_back(val);
  taintedInstList.push_back(inst);
}

void TaintEngine::visitInvokeInst (InvokeInst &invokeInst) {
  visitCallSite(&invokeInst);
  visitTerminatorInst(invokeInst);
}

void TaintEngine::visitInstruction(Instruction &inst) {
  // If a new instruction is added to LLVM that we don't handle.
  LOG(L_WARNING) << "!!!!!!!!!!!!!!!!!!!!TaintEngine: Don't know how to handle:";
  inst.print(errs());
  LOG(L_WARNING) << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
}

void TaintEngine::SetSource(Value *val) {
  workList.push_back(val);
  taintedValList.push_back(val);
}

/*Propagate the tainted value
*
*/
void TaintEngine::Propagate(){
  LOG(L2_DEBUG) << "Entering TaintEngine::Propagate...";
  while(!workList.empty()){
    Value *val = workList.front();
    for (User *taintUser : val->users()){
      if (Instruction *inst = dyn_cast<Instruction>(taintUser)){
        inst->print(errs()); errs()<<"\n";
        VisitInst(inst);
      }
    }
    workList.pop_front();
  }
}

void TaintEngine::VisitInst(Instruction *inst) {
  LOG(L2_DEBUG) << "Entering VisitInst...";
  visit(*inst);//Originally defined in native llvm/IR/InstVisitor.h
}

void TaintEngine::visitCatchSwitchInst(CatchSwitchInst &cwInst) {
  LOG(L2_DEBUG)<<"Entering visitCatchSwitchInst.......";
  visitTerminatorInst(cwInst);
}

void TaintEngine::visitStoreInst(StoreInst &storeInst) {
  //TODO:
  LOG(L2_DEBUG) << "Entering TaintEngine::visitStoreInst...";
  // If this store is of a struct, ignore it. Why?
}

void TaintEngine::visitPHINode(PHINode &pNode) {
  // TODO: 
  LOG(L2_DEBUG)<<"Entering visitPHINode.......";
}

void TaintEngine::visitReturnInst(ReturnInst &retInst) {
  LOG(L2_DEBUG)<<"Entering visitReturnInst.......";
  // TODO: 
}

void TaintEngine::visitTerminatorInst(TerminatorInst &termInst) {
  // TODO: 
  LOG(L2_DEBUG)<<"Entering visitTerminatorInst.......";
}

void TaintEngine::visitCastInst(CastInst &castInst) {
  // TODO: 
  LOG(L2_DEBUG)<<"Entering visitCastInst.......";
}


void TaintEngine::visitExtractValueInst(ExtractValueInst &evInst) {
  // TODO: 
  LOG(L2_DEBUG)<<"Entering visitExtractValueInst.......";
}

void TaintEngine::visitInsertValueInst(InsertValueInst &ivInst) {
  // TODO: 
  LOG(L2_DEBUG)<<"Entering visitInsertValueInst.......";
}

void TaintEngine::visitSelectInst(SelectInst &selInst) {
  // TODO: 
  LOG(L2_DEBUG)<<"Entering visitSelectInst.......";
}

// Handle Binary Operators.
void TaintEngine::visitBinaryOperator(Instruction &bopInst) {
  // TODO: 
  LOG(L2_DEBUG)<<"Entering BinaryOperator.......";
}

// Handle ICmpInst instruction.
void TaintEngine::visitCmpInst(CmpInst &cmpInst) {
  // TODO: 
  LOG(L2_DEBUG)<<"Entering visitCmpInst.......";
}

void TaintEngine::visitExtractElementInst(ExtractElementInst &eeInst) {
  LOG(L2_DEBUG)<<"Entering visitExtractInst.......";
  // TODO :
}

void TaintEngine::visitInsertElementInst(InsertElementInst &ieInst) {
  LOG(L2_DEBUG)<<"Entering visitInsertElementInst.......";
  // TODO : 
}

void TaintEngine::visitShuffleVectorInst(ShuffleVectorInst &svInst) {
  LOG(L2_DEBUG)<<"Entering visitShuffleVectorInst.......";
  // TODO : 
}

// Handle getelementptr instructions.  If all operands are constants then we
// can turn this into a getelementptr ConstantExpr.
//
void TaintEngine::visitGetElementPtrInst(GetElementPtrInst &gepInst) {
  // TODO : 
  LOG(L2_DEBUG)<<"Entering visitGetElementPtrInst.......";
}


// Handle load instructions.  If the operand is a constant pointer to a constant
// global, we can replace the load with the loaded constant value!
void TaintEngine::visitLoadInst(LoadInst &loadInst) {
  // TODO : 
  LOG(L2_DEBUG)<<"Entering visitGetLoadInst.......";
  Value* val = loadInst.getOperand(0);
  MarkTainted(val, &loadInst);
}

void TaintEngine::visitCallSite(CallSite callSite) {
  LOG(L2_DEBUG)<<"Entering visitCallSite.......";
  Function *F = callSite.getCalledFunction();
  Instruction *inst = callSite.getInstruction();
  // TODO : 
}
}

namespace{
struct TaintPass : public FunctionPass {
  static char ID; 
  TaintPass() : FunctionPass(ID) {}
  void PrintIR(Function *);
  void PrintTaintedIR(TaintEngine *);
  //Main function
  virtual bool runOnFunction(Function &F){
    LOG(L2_DEBUG) << "Entering runOnFunction...Function: " << F.getName().str();
    const DataLayout &dataLayout = F.getParent()->getDataLayout();
    //const TargetLibraryInfo *targetLib = &getAnalysis<TargetLibraryInfoWrapperPass>().getTLI();
    TaintEngine taintEngine(dataLayout);

    //taintEngine.MarkBlockExecutable(&F.front());
    // Set taint source: all arguments
    for (Function::arg_iterator argIt = F.arg_begin(); argIt != F.arg_end(); ++argIt){
      Value *argValue = &*argIt;
      LOG(L2_DEBUG) << "Argument: " << argValue->getName().str();
      taintEngine.SetSource(argValue);
    }
    
    taintEngine.Propagate();
    
    PrintTaintedIR(&taintEngine);
    PrintIR(&F);
    return false;
  }
};

void TaintPass::PrintTaintedIR(TaintEngine* taintEngine){
  LOG(L2_DEBUG) << "=====================Print Tainted IR==========================";
  for(list<Instruction*>::iterator it = taintEngine->taintedInstList.begin(); it!=taintEngine->taintedInstList.end(); ++it){
    Instruction *inst = *it;
    inst->print(errs());
  }
}

void TaintPass::PrintIR(Function* func){
//we print the whole IR for development
  LOG(L2_DEBUG) << "=====================Print IR==========================";
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
}

char TaintPass::ID = 0;
static RegisterPass<TaintPass> X("taint", "taint llvm instructions");
