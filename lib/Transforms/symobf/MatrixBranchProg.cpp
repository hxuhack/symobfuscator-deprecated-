#include"MatrixBranchProg.hpp"

using namespace llvm;
using namespace std;


class MatrixInIR{

private:
  int width = 0;
  int height = 0;
public:
  ConstantInt*** conIntMatrix;
//TODO: Impelemt array structure in IR.
/*
  %mat = alloca [2 x [2 x i32]], align 16
  %arrayidx = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat, i64 0, i64 0
  %arrayidx1 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx, i64 0, i64 0
  store i32 0, i32* %arrayidx1, align 16
  %arrayidx2 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat, i64 0, i64 0
  %arrayidx3 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx2, i64 0, i64 1
  store i32 1, i32* %arrayidx3, align 4
  %arrayidx4 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat, i64 0, i64 1
  %arrayidx5 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx4, i64 0, i64 0
  store i32 0, i32* %arrayidx5, align 8
  %arrayidx6 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat, i64 0, i64 1
  %arrayidx7 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayidx6, i64 0, i64 1
  store i32 1, i32* %arrayidx7, align 4
  ret void
*/

  MatrixInIR(ICmpInst* icmpInst, const int width, const int height, int idx_row, int idx_col, Type* type):width(width),height(height){

	conIntMatrix = (ConstantInt***) malloc (sizeof(ConstantInt**)*height);

	//Construct an allocaInst:
	//AllocaInst (Type *Ty, const Twine &Name="", Instruction *InsertBefore=nullptr)
	Type* i32Type = IntegerType::getInt32Ty(icmpInst->getContext());
	Type* i64Type = IntegerType::getInt64Ty(icmpInst->getContext());
	ArrayType* l2ArrayType = ArrayType::get(i64Type, height);
	ArrayType* l1ArrayType = ArrayType::get(l2ArrayType, width);

	AllocaInst* allocaInst = new AllocaInst(l1ArrayType,"", icmpInst);


	for(int i=0; i<height; i++){
	  conIntMatrix[i] = (ConstantInt**) malloc (sizeof(ConstantInt*) * width);

	  //Construct an GetElementPtrInst;
	  //Create (Type *PointeeType, Value *Ptr, ArrayRef< Value * > IdxList, const Twine &NameStr="", Instruction *InsertBefore=nullptr) 
	  //http://llvm.org/docs/GetElementPtr.html

      ConstantInt* idx0 = (ConstantInt*) ConstantInt::getSigned(i64Type,0);
      ConstantInt* idx1 = (ConstantInt*) ConstantInt::getSigned(i64Type,i);
	  vector<Value*> idxVec;
	  idxVec.push_back(idx0);
	  idxVec.push_back(idx1);
	  ArrayRef<Value*> idxArrayRef(idxVec);
	  GetElementPtrInst* getEPInst = GetElementPtrInst::CreateInBounds(l1ArrayType, (Value*) allocaInst, idxArrayRef,"", icmpInst);

	  for(int j=0; j<width; j++){
        ConstantInt* idx2 = (ConstantInt*) ConstantInt::getSigned(i64Type,j);
	    vector<Value*> l2IdxVec;
	    l2IdxVec.push_back(idx0);
	    l2IdxVec.push_back(idx2);
	    ArrayRef<Value*> idxArrayRef(l2IdxVec);
	    GetElementPtrInst* l2GetPEInst = GetElementPtrInst::CreateInBounds(l2ArrayType, (Value*) getEPInst, idxArrayRef,"", icmpInst);
		ConstantInt* tmpConstInt;
		if((i==j) || (i==idx_row && j == idx_col)){
          tmpConstInt = (ConstantInt*) ConstantInt::getSigned(i64Type,1);
		}else{
          tmpConstInt = (ConstantInt*) ConstantInt::getSigned(i64Type,0);
		}
		conIntMatrix[i][j] = tmpConstInt;
		StoreInst* storeInst = new StoreInst((Value *) tmpConstInt, (Value *) l2GetPEInst, icmpInst);
	  }
	}
  }

  MatrixInIR(ICmpInst* icmpInst, const int width, const int height):width(width),height(height){
	Type* i64Type = IntegerType::getInt64Ty(icmpInst->getContext());
	ArrayType* l2ArrayType = ArrayType::get(i64Type, height);
	ArrayType* l1ArrayType = ArrayType::get(l2ArrayType, width);

	AllocaInst* allocaInst = new AllocaInst(l1ArrayType,"", icmpInst);


	for(int i=0; i<height; i++){
	  conIntMatrix[i] = (ConstantInt**) malloc (sizeof(ConstantInt*) * width);

      ConstantInt* idx0 = (ConstantInt*) ConstantInt::getSigned(i64Type,0);
      ConstantInt* idx1 = (ConstantInt*) ConstantInt::getSigned(i64Type,i);
	  vector<Value*> idxVec;
	  idxVec.push_back(idx0);
	  idxVec.push_back(idx1);
	  ArrayRef<Value*> idxArrayRef(idxVec);
	  GetElementPtrInst* getEPInst = GetElementPtrInst::CreateInBounds(l1ArrayType, (Value*) allocaInst, idxArrayRef,"", icmpInst);

	  for(int j=0; j<width; j++){
        ConstantInt* idx2 = (ConstantInt*) ConstantInt::getSigned(i64Type,j);
	    vector<Value*> l2IdxVec;
	    l2IdxVec.push_back(idx0);
	    l2IdxVec.push_back(idx2);
	    ArrayRef<Value*> idxArrayRef(l2IdxVec);
	    GetElementPtrInst* l2GetPEInst = GetElementPtrInst::CreateInBounds(l2ArrayType, (Value*) getEPInst, idxArrayRef,"", icmpInst);
		ConstantInt* tmpConstInt = (ConstantInt*) ConstantInt::getSigned(i64Type,0);
		conIntMatrix[i][j] = tmpConstInt;
		StoreInst* storeInst = new StoreInst((Value *) tmpConstInt, (Value *) l2GetPEInst, icmpInst);
	  }
	}
  }

  MatrixInIR(Value* matValue, BasicBlock* bb){
	Type* i64Type = IntegerType::getInt64Ty(bb->getContext());
	ArrayType* l2ArrayType = ArrayType::get(i64Type, height);
	ArrayType* l1ArrayType = ArrayType::get(l2ArrayType, width);

	AllocaInst* allocaInst = new AllocaInst(l1ArrayType, "" , bb);
  }

  //Constant has to be loaded
  void CreateLoadInst(){
  }

  int GetWidth(){
	return width;
  }

  int GetHeight(){
	return height;
  }

  void Randomize(){
    //TODO: to be implement
  }
  void GradEncode(){
    //TODO: to be implement
  }

  ~MatrixInIR(){
	for(int i=0; i<height; i++){
	  free(conIntMatrix[i]);
	}
	free(conIntMatrix);
  }

};

class MBP{

private:
  MatrixInIR* headMat;
  MatrixInIR* tailMat;
  list<MatrixInIR*> matListInp0;
  list<MatrixInIR*> matListInp1;

  void GenerateMatrix(ICmpInst *icmpInst){
	LOG(L2_DEBUG) << "ConvertIcmp2Mbp...";
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
		MatrixInIR* mat1 = new MatrixInIR(icmpInst,dim,dim,idx_row,idx_col1,boolType); //bit
		MatrixInIR* mat0 = new MatrixInIR(icmpInst,dim,dim,idx_row,idx_col2,boolType); //~bit
		matListInp0.push_back(mat0);
		matListInp1.push_back(mat1);
	}
	headMat = new MatrixInIR(icmpInst,1,dim,0,0,boolType); //bit
	tailMat = new MatrixInIR(icmpInst,dim,1,dim-1,0,boolType); //~bit
  }

  void GenerateMMLogic(){
	int numMatrix = matListInp0.size(); 
    for(int i=0; i<numMatrix; i++){
      //Create a CallInst
    }
  }

public:
  MBP(ICmpInst *icmpInst){
    GenerateMatrix(icmpInst);
    GenerateMMLogic();//Matrix multiplication logic
  }
};

Function* GenMatMulFunc(LLVMContext& context, Module& module){
  LOG(L2_DEBUG) << "GenMatMulFunc...";
  //We construct a type of 2-level int array as the type for the matrix. 
  Type* i64Type = IntegerType::getInt64Ty(context);
  ArrayType* l2ArrayType = ArrayType::get(i64Type, 2);
  ArrayType* l1ArrayType = ArrayType::get(l2ArrayType, 2);
  PointerType* l1Ptr = PointerType::getUnqual(l1ArrayType);

  //We declare the parameters of the function
  vector<Type*> paramVec;
  paramVec.push_back((Type *) l1Ptr);
  paramVec.push_back((Type *) l1Ptr);
  ArrayRef<Type*> paramArrayType(paramVec);

  //We wrap the type of the function
  //Params: (Type *Result, ArrayRef< Type * > Params, bool isVarArg)
  FunctionType* funcType = FunctionType::get((Type *) l1Ptr,paramArrayType,false);

  //We define the function name as "MatrixMult"
  const Twine* funcName = new Twine("MatrixMult");
  //Params: (FunctionType *Ty, LinkageTypes Linkage, const Twine &N="", Module *M=nullptr)
  Function* func = Function::Create(funcType, GlobalValue::ExternalLinkage, *funcName, &module);

  BasicBlock* bb = BasicBlock::Create(context, "entry", func);
  IRBuilder<> builder(bb);

  AllocaInst* allocaInst = new AllocaInst(l1ArrayType,"", bb);

  //Get the two matrix from arguments
  MatrixInIR* mat1;
  MatrixInIR* mat2;
  int argNumChecker = 0;
  for (Function::arg_iterator arg = func->arg_begin(); arg!=func->arg_end(); ++arg){
    ++argNumChecker;
	if(argNumChecker == 1){
	  *mat1 = MatrixInIR((Value*) &*arg, bb);
	}
	if(argNumChecker == 2){
	  *mat2 = MatrixInIR((Value*) &*arg, bb);
	}
	if(argNumChecker>2){
	  //We return an empty matrix;
	  //TODO:Throw an alert;
      ReturnInst* retInst = ReturnInst::Create(context, allocaInst, bb);
	}
	//TODO:Get the argument
  }
  //TODO: Muliply the arg

  //TODO:
  ReturnInst* retInst = ReturnInst::Create(context, allocaInst, bb);

  return func;
}

void ConvertIcmp2Mbp(ICmpInst *icmpInst){
  LOG(L2_DEBUG) << "ConvertIcmp2Mbp...";
  MBP* mbp = new MBP(icmpInst);
}
