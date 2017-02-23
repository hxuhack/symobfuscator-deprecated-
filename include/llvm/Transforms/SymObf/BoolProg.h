#ifndef _SYMOBF_BOOLPROG_HPP_
#define _SYMOBF_BOOLPROG_HPP_

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
#include "Logger.h"
#include <list>


std::list<llvm::ConstantInt*> ConvertConstInt2Bool(llvm::ConstantInt*);
std::list<llvm::Value*> ConvertVal2Bool(llvm::Value *);
llvm::BasicBlock* ConvertInst2Bool(llvm::Instruction *);

#endif
