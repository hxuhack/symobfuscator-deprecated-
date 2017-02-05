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

//STATISTIC(TaintedInst, "Number of tainted llvm instructions");
loglevel_e loglevel = L1_DEBUG;

//This is to set parameters passing to opt
//const std::string default_symvar = "argv";
//static cl::opt<std::string> symvar("symvar", cl::desc("choose a symbolic variable to obfuscate"), cl::value_desc("variable name"), cl::init(default_symvar), cl::Optional);

namespace {
/// LatticeVal class - This class represents the different lattice values that an LLVM value may occupy.  It is a simple class with value semantics.
class LatticeVal {
  enum LvType {
    unknown,
    constant,
    /// forcedconstant - This LLVM Value was thought to be undef until
    /// ResolvedUndefsIn.  This is treated just like 'constant', but if merged
    /// with another (different) constant, it goes to overdefined, instead of
    /// asserting.
    forcedconstant,
    /// overdefined - This instruction is not known to be constant, and we know
    /// it has a value.
    overdefined
  };

  /// Val: This stores the current lattice value along with the Constant* for
  /// the constant if this is a 'constant' or 'forcedconstant' value.
  //PointerIntPair<pointer, bits, types> is a LLVM native class.
  PointerIntPair<Constant *, 2, LvType> pipVal;

  LvType GetType() const {
    return pipVal.getInt();
  }

public:
  LatticeVal() : pipVal(nullptr, unknown) {}

  bool IsUnknown() const { return GetType() == unknown; }
  bool IsConstant() const { return GetType() == constant || GetType() == forcedconstant; }
  bool IsOverdefined() const { return GetType() == overdefined; }

  Constant *GetConstant() const; 
  /// MarkOverdefined - Return true if this is a change in status.
  bool MarkOverdefined(); 
  /// markConstant - Return true if this is a change in status.
  bool MarkConstant(Constant *); 
  /// GetConstantInt - If this is a constant with a ConstantInt value, return it
  /// otherwise return null.
  ConstantInt *GetConstantInt() const;

  void MarkForcedConstant(Constant *);
};

Constant* LatticeVal::GetConstant() const {
  assert(IsConstant() && "Cannot get the constant of a non-constant!");
  return pipVal.getPointer();
}

/// MarkOverdefined - Return true if this is a change in status.
bool LatticeVal::MarkOverdefined() {
  if (IsOverdefined()){
    return false;
  }
  pipVal.setInt(overdefined);
  return true;
}

  /// markConstant - Return true if this is a change in status.
bool LatticeVal::MarkConstant(Constant *conVal) {
  if (GetType() == constant) { // Constant but not forcedconstant.
    assert(GetConstant() == conVal && "Marking constant with different value");
    return false;
  }

  if (IsUnknown()) {
    pipVal.setInt(constant);
    assert(conVal && "Marking constant with NULL");
    pipVal.setPointer(conVal);
  } else {
    assert(GetType() == forcedconstant &&
           "Cannot move from overdefined to constant!");
    // Stay at forcedconstant if the constant is the same.
    if (conVal == GetConstant()) return false;

    // Otherwise, we go to overdefined.  Assumptions made based on the
    // forced value are possibly wrong.  Assuming this is another constant
    // could expose a contradiction.
    pipVal.setInt(overdefined);
  }
  return true;
}

/// GetConstantInt - If this is a constant with a ConstantInt value, return it
/// otherwise return null.
ConstantInt* LatticeVal::GetConstantInt() const {
  if (IsConstant())
    return dyn_cast<ConstantInt>(GetConstant());
  return nullptr;
}

void LatticeVal::MarkForcedConstant(Constant *V) {
  assert(IsUnknown() && "Can't force a defined value!");
  pipVal.setInt(forcedconstant);
  pipVal.setPointer(V);
}
} // end anonymous namespace.



namespace{
class TaintEngine : public InstVisitor<TaintEngine>{


  const DataLayout &DL;
  const TargetLibraryInfo *TLI;

  SmallPtrSet<BasicBlock*, 8> bbExe;

  SmallVector<Value*, 32> taintSourceVec;
  SmallVector<Value*, 64> instWorkVec;
  SmallVector<BasicBlock*, 64> bbWorkVec;
  /// GlobalValue - If we are tracking any values for the contents of a global
  /// variable, we keep a mapping from the constant accessor to the element of
  /// the global, to the currently known value.  If the value becomes
  /// overdefined, it's entry is simply removed from this map.
  DenseMap<GlobalVariable*, LatticeVal> trackedGlobals;
  DenseMap<Function*, LatticeVal> trackedRetVals;
  DenseMap<std::pair<Function*, unsigned>, LatticeVal> trackedMultipleRetVals;
  SmallPtrSet<Function*, 16> trackingIncomingArguments;

  DenseMap<Value*, LatticeVal> valueState; 
  DenseMap<std::pair<Value*, unsigned>, LatticeVal> structValueState;

  typedef std::pair<BasicBlock*, BasicBlock*> Edge;
  DenseSet<Edge> knownFeasibleEdges;

  SmallPtrSet<Function*, 16> mrvFunctionsTracked;

public:
  //TaintEngine(const DataLayout &dataLayout, const TargetLibraryInfo *targetLib): dataLayout(dataLayout), targetLib(targetLib) {}
  TaintEngine(const DataLayout &DL) : DL(DL) {}
  void SetSource(Value*);
  void Propagate();
  void VisitInst(Instruction*);
  bool MarkBlockExecutable(BasicBlock *);
  void MarkOverdefined(Value *);
  bool ResolvedUndefsIn(Function &);
  bool IsEdgeFeasible(BasicBlock *From, BasicBlock *To);
  void GetFeasibleSuccessors(TerminatorInst &TI, SmallVectorImpl<bool> &Succs);
  /// MarkAnythingOverdefined - Mark the specified value overdefined.  This
  /// works with both scalars and structs.
  void MarkAnythingOverdefined(Value *);


private:
  friend class InstVisitor<TaintEngine>;
  void visitStoreInst     (StoreInst &I);
  void visitLoadInst      (LoadInst &I);
  void visitPHINode(PHINode &I);
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
  void visitLandingPadInst(LandingPadInst &I) { MarkAnythingOverdefined(&I); }
  void visitFuncletPadInst(FuncletPadInst &FPI) { MarkAnythingOverdefined(&FPI);}
  void visitCatchSwitchInst(CatchSwitchInst &);
  void visitGetElementPtrInst(GetElementPtrInst &I);
  void visitCallInst      (CallInst &I) {visitCallSite(&I);}
  void visitInvokeInst    (InvokeInst &);
  void visitCallSite      (CallSite CS);
  void visitResumeInst    (TerminatorInst &I) {  }
  void visitUnreachableInst(TerminatorInst &I) { }
  void visitFenceInst     (FenceInst &I) { }
  void visitAtomicCmpXchgInst(AtomicCmpXchgInst &I) {MarkAnythingOverdefined(&I);}
  void visitAtomicRMWInst (AtomicRMWInst &I) { MarkOverdefined(&I); }
  void visitAllocaInst    (Instruction &I) { MarkOverdefined(&I); }
  void visitVAArgInst     (Instruction &I) { MarkAnythingOverdefined(&I); }
  void visitInstruction(Instruction &); 
  LatticeVal &GetValueState(Value *); 
  LatticeVal &GetStructValueState(Value *, unsigned );
  void MarkOverdefined(LatticeVal &, Value *);
  void MarkForcedConstant(Value *, Constant *);
  void MarkEdgeExecutable(BasicBlock *, BasicBlock *);
  void PushToWorkList(LatticeVal &, Value *); 
  void MarkConstant(LatticeVal &, Value *, Constant *);
  void MarkConstant(Value *, Constant *);
  void MergeInValue(LatticeVal &, Value *, LatticeVal);
  void MergeInValue(Value *, LatticeVal);
};
//End TaintEngine Definition

void TaintEngine::MergeInValue(Value *val, LatticeVal mergeWithV) {
  assert(!val->getType()->isStructTy() && "Should use other method");
  MergeInValue(valueState[val], val, mergeWithV);
}

void TaintEngine::MarkConstant(LatticeVal &latticeVal, Value *val, Constant *con) {
  if (!latticeVal.MarkConstant(con)) 
    return;
  PushToWorkList(latticeVal, val);
}

void TaintEngine::MarkConstant(Value *val, Constant *con) {
  assert(!val->getType()->isStructTy() && "Should use other method");
  MarkConstant(valueState[val], val, con);
}

void TaintEngine::PushToWorkList(LatticeVal &latticeVal, Value *val) {
  if (latticeVal.IsOverdefined())
    return taintSourceVec.push_back(val);
  instWorkVec.push_back(val);
}

void TaintEngine::visitInvokeInst (InvokeInst &invokeInst) {
  visitCallSite(&invokeInst);
  visitTerminatorInst(invokeInst);
}

void TaintEngine::MarkOverdefined(LatticeVal &latticeVal, Value *val) {
  if (!latticeVal.MarkOverdefined()) 
    return;
  // Only instructions go on the work list
  taintSourceVec.push_back(val);
}

void TaintEngine::MarkForcedConstant(Value *val, Constant *con) {
  assert(!val->getType()->isStructTy() && "Should use other method");
  LatticeVal &latticeVal = valueState[val];
  latticeVal.MarkForcedConstant(con);
  PushToWorkList(latticeVal, val);
}

void TaintEngine::MergeInValue(LatticeVal &IV, Value *V, LatticeVal MergeWithV) {
  if (IV.IsOverdefined() || MergeWithV.IsUnknown())
    return;  // Noop.
  if (MergeWithV.IsOverdefined())
    return MarkOverdefined(IV, V);
  if (IV.IsUnknown())
    return MarkConstant(IV, V, MergeWithV.GetConstant());
  if (IV.GetConstant() != MergeWithV.GetConstant())
    return MarkOverdefined(IV, V);
 }


void TaintEngine::MarkAnythingOverdefined(Value *V) {
  if (StructType *STy = dyn_cast<StructType>(V->getType()))
    for (unsigned i = 0, e = STy->getNumElements(); i != e; ++i)
      MarkOverdefined(GetStructValueState(V, i), V);
  else
    MarkOverdefined(V);
}

void TaintEngine::MarkEdgeExecutable(BasicBlock *srcBlock, BasicBlock *dstBlock) {
  if (!knownFeasibleEdges.insert(Edge(srcBlock, dstBlock)).second)
    return;  // This edge is already known to be executable!

  if (!MarkBlockExecutable(dstBlock)) {
    // If the destination is already executable, we just made an *edge*
    // feasible that wasn't before.  Revisit the PHI nodes in the block
    // because they have potentially new operands.
    PHINode *pNode;
    for (BasicBlock::iterator it = dstBlock->begin(); (pNode = dyn_cast<PHINode>(it)); ++it)
      visitPHINode(*pNode);
  }
}

void TaintEngine::visitInstruction(Instruction &inst) {
  // If a new instruction is added to LLVM that we don't handle.
  LOG(L_WARNING) << "!!!!!!!!!!!!!!!!!!!!TaintEngine: Don't know how to handle:";
  inst.print(errs());
  LOG(L_WARNING) << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
  MarkAnythingOverdefined(&inst);   // Just in case
}

LatticeVal& TaintEngine::GetStructValueState(Value *V, unsigned i) {
  assert(V->getType()->isStructTy() && "Should use GetValueState");
  assert(i < cast<StructType>(V->getType())->getNumElements() && "Invalid element #");
  std::pair<DenseMap<std::pair<Value*, unsigned>, LatticeVal>::iterator, bool> I = structValueState.insert(
                        std::make_pair(std::make_pair(V, i), LatticeVal()));
  LatticeVal &LV = I.first->second;

  if (!I.second)
    return LV;  // Common case, already in the map.

  if (Constant *C = dyn_cast<Constant>(V)) {
    Constant *Elt = C->getAggregateElement(i);
    if (!Elt)
      LV.MarkOverdefined();      // Unknown sort of constant.
    else if (isa<UndefValue>(Elt))
      ; // Undef values remain unknown.
    else
      LV.MarkConstant(Elt);      // Constants are constant.
  }

    // All others are underdefined by default.
  return LV;
 }

LatticeVal& TaintEngine::GetValueState(Value *val) {
  assert(!val->getType()->isStructTy() && "Should use GetStructValueState");
  std::pair<DenseMap<Value*, LatticeVal>::iterator, bool> lvPair =valueState.insert(std::make_pair(val, LatticeVal()));
  LatticeVal &lv = lvPair.first->second;
  if (!lvPair.second)
    return lv;  // Common case, already in the map.
  if (Constant *con = dyn_cast<Constant>(val)) {
    // Undef values remain unknown.
    if (!isa<UndefValue>(val)){
      lv.MarkConstant(con);          // Constants are constant
    }
  }
  // All others are underdefined by default.
  return lv;
}

bool TaintEngine::MarkBlockExecutable(BasicBlock *bb) {
    if (!bbExe.insert(bb).second) 
      return false;
    LOG(L2_DEBUG) << "Marking Block Executable: " << bb->getName().str();
    bbWorkVec.push_back(bb);  // Add the block to the work list!
    return true;
}

void TaintEngine::MarkOverdefined(Value *V) {
    assert(!V->getType()->isStructTy() && "Should use other method");
    MarkOverdefined(valueState[V], V);
}

void TaintEngine::SetSource(Value *val) {
  taintSourceVec.push_back(val);
}

void TaintEngine::Propagate(){
  LOG(L2_DEBUG) << "Entering TaintEngine::Propagate...";
  while(!taintSourceVec.empty()||!instWorkVec.empty()||!bbWorkVec.empty()){
    LOG(L1_DEBUG) << "++++++++++++++++ROUND++++++++++++++++++++";
    while(!taintSourceVec.empty()){
      LOG(L2_DEBUG) << "TaintEngine is processing taintSourceVec...";
      Value *val = taintSourceVec.pop_back_val();
      for (User *taintUser : val->users()){
        if (Instruction *inst = dyn_cast<Instruction>(taintUser)){
          LOG(L2_DEBUG) << "================TAINTED INSTRUCTIONS=====================";
          inst->print(errs()); errs()<<"\n";
          LOG(L2_DEBUG) << "=========================================================";
          VisitInst(inst);
        }
      }
    }
    //TODO:
    while (!instWorkVec.empty()) {
      LOG(L2_DEBUG) << "TaintEngine is processing instWorkVec...";
      Value *val = instWorkVec.pop_back_val();
      LOG(L2_DEBUG) <<val->getName().str();

      // "val" got into the work list because it made the transition from undef to
      // constant.
      //
      // Anything on this worklist that is overdefined need not be visited
      // since all of its users will have already been marked as overdefined.
      // Update all of the users of this instruction's value.
      //
      if (val->getType()->isStructTy() || !GetValueState(val).IsOverdefined()){
        for (User *valUser : val->users()){
          if (Instruction *inst = dyn_cast<Instruction>(valUser)){
            VisitInst(inst);
          }
        }
      }
    }

    // Process the basic block work list.
    while (!bbWorkVec.empty()) {
      LOG(L2_DEBUG) << "TaintEngine is processing bbWorkVec...";
      BasicBlock *bb = bbWorkVec.back();
      bbWorkVec.pop_back();
      //bb->print(errs());

      // Notify all instructions in this basic block that they are newly
      // executable.
      visit(bb);
    }
  }
}

void TaintEngine::GetFeasibleSuccessors(TerminatorInst &TI, SmallVectorImpl<bool> &Succs) {
  Succs.resize(TI.getNumSuccessors());
  if (BranchInst *BI = dyn_cast<BranchInst>(&TI)) {
    if (BI->isUnconditional()) {
      Succs[0] = true;
      return;
    }

    LatticeVal BCValue = GetValueState(BI->getCondition());
    ConstantInt *CI = BCValue.GetConstantInt();
    if (!CI) {
      // Overdefined condition variables, and branches on unfoldable constant
      // conditions, mean the branch could go either way.
      if (!BCValue.IsUnknown())
        Succs[0] = Succs[1] = true;
      return;
    }

    // Constant condition variables mean the branch can only go a single way.
    Succs[CI->isZero()] = true;
    return;
  }

  // Unwinding instructions successors are always executable.
  if (TI.isExceptional()) {
    Succs.assign(TI.getNumSuccessors(), true);
    return;
  }

  if (SwitchInst *SI = dyn_cast<SwitchInst>(&TI)) {
    if (!SI->getNumCases()) {
      Succs[0] = true;
      return;
    }
    LatticeVal SCValue = GetValueState(SI->getCondition());
    ConstantInt *CI = SCValue.GetConstantInt();

    if (!CI) {   // Overdefined or unknown condition?
      // All destinations are executable!
      if (!SCValue.IsUnknown())
        Succs.assign(TI.getNumSuccessors(), true);
      return;
    }

    Succs[SI->findCaseValue(CI).getSuccessorIndex()] = true;
    return;
  }

  // TODO: This could be improved if the operand is a [cast of a] BlockAddress.
  if (isa<IndirectBrInst>(&TI)) {
    // Just mark all destinations executable!
    Succs.assign(TI.getNumSuccessors(), true);
    return;
  }

#ifndef NDEBUG
  dbgs() << "Unknown terminator instruction: " << TI << '\n';
#endif
  llvm_unreachable("SCCP: Don't know how to handle this terminator!");
}

bool TaintEngine::IsEdgeFeasible(BasicBlock *From, BasicBlock *To) {
  assert(bbExe.count(To) && "Dest should always be alive!");

  // Make sure the source basic block is executable!!
  if (!bbExe.count(From)) return false;

  // Check to make sure this edge itself is actually feasible now.
  TerminatorInst *TI = From->getTerminator();
  if (BranchInst *BI = dyn_cast<BranchInst>(TI)) {
    if (BI->isUnconditional())
      return true;

    LatticeVal BCValue = GetValueState(BI->getCondition());

    // Overdefined condition variables mean the branch could go either way,
    // undef conditions mean that neither edge is feasible yet.
    ConstantInt *CI = BCValue.GetConstantInt();
    if (!CI)
      return !BCValue.IsUnknown();

    // Constant condition variables mean the branch can only go a single way.
    return BI->getSuccessor(CI->isZero()) == To;
  }

  // Unwinding instructions successors are always executable.
  if (TI->isExceptional())
    return true;

  if (SwitchInst *SI = dyn_cast<SwitchInst>(TI)) {
    if (SI->getNumCases() < 1)
      return true;

    LatticeVal SCValue = GetValueState(SI->getCondition());
    ConstantInt *CI = SCValue.GetConstantInt();

    if (!CI)
      return !SCValue.IsUnknown();

    return SI->findCaseValue(CI).getCaseSuccessor() == To;
  }

  // Just mark all destinations executable!
  // TODO: This could be improved if the operand is a [cast of a] BlockAddress.
  if (isa<IndirectBrInst>(TI))
    return true;

#ifndef NDEBUG
  dbgs() << "Unknown terminator instruction: " << *TI << '\n';
#endif
  llvm_unreachable("SCCP: Don't know how to handle this terminator!");
}

void TaintEngine::VisitInst(Instruction *inst) {
  LOG(L2_DEBUG) << "Entering VisitInst...";
  if (bbExe.count(inst->getParent())){   // Instruction is executable?
    visit(*inst);//Originally defined in native llvm/IR/InstVisitor.h
  }
}

void TaintEngine::visitCatchSwitchInst(CatchSwitchInst &cwInst) {
  LOG(L2_DEBUG)<<"Entering visitCatchSwitchInst.......";
  MarkAnythingOverdefined(&cwInst);
  visitTerminatorInst(cwInst);
}

void TaintEngine::visitStoreInst(StoreInst &storeInst) {
  LOG(L2_DEBUG) << "Entering TaintEngine::visitStoreInst...";
  // If this store is of a struct, ignore it. Why?
  if (storeInst.getOperand(0)->getType()->isStructTy()){
    LOG(L2_DEBUG) << "isStructTy(), return";
    return;
  }

  if (trackedGlobals.empty() || !isa<GlobalVariable>(storeInst.getOperand(1))){
    
    LOG(L2_DEBUG) << "trackedGlobals.empty() || !isa<GlobalVariable>(storeInst.getOperand(1)), return";
    return;
  }

  GlobalVariable *globalVar = cast<GlobalVariable>(storeInst.getOperand(1));
  DenseMap<GlobalVariable*, LatticeVal>::iterator itGV = trackedGlobals.find(globalVar);
  if (itGV == trackedGlobals.end() || itGV->second.IsOverdefined()) 
    return;
  // Get the value we are storing into the global, then merge it.
  MergeInValue(itGV->second, globalVar, GetValueState(storeInst.getOperand(0)));
  if (itGV->second.IsOverdefined())
    trackedGlobals.erase(itGV);      // No need to keep tracking this!
}

void TaintEngine::visitPHINode(PHINode &pNode) {
  LOG(L2_DEBUG)<<"Entering visitPHINode.......";
  // If this PN returns a struct, just mark the result overdefined.
  // TODO: We could do a lot better than this if code actually uses this.
  if (pNode.getType()->isStructTy())
    return MarkAnythingOverdefined(&pNode);

  if (GetValueState(&pNode).IsOverdefined())
    return;  // Quick exit

  // Super-extra-high-degree PHI nodes are unlikely to ever be marked constant,
  // and slow us down a lot.  Just mark them overdefined.
  if (pNode.getNumIncomingValues() > 64)
    return MarkOverdefined(&pNode);

  // Look at all of the executable operands of the PHI node.  If any of them
  // are overdefined, the PHI becomes overdefined as well.  If they are all
  // constant, and they agree with each other, the PHI becomes the identical
  // constant.  If they are constant and don't agree, the PHI is overdefined.
  // If there are no executable operands, the PHI remains unknown.
  //
  Constant *conOperandVal = nullptr;
  for (unsigned i = 0, e = pNode.getNumIncomingValues(); i != e; ++i) {
    LatticeVal latticeVal = GetValueState(pNode.getIncomingValue(i));
    if (latticeVal.IsUnknown()) continue;  // Doesn't influence PHI node.

    if (!IsEdgeFeasible(pNode.getIncomingBlock(i), pNode.getParent()))
      continue;

    if (latticeVal.IsOverdefined())    // PHI node becomes overdefined!
      return MarkOverdefined(&pNode);

    if (!conOperandVal) {   // Grab the first value.
      conOperandVal = latticeVal.GetConstant();
      continue;
    }

    // There is already a reachable operand.  If we conflict with it,
    // then the PHI node becomes overdefined.  If we agree with it, we
    // can continue on.

    // Check to see if there are two different constants merging, if so, the PHI
    // node is overdefined.
    if (latticeVal.GetConstant() != conOperandVal)
      return MarkOverdefined(&pNode);
  }

  // If we exited the loop, this means that the PHI node only has constant
  // arguments that agree with each other(and OperandVal is the constant) or
  // OperandVal is null because there are no defined incoming arguments.  If
  // this is the case, the PHI remains unknown.
  //
  if (conOperandVal)
    MarkConstant(&pNode, conOperandVal);      // Acquire operand value
}

void TaintEngine::visitReturnInst(ReturnInst &retInst) {
  LOG(L2_DEBUG)<<"Entering visitReturnInst.......";
  if (retInst.getNumOperands() == 0){
     LOG(L2_DEBUG)<<"retInst.getNumOperands() = 0, return";
     return;  // ret void
  }

  Function *F = retInst.getParent()->getParent();
  Value *retVal = retInst.getOperand(0);

  // If we are tracking the return value of this function, merge it in.
  if (!trackedRetVals.empty() && !retVal->getType()->isStructTy()) {
    DenseMap<Function*, LatticeVal>::iterator it =
      trackedRetVals.find(F);
    if (it != trackedRetVals.end()) {
      MergeInValue(it->second, F, GetValueState(retVal));
      return;
    }
  }

  // Handle functions that return multiple values.
  if (!trackedMultipleRetVals.empty()) {
    if (StructType *strType = dyn_cast<StructType>(retVal->getType()))
      if (mrvFunctionsTracked.count(F))
        for (unsigned i = 0, e = strType->getNumElements(); i != e; ++i)
          MergeInValue(trackedMultipleRetVals[std::make_pair(F, i)], F,
                       GetStructValueState(retVal, i));

  }
}

void TaintEngine::visitTerminatorInst(TerminatorInst &termInst) {
  LOG(L2_DEBUG)<<"Entering visitTerminatorInst.......";
  SmallVector<bool, 16> SuccFeasible;
  GetFeasibleSuccessors(termInst, SuccFeasible);

  BasicBlock *BB = termInst.getParent();

  // Mark all feasible successors executable.
  for (unsigned i = 0, e = SuccFeasible.size(); i != e; ++i)
    if (SuccFeasible[i])
      MarkEdgeExecutable(BB, termInst.getSuccessor(i));
}

void TaintEngine::visitCastInst(CastInst &castInst) {
  LOG(L2_DEBUG)<<"Entering visitCastInst.......";
  LatticeVal latticeVal = GetValueState(castInst.getOperand(0));
  if (latticeVal.IsOverdefined())          // Inherit overdefinedness of operand
    MarkOverdefined(&castInst);
  else if (latticeVal.IsConstant()) {
    // Fold the constant as we build.
    Constant *C = ConstantFoldCastOperand(castInst.getOpcode(), latticeVal.GetConstant(), castInst.getType(), DL);
    if (isa<UndefValue>(C))
      return;
    // Propagate constant value
    MarkConstant(&castInst, C);
  }
}


void TaintEngine::visitExtractValueInst(ExtractValueInst &evInst) {
  LOG(L2_DEBUG)<<"Entering visitExtractValueInst.......";
  // If this returns a struct, mark all elements over defined, we don't track
  // structs in structs.
  if (evInst.getType()->isStructTy())
    return MarkAnythingOverdefined(&evInst);

  // If this is extracting from more than one level of struct, we don't know.
  if (evInst.getNumIndices() != 1)
    return MarkOverdefined(&evInst);

  Value *val = evInst.getAggregateOperand();
  if (val->getType()->isStructTy()) {
    unsigned i = *evInst.idx_begin();
    LatticeVal latticeVal = GetStructValueState(val, i);
    MergeInValue(GetValueState(&evInst), &evInst, latticeVal);
  } else {
    // Otherwise, must be extracting from an array.
    return MarkOverdefined(&evInst);
  }
}

void TaintEngine::visitInsertValueInst(InsertValueInst &ivInst) {
  LOG(L2_DEBUG)<<"Entering visitInsertValueInst.......";
  StructType *STy = dyn_cast<StructType>(ivInst.getType());
  if (!STy)
    return MarkOverdefined(&ivInst);

  // If this has more than one index, we can't handle it, drive all results to
  // undef.
  if (ivInst.getNumIndices() != 1)
    return MarkAnythingOverdefined(&ivInst);

  Value *val = ivInst.getAggregateOperand();
  unsigned idx = *ivInst.idx_begin();

  // Compute the result based on what we're inserting.
  for (unsigned i = 0, e = STy->getNumElements(); i != e; ++i) {
    // This passes through all values that aren't the inserted element.
    if (i != idx) {
      LatticeVal latticeVal = GetStructValueState(val, i);
      MergeInValue(GetStructValueState(&ivInst, i), &ivInst, latticeVal);
      continue;
    }

    Value *val2 = ivInst.getInsertedValueOperand();
    if (val2->getType()->isStructTy())
      // We don't track structs in structs.
      MarkOverdefined(GetStructValueState(&ivInst, i), &ivInst);
    else {
      LatticeVal latticeVal2 = GetValueState(val2);
      MergeInValue(GetStructValueState(&ivInst, i), &ivInst, latticeVal2);
    }
  }
}

void TaintEngine::visitSelectInst(SelectInst &selInst) {
  LOG(L2_DEBUG)<<"Entering visitSelectInst.......";
  // If this select returns a struct, just mark the result overdefined.
  // TODO: We could do a lot better than this if code actually uses this.
  if (selInst.getType()->isStructTy())
    return MarkAnythingOverdefined(&selInst);

  LatticeVal condValue = GetValueState(selInst.getCondition());
  if (condValue.IsUnknown())
    return;

  if (ConstantInt *condCB = condValue.GetConstantInt()) {
    Value *opVal = condCB->isZero() ? selInst.getFalseValue() : selInst.getTrueValue();
    MergeInValue(&selInst, GetValueState(opVal));
    return;
  }

  // Otherwise, the condition is overdefined or a constant we can't evaluate.
  // See if we can produce something better than overdefined based on the T/F
  // value.
  LatticeVal tVal = GetValueState(selInst.getTrueValue());
  LatticeVal fVal = GetValueState(selInst.getFalseValue());

  // select ?, C, C -> C.
  if (tVal.IsConstant() && fVal.IsConstant() &&
      tVal.GetConstant() == fVal.GetConstant())
    return MarkConstant(&selInst, fVal.GetConstant());

  if (tVal.IsUnknown())   // select ?, undef, X -> X.
    return MergeInValue(&selInst, fVal);
  if (tVal.IsUnknown())   // select ?, X, undef -> X.
    return MergeInValue(&selInst, tVal);
  MarkOverdefined(&selInst);
}

// Handle Binary Operators.
void TaintEngine::visitBinaryOperator(Instruction &bopInst) {
  LOG(L2_DEBUG)<<"Entering BinaryOperator.......";
  LatticeVal v1State = GetValueState(bopInst.getOperand(0));
  LatticeVal v2State = GetValueState(bopInst.getOperand(1));

  LatticeVal &latticeVal = valueState[&bopInst];
  if (latticeVal.IsOverdefined()) return;

  if (v1State.IsConstant() && v2State.IsConstant()) {
    Constant *con = ConstantExpr::get(bopInst.getOpcode(), v1State.GetConstant(),
                                    v2State.GetConstant());
    // X op Y -> undef.
    if (isa<UndefValue>(con))
      return;
    return MarkConstant(latticeVal, &bopInst, con);
  }

  // If something is undef, wait for it to resolve.
  if (!v1State.IsOverdefined() && !v2State.IsOverdefined())
    return;

  // Otherwise, one of our operands is overdefined.  Try to produce something
  // better than overdefined with some tricks.

  // If this is an AND or OR with 0 or -1, it doesn't matter that the other
  // operand is overdefined.
  if (bopInst.getOpcode() == Instruction::And || bopInst.getOpcode() == Instruction::Or) {
    LatticeVal *nonOverdefVal = nullptr;
    if (!v1State.IsOverdefined())
      nonOverdefVal = &v1State;
    else if (!v2State.IsOverdefined())
      nonOverdefVal = &v2State;

    if (nonOverdefVal) {
      if (nonOverdefVal->IsUnknown()) {
        // Could annihilate value.
        if (bopInst.getOpcode() == Instruction::And)
          MarkConstant(latticeVal, &bopInst, Constant::getNullValue(bopInst.getType()));
        else if (VectorType *vector = dyn_cast<VectorType>(bopInst.getType()))
          MarkConstant(latticeVal, &bopInst, Constant::getAllOnesValue(vector));
        else
          MarkConstant(latticeVal, &bopInst,
                       Constant::getAllOnesValue(bopInst.getType()));
        return;
      }

      if (bopInst.getOpcode() == Instruction::And) {
        // X and 0 = 0
        if (nonOverdefVal->GetConstant()->isNullValue())
          return MarkConstant(latticeVal, &bopInst, nonOverdefVal->GetConstant());
      } else {
        if (ConstantInt *conInt = nonOverdefVal->GetConstantInt())
          if (conInt->isAllOnesValue())     // X or -1 = -1
            return MarkConstant(latticeVal, &bopInst, nonOverdefVal->GetConstant());
      }
    }
  }


  MarkOverdefined(&bopInst);
}

// Handle ICmpInst instruction.
void TaintEngine::visitCmpInst(CmpInst &cmpInst) {
  LOG(L2_DEBUG)<<"Entering visitCmpInst.......";
  LatticeVal v1State = GetValueState(cmpInst.getOperand(0));
  LatticeVal v2State = GetValueState(cmpInst.getOperand(1));

  LatticeVal &latticeVal = valueState[&cmpInst];
  if (latticeVal.IsOverdefined()) return;

  if (v1State.IsConstant() && v2State.IsConstant()) {
    Constant *con = ConstantExpr::getCompare(cmpInst.getPredicate(), v1State.GetConstant(), v2State.GetConstant());
    if (isa<UndefValue>(con))
      return;
    return MarkConstant(latticeVal, &cmpInst, con);
  }

  // If operands are still unknown, wait for it to resolve.
  if (!v1State.IsOverdefined() && !v2State.IsOverdefined())
    return;

  MarkOverdefined(&cmpInst);
}

void TaintEngine::visitExtractElementInst(ExtractElementInst &eeInst) {
  LOG(L2_DEBUG)<<"Entering visitExtractInst.......";
  // TODO : TaintEngine does not handle vectors properly.
  return MarkOverdefined(&eeInst);
}

void TaintEngine::visitInsertElementInst(InsertElementInst &ieInst) {
  LOG(L2_DEBUG)<<"Entering visitInsertElementInst.......";
  // TODO : TaintEngine does not handle vectors properly.
  return MarkOverdefined(&ieInst);
}

void TaintEngine::visitShuffleVectorInst(ShuffleVectorInst &svInst) {
  LOG(L2_DEBUG)<<"Entering visitShuffleVectorInst.......";
  // TODO : TaintEngine does not handle vectors properly.
  return MarkOverdefined(&svInst);
}

// Handle getelementptr instructions.  If all operands are constants then we
// can turn this into a getelementptr ConstantExpr.
//
void TaintEngine::visitGetElementPtrInst(GetElementPtrInst &gepInst) {
  LOG(L2_DEBUG)<<"Entering visitGetElementPtrInst.......";
  if (valueState[&gepInst].IsOverdefined()) return;

  SmallVector<Constant*, 8> vecOperands;
  vecOperands.reserve(gepInst.getNumOperands());

  for (unsigned i = 0, e = gepInst.getNumOperands(); i != e; ++i) {
    LatticeVal latticeVal = GetValueState(gepInst.getOperand(i));
    if (latticeVal.IsUnknown())
      return;  // Operands are not resolved yet.

    if (latticeVal.IsOverdefined())
      return MarkOverdefined(&gepInst);

    assert(latticeVal.IsConstant() && "Unknown state!");
    vecOperands.push_back(latticeVal.GetConstant());
  }

  Constant *conPtr = vecOperands[0];
  auto indices = makeArrayRef(vecOperands.begin() + 1, vecOperands.end());
  Constant *con =
      ConstantExpr::getGetElementPtr(gepInst.getSourceElementType(), conPtr, indices);
  if (isa<UndefValue>(con))
      return;
  MarkConstant(&gepInst, con);
}


// Handle load instructions.  If the operand is a constant pointer to a constant
// global, we can replace the load with the loaded constant value!
void TaintEngine::visitLoadInst(LoadInst &loadInst) {
  LOG(L2_DEBUG)<<"Entering visitGetLoadInst.......";
  // If this load is of a struct, just mark the result overdefined.
  if (loadInst.getType()->isStructTy())
    return MarkAnythingOverdefined(&loadInst);

  LatticeVal ptrVal = GetValueState(loadInst.getOperand(0));
  if (ptrVal.IsUnknown()) return;   // The pointer is not resolved yet!

  LatticeVal &latticeVal = valueState[&loadInst];
  if (latticeVal.IsOverdefined()) return;

  if (!ptrVal.IsConstant() || loadInst.isVolatile())
    return MarkOverdefined(latticeVal, &loadInst);

  Constant *conPtr = ptrVal.GetConstant();

  // load null is undefined.
  if (isa<ConstantPointerNull>(conPtr) && loadInst.getPointerAddressSpace() == 0)
    return;

  // Transform load (constant global) into the value loaded.
  if (GlobalVariable *globalVal = dyn_cast<GlobalVariable>(conPtr)) {
    if (!trackedGlobals.empty()) {
      // If we are tracking this global, merge in the known value for it.
      DenseMap<GlobalVariable*, LatticeVal>::iterator it =
        trackedGlobals.find(globalVal);
      if (it != trackedGlobals.end()) {
        MergeInValue(latticeVal, &loadInst, it->second);
        return;
      }
    }
  }

  // Transform load from a constant into a constant if possible.
  if (Constant *con = ConstantFoldLoadFromConstPtr(conPtr, loadInst.getType(), DL)) {
    if (isa<UndefValue>(con))
      return;
    return MarkConstant(latticeVal, &loadInst, con);
  }

  // Otherwise we cannot say for certain what value this load will produce.
  // Bail out.
  MarkOverdefined(latticeVal, &loadInst);
}

void TaintEngine::visitCallSite(CallSite callSite) {
  LOG(L2_DEBUG)<<"Entering visitCallSite.......";
  Function *F = callSite.getCalledFunction();
  Instruction *inst = callSite.getInstruction();
  // The common case is that we aren't tracking the callee, either because we
  // are not doing interprocedural analysis or the callee is indirect, or is
  // external.  Handle these cases first.
  if (!F || F->isDeclaration()) {
CallOverdefined:
    // Void return and not tracking callee, just bail.
    if (inst->getType()->isVoidTy()) return;

    // Otherwise, if we have a single return value case, and if the function is
    // a declaration, maybe we can constant fold it.
    if (F && F->isDeclaration() && !inst->getType()->isStructTy() &&
        canConstantFoldCallTo(F)) {

      SmallVector<Constant*, 8> vecOperands;
      for (CallSite::arg_iterator it = callSite.arg_begin(), E = callSite.arg_end();
           it != E; ++it) {
        LatticeVal latticeVal = GetValueState(*it);

        if (latticeVal.IsUnknown())
          return;  // Operands are not resolved yet.
        if (latticeVal.IsOverdefined())
          return MarkOverdefined(inst);
        assert(latticeVal.IsConstant() && "Unknown state!");
        vecOperands.push_back(latticeVal.GetConstant());
      }

      if (GetValueState(inst).IsOverdefined())
        return;

      // If we can constant fold this, mark the result of the call as a
      // constant.
      if (Constant *con = ConstantFoldCall(F, vecOperands, TLI)) {
        // call -> undef.
        if (isa<UndefValue>(con))
          return;
        return MarkConstant(inst, con);
      }
    }

    // Otherwise, we don't know anything about this call, Mark it overdefined.
    return MarkAnythingOverdefined(inst);
  }

  // If this is a local function that doesn't have its address taken, mark its
  // entry block executable and merge in the actual arguments to the call into
  // the formal arguments of the function.
  if (!trackingIncomingArguments.empty() && trackingIncomingArguments.count(F)){
    MarkBlockExecutable(&F->front());

    // Propagate information from this call site into the callee.
    CallSite::arg_iterator csArgIt = callSite.arg_begin();
    for (Function::arg_iterator argIt = F->arg_begin(), E = F->arg_end();
         argIt != E; ++argIt, ++csArgIt) {
      // If this argument is byval, and if the function is not readonly, there
      // will be an implicit copy formed of the input aggregate.
      if (argIt->hasByValAttr() && !F->onlyReadsMemory()) {
        MarkOverdefined(&*argIt);
        continue;
      }

      if (StructType *STy = dyn_cast<StructType>(argIt->getType())) {
        for (unsigned i = 0, e = STy->getNumElements(); i != e; ++i) {
          LatticeVal callArg = GetStructValueState(*csArgIt, i);
          MergeInValue(GetStructValueState(&*argIt, i), &*argIt, callArg);
        }
      } else {
        MergeInValue(&*argIt, GetValueState(*csArgIt));
      }
    }
  }

  // If this is a single/zero retval case, see if we're tracking the function.
  if (StructType *STy = dyn_cast<StructType>(F->getReturnType())) {
    if (!mrvFunctionsTracked.count(F))
      goto CallOverdefined;  // Not tracking this callee.

    // If we are tracking this callee, propagate the result of the function
    // into this call site.
    for (unsigned i = 0, e = STy->getNumElements(); i != e; ++i)
      MergeInValue(GetStructValueState(inst, i), inst,
                   trackedMultipleRetVals[std::make_pair(F, i)]);
  } else {
    DenseMap<Function*, LatticeVal>::iterator TFRVI = trackedRetVals.find(F);
    if (TFRVI == trackedRetVals.end())
      goto CallOverdefined;  // Not tracking this callee.

    // If so, propagate the return value of the callee into this call result.
    MergeInValue(inst, TFRVI->second);
  }
}

bool TaintEngine::ResolvedUndefsIn(Function &F) {
  LOG(L2_DEBUG)<<"Entering ResolvedUndefsIn......";
  for (BasicBlock &BB : F) {
    if (!bbExe.count(&BB))
      continue;

    for (Instruction &I : BB) {
      // Look for instructions which produce undef values.
      if (I.getType()->isVoidTy()) continue;

      if (StructType *STy = dyn_cast<StructType>(I.getType())) {
        // Only a few things that can be structs matter for undef.

        // Tracked calls must never be marked overdefined in ResolvedUndefsIn.
        if (CallSite CS = CallSite(&I))
          if (Function *F = CS.getCalledFunction())
            if (mrvFunctionsTracked.count(F))
              continue;

        // extractvalue and insertvalue don't need to be marked; they are
        // tracked as precisely as their operands.
        if (isa<ExtractValueInst>(I) || isa<InsertValueInst>(I))
          continue;

        // Send the results of everything else to overdefined.  We could be
        // more precise than this but it isn't worth bothering.
        for (unsigned i = 0, e = STy->getNumElements(); i != e; ++i) {
          LatticeVal &LV = GetStructValueState(&I, i);
          if (LV.IsUnknown())
            MarkOverdefined(LV, &I);
        }
        continue;
      }

      LatticeVal &LV = GetValueState(&I);
      if (!LV.IsUnknown()) continue;

      // extractvalue is safe; check here because the argument is a struct.
      if (isa<ExtractValueInst>(I))
        continue;

      // Compute the operand LatticeVals, for convenience below.
      // Anything taking a struct is conservatively assumed to require
      // overdefined markings.
      if (I.getOperand(0)->getType()->isStructTy()) {
        MarkOverdefined(&I);
        return true;
      }
      LatticeVal Op0LV = GetValueState(I.getOperand(0));
      LatticeVal Op1LV;
      if (I.getNumOperands() == 2) {
        if (I.getOperand(1)->getType()->isStructTy()) {
          MarkOverdefined(&I);
          return true;
        }

        Op1LV = GetValueState(I.getOperand(1));
      }
      // If this is an instructions whose result is defined even if the input is
      // not fully defined, propagate the information.
      Type *ITy = I.getType();
      switch (I.getOpcode()) {
      case Instruction::Add:
      case Instruction::Sub:
      case Instruction::Trunc:
      case Instruction::FPTrunc:
      case Instruction::BitCast:
        break; // Any undef -> undef
      case Instruction::FSub:
      case Instruction::FAdd:
      case Instruction::FMul:
      case Instruction::FDiv:
      case Instruction::FRem:
        // Floating-point binary operation: be conservative.
        if (Op0LV.IsUnknown() && Op1LV.IsUnknown())
          MarkForcedConstant(&I, Constant::getNullValue(ITy));
        else
          MarkOverdefined(&I);
        return true;
      case Instruction::ZExt:
      case Instruction::SExt:
      case Instruction::FPToUI:
      case Instruction::FPToSI:
      case Instruction::FPExt:
      case Instruction::PtrToInt:
      case Instruction::IntToPtr:
      case Instruction::SIToFP:
      case Instruction::UIToFP:
        // undef -> 0; some outputs are impossible
        MarkForcedConstant(&I, Constant::getNullValue(ITy));
        return true;
      case Instruction::Mul:
      case Instruction::And:
        // Both operands undef -> undef
        if (Op0LV.IsUnknown() && Op1LV.IsUnknown())
          break;
        // undef * X -> 0.   X could be zero.
        // undef & X -> 0.   X could be zero.
        MarkForcedConstant(&I, Constant::getNullValue(ITy));
        return true;

      case Instruction::Or:
        // Both operands undef -> undef
        if (Op0LV.IsUnknown() && Op1LV.IsUnknown())
          break;
        // undef | X -> -1.   X could be -1.
        MarkForcedConstant(&I, Constant::getAllOnesValue(ITy));
        return true;

      case Instruction::Xor:
        // undef ^ undef -> 0; strictly speaking, this is not strictly
        // necessary, but we try to be nice to people who expect this
        // behavior in simple cases
        if (Op0LV.IsUnknown() && Op1LV.IsUnknown()) {
          MarkForcedConstant(&I, Constant::getNullValue(ITy));
          return true;
        }
        // undef ^ X -> undef
        break;

      case Instruction::SDiv:
      case Instruction::UDiv:
      case Instruction::SRem:
      case Instruction::URem:
        // X / undef -> undef.  No change.
        // X % undef -> undef.  No change.
        if (Op1LV.IsUnknown()) break;

        // X / 0 -> undef.  No change.
        // X % 0 -> undef.  No change.
        if (Op1LV.IsConstant() && Op1LV.GetConstant()->isZeroValue())
          break;

        // undef / X -> 0.   X could be maxint.
        // undef % X -> 0.   X could be 1.
        MarkForcedConstant(&I, Constant::getNullValue(ITy));
        return true;

      case Instruction::AShr:
        // X >>a undef -> undef.
        if (Op1LV.IsUnknown()) break;

        // Shifting by the bitwidth or more is undefined.
        if (Op1LV.IsConstant()) {
          if (auto *ShiftAmt = Op1LV.GetConstantInt())
            if (ShiftAmt->getLimitedValue() >=
                ShiftAmt->getType()->getScalarSizeInBits())
              break;
        }

        // undef >>a X -> all ones
        MarkForcedConstant(&I, Constant::getAllOnesValue(ITy));
        return true;
      case Instruction::LShr:
      case Instruction::Shl:
        // X << undef -> undef.
        // X >> undef -> undef.
        if (Op1LV.IsUnknown()) break;

        // Shifting by the bitwidth or more is undefined.
        if (Op1LV.IsConstant()) {
          if (auto *ShiftAmt = Op1LV.GetConstantInt())
            if (ShiftAmt->getLimitedValue() >=
                ShiftAmt->getType()->getScalarSizeInBits())
              break;
        }

        // undef << X -> 0
        // undef >> X -> 0
        MarkForcedConstant(&I, Constant::getNullValue(ITy));
        return true;
      case Instruction::Select:
        Op1LV = GetValueState(I.getOperand(1));
        // undef ? X : Y  -> X or Y.  There could be commonality between X/Y.
        if (Op0LV.IsUnknown()) {
          if (!Op1LV.IsConstant())  // Pick the constant one if there is any.
            Op1LV = GetValueState(I.getOperand(2));
        } else if (Op1LV.IsUnknown()) {
          // c ? undef : undef -> undef.  No change.
          Op1LV = GetValueState(I.getOperand(2));
          if (Op1LV.IsUnknown())
            break;
          // Otherwise, c ? undef : x -> x.
        } else {
          // Leave Op1LV as Operand(1)'s LatticeValue.
        }

        if (Op1LV.IsConstant())
          MarkForcedConstant(&I, Op1LV.GetConstant());
        else
          MarkOverdefined(&I);
        return true;
      case Instruction::Load:
        // A load here means one of two things: a load of undef from a global,
        // a load from an unknown pointer.  Either way, having it return undef
        // is okay.
        break;
      case Instruction::ICmp:
        // X == undef -> undef.  Other comparisons get more complicated.
        if (cast<ICmpInst>(&I)->isEquality())
          break;
        MarkOverdefined(&I);
        return true;
      case Instruction::Call:
      case Instruction::Invoke: {
        // There are two reasons a call can have an undef result
        // 1. It could be tracked.
        // 2. It could be constant-foldable.
        // Because of the way we solve return values, tracked calls must
        // never be marked overdefined in ResolvedUndefsIn.
        if (Function *F = CallSite(&I).getCalledFunction())
          if (trackedRetVals.count(F))
            break;

        // If the call is constant-foldable, we mark it overdefined because
        // we do not know what return values are valid.
        MarkOverdefined(&I);
        return true;
      }
      default:
        // If we don't know what should happen here, conservatively mark it
        // overdefined.
        MarkOverdefined(&I);
        return true;
      }
    }

    // Check to see if we have a branch or switch on an undefined value.  If so
    // we force the branch to go one way or the other to make the successor
    // values live.  It doesn't really matter which way we force it.
    TerminatorInst *TI = BB.getTerminator();
    if (BranchInst *BI = dyn_cast<BranchInst>(TI)) {
      if (!BI->isConditional()) continue;
      if (!GetValueState(BI->getCondition()).IsUnknown())
        continue;

      // If the input to SCCP is actually branch on undef, fix the undef to
      // false.
      if (isa<UndefValue>(BI->getCondition())) {
        BI->setCondition(ConstantInt::getFalse(BI->getContext()));
        MarkEdgeExecutable(&BB, TI->getSuccessor(1));
        return true;
      }

      // Otherwise, it is a branch on a symbolic value which is currently
      // considered to be undef.  Handle this by forcing the input value to the
      // branch to false.
      MarkForcedConstant(BI->getCondition(),
                         ConstantInt::getFalse(TI->getContext()));
      return true;
    }

    if (SwitchInst *SI = dyn_cast<SwitchInst>(TI)) {
      if (!SI->getNumCases() || !GetValueState(SI->getCondition()).IsUnknown())
        continue;

      // If the input to SCCP is actually switch on undef, fix the undef to
      // the first constant.
      if (isa<UndefValue>(SI->getCondition())) {
        SI->setCondition(SI->case_begin().getCaseValue());
        MarkEdgeExecutable(&BB, SI->case_begin().getCaseSuccessor());
        return true;
      }

      MarkForcedConstant(SI->getCondition(), SI->case_begin().getCaseValue());
      return true;
    }
  }

  return false;
}
//End TaintEngine Implementation
}

namespace{
struct TaintPass : public FunctionPass {
  static char ID; 
  TaintPass() : FunctionPass(ID) {}

  //Main function
  virtual bool runOnFunction(Function &F){
    LOG(L2_DEBUG) << "Entering runOnFunction...Function: " << F.getName().str();
    const DataLayout &dataLayout = F.getParent()->getDataLayout();
    //const TargetLibraryInfo *targetLib = &getAnalysis<TargetLibraryInfoWrapperPass>().getTLI();
    TaintEngine taintEngine(dataLayout);

    taintEngine.MarkBlockExecutable(&F.front());
    // Set taint source: all arguments
    for (Function::arg_iterator argIt = F.arg_begin(); argIt != F.arg_end(); ++argIt){
      Value *argValue = &*argIt;
      LOG(L2_DEBUG) << "Argument: " << argValue->getName().str();
      taintEngine.SetSource(argValue);
    }
    
    bool resolvedUndefs = true;
    int counter = 0;
    while (resolvedUndefs){
      LOG(L1_DEBUG) << "+++++++++++++++++RESOLVE UNDEFS+++++++++++++++++: " << ++counter;
      taintEngine.Propagate();
      resolvedUndefs = taintEngine.ResolvedUndefsIn(F);
    }
    //=====================================
    //we print the whole IR for development
    //=====================================
    if(loglevel >= L1_DEBUG){
      std::list<BasicBlock *> basicBlocks;
      for (Function::iterator i=F.begin();i!=F.end();++i) {
        basicBlocks.push_back((BasicBlock *)i);
      }
      while(!basicBlocks.empty()){
        BasicBlock* basicBlock = basicBlocks.front();
        basicBlock->print(errs());
        basicBlocks.pop_front();
    }
    return false;
    }
  }
};
}

char TaintPass::ID = 0;
static RegisterPass<TaintPass> X("taint", "taint llvm instructions");
