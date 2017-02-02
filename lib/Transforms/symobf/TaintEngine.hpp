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
#include <list> 

using namespace llvm;

namespace Taint{

  class TaintEngine : public InstVisitor<TaintEngine>{

    SmallVector<Value*, 32> taintSourceVec;

    public:
    TaintEngine();

    void SetSource(Value *val);
    void Propagate();

  };
}
