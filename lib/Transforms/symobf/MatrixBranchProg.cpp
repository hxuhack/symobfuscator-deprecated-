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

		  /* We should uncomment this section in the final version
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
*/
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
  MatrixInIR[2][] matListInp0;

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

  void GenerateMMLogic(ICmpInst* icmpInst){
	LLVMContext& context = icmpInst->getContext();
	Function* parentFunc = icmpInst->getFunction();
	BasicBlock* parentBB = icmpInst->getParent();

	BasicBlock* forBB = parentBB->splitBasicBlock(icmpInst, "for_cond_loop");

    Type* i64Type = IntegerType::getInt64Ty(context);
    ConstantInt* conInt0 = (ConstantInt*) ConstantInt::getSigned(i64Type,matListInp0.size()); 
	//CallInst callInst = CallInst::Create("MatrixMult", ,"",icmpInst);
  }

public:
  MBP(ICmpInst *icmpInst){
    GenerateMatrix(icmpInst);
    GenerateMMLogic(icmpInst);//Matrix multiplication logic
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

  ConstantInt* conInt0 = (ConstantInt*) ConstantInt::getSigned(i64Type,0); 
  ConstantInt* conInt1 = (ConstantInt*) ConstantInt::getSigned(i64Type,1); 

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
  BasicBlock* forCond1BB = BasicBlock::Create(context, "for_cond1", func);
  BasicBlock* forBody1BB = BasicBlock::Create(context, "for_body1", func);
  BasicBlock* forCond2BB = BasicBlock::Create(context, "for_cond2", func);
  BasicBlock* forBody2BB = BasicBlock::Create(context, "for_body2", func);
  BasicBlock* forCond3BB = BasicBlock::Create(context, "for_cond3", func);
  BasicBlock* forBody3BB = BasicBlock::Create(context, "for_body3", func);
  BasicBlock* forInc1BB = BasicBlock::Create(context, "for_inc1", func);
  BasicBlock* forEnd1BB = BasicBlock::Create(context, "for_end1", func);
  BasicBlock* forInc2BB = BasicBlock::Create(context, "for_inc2", func);
  BasicBlock* forEnd2BB = BasicBlock::Create(context, "for_end2", func);
  BasicBlock* forInc3BB = BasicBlock::Create(context, "for_inc3", func);
  BasicBlock* forEnd3BB = BasicBlock::Create(context, "for_end3", func);
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
  AllocaInst* jAllocaInst = new AllocaInst(i64Type,"j", entryBB);
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

  StoreInst* iStoreInst = new StoreInst((Value*) conInt0, (Value*) iAllocaInst, ifDimCheckBB);
  BranchInst::Create(forCond1BB,ifDimCheckBB);

  //BasicBlock* for_cond1
  LoadInst* iLoadInst = new LoadInst((Value*) iAllocaInst, "", forCond1BB);
  //LoadInst* m1HeightAddrLoadInst = new LoadInst((Value*) m1HeightAddrAllocaInst, "", forCond1BB);
  ICmpInst* icmpInst1 = new ICmpInst(*forCond1BB,CmpInst::ICMP_SLT,(Value*) iLoadInst, (Value*) m1HeightLoadInst, "");
  BranchInst::Create(forBody1BB,forEnd3BB,icmpInst1, forCond1BB);
  
  //BasicBlock* for_body1
  StoreInst* jStoreInst = new StoreInst((Value*) conInt0, (Value*) jAllocaInst, forBody1BB);
  BranchInst::Create(forCond2BB,forBody1BB);
  
  //BasicBlock* for_cond2
  LoadInst* jLoadInst = new LoadInst((Value*) jAllocaInst, "", forCond2BB);
  ICmpInst* icmpInst2 = new ICmpInst(*forCond2BB,CmpInst::ICMP_SLT,(Value*) iLoadInst, (Value*) m2WidthLoadInst, "");
  BranchInst::Create(forBody2BB, forEnd2BB, icmpInst2, forCond2BB);
  
  //BasicBlock* for_body2
  StoreInst* kStoreInst = new StoreInst((Value*) conInt0, (Value*) kAllocaInst, forBody2BB);
  StoreInst* eleStoreInst = new StoreInst((Value*) conInt0, (Value*) eleAllocaInst, forBody2BB);
  BranchInst::Create(forCond3BB,forBody2BB);
  
  //BasicBlock* for_cond3
  LoadInst* kLoadInst = new LoadInst((Value*) kAllocaInst, "", forCond3BB);
  ICmpInst* icmpInst3 = new ICmpInst(*forCond3BB,CmpInst::ICMP_SLT,(Value*) kLoadInst, (Value*) m1WidthLoadInst, "");
  BranchInst::Create(forBody3BB, forEnd1BB, icmpInst3, forCond3BB);
  
  //BasicBlock* for_body3
  LoadInst* eleLoadInst = new LoadInst((Value*) eleAllocaInst, "", forBody3BB);
  //load an element from the first matrix
  LoadInst* mat1AddrLoadInst = new LoadInst((Value*) mat1AddrAllocaInst, "", forBody3BB);
  BitCastInst* bitCastInst2 = new BitCastInst((Value*) mat1AddrLoadInst, l2PtrType, "", forBody3BB);
  LoadInst* iLoadInst2 = new LoadInst((Value*) iAllocaInst, "", forBody3BB);
  BinaryOperator* bopMul1 = BinaryOperator::CreateNUWMul((Value*) m1WidthLoadInst, (Value*) iLoadInst2, "", forBody3BB);
  GetElementPtrInst* getEPInst = GetElementPtrInst::CreateInBounds(i64Type, (Value*) bitCastInst2, bopMul1,"", forBody3BB);
  LoadInst* kLoadInst2 = new LoadInst((Value*) kAllocaInst, "", forBody3BB);
  GetElementPtrInst* getEPInst2 = GetElementPtrInst::CreateInBounds(i64Type, (Value*) getEPInst, kLoadInst2,"", forBody3BB);
  LoadInst* eLoadInst31 = new LoadInst((Value*) getEPInst2, "", forBody3BB);

  //load an element from the first matrix
  LoadInst* mat2AddrLoadInst = new LoadInst((Value*) mat2AddrAllocaInst, "", forBody3BB);
  BitCastInst* bitCastInst3 = new BitCastInst((Value*) mat2AddrLoadInst, l2PtrType, "", forBody3BB);
  LoadInst* kLoadInst3 = new LoadInst((Value*) kAllocaInst, "", forBody3BB);
  BinaryOperator* bopMul3 = BinaryOperator::CreateNUWMul((Value*) m2WidthLoadInst, (Value*) kLoadInst2, "", forBody3BB);
  GetElementPtrInst* getEPInst31 = GetElementPtrInst::CreateInBounds(i64Type, (Value*) bitCastInst3, bopMul3,"", forBody3BB);
  LoadInst* jLoadInst3 = new LoadInst((Value*) jAllocaInst, "", forBody3BB);
  GetElementPtrInst* getEPInst32 = GetElementPtrInst::CreateInBounds(i64Type, (Value*) getEPInst31, jLoadInst3,"", forBody3BB);
  LoadInst* eLoadInst32 = new LoadInst((Value*) getEPInst32, "", forBody3BB);
  BinaryOperator* bopMul32 = BinaryOperator::CreateNSWMul((Value*) eLoadInst31, (Value*) eLoadInst32, "", forBody3BB);
  BinaryOperator* bopAdd31 = BinaryOperator::CreateNSWAdd((Value*) bopMul32, (Value*) eleLoadInst, "", forBody3BB);
  StoreInst* eleStoreInst3 = new StoreInst((Value*) bopAdd31, (Value*) eleAllocaInst, forBody3BB);
  BranchInst::Create(forInc1BB,forBody3BB);
  
  //BasicBlock* for_inc1
  LoadInst* kLoadInstInc1 = new LoadInst((Value*) kAllocaInst, "", forInc1BB);
  BinaryOperator* bopAddInc1 = BinaryOperator::CreateNSWAdd((Value*) kLoadInstInc1, (Value*) conInt1, "", forInc1BB);
  StoreInst* kStoreInstInc1 = new StoreInst((Value*) bopAddInc1, (Value*) kAllocaInst, forInc1BB);
  BranchInst::Create(forCond3BB,forInc1BB);
  
  //BasicBlock* for_end1
  LoadInst* eleLoadInstEnd1 = new LoadInst((Value*) eleAllocaInst, "", forEnd1BB);
  LoadInst* jLoadInstEnd1 = new LoadInst((Value*) jAllocaInst, "", forEnd1BB);
  LoadInst* iLoadInstEnd1 = new LoadInst((Value*) iAllocaInst, "", forEnd1BB);
  BinaryOperator* bopMulEnd1 = BinaryOperator::CreateNSWMul((Value*) iLoadInstEnd1, (Value*) m2WidthLoadInst, "", forEnd1BB);
  GetElementPtrInst* getEPInstEnd1 = GetElementPtrInst::CreateInBounds(i64Type, (Value*) vlaAllocaInst, bopMulEnd1,"", forEnd1BB);
  GetElementPtrInst* getEPInstEnd2 = GetElementPtrInst::CreateInBounds(i64Type, (Value*) getEPInstEnd1, jLoadInstEnd1,"", forEnd1BB);
  StoreInst* eleStoreInstEnd1 = new StoreInst((Value*) eleLoadInstEnd1, (Value*) getEPInstEnd2, forEnd1BB);
  BranchInst::Create(forInc2BB,forEnd1BB);
  
  //BasicBlock* for_inc2
  LoadInst* jLoadInstInc2 = new LoadInst((Value*) jAllocaInst, "", forInc2BB);
  BinaryOperator* bopAddInc2 = BinaryOperator::CreateNSWAdd((Value*) jLoadInstInc2, (Value*) conInt1, "", forInc2BB);
  StoreInst* jStoreInstInc2 = new StoreInst((Value*) bopAddInc2, (Value*) jAllocaInst, forInc2BB);
  BranchInst::Create(forCond2BB,forInc2BB);
  
  //BasicBlock* for_end2
  BranchInst::Create(forInc3BB,forEnd2BB);
  
  //BasicBlock* for_inc3
  LoadInst* iLoadInstInc3 = new LoadInst((Value*) iAllocaInst, "", forInc3BB);
  BinaryOperator* bopAddInc3 = BinaryOperator::CreateNSWAdd((Value*) iLoadInstInc3, (Value*) conInt1, "", forInc3BB);
  StoreInst* iStoreInstInc3 = new StoreInst((Value*) bopAddInc3, (Value*) iAllocaInst, forInc3BB);
  BranchInst::Create(forCond1BB,forInc3BB);
  
  //BasicBlock* for_end3
  BitCastInst* bitCastInstEnd3 = new BitCastInst((Value*) vlaAllocaInst, l1PtrType, "", forEnd3BB);
  StoreInst* storeInstEnd3 = new StoreInst((Value*) bitCastInstEnd3, (Value*) retAllocaInst, forEnd3BB);
  BranchInst::Create(cleanupBB,forEnd3BB);

  //BasicBlock* cleanup
  LoadInst* retLoadInst= new LoadInst((Value*) retAllocaInst,"",cleanupBB); 
  ReturnInst* retInst = ReturnInst::Create(context, retLoadInst, cleanupBB);

  return func;
}

void ConvertIcmp2Mbp(ICmpInst *icmpInst){
  LOG(L2_DEBUG) << "ConvertIcmp2Mbp...";
  MBP* mbp = new MBP(icmpInst);
}
