#include"MatrixBranchProg.hpp"

using namespace llvm;
using namespace std;

class BPMatrix{

private:
  const int width;
  const int height;
public:
  ConstantInt** conIntMatrix;

  BPMatrix(const int width, const int height):width(width),height(height){
	conIntMatrix = (ConstantInt**) malloc (sizeof(ConstantInt*)*height);
	for(int i=0; i<height; i++){
	  conIntMatrix[i] = (ConstantInt*) malloc (sizeof(ConstantInt)*width);
	}
  }

  BPMatrix(const int width, const int height, int idx_row, int idx_col, Type* type):width(width),height(height){
	conIntMatrix = (ConstantInt**) malloc (sizeof(ConstantInt*)*height);
	for(int i=0; i<height; i++){
	  conIntMatrix[i] = (ConstantInt*) malloc (sizeof(ConstantInt)*width);
	  for(int j=0; j<width; j++){
		if(i==idx_row && j == idx_col){
		  conIntMatrix[i][j].get(type, 1);
		}else{
		  conIntMatrix[i][j].get(type, 0);
		}
	  }
	}
  }

  int GetWidth(){
	return width;
  }

  int GetHeight(){
	return height;
  }

  void Randomize(){}
  void GradEncode(){}

  void Convert2IR(){
  
  }

  ~BPMatrix(){
	for(int i=0; i<height; i++){
	  free(conIntMatrix[i]);
	}
	free(conIntMatrix);
  }

};

//Generate the IR that multiplies two matrix
//We cannot compute get the result directly.
//We should let the IR computes the multiplication.
//Choice: Constants, or With 2-d structure in IR?
BasicBlock* MatrixMultiply(BPMatrix* bpMat1, BPMatrix* bpMat2){
//Step 1: define a new matrix.
//Step 2: load the original matrix, and compute each position 
//Step 3:  load to the new matrix
  BPMatrix* restMat = new BPMatrix(bpMat1->GetHeight(),bpMat2->GetWidth());  
	for(int i=0; i<restMat->GetHeight();i++){
  	  for(int i=0; i<restMat->GetWidth();i++){
	    for(int i=0; i<restMat->GetWidth();i++){
		  //Multiplication Instruction
		}
		//Add Instruction
	  }
	}
}

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
		BPMatrix* mat1 = new BPMatrix(dim,dim,idx_row,idx_col1,boolType); //bit
		BPMatrix* mat2 = new BPMatrix(dim,dim,idx_row,idx_col2,boolType); //~bit
		//TODO: Convert BPMatrix to IR
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
