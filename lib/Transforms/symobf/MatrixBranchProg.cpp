#include"MatrixBranchProg.hpp"

using namespace llvm;
using namespace std;

class BPMatrix{
public:

  BPMatrix(int dim, int idx_row, int idx_col, Type* type){
	ConstantInt* conIntMatrix[dim][dim];

	for(int i=0; i<dim; i++){
	  for(int j=0; j<dim; j++){
		if(i==idx_row && j == idx_col){
		  ConstantInt* boolConst = (ConstantInt *) ConstantInt::get(type, 1);
		}
		ConstantInt* boolConst = (ConstantInt *) ConstantInt::get(type, 0);
	  }
	}
  }

  void Randomize(){}
  void GradEncode(){}
};


BasicBlock*	ConvertIcmp2Mbp(ICmpInst *icmpInst){
	LOG(L2_DEBUG) << "ConvertIcmp2Mbp...";
	BasicBlock* bb;
	Type* type = icmpInst->getType();
	Instruction::OtherOps opCode = icmpInst->getOpcode();
	CmpInst::Predicate predicate = icmpInst->getPredicate();
	Value* op0 = icmpInst->getOperand(0);
	Value* op1 = icmpInst->getOperand(1);
    if(!(isa<ConstantInt> (*op0) || isa<ConstantInt>(*op1))){
		LOG(L_INFO) << "[-PI-]:ICmpInst has two symbolic variables; Don't obfuscate!";
	}

	ConstantInt* conInt;
    if(isa<ConstantInt> (*op0))
		conInt = (ConstantInt*)op0;
	else	
		conInt = (ConstantInt*)op1;

	int len = conInt->getBitWidth();
	int dim = len + 1;
	IntegerType* boolType = IntegerType::get(conInt->getContext(),1);
	for(int i=0; i<len; i++){
		bool bit = conInt->getValue()[i];
		int idx_row = i;
		int idx_col1 = i+1;
		int idx_col2 = len;
		BPMatrix* mat1 = new BPMatrix(dim,idx_row,idx_col1,boolType); //bit
		BPMatrix* mat2 = new BPMatrix(dim,idx_row,idx_col2,boolType); //~bit
		//ConstantInt* boolConst = ConstantInt::get(boolType, bit);
	}
/*
	list<ConstantInt*> boolConstIntList = ConvertConstInt2Bool((ConstantInt*)op1);
		while(!boolConstIntList.empty()){
			ConstantInt* conInt = boolConstIntList.front();
			conInt->getValue().print(errs(),false); errs()<<"\n";
			boolConstIntList.pop_front();
		}
	}
	*/
}
