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
#include "Logger.hpp"
#include <list> 

using namespace llvm;

//STATISTIC(TaintedInst, "Number of tainted llvm instructions");
loglevel_e loglevel = L2_DEBUG;

//This is to set parameters passing to opt
//const std::string default_symvar = "argv";
//static cl::opt<std::string> symvar("symvar", cl::desc("choose a symbolic variable to obfuscate"), cl::value_desc("variable name"), cl::init(default_symvar), cl::Optional);

namespace {
/// LatticeVal class - This class represents the different lattice values that an LLVM value may occupy.  It is a simple class with value semantics.
class LatticeVal {
  enum LvType {
    /// unknown - This LLVM Value has no known value yet.
    unknown,

    /// constant - This LLVM Value has a specific constant value.
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
  bool IsConstant() const {
    return GetType() == constant || GetType() == forcedconstant;
  }
  bool IsOverdefined() const { return GetType() == overdefined; }

  Constant *GetConstant() const {
    assert(IsConstant() && "Cannot get the constant of a non-constant!");
    return pipVal.getPointer();
  }

  /// MarkOverdefined - Return true if this is a change in status.
  bool MarkOverdefined() {
    if (IsOverdefined())
      return false;

    pipVal.setInt(overdefined);
    return true;
  }

  /// markConstant - Return true if this is a change in status.
  bool MarkConstant(Constant *conVal) {
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
  ConstantInt *GetConstantInt() const {
    if (IsConstant())
      return dyn_cast<ConstantInt>(GetConstant());
    return nullptr;
  }

  void MarkForcedConstant(Constant *V) {
    assert(IsUnknown() && "Can't force a defined value!");
    pipVal.setInt(forcedconstant);
    pipVal.setPointer(V);
  }
};
} // end anonymous namespace.

namespace {

class TaintEngine : public InstVisitor<TaintEngine>{

  //const DataLayout &dataLayout;
  //const TargetLibraryInfo *targetLib;

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

  DenseMap<Value*, LatticeVal> valueState; 
  DenseMap<std::pair<Value*, unsigned>, LatticeVal> structValueState;

  typedef std::pair<BasicBlock*, BasicBlock*> Edge;
  DenseSet<Edge> knownFeasibleEdges;

  SmallPtrSet<Function*, 16> mrvFunctionsTracked;

public:
  //TaintEngine(const DataLayout &dataLayout, const TargetLibraryInfo *targetLib): dataLayout(dataLayout), targetLib(targetLib) {}
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
  void MarkAnythingOverdefined(Value *V) {
    if (StructType *STy = dyn_cast<StructType>(V->getType()))
      for (unsigned i = 0, e = STy->getNumElements(); i != e; ++i)
        MarkOverdefined(GetStructValueState(V, i), V);
    else
      MarkOverdefined(V);
  }


private:
  friend class InstVisitor<TaintEngine>;
  void visitStoreInst     (StoreInst &I);
  void visitLoadInst      (LoadInst &I);
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
  void visitLandingPadInst(LandingPadInst &I) { MarkAnythingOverdefined(&I); }
  void visitFuncletPadInst(FuncletPadInst &FPI) { MarkAnythingOverdefined(&FPI);}
  void visitCatchSwitchInst(CatchSwitchInst &CPI) {
      MarkAnythingOverdefined(&CPI);
      visitTerminatorInst(CPI);
  }
    // Instructions that cannot be folded away.
  void visitGetElementPtrInst(GetElementPtrInst &I);
  void visitCallInst      (CallInst &I) {visitCallSite(&I);}
  void visitInvokeInst    (InvokeInst &II) {
    visitCallSite(&II);
    visitTerminatorInst(II);
  }
  void visitCallSite      (CallSite CS);
  void visitResumeInst    (TerminatorInst &I) {  }
  void visitUnreachableInst(TerminatorInst &I) { }
  void visitFenceInst     (FenceInst &I) { }
  void visitAtomicCmpXchgInst(AtomicCmpXchgInst &I) {MarkAnythingOverdefined(&I);}
  void visitAtomicRMWInst (AtomicRMWInst &I) { MarkOverdefined(&I); }
  void visitAllocaInst    (Instruction &I) { MarkOverdefined(&I); }
  void visitVAArgInst     (Instruction &I) { MarkAnythingOverdefined(&I); }
  void visitInstruction(Instruction &inst) {
    // If a new instruction is added to LLVM that we don't handle.
    LOG(L_WARNING) << "TaintEngine: Don't know how to handle:";
    inst.print(errs());
    MarkAnythingOverdefined(&inst);   // Just in case
  }

  LatticeVal &GetValueState(Value *val) {
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

  LatticeVal &GetStructValueState(Value *V, unsigned i) {
    assert(V->getType()->isStructTy() && "Should use GetValueState");
    assert(i < cast<StructType>(V->getType())->getNumElements() &&
           "Invalid element #");

    std::pair<DenseMap<std::pair<Value*, unsigned>, LatticeVal>::iterator,
              bool> I = structValueState.insert(
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

  void MarkOverdefined(LatticeVal &IV, Value *V) {
    if (!IV.MarkOverdefined()) return;
    // Only instructions go on the work list
    taintSourceVec.push_back(V);
  }

  void MarkForcedConstant(Value *V, Constant *C) {
    assert(!V->getType()->isStructTy() && "Should use other method");
    LatticeVal &IV = valueState[V];
    IV.MarkForcedConstant(C);
    PushToWorkList(IV, V);
  }

  void MarkEdgeExecutable(BasicBlock *Source, BasicBlock *Dest) {
    if (!knownFeasibleEdges.insert(Edge(Source, Dest)).second)
      return;  // This edge is already known to be executable!

    if (!MarkBlockExecutable(Dest)) {
      // If the destination is already executable, we just made an *edge*
      // feasible that wasn't before.  Revisit the PHI nodes in the block
      // because they have potentially new operands.
      PHINode *PN;
      for (BasicBlock::iterator I = Dest->begin(); (PN = dyn_cast<PHINode>(I)); ++I)
        visitPHINode(*PN);
    }
  }

  void PushToWorkList(LatticeVal &IV, Value *V) {
    if (IV.IsOverdefined())
      return taintSourceVec.push_back(V);
    instWorkVec.push_back(V);
  }

  void MarkConstant(LatticeVal &IV, Value *V, Constant *C) {
    if (!IV.MarkConstant(C)) return;
    PushToWorkList(IV, V);
  }

  void MarkConstant(Value *V, Constant *C) {
    assert(!V->getType()->isStructTy() && "Should use other method");
    MarkConstant(valueState[V], V, C);
  }

  void MergeInValue(LatticeVal &IV, Value *V, LatticeVal MergeWithV) {
    if (IV.IsOverdefined() || MergeWithV.IsUnknown())
      return;  // Noop.
    if (MergeWithV.IsOverdefined())
      return MarkOverdefined(IV, V);
    if (IV.IsUnknown())
      return MarkConstant(IV, V, MergeWithV.GetConstant());
    if (IV.GetConstant() != MergeWithV.GetConstant())
      return MarkOverdefined(IV, V);
  }

  void MergeInValue(Value *V, LatticeVal MergeWithV) {
    assert(!V->getType()->isStructTy() && "Should use other method");
    MergeInValue(valueState[V], V, MergeWithV);
  }
};
//End TaintEngine Definition

bool TaintEngine::MarkBlockExecutable(BasicBlock *bb) {
    if (!bbExe.insert(bb).second) 
      return false;
    LOG(L_DEBUG) << "Marking Block Executable: " << bb->getName().str();
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
  LOG(L_DEBUG) << "Entering TaintEngine::Propagate...";
  while(!taintSourceVec.empty()||!instWorkVec.empty()||!bbWorkVec.empty()){
    while(!taintSourceVec.empty()){
      Value *val = taintSourceVec.pop_back_val();
      LOG(L_DEBUG) << val->getName().str();
      for (User *taintUser : val->users()){
        if (Instruction *inst = dyn_cast<Instruction>(taintUser)){
          inst->print(errs()); errs()<<"\n";
          VisitInst(inst);
        }
      }
    }
    while (!instWorkVec.empty()) {
      Value *val = instWorkVec.pop_back_val();
      LOG(L_DEBUG) <<val->getName().str();

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
      BasicBlock *bb = bbWorkVec.back();
      bbWorkVec.pop_back();
      bb->print(errs());

      // Notify all instructions in this basic block that they are newly
      // executable.
      visit(bb);
    }
  }
}

void TaintEngine::GetFeasibleSuccessors(TerminatorInst &TI,
                                       SmallVectorImpl<bool> &Succs) {
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
  LOG(L_DEBUG) << "Entering VisitInst...";
  if (bbExe.count(inst->getParent())){   // Instruction is executable?
    visit(*inst);//Originally defined in native llvm/IR/InstVisitor.h
  }
}

void TaintEngine::visitStoreInst(StoreInst &inst) {
  LOG(L_DEBUG) << "Entering TaintEngine::visitStoreInst...";
  LOG(L_DEBUG) << "Operand(0):" << inst.getOperand(0) <<", Operand(1):" << inst.getOperand(1);
  // If this store is of a struct, ignore it. Why?
  if (inst.getOperand(0)->getType()->isStructTy())
    return;

  if (trackedGlobals.empty() || !isa<GlobalVariable>(inst.getOperand(1)))
    return;

  GlobalVariable *globalVar = cast<GlobalVariable>(inst.getOperand(1));
  DenseMap<GlobalVariable*, LatticeVal>::iterator itGV = trackedGlobals.find(globalVar);
  if (itGV == trackedGlobals.end() || itGV->second.IsOverdefined()) 
    return;
  LOG(L_DEBUG) << "---------------";
  // Get the value we are storing into the global, then merge it.
  //MergeInValue(itGV->second, globalVar, GetValueState(inst.getOperand(0)));
  if (itGV->second.IsOverdefined())
    trackedGlobals.erase(itGV);      // No need to keep tracking this!
}

void TaintEngine::visitPHINode(PHINode &PN) {
  // If this PN returns a struct, just mark the result overdefined.
  // TODO: We could do a lot better than this if code actually uses this.
  if (PN.getType()->isStructTy())
    return MarkAnythingOverdefined(&PN);

  if (GetValueState(&PN).IsOverdefined())
    return;  // Quick exit

  // Super-extra-high-degree PHI nodes are unlikely to ever be marked constant,
  // and slow us down a lot.  Just mark them overdefined.
  if (PN.getNumIncomingValues() > 64)
    return MarkOverdefined(&PN);

  // Look at all of the executable operands of the PHI node.  If any of them
  // are overdefined, the PHI becomes overdefined as well.  If they are all
  // constant, and they agree with each other, the PHI becomes the identical
  // constant.  If they are constant and don't agree, the PHI is overdefined.
  // If there are no executable operands, the PHI remains unknown.
  //
  Constant *OperandVal = nullptr;
  for (unsigned i = 0, e = PN.getNumIncomingValues(); i != e; ++i) {
    LatticeVal IV = GetValueState(PN.getIncomingValue(i));
    if (IV.IsUnknown()) continue;  // Doesn't influence PHI node.

    if (!IsEdgeFeasible(PN.getIncomingBlock(i), PN.getParent()))
      continue;

    if (IV.IsOverdefined())    // PHI node becomes overdefined!
      return MarkOverdefined(&PN);

    if (!OperandVal) {   // Grab the first value.
      OperandVal = IV.GetConstant();
      continue;
    }

    // There is already a reachable operand.  If we conflict with it,
    // then the PHI node becomes overdefined.  If we agree with it, we
    // can continue on.

    // Check to see if there are two different constants merging, if so, the PHI
    // node is overdefined.
    if (IV.GetConstant() != OperandVal)
      return MarkOverdefined(&PN);
  }

  // If we exited the loop, this means that the PHI node only has constant
  // arguments that agree with each other(and OperandVal is the constant) or
  // OperandVal is null because there are no defined incoming arguments.  If
  // this is the case, the PHI remains unknown.
  //
  if (OperandVal)
    MarkConstant(&PN, OperandVal);      // Acquire operand value
}

void TaintEngine::visitReturnInst(ReturnInst &I) {
  if (I.getNumOperands() == 0) return;  // ret void

  Function *F = I.getParent()->getParent();
  Value *ResultOp = I.getOperand(0);

  // If we are tracking the return value of this function, merge it in.
  if (!trackedRetVals.empty() && !ResultOp->getType()->isStructTy()) {
    DenseMap<Function*, LatticeVal>::iterator TFRVI =
      trackedRetVals.find(F);
    if (TFRVI != trackedRetVals.end()) {
      MergeInValue(TFRVI->second, F, GetValueState(ResultOp));
      return;
    }
  }

  // Handle functions that return multiple values.
  if (!trackedMultipleRetVals.empty()) {
    if (StructType *STy = dyn_cast<StructType>(ResultOp->getType()))
      if (mrvFunctionsTracked.count(F))
        for (unsigned i = 0, e = STy->getNumElements(); i != e; ++i)
          MergeInValue(trackedMultipleRetVals[std::make_pair(F, i)], F,
                       GetStructValueState(ResultOp, i));

  }
}

void TaintEngine::visitTerminatorInst(TerminatorInst &TI) {
  SmallVector<bool, 16> SuccFeasible;
  GetFeasibleSuccessors(TI, SuccFeasible);

  BasicBlock *BB = TI.getParent();

  // Mark all feasible successors executable.
  for (unsigned i = 0, e = SuccFeasible.size(); i != e; ++i)
    if (SuccFeasible[i])
      MarkEdgeExecutable(BB, TI.getSuccessor(i));
}

void TaintEngine::visitCastInst(CastInst &I) {
  LatticeVal OpSt = GetValueState(I.getOperand(0));
/*
  if (OpSt.IsOverdefined())          // Inherit overdefinedness of operand
    MarkOverdefined(&I);
  else if (OpSt.IsConstant()) {
    // Fold the constant as we build.
    Constant *C = ConstantFoldCastOperand(I.getOpcode(), OpSt.GetConstant(), I.getType(), DL);
    if (isa<UndefValue>(C))
      return;
    // Propagate constant value
    MarkConstant(&I, C);
  }
*/
}


void TaintEngine::visitExtractValueInst(ExtractValueInst &EVI) {
  // If this returns a struct, mark all elements over defined, we don't track
  // structs in structs.
  if (EVI.getType()->isStructTy())
    return MarkAnythingOverdefined(&EVI);

  // If this is extracting from more than one level of struct, we don't know.
  if (EVI.getNumIndices() != 1)
    return MarkOverdefined(&EVI);

  Value *AggVal = EVI.getAggregateOperand();
  if (AggVal->getType()->isStructTy()) {
    unsigned i = *EVI.idx_begin();
    LatticeVal EltVal = GetStructValueState(AggVal, i);
    MergeInValue(GetValueState(&EVI), &EVI, EltVal);
  } else {
    // Otherwise, must be extracting from an array.
    return MarkOverdefined(&EVI);
  }
}

void TaintEngine::visitInsertValueInst(InsertValueInst &IVI) {
  StructType *STy = dyn_cast<StructType>(IVI.getType());
  if (!STy)
    return MarkOverdefined(&IVI);

  // If this has more than one index, we can't handle it, drive all results to
  // undef.
  if (IVI.getNumIndices() != 1)
    return MarkAnythingOverdefined(&IVI);

  Value *Aggr = IVI.getAggregateOperand();
  unsigned Idx = *IVI.idx_begin();

  // Compute the result based on what we're inserting.
  for (unsigned i = 0, e = STy->getNumElements(); i != e; ++i) {
    // This passes through all values that aren't the inserted element.
    if (i != Idx) {
      LatticeVal EltVal = GetStructValueState(Aggr, i);
      MergeInValue(GetStructValueState(&IVI, i), &IVI, EltVal);
      continue;
    }

    Value *Val = IVI.getInsertedValueOperand();
    if (Val->getType()->isStructTy())
      // We don't track structs in structs.
      MarkOverdefined(GetStructValueState(&IVI, i), &IVI);
    else {
      LatticeVal InVal = GetValueState(Val);
      MergeInValue(GetStructValueState(&IVI, i), &IVI, InVal);
    }
  }
}

void TaintEngine::visitSelectInst(SelectInst &I) {
  // If this select returns a struct, just mark the result overdefined.
  // TODO: We could do a lot better than this if code actually uses this.
  if (I.getType()->isStructTy())
    return MarkAnythingOverdefined(&I);

  LatticeVal CondValue = GetValueState(I.getCondition());
  if (CondValue.IsUnknown())
    return;

  if (ConstantInt *CondCB = CondValue.GetConstantInt()) {
    Value *OpVal = CondCB->isZero() ? I.getFalseValue() : I.getTrueValue();
    MergeInValue(&I, GetValueState(OpVal));
    return;
  }

  // Otherwise, the condition is overdefined or a constant we can't evaluate.
  // See if we can produce something better than overdefined based on the T/F
  // value.
  LatticeVal TVal = GetValueState(I.getTrueValue());
  LatticeVal FVal = GetValueState(I.getFalseValue());

  // select ?, C, C -> C.
  if (TVal.IsConstant() && FVal.IsConstant() &&
      TVal.GetConstant() == FVal.GetConstant())
    return MarkConstant(&I, FVal.GetConstant());

  if (TVal.IsUnknown())   // select ?, undef, X -> X.
    return MergeInValue(&I, FVal);
  if (FVal.IsUnknown())   // select ?, X, undef -> X.
    return MergeInValue(&I, TVal);
  MarkOverdefined(&I);
}

// Handle Binary Operators.
void TaintEngine::visitBinaryOperator(Instruction &I) {
  LatticeVal V1State = GetValueState(I.getOperand(0));
  LatticeVal V2State = GetValueState(I.getOperand(1));

  LatticeVal &IV = valueState[&I];
  if (IV.IsOverdefined()) return;

  if (V1State.IsConstant() && V2State.IsConstant()) {
    Constant *C = ConstantExpr::get(I.getOpcode(), V1State.GetConstant(),
                                    V2State.GetConstant());
    // X op Y -> undef.
    if (isa<UndefValue>(C))
      return;
    return MarkConstant(IV, &I, C);
  }

  // If something is undef, wait for it to resolve.
  if (!V1State.IsOverdefined() && !V2State.IsOverdefined())
    return;

  // Otherwise, one of our operands is overdefined.  Try to produce something
  // better than overdefined with some tricks.

  // If this is an AND or OR with 0 or -1, it doesn't matter that the other
  // operand is overdefined.
  if (I.getOpcode() == Instruction::And || I.getOpcode() == Instruction::Or) {
    LatticeVal *NonOverdefVal = nullptr;
    if (!V1State.IsOverdefined())
      NonOverdefVal = &V1State;
    else if (!V2State.IsOverdefined())
      NonOverdefVal = &V2State;

    if (NonOverdefVal) {
      if (NonOverdefVal->IsUnknown()) {
        // Could annihilate value.
        if (I.getOpcode() == Instruction::And)
          MarkConstant(IV, &I, Constant::getNullValue(I.getType()));
        else if (VectorType *PT = dyn_cast<VectorType>(I.getType()))
          MarkConstant(IV, &I, Constant::getAllOnesValue(PT));
        else
          MarkConstant(IV, &I,
                       Constant::getAllOnesValue(I.getType()));
        return;
      }

      if (I.getOpcode() == Instruction::And) {
        // X and 0 = 0
        if (NonOverdefVal->GetConstant()->isNullValue())
          return MarkConstant(IV, &I, NonOverdefVal->GetConstant());
      } else {
        if (ConstantInt *CI = NonOverdefVal->GetConstantInt())
          if (CI->isAllOnesValue())     // X or -1 = -1
            return MarkConstant(IV, &I, NonOverdefVal->GetConstant());
      }
    }
  }


  MarkOverdefined(&I);
}

// Handle ICmpInst instruction.
void TaintEngine::visitCmpInst(CmpInst &I) {
  LatticeVal V1State = GetValueState(I.getOperand(0));
  LatticeVal V2State = GetValueState(I.getOperand(1));

  LatticeVal &IV = valueState[&I];
  if (IV.IsOverdefined()) return;

  if (V1State.IsConstant() && V2State.IsConstant()) {
    Constant *C = ConstantExpr::getCompare(
        I.getPredicate(), V1State.GetConstant(), V2State.GetConstant());
    if (isa<UndefValue>(C))
      return;
    return MarkConstant(IV, &I, C);
  }

  // If operands are still unknown, wait for it to resolve.
  if (!V1State.IsOverdefined() && !V2State.IsOverdefined())
    return;

  MarkOverdefined(&I);
}

void TaintEngine::visitExtractElementInst(ExtractElementInst &I) {
  // TODO : SCCP does not handle vectors properly.
  return MarkOverdefined(&I);
}

void TaintEngine::visitInsertElementInst(InsertElementInst &I) {
  // TODO : SCCP does not handle vectors properly.
  return MarkOverdefined(&I);
}

void TaintEngine::visitShuffleVectorInst(ShuffleVectorInst &I) {
  // TODO : SCCP does not handle vectors properly.
  return MarkOverdefined(&I);
}

// Handle getelementptr instructions.  If all operands are constants then we
// can turn this into a getelementptr ConstantExpr.
//
void TaintEngine::visitGetElementPtrInst(GetElementPtrInst &I) {
  if (valueState[&I].IsOverdefined()) return;

  SmallVector<Constant*, 8> Operands;
  Operands.reserve(I.getNumOperands());

  for (unsigned i = 0, e = I.getNumOperands(); i != e; ++i) {
    LatticeVal State = GetValueState(I.getOperand(i));
    if (State.IsUnknown())
      return;  // Operands are not resolved yet.

    if (State.IsOverdefined())
      return MarkOverdefined(&I);

    assert(State.IsConstant() && "Unknown state!");
    Operands.push_back(State.GetConstant());
  }

  Constant *Ptr = Operands[0];
  auto Indices = makeArrayRef(Operands.begin() + 1, Operands.end());
  Constant *C =
      ConstantExpr::getGetElementPtr(I.getSourceElementType(), Ptr, Indices);
  if (isa<UndefValue>(C))
      return;
  MarkConstant(&I, C);
}


// Handle load instructions.  If the operand is a constant pointer to a constant
// global, we can replace the load with the loaded constant value!
void TaintEngine::visitLoadInst(LoadInst &I) {
  // If this load is of a struct, just mark the result overdefined.
/*
  if (I.getType()->isStructTy())
    return MarkAnythingOverdefined(&I);

  LatticeVal PtrVal = GetValueState(I.getOperand(0));
  if (PtrVal.IsUnknown()) return;   // The pointer is not resolved yet!

  LatticeVal &IV = valueState[&I];
  if (IV.IsOverdefined()) return;

  if (!PtrVal.IsConstant() || I.isVolatile())
    return MarkOverdefined(IV, &I);

  Constant *Ptr = PtrVal.GetConstant();

  // load null is undefined.
  if (isa<ConstantPointerNull>(Ptr) && I.getPointerAddressSpace() == 0)
    return;

  // Transform load (constant global) into the value loaded.
  if (GlobalVariable *GV = dyn_cast<GlobalVariable>(Ptr)) {
    if (!trackedGlobals.empty()) {
      // If we are tracking this global, merge in the known value for it.
      DenseMap<GlobalVariable*, LatticeVal>::iterator It =
        trackedGlobals.find(GV);
      if (It != trackedGlobals.end()) {
        MergeInValue(IV, &I, It->second);
        return;
      }
    }
  }

  // Transform load from a constant into a constant if possible.
  if (Constant *C = ConstantFoldLoadFromConstPtr(Ptr, I.getType(), DL)) {
    if (isa<UndefValue>(C))
      return;
    return MarkConstant(IV, &I, C);
  }

  // Otherwise we cannot say for certain what value this load will produce.
  // Bail out.
  MarkOverdefined(IV, &I);
*/
}

void TaintEngine::visitCallSite(CallSite CS) {
  Function *F = CS.getCalledFunction();
  Instruction *I = CS.getInstruction();
/*
  // The common case is that we aren't tracking the callee, either because we
  // are not doing interprocedural analysis or the callee is indirect, or is
  // external.  Handle these cases first.
  if (!F || F->isDeclaration()) {
CallOverdefined:
    // Void return and not tracking callee, just bail.
    if (I->getType()->isVoidTy()) return;

    // Otherwise, if we have a single return value case, and if the function is
    // a declaration, maybe we can constant fold it.
    if (F && F->isDeclaration() && !I->getType()->isStructTy() &&
        canConstantFoldCallTo(F)) {

      SmallVector<Constant*, 8> Operands;
      for (CallSite::arg_iterator AI = CS.arg_begin(), E = CS.arg_end();
           AI != E; ++AI) {
        LatticeVal State = GetValueState(*AI);

        if (State.IsUnknown())
          return;  // Operands are not resolved yet.
        if (State.IsOverdefined())
          return MarkOverdefined(I);
        assert(State.IsConstant() && "Unknown state!");
        Operands.push_back(State.GetConstant());
      }

      if (GetValueState(I).IsOverdefined())
        return;

      // If we can constant fold this, mark the result of the call as a
      // constant.
      if (Constant *C = ConstantFoldCall(F, Operands, TLI)) {
        // call -> undef.
        if (isa<UndefValue>(C))
          return;
        return MarkConstant(I, C);
      }
    }

    // Otherwise, we don't know anything about this call, Mark it overdefined.
    return MarkAnythingOverdefined(I);
  }

  // If this is a local function that doesn't have its address taken, mark its
  // entry block executable and merge in the actual arguments to the call into
  // the formal arguments of the function.
  if (!TrackingIncomingArguments.empty() && TrackingIncomingArguments.count(F)){
    MarkBlockExecutable(&F->front());

    // Propagate information from this call site into the callee.
    CallSite::arg_iterator CAI = CS.arg_begin();
    for (Function::arg_iterator AI = F->arg_begin(), E = F->arg_end();
         AI != E; ++AI, ++CAI) {
      // If this argument is byval, and if the function is not readonly, there
      // will be an implicit copy formed of the input aggregate.
      if (AI->hasByValAttr() && !F->onlyReadsMemory()) {
        MarkOverdefined(&*AI);
        continue;
      }

      if (StructType *STy = dyn_cast<StructType>(AI->getType())) {
        for (unsigned i = 0, e = STy->getNumElements(); i != e; ++i) {
          LatticeVal CallArg = GetStructValueState(*CAI, i);
          MergeInValue(GetStructValueState(&*AI, i), &*AI, CallArg);
        }
      } else {
        MergeInValue(&*AI, GetValueState(*CAI));
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
      MergeInValue(GetStructValueState(I, i), I,
                   trackedMultipleRetVals[std::make_pair(F, i)]);
  } else {
    DenseMap<Function*, LatticeVal>::iterator TFRVI = trackedRetVals.find(F);
    if (TFRVI == trackedRetVals.end())
      goto CallOverdefined;  // Not tracking this callee.

    // If so, propagate the return value of the callee into this call result.
    MergeInValue(I, TFRVI->second);
  }
*/
}
bool TaintEngine::ResolvedUndefsIn(Function &F) {
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
//End TaingEngine Namespace

namespace {

struct TaintPass : public FunctionPass {
  static char ID; 
  TaintPass() : FunctionPass(ID) {}

  //Main function
  virtual bool runOnFunction(Function &F){
    LOG(L_DEBUG) << "Function: " << F.getName().str();
    //const DataLayout &dataLayout = F.getParent()->getDataLayout();
    //const TargetLibraryInfo *targetLib = &getAnalysis<TargetLibraryInfoWrapperPass>().getTLI();
    //TaintEngine taintEngine(dataLayout,targetLib);
    TaintEngine taintEngine;

    taintEngine.MarkBlockExecutable(&F.front());
    // Set taint source: all arguments
    for (Function::arg_iterator argIt = F.arg_begin(); argIt != F.arg_end(); ++argIt){
      Value *argValue = &*argIt;
      LOG(L_DEBUG) << "Argument: " << argValue->getName().str();
      taintEngine.SetSource(argValue);
    }
    
    bool resolvedUndefs = true;
    while (resolvedUndefs){
      taintEngine.Propagate();
      resolvedUndefs = taintEngine.ResolvedUndefsIn(F);
    }

    //=====================================
    //we print the whole IR for development
    //=====================================
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
};
}

char TaintPass::ID = 0;
static RegisterPass<TaintPass> X("taint", "taint llvm instructions");

