#include "llvm/Transforms/SymObf/Utils.h"

using namespace llvm;
using namespace std;

void PrintInIR(Module& module, BasicBlock* bb, const char* arg1, int len, Value* arg2){
  LLVMContext& context = bb->getContext();
	
  vector<Value*> vec00;
  vec00.push_back(ci0);
  vec00.push_back(ci0);
  ArrayRef<Value*> ar00(vec00);
  ArrayType* i8AT = ArrayType::get(i8Type, len);
  GlobalVariable* pfmGV = new GlobalVariable(module, i8AT, true, GlobalValue::PrivateLinkage, 0, "");
  pfmGV->setAlignment(1);
  Constant* strPf = ConstantDataArray::getString(context,arg1, true);
  pfmGV->setInitializer(strPf);
  GetElementPtrInst* getPfm = GetElementPtrInst::CreateInBounds(i8AT, pfmGV, ar00,"", bb);

  vector<Value*> vecPrint;
  vecPrint.push_back(getPfm);
  if(arg2 != nullptr)
	vecPrint.push_back(arg2);
  ArrayRef<Value*> arPrint(vecPrint);
  CallInst::Create(printFunc, arPrint, "", bb);
}

void PrintIR(Function* func){
//we print the whole IR for development
  LOG(L2_DEBUG) << "=====================Print Function IR==========================";

  list<BasicBlock *> basicBlocks;
  for (Function::iterator i=func->begin();i!=func->end();++i) {
    basicBlocks.push_back((BasicBlock *)i);
  }
  while(!basicBlocks.empty()){
    BasicBlock* basicBlock = basicBlocks.front();
    basicBlock->print(errs());
    basicBlocks.pop_front();
  }
}

void PrintIR(list<Instruction*> instList){
  LOG(L2_DEBUG) << "=====================Print List IR==========================";
  for(list<Instruction*>::iterator it = instList.begin(); it!=instList.end(); ++it){
    Instruction *inst = *it;
    inst->print(errs()); errs()<<"\n";
  }
}
