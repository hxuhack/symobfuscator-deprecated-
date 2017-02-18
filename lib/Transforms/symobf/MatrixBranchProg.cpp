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

  MatrixInIR(){
  
  }
  

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

  MatrixInIR(Value* matValue, BasicBlock* entryBB){
	Type* i64Type = IntegerType::getInt64Ty(entryBB->getContext());
	/*
	ArrayType* l2ArrayType = ArrayType::get(i64Type, height);
	ArrayType* l1ArrayType = ArrayType::get(l2ArrayType, width);

	AllocaInst* allocaInst = new AllocaInst(l1ArrayType, "" , entryBB);
	*/
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
	/*
	for(int i=0; i<height; i++){
	  free(conIntMatrix[i]);
	}
	if(conIntMatrix != nullptr){
	  free(conIntMatrix);
	}
	*/
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

/*
 *Function Define:
 *i32** @MM(i32** %mat1, i32** %mat2, i32 %m1Height, i32 %m1Width, i32 %m2Height, i32 %m2Width)
*/

Function* GenMatMulFunc(LLVMContext& context, Module& module){
  LOG(L2_DEBUG) << "GenMatMulFunc...";
  //We construct a type of 2-level int array as the type for the matrix. 
  Type* i64Type = IntegerType::getInt64Ty(context);
  Type* i32Type = IntegerType::getInt32Ty(context);

  PointerType* l2PtrType = PointerType::getUnqual(i64Type);
  PointerType* l1PtrType = PointerType::getUnqual(l2PtrType);


  //We declare the parameters of the function
  vector<Type*> paramVec;
  paramVec.push_back((Type *) l1PtrType);
  paramVec.push_back((Type *) l1PtrType);
  paramVec.push_back(i64Type);
  paramVec.push_back(i64Type);
  paramVec.push_back(i64Type);
  paramVec.push_back(i64Type);
  ArrayRef<Type*> paramArrayType(paramVec);


  //We wrap the type of the function
  //Params: (Type *Result, ArrayRef< Type * > Params, bool isVarArg)
  FunctionType* funcType = FunctionType::get((Type *) l1PtrType, paramArrayType, false);

  //We define the function name as "MatrixMult"
  const Twine* funcName = new Twine("MatrixMult");
  //Params: (FunctionType *Ty, LinkageTypes Linkage, const Twine &N="", Module *M=nullptr)
  Function* func = Function::Create(funcType, GlobalValue::ExternalLinkage, *funcName, &module);

  BasicBlock* entryBB = BasicBlock::Create(context, "entry", func);
  BasicBlock* ifDimCheckBB = BasicBlock::Create(context, "if_dimCheck", func);
  BasicBlock* ifDimCheckEndBB = BasicBlock::Create(context, "if_dimCheckEnd", func);
  BasicBlock* forCondBB = BasicBlock::Create(context, "for_cond", func);
  BasicBlock* forBodyBB = BasicBlock::Create(context, "for_body", func);
  BasicBlock* forCond2BB = BasicBlock::Create(context, "for_cond2", func);
  BasicBlock* forBody2BB = BasicBlock::Create(context, "for_body2", func);
  BasicBlock* forCond3BB = BasicBlock::Create(context, "for_cond3", func);
  BasicBlock* forBody3BB = BasicBlock::Create(context, "for_body3", func);
  BasicBlock* cleanupBB = BasicBlock::Create(context, "cleanup", func);
  IRBuilder<> builder(entryBB);

  //Get the two matrix from arguments
  Value* mat1;
  Value* mat2;
  Value* mat1Height;
  Value* mat1Width;
  Value* mat2Height;
  Value* mat2Width;
  int argId = 0;
  for (Function::arg_iterator arg = func->arg_begin(); arg!=func->arg_end(); ++arg){
    ++argId;
	switch (argId)
	{
	  case 1:
	  {
		LOG(L2_DEBUG)<<"argId:" << argId;
	    mat1 = (Value*) &*arg;
		break;
	  }
	  case 2:
	  {
		LOG(L2_DEBUG)<<"argId:" << argId;
		mat2 = (Value*) &*arg;
		break;
	  }
	  case 3:
	  {
		if(arg->getType()->isIntegerTy()){
		  LOG(L2_DEBUG)<<"argId:" << argId;
	      mat1Height = (Value*) &*arg;
		}
		else{
		  LOG(L2_DEBUG)<<"Wrong parameters:" << arg->getType()->getTypeID();
		}
		break;
	  }
	  case 4:
	  {
		if(arg->getType()->isIntegerTy()){
		  LOG(L2_DEBUG)<<"argId:" << argId;
	      mat1Width = (Value*) &*arg;
		}
		else{
		  LOG(L2_DEBUG)<<"Wrong parameters:" << arg->getType()->getTypeID();
		}
		break;
	  }
	  case 5:
	  {
		if(arg->getType()->isIntegerTy()){
		  LOG(L2_DEBUG)<<"argId:" << argId;
	      mat2Height = (Value*) &*arg;
		}
		else{
		  LOG(L2_DEBUG)<<"Wrong parameters:" << arg->getType()->getTypeID();
		}
		break;
	  }
	  case 6:
	  {
		if(arg->getType()->isIntegerTy()){
		  LOG(L2_DEBUG)<<"argId:" << argId;
	      mat2Width = (Value*) &*arg;
		}
		else{
		  LOG(L2_DEBUG)<<"Wrong parameters:" << arg->getType()->getTypeID();
		}
		break;
	  }
	  default:
	    break;
	  //TODO:
	  //We return an empty matrix;
	  //TODO:Throw an alert;
	}
	//TODO:Get the argument
  }
  //TODO: Muliply the arg

  //TODO:
  AllocaInst* retAllocaInst = new AllocaInst(l1PtrType,"retval", entryBB);

  AllocaInst* mat1AddrAllocaInst = new AllocaInst(l1PtrType,"mat1.addr", entryBB);
  AllocaInst* mat2AddrAllocaInst = new AllocaInst(l1PtrType,"mat2.addr", entryBB);
  AllocaInst* m1HeightAddrAllocaInst = new AllocaInst(i64Type,"m1Heigh.addr", entryBB);
  AllocaInst* m1WidthAddrAllocaInst = new AllocaInst(i64Type,"m1Width.addr", entryBB);
  AllocaInst* m2HeightAddrAllocaInst = new AllocaInst(i64Type,"m2Heigh.addr", entryBB);
  AllocaInst* m2WidthAddrAllocaInst = new AllocaInst(i64Type,"m2Width.addr", entryBB);
  AllocaInst* iAllocaInst = new AllocaInst(i64Type,"i", entryBB);
  AllocaInst* jAllocaAllocaInst = new AllocaInst(i64Type,"j", entryBB);
  AllocaInst* eleAllocaInst = new AllocaInst(i64Type,"ele", entryBB);
  AllocaInst* kAllocaInst = new AllocaInst(i64Type,"k", entryBB);

  StoreInst* mat1StoreInst = new StoreInst(mat1, (Value *) mat1AddrAllocaInst, entryBB);
  StoreInst* mat2StoreInst = new StoreInst(mat2, (Value *) mat2AddrAllocaInst, entryBB);
  StoreInst* m1HeightStoreInst = new StoreInst(mat1Height, (Value *) m1HeightAddrAllocaInst, entryBB);
  StoreInst* m1WidthStoreInst = new StoreInst(mat1Width, (Value *) m1WidthAddrAllocaInst, entryBB);
  StoreInst* m2HeightStoreInst = new StoreInst(mat2Height, (Value *) m2HeightAddrAllocaInst, entryBB);
  StoreInst* m2WidthStoreInst = new StoreInst(mat2Width, (Value *) m2WidthAddrAllocaInst, entryBB);
  
  LoadInst* m1HeightLoadInst = new LoadInst((Value*) m1HeightAddrAllocaInst, "", entryBB);
  LoadInst* m1WidthLoadInst = new LoadInst((Value*) m1WidthAddrAllocaInst, "", entryBB);
  LoadInst* m2HeightLoadInst = new LoadInst((Value*) m2HeightAddrAllocaInst, "", entryBB);
  LoadInst* m2WidthLoadInst = new LoadInst((Value*) m2WidthAddrAllocaInst, "", entryBB);

  BinaryOperator* getMatSizeMul = BinaryOperator::CreateNUWMul((Value*) m1HeightLoadInst,(Value*) m2WidthLoadInst, "retMatSize", entryBB);

  AllocaInst* vlaAllocaInst = new AllocaInst(i64Type,(Value *) getMatSizeMul,"vla", entryBB);
  ICmpInst* icmpInst = new ICmpInst(*entryBB,CmpInst::ICMP_NE,(Value*) m1WidthLoadInst, (Value*) m2HeightLoadInst,"dimCheck");
  BranchInst::Create(ifDimCheckBB,ifDimCheckEndBB,icmpInst, entryBB);

  //BasicBlock* if_dimCheck
  BitCastInst* bitCastInst = new BitCastInst((Value*) vlaAllocaInst, l1PtrType, "", ifDimCheckBB);
  StoreInst* retStoreInst = new StoreInst((Value*) bitCastInst, (Value*) retAllocaInst, ifDimCheckBB);
  BranchInst::Create(cleanupBB,ifDimCheckBB);

  //BasicBlock* if_dimCheckEnd

  ConstantInt* conInt0 = (ConstantInt*) ConstantInt::getSigned(i64Type,0); 
  StoreInst* tmpStoreInst111 = new StoreInst((Value*) conInt0, (Value*) iAllocaInst, ifDimCheckBB);
  BranchInst::Create(forCondBB,ifDimCheckBB);

  //BasicBlock* for_cond

  //BasicBlock* cleanup
  LoadInst* retLoadInst= new LoadInst((Value*) retAllocaInst,"",cleanupBB); 
  ReturnInst* retInst = ReturnInst::Create(context, retLoadInst, cleanupBB);

  return func;
}

void ConvertIcmp2Mbp(ICmpInst *icmpInst){
  LOG(L2_DEBUG) << "ConvertIcmp2Mbp...";
  MBP* mbp = new MBP(icmpInst);
}
