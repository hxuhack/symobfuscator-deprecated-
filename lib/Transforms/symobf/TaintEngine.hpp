#ifndef _TAINT_ENGINE_HPP_
#define _TAINT_ENGINE_HPP_

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/InstVisitor.h"
#include "Logger.hpp"
#include <list> 

using namespace llvm;

namespace taint{

  class TaintEngine : public InstVisitor<TaintEngine>{

    SmallVector<Value*, 32> taintSourceVec;
    SmallPtrSet<BasicBlock*, 8> bbSet;

    public:
    TaintEngine();

    void SetSource(Value *);
    void Propagate();
 
    private:
    void OperandChangedState(Instruction *);

  };
}

#endif
