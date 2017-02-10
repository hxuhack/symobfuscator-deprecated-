#include "BoolProg.hpp"

using namespace llvm;
using namespace std;

list<ConstantInt*> ConvertConstInt2Bool(ConstantInt* conInt){
  LOG(L2_DEBUG) << "Convert a ContantInt bool";
  list<ConstantInt*> retValList;
  int len = conInt->getBitWidth();
  IntegerType* boolType = IntegerType::get(conInt->getContext(),1);
  for(int i=0; i<len; i++){
    bool bit = conInt->getValue()[i];
	ConstantInt* boolConst = ConstantInt::get(boolType, bit);
	retValList.push_back(boolConst);
  }
  return retValList;
}

//TODO:
list<Value*> ConvertVal2Bool(Value* val){
  list<Value*> retValList;
  Type* type = val->getType();
  //list<Value*> retValList;
  if(type->isIntegerTy()){
    LOG(L2_DEBUG) << "Converting an IntegerType";
	int len = type->getIntegerBitWidth();
	IntegerType* boolType = IntegerType::get(type->getContext(),1);
	if(isa<Constant> (val)){
      LOG(L2_DEBUG) << "The integer is a Constant";
	}
	if(isa<LoadInst> (val)){
      LOG(L2_DEBUG) << "The integer is a LoadInst";
 	  LoadInst* loadInst= (LoadInst*) val; 
	  for(int i=0; i<len; i++){
		//LoadInst* newLoadInst= new LoadInst((Value*) val,"",(Instruction*) loadInst); 
		//newLoadInst->print(errs()); errs()<<"\n";
	  }
	}
  }
  return retValList;
}

BasicBlock* ConvertInst2Bool(Instruction* inst){
  if(isa<CmpInst> (*inst)){
    if(isa<ICmpInst> (*inst)){ //It is already boolean.
	
      LOG(L2_DEBUG) << "Converting a ICmpInst";
	  
	}

	if(isa<FCmpInst> (*inst)){
      LOG(L2_DEBUG) << "Converting a FCmpInst";
	}
  }

  if(isa<LoadInst> (*inst)){
    LOG(L2_DEBUG) << "Converting a LoadInst";
	Value* oriOprand = inst->getOperand(0);
	Type* dstType = inst->getType();
	if (dstType->isIntegerTy()){
	  int len = dstType->getIntegerBitWidth();
      LOG(L2_DEBUG) << "Number of Bits:" << len;
	}
	if (dstType->isPointerTy()){
      LOG(L2_DEBUG) << "Pointer Type:";
	}
	
  }
}

