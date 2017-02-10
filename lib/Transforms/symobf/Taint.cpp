/*
Author: Hui Xu @ CUHK
Feature: Taint LLVM IR instructions
==========================================================
Format of instructions

LoadInst
%0 = load i8**, i8*** %argv.addr, align 8
(Value*) dest = Opcode (Type*) destType, Operand(0) (Type*) operandType, 

CmpInst
---------
ICmpInst
%cmp = icmp eq i32 %2, 123
(Value*) dest = Opcode  Operand(0), Operand(1)
=========================================================
*/
#include "llvm/Analysis/ConstantFolding.h"
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
#include "llvm/Transforms/Scalar.h"
#include "llvm/InitializePasses.h"
#include "Logger.hpp"
#include "Utils.hpp"
#include "BoolProg.hpp"
#include "BranchProg.hpp"
#include "MatrixBranchProg.hpp"
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
  void visitBranchInst(BranchInst &);
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
  void visitCallInst      (CallInst &);
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
  void MarkTainted(Value *); 
};
//End TaintEngine Definition

void TaintEngine::MarkTainted(Value *val) {

}

void TaintEngine::MarkTainted(Value *val, Instruction *inst) {
  for(list<Instruction*>::iterator it = taintedInstList.begin(); it!=taintedInstList.end(); ++it){
    Instruction* tmpInst = *it;
    if(tmpInst == inst){
      LOG(L_WARNING) << "Instruction already tainted...";
      return ;
    }
  }
  LOG(L_WARNING) << "Taint a new instruction!!!";
  taintedInstList.push_back(inst);
  inst->print(errs()); errs()<<"\n";
  if(val == nullptr){
    return ;
  }
  LOG(L_WARNING) << "Taint a new value:" << val->getName().str(); 
  workList.push_back(val);
  taintedValList.push_back(val);
}

void TaintEngine::visitInvokeInst (InvokeInst &invokeInst) {
  visitCallSite(&invokeInst);
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


void TaintEngine::VisitInst(Instruction *inst) {
  visit(*inst);//Originally defined in native llvm/IR/InstVisitor.h
}

void TaintEngine::visitCatchSwitchInst(CatchSwitchInst &cwInst) {
  LOG(L2_DEBUG)<<"Entering visitCatchSwitchInst.......";
}

void TaintEngine::visitStoreInst(StoreInst &storeInst) {
  //TODO:
  LOG(L2_DEBUG) << "Entering TaintEngine::visitStoreInst...";
  Value* val = storeInst.getOperand(1);
  MarkTainted(val, &storeInst);
}

void TaintEngine::visitLoadInst(LoadInst &loadInst) {
  // TODO : 
  LOG(L2_DEBUG)<<"Entering visitLoadInst.......";
  Value* val = &loadInst;
  MarkTainted(val,&loadInst);
}

void TaintEngine::visitPHINode(PHINode &pNode) {
  // TODO: 
  LOG(L2_DEBUG)<<"Entering visitPHINode.......";
}

void TaintEngine::visitReturnInst(ReturnInst &retInst) {
  LOG(L2_DEBUG)<<"Entering visitReturnInst.......";
  // TODO: 
}

void TaintEngine::visitBranchInst(BranchInst &branchInst) {
  LOG(L2_DEBUG)<<"Entering visitBranchInst.......";
  LOG(L2_DEBUG)<<"BranchInst is not our interest.......";
}

void TaintEngine::visitCastInst(CastInst &castInst) {
  // TODO: 
  LOG(L2_DEBUG)<<"Entering visitCastInst.......";
  Value* val = &castInst;
  MarkTainted(val, &castInst);
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
  Value* val = &bopInst;
  MarkTainted(val, &bopInst);
}

// Handle ICmpInst instruction.
void TaintEngine::visitCmpInst(CmpInst &cmpInst) {
  // TODO: 
  LOG(L2_DEBUG)<<"Entering visitCmpInst.......";
  Value* val = &cmpInst;
  MarkTainted(val, &cmpInst);
}

void TaintEngine::visitExtractElementInst(ExtractElementInst &eeInst) {
  // TODO :
  LOG(L2_DEBUG)<<"Entering visitExtractInst.......";
  Value* val = &eeInst;
  MarkTainted(val, &eeInst);
}

void TaintEngine::visitCallInst (CallInst &callInst) {
  // TODO :
  LOG(L2_DEBUG)<<"Entering visitCallInst.......";
  Value* val = &callInst;
  MarkTainted(val, &callInst);
}

void TaintEngine::visitInsertElementInst(InsertElementInst &ieInst) {
  LOG(L2_DEBUG)<<"Entering visitInsertElementInst.......";
  // TODO : 
}

void TaintEngine::visitShuffleVectorInst(ShuffleVectorInst &svInst) {
  LOG(L2_DEBUG)<<"Entering visitShuffleVectorInst.......";
  // TODO : 
}

void TaintEngine::visitGetElementPtrInst(GetElementPtrInst &gepInst) {
  // TODO : 
  LOG(L2_DEBUG)<<"Entering visitGetElementPtrInst.......";
  Value* val = &gepInst;
  MarkTainted(val, &gepInst);
}


// Handle load instructions.  If the operand is a constant pointer to a constant
// global, we can replace the load with the loaded constant value!

void TaintEngine::visitCallSite(CallSite callSite) {
  LOG(L2_DEBUG)<<"Entering visitCallSite.......";
  Function *F = callSite.getCalledFunction();
  Instruction *inst = callSite.getInstruction();
  // TODO : 
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
        //inst->print(errs()); errs()<<"\n";
        VisitInst(inst);
      }
    }
    workList.pop_front();
  }
}

//end namespace
}


namespace {
struct Symobf : public FunctionPass {
  static char ID; 
  Symobf() : FunctionPass(ID) {} //initializeSymobfPass(*PassRegistry::getPassRegistry());

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
    PrintIR(taintEngine.taintedInstList);

	for(list<Instruction*>::iterator it = taintEngine.taintedInstList.begin(); it!=taintEngine.taintedInstList.end(); ++it){
	  Instruction *inst = *it;
	  ConvertInst2Bool(inst);
	}

	for(list<Instruction*>::iterator it = taintEngine.taintedInstList.begin(); it!=taintEngine.taintedInstList.end(); ++it){
	  Instruction *inst = *it;
	  if(isa<ICmpInst> (*inst)){ //It is already boolean.
	    ConvertIcmp2Mbp((ICmpInst*)inst);
	  }
	}
    //PrintIR(&F);
    return false;
  }

  void getAnalysisUsage (AnalysisUsage& anaUsage) const override{
    anaUsage.setPreservesCFG();
  }
};


}

char Symobf::ID = 0;
//INITIALIZE_PASS(Symobf, "symobf", "An symbolic obfuscation pass", false, false)
static RegisterPass<Symobf> X("symobf", "obfuscate llvm instructions");
