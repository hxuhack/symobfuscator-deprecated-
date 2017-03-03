#ifndef _SYMOBF_UTILS_H_
#define _SYMOBF_UTILS_H_

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

void PrintIR(llvm::Function *);
void PrintIR(std::list<llvm::Instruction*>);
void PrintInIR(llvm::Module&, llvm::BasicBlock*, const char*, int, llvm::Value*);

extern llvm::Type *boolType, *i8Type, *i32Type, *i64Type;
extern llvm::Type *doubleType;
extern llvm::PointerType *fpPT, *ptrPT;
extern llvm::ConstantInt *ci0, *ci1;
extern std::vector<llvm::Value*> vec00,vec01;
//Create the blocks
#endif
