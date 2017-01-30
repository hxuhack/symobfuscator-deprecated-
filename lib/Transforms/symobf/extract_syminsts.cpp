//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "Extract_SymInsts"

STATISTIC(HelloCounter, "Counts number of functions greeted");

namespace {
  // Hello - The first implementation, without getAnalysisUsage.
  struct extract_syminsts : public BasicBlockPass {
    static char ID; // Pass identification, replacement for typeid
    extract_syminsts() : BasicBlockPass(ID) {}
    virtual bool runOnBasicBlock(BasicBlock &BB) {
      for (BasicBlock::iterator it = BB.begin(); it != BB.end(); ++it){
        errs()<<it->getName() << '\n';
      }
      return false;
    }
  };
}

char extract_syminsts::ID = 0;
static RegisterPass<extract_syminsts> X("extract_syminsts", "extract symbolic instructions");
