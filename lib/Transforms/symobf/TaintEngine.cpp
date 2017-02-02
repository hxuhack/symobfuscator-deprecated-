#include "TaintEngine.hpp"

using namespace llvm;

namespace Taint{

  void TaintEngine::SetSource(Value *val) {
    taintSourceVec.push_back(val);
  }

  void TaintEngine::Propagate(){

  }
}
