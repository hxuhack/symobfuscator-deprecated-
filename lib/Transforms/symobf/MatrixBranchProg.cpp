#include"MatrixBranchProg.hpp"

using namespace llvm;
using namespace std;

class BPMatrix{
	public:
		BPMatrix(bool bit, int row, int col, int matrixWidth){};
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
	int matrixWidth = len + 1;
	IntegerType* boolType = IntegerType::get(conInt->getContext(),1);
	for(int i=0; i<len; i++){
		bool bit = conInt->getValue()[i];
		int row = i;
		int col1 = i+1;
		int col2 = len;
		BPMatrix* mat1 = new BPMatrix(bit,row,col1,matrixWidth);
		BPMatrix* mat2 = new BPMatrix(~bit,row,col2,matrixWidth);
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
