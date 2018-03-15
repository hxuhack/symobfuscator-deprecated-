#ifndef __UTILS_OBF__
#define __UTILS_OBF__

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/Local.h" // For DemoteRegToStack and DemotePHIToStack
#include <stdio.h>
#include <vector>
#include <list>

using namespace llvm;
using namespace std;

void fixStack(Function *f);
std::string readAnnotate(Function *f);
bool toObfuscate(bool flag, Function *f, std::string attribute);

extern IntegerType *boolType, *i8Type, *i32Type, *i64Type;
extern Type *floatType, *doubleType;
extern PointerType *i64PT, *l2Ptr_64, *l3Ptr_64, *l4Ptr_64;
extern ConstantInt *ci0_64, *ci1_64, *ci2_64, *ci3_64, *ci4_64, *ci5_64, *ci6_64, *ci7_64, *ci8_64, *ci9_64, *ci10_64, *ci11_64, *bFalse;
extern ConstantInt *ci0_32, *ci1_32, *ci2_32, *ci3_32, *ci4_32, *ci5_32, *ci6_32, *ci7_32, *ci8_32, *ci9_32, *ci10_32, *ci11_32;
extern vector<Value*> vec00_32,vec01_32;
extern vector<Value*> vec00_64,vec01_64;
extern llvm::Constant *printFunc, *mallocFunc, *pipeFunc, *forkFunc, *multMatFunc; 

#endif