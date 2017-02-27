#include"llvm/Transforms/SymObf/MatrixBranchProg.h"

using namespace llvm;
using namespace std;


class MatrixInIR{

private:
  int width = 0;
  int height = 0;
  AllocaInst* matAI;

public:
//TODO: Impelemt array structure in IR.
/*
  %mat = alloca [2 x [2 x i32]], align 16
  %arrayconInt = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat, i64 0, i64 0
  %arrayci1 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayconInt, i64 0, i64 0
  store i32 0, i32* %arrayci1, align 16
  %arrayconInt2 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat, i64 0, i64 0
  %arrayconInt3 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayconInt2, i64 0, i64 1
  store i32 1, i32* %arrayconInt3, align 4
  %arrayconInt4 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat, i64 0, i64 1
  %arrayconInt5 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayconInt4, i64 0, i64 0
  store i32 0, i32* %arrayconInt5, align 8
  %arrayconInt6 = getelementptr inbounds [2 x [2 x i32]], [2 x [2 x i32]]* %mat, i64 0, i64 1
  %arrayconInt7 = getelementptr inbounds [2 x i32], [2 x i32]* %arrayconInt6, i64 0, i64 1
  store i32 1, i32* %arrayconInt7, align 4
  ret void
*/

  MatrixInIR(){
  
  }
  

  MatrixInIR(LLVMContext& context, BasicBlock* bb, const int width, const int height, int conInt_row, int conInt_col, Type* type):width(width),height(height){

    ConstantInt*** conIntMatrix = (ConstantInt***) malloc (sizeof(ConstantInt**)*height);

	//Construct an allocaInst:
	//AllocaInst (Type *Ty, const Twine &Name="", Instruction *InsertBefore=nullptr)
	Type* i32Type = IntegerType::getInt32Ty(context);
	Type* i64Type = IntegerType::getInt64Ty(context);
	ArrayType* l2ArrayType = ArrayType::get(i64Type, height);
	ArrayType* l1ArrayType = ArrayType::get(l2ArrayType, width);

	matAI = new AllocaInst(l1ArrayType,"", bb);


	for(int i=0; i<height; i++){
	  conIntMatrix[i] = (ConstantInt**) malloc (sizeof(ConstantInt*) * width);

	  //Construct an GetElementPtrInst;
	  //Create (Type *PointeeType, Value *Ptr, ArrayRef< Value * > IdxList, const Twine &NameStr="", Instruction *InsertBefore=nullptr) 
	  //http://llvm.org/docs/GetElementPtr.html
      ConstantInt* ci0 = (ConstantInt*) ConstantInt::getSigned(i64Type,0);
      ConstantInt* ci1 = (ConstantInt*) ConstantInt::getSigned(i64Type,i);
	  vector<Value*> conIntVec;
	  conIntVec.push_back(ci0);
	  conIntVec.push_back(ci1);
	  ArrayRef<Value*> conIntArrayRef(conIntVec);
	  GetElementPtrInst* getEPInst = GetElementPtrInst::CreateInBounds(l1ArrayType, (Value*) matAI, conIntArrayRef,"", bb);

	  for(int j=0; j<width; j++){
        ConstantInt* conInt2 = (ConstantInt*) ConstantInt::getSigned(i64Type,j);
	    vector<Value*> l2IdxVec;
	    l2IdxVec.push_back(ci0);
	    l2IdxVec.push_back(conInt2);
	    ArrayRef<Value*> conIntArrayRef(l2IdxVec);
	    GetElementPtrInst* l2GetPEInst = GetElementPtrInst::CreateInBounds(l2ArrayType, (Value*) getEPInst, conIntArrayRef,"", bb);
		ConstantInt* tmpConstInt;
		if((i==j) || (i==conInt_row && j == conInt_col)){
          tmpConstInt = (ConstantInt*) ConstantInt::getSigned(i64Type,1);
		}else{
          tmpConstInt = (ConstantInt*) ConstantInt::getSigned(i64Type,0);
		}
		conIntMatrix[i][j] = tmpConstInt;
		StoreInst* storeInst = new StoreInst((Value *) tmpConstInt, (Value *) l2GetPEInst, bb);
	  }
	}
  }

  MatrixInIR(LLVMContext& context, BasicBlock* bb,  const int width, const int height):width(width),height(height){
    ConstantInt*** conIntMatrix = (ConstantInt***) malloc (sizeof(ConstantInt**)*height);
	Type* i64Type = IntegerType::getInt64Ty(context);
	ArrayType* l2ArrayType = ArrayType::get(i64Type, height);
	ArrayType* l1ArrayType = ArrayType::get(l2ArrayType, width);

	matAI = new AllocaInst(l1ArrayType,"", bb);


	for(int i=0; i<height; i++){
	  conIntMatrix[i] = (ConstantInt**) malloc (sizeof(ConstantInt*) * width);

      ConstantInt* ci0 = (ConstantInt*) ConstantInt::getSigned(i64Type,0);
      ConstantInt* ci1 = (ConstantInt*) ConstantInt::getSigned(i64Type,i);
	  vector<Value*> conIntVec;
	  conIntVec.push_back(ci0);
	  conIntVec.push_back(ci1);
	  ArrayRef<Value*> conIntArrayRef(conIntVec);
	  GetElementPtrInst* getEPInst = GetElementPtrInst::CreateInBounds(l1ArrayType, (Value*) matAI, conIntArrayRef,"", bb);

	  for(int j=0; j<width; j++){
        ConstantInt* conInt2 = (ConstantInt*) ConstantInt::getSigned(i64Type,j);
	    vector<Value*> l2IdxVec;
	    l2IdxVec.push_back(ci0);
	    l2IdxVec.push_back(conInt2);
	    ArrayRef<Value*> conIntArrayRef(l2IdxVec);
	    GetElementPtrInst* l2GetPEInst = GetElementPtrInst::CreateInBounds(l2ArrayType, (Value*) getEPInst, conIntArrayRef,"", bb);
		ConstantInt* tmpConstInt = (ConstantInt*) ConstantInt::getSigned(i64Type,0);
		conIntMatrix[i][j] = tmpConstInt;
		StoreInst* storeInst = new StoreInst((Value *) tmpConstInt, (Value *) l2GetPEInst, bb);
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

  AllocaInst* getMatAI(){
	return matAI; 
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


void ConvertIcmp2Mbp(Module& module, ICmpInst *icmpInst, Function* funcMM){
  LOG(L2_DEBUG) << "ConvertIcmp2Mbp...";
  LLVMContext& context = icmpInst->getContext();
  Function* pFunc = icmpInst->getFunction();
  BasicBlock* pBB = icmpInst->getParent();


  BasicBlock* forCondBB = pBB->splitBasicBlock(icmpInst, "for_cond_loop");
  pBB->getTerminator()->eraseFromParent();
  BasicBlock* forBodyBB = forCondBB->splitBasicBlock(icmpInst, "for_body_loop");
  forCondBB->getTerminator()->eraseFromParent();
  BasicBlock* forIncBB = forBodyBB->splitBasicBlock(icmpInst, "for_inc_loop");
  forBodyBB->getTerminator()->eraseFromParent();
  BasicBlock* forEndBB = forIncBB->splitBasicBlock(icmpInst, "for_end_loop");
  forIncBB->getTerminator()->eraseFromParent();
  BasicBlock* conBB = forEndBB->splitBasicBlock(icmpInst, "entry_continue");

  MatrixInIR* headMat;
  MatrixInIR* tailMat;
  list<MatrixInIR*> matListInp0;
  list<MatrixInIR*> matListInp1;

  Type* type = icmpInst->getType();
  Instruction::OtherOps opCode = icmpInst->getOpcode();
  CmpInst::Predicate predicate = icmpInst->getPredicate();
  Value* op0 = icmpInst->getOperand(0);
  Value* op1 = icmpInst->getOperand(1);
  if(!(isa<ConstantInt> (*op0) || isa<ConstantInt>(*op1))){
	LOG(L_INFO) << "[-PI-]:ICmpInst has two symbolic variables; Don't obfuscate!";
  }


  ConstantInt* conInt;
  IntegerType* inp;

  if(isa<ConstantInt> (*op0)){
	conInt = (ConstantInt*)op0;
	inp = (IntegerType*) op1;
  }
  else{	
	conInt = (ConstantInt*) op1;
	inp = (IntegerType*) op0;
  }

  int len = conInt->getBitWidth();
  int dim = len + 1;
  IntegerType* boolType = IntegerType::get(conInt->getContext(),1);

  Type* i8Type = IntegerType::getInt8Ty(context);
  Type* i64Type = IntegerType::getInt64Ty(context);
  Type* i32Type = IntegerType::getInt32Ty(context);
  ArrayType* l2ArrayType = ArrayType::get(i64Type, dim);
  ArrayType* l1ArrayType = ArrayType::get(l2ArrayType, dim);
  ArrayType* i8AT = ArrayType::get(i8Type, 26);

  //We create an level-2 array that points to matrix 
  PointerType* l2PtrType = PointerType::getUnqual(i64Type);
  PointerType* l1PtrType = PointerType::getUnqual(l2PtrType);
  ArrayType* l2MatArrayType = ArrayType::get(l1PtrType, len);
  ArrayType* l1MatArrayType = ArrayType::get(l2MatArrayType, 2);

  AllocaInst* matAI = new AllocaInst(l1MatArrayType,"matrix", pBB);

  ConstantInt* ci0 = (ConstantInt*) ConstantInt::getSigned(i64Type,0);
  ConstantInt* ci1 = (ConstantInt*) ConstantInt::getSigned(i64Type,1);
  ConstantInt* ciDim = (ConstantInt*) ConstantInt::getSigned(i64Type,dim);
  ConstantInt* ciLen = (ConstantInt*) ConstantInt::getSigned(i64Type,len);

  vector<Value*> vec00,vec01;
  vec00.push_back(ci0);
  vec00.push_back(ci0);
  ArrayRef<Value*> ar00(vec00);
  vec01.push_back(ci0);
  vec01.push_back(ci1);
  ArrayRef<Value*> ar01(vec01);
  GetElementPtrInst* getEPl10Inst = GetElementPtrInst::CreateInBounds(l1MatArrayType, (Value*) matAI, ar00,"", pBB);
  GetElementPtrInst* getEPl11Inst = GetElementPtrInst::CreateInBounds(l1MatArrayType, (Value*) matAI, ar01,"", pBB);

  for(int i=0; i<len; i++){
	bool bit = conInt->getValue()[i];
	int conInt_row = i;
	int conInt_col1 = i+1;
	int conInt_col2 = len;
	MatrixInIR* mat1 = new MatrixInIR(context, pBB, dim,dim,conInt_row,conInt_col1,boolType); //bit
	MatrixInIR* mat0 = new MatrixInIR(context, pBB, dim,dim,conInt_row,conInt_col2,boolType); //~bit
    ConstantInt* conInti = (ConstantInt*) ConstantInt::getSigned(i64Type,i);
    vector<Value*> vec0i;
    vec0i.push_back(ci0);
    vec0i.push_back(conInti);
    ArrayRef<Value*> ar0i(vec0i);
    GetElementPtrInst* getEPl20Inst = GetElementPtrInst::CreateInBounds(l2MatArrayType, (Value*) getEPl10Inst, ar0i,"", pBB);
    GetElementPtrInst* getEPl21Inst = GetElementPtrInst::CreateInBounds(l2MatArrayType, (Value*) getEPl11Inst, ar0i,"", pBB);


    GetElementPtrInst* getEPMat0Inst = GetElementPtrInst::CreateInBounds(l1ArrayType, (Value*) mat0->getMatAI(), ar00,"", pBB);
    GetElementPtrInst* getEPMat1Inst = GetElementPtrInst::CreateInBounds(l1ArrayType, (Value*) mat1->getMatAI(), ar00,"", pBB);

    BitCastInst* mat0BCI = new BitCastInst((Value*) getEPMat0Inst, l1PtrType, "", pBB);
    BitCastInst* mat1BCI = new BitCastInst((Value*) getEPMat1Inst, l1PtrType, "", pBB);

    StoreInst* mat0StoreInst = new StoreInst(mat0BCI, (Value *) getEPl20Inst, pBB);
    StoreInst* mat1StoreInst = new StoreInst(mat1BCI, (Value *) getEPl21Inst, pBB);

	matListInp0.push_back(mat0);
	matListInp1.push_back(mat1);
  }
  headMat = new MatrixInIR(context, pBB, 1,dim,0,0,boolType); //bit
  tailMat = new MatrixInIR(context, pBB, dim,1,dim-1,0,boolType); //~bit

  //Init the parameter for the for loop; 
  AllocaInst* iAI = new AllocaInst(i64Type,"", pBB);
  StoreInst* iSI = new StoreInst(ci1, (Value *) iAI, pBB);
  AllocaInst* lenAllocaInst = new AllocaInst(i64Type,"", pBB);
  StoreInst* lenStoreInst = new StoreInst(ciLen, (Value *) lenAllocaInst, pBB);

  AllocaInst* matIdAI = new AllocaInst(i64Type,"mat_idx", pBB);
  SExtInst* inp64 = new SExtInst((Value*) inp, i64Type, "", pBB);
  BinaryOperator* andBO = BinaryOperator::Create(Instruction::And, (Value*) inp64, ci1, "", pBB);
  StoreInst* matIdSI = new StoreInst((Value *) andBO, matIdAI, pBB);
  LoadInst* matIdLI = new LoadInst((Value *) matIdAI, "", pBB);
  
  vector<Value*> vec0li;
  vec0li.push_back(ci0);
  vec0li.push_back(matIdLI);
  ArrayRef<Value*> ar0li(vec0li);
  GetElementPtrInst* getBinEPI = GetElementPtrInst::CreateInBounds(l1MatArrayType, (Value*) matAI, ar0li,"", pBB);
  GetElementPtrInst* getLenEPI = GetElementPtrInst::CreateInBounds(l2MatArrayType, (Value*) getBinEPI, ar00,"", pBB);
  LoadInst* ldMatLI = new LoadInst(getLenEPI,"",pBB);

  BitCastInst* headMatBI = new BitCastInst((Value*) headMat->getMatAI(), l1PtrType, "", pBB);

  AllocaInst* interMatAI = new AllocaInst(l1PtrType,"interMat",pBB);
  
  vector<Value*> vecMM;
  vecMM.push_back(headMatBI);
  vecMM.push_back(ldMatLI);
  vecMM.push_back(ci1);
  vecMM.push_back(ciDim);
  vecMM.push_back(ciDim);
  vecMM.push_back(ciDim);
  ArrayRef<Value*> arMM(vecMM);
  CallInst* mmCI = CallInst::Create(funcMM, arMM, "", pBB);
  //StoreInst* matSI = new StoreInst((Value *) mmCI, interMatAI, pBB);

  StoreInst* interMatSI = new StoreInst((Value*) mmCI, interMatAI, "", pBB);
  BranchInst::Create(forCondBB, pBB);

  LoadInst* iLI = new LoadInst((Value*) iAI, "", forCondBB);
  LoadInst* lenLoadInst = new LoadInst((Value*) lenAllocaInst, "", forCondBB);
  ICmpInst* forII = new ICmpInst(*forCondBB,CmpInst::ICMP_SLT,(Value*) iLI, (Value*) lenLoadInst,"");
  BranchInst::Create(forBodyBB,forEndBB, forII, forCondBB);

  //For body

  LoadInst* iFbLI = new LoadInst((Value*) iAI, "", forBodyBB);
  BinaryOperator* shlFbBO = BinaryOperator::Create(Instruction::Shl, (Value*) ci0, iFbLI, "", forBodyBB);
  BinaryOperator* andFbBO = BinaryOperator::Create(Instruction::And, (Value*) inp64, shlFbBO, "", forBodyBB);
  BinaryOperator* ashrFbBO = BinaryOperator::Create(Instruction::AShr, (Value*) andFbBO, iFbLI, "", forBodyBB);
  StoreInst* matIdFbSI = new StoreInst((Value *) ashrFbBO, matIdAI, forBodyBB);

  LoadInst* matIdFbLI = new LoadInst((Value *) matIdAI, "", forBodyBB);
  LoadInst* iFbLI02 = new LoadInst((Value*) iAI, "", forBodyBB);

  vector<Value*> vecFb0I0;
  vecFb0I0.push_back(ci0);
  vecFb0I0.push_back(matIdFbLI);
  ArrayRef<Value*> arFb0I0(vecFb0I0);

  vector<Value*> vecFb0I1;
  vecFb0I1.push_back(ci0);
  vecFb0I1.push_back(iFbLI02);
  ArrayRef<Value*> arFb0I1(vecFb0I1);

  GetElementPtrInst* getBinFbEPI = GetElementPtrInst::CreateInBounds(l1MatArrayType, (Value*) matAI, arFb0I0,"", forBodyBB);
  GetElementPtrInst* getLenFbEPI = GetElementPtrInst::CreateInBounds(l2MatArrayType, (Value*) getBinFbEPI, arFb0I1,"", forBodyBB);
  LoadInst* ldFbMatLI = new LoadInst(getLenFbEPI,"",forBodyBB);
  LoadInst* interMatLI = new LoadInst(interMatAI,"",forBodyBB);

  vector<Value*> vecFbMM;
  vecFbMM.push_back(interMatLI);
  vecFbMM.push_back(ldFbMatLI);
  vecFbMM.push_back(ci1);
  vecFbMM.push_back(ciDim);
  vecFbMM.push_back(ciDim);
  vecFbMM.push_back(ciDim);
  ArrayRef<Value*> arFbMM(vecFbMM);
  CallInst* mmFbCI = CallInst::Create(funcMM, arFbMM, "", forBodyBB);
  StoreInst* interMatFbSI = new StoreInst((Value*) mmFbCI, interMatAI, "", forBodyBB);
  BranchInst::Create(forIncBB, forBodyBB);
  
  //For inc 
  LoadInst* iFiLI = new LoadInst((Value*) iAI, "", forIncBB);
  BinaryOperator* iFiAdd = BinaryOperator::CreateNSWAdd((Value*) iFiLI, ci1, "", forIncBB);
  StoreInst* iFiSI = new StoreInst((Value*) iFiAdd, iAI, forIncBB);
  BranchInst::Create(forCondBB, forIncBB);

  //Continue
  BranchInst* oriBI = (BranchInst*) conBB->getTerminator();
  BasicBlock* trueBB = oriBI->getSuccessor(0);
  BasicBlock* falseBB = oriBI->getSuccessor(1);
  oriBI->eraseFromParent();

  BitCastInst* tailMatBI = new BitCastInst((Value*) tailMat->getMatAI(), l1PtrType, "", conBB);
  LoadInst* interMatConLI = new LoadInst(interMatAI,"",conBB);
  vector<Value*> vecConMM;
  vecConMM.push_back(interMatConLI);
  vecConMM.push_back(tailMatBI);
  vecConMM.push_back(ci1);
  vecConMM.push_back(ciDim);
  vecConMM.push_back(ciDim);
  vecConMM.push_back(ci1);
  ArrayRef<Value*> arConMM(vecConMM);
  CallInst* mmConCI = CallInst::Create(funcMM, arConMM, "", conBB);
  StoreInst* interMatConSI = new StoreInst((Value*) mmConCI, interMatAI, "", conBB);
  
  AllocaInst* cmpAI = new AllocaInst(i64Type,"cmpAI", conBB);
  LoadInst* interMatConLI2 = new LoadInst(interMatAI,"",conBB);
  BitCastInst* cmpCI = new BitCastInst((Value*) interMatConLI2, l2PtrType, "", conBB);
  GetElementPtrInst* getCmpL1EPI = GetElementPtrInst::CreateInBounds(i64Type, (Value*) cmpCI, ci0,"", conBB);
  GetElementPtrInst* getCmpEPI = GetElementPtrInst::CreateInBounds(i64Type, (Value*) getCmpL1EPI, ci0,"", conBB);
  LoadInst* getCmpLI = new LoadInst(getCmpEPI,"",conBB);
  StoreInst* cmpSI = new StoreInst((Value*) getCmpLI, cmpAI, "", conBB);
  LoadInst* cmpLI = new LoadInst(cmpAI,"",conBB);

  GlobalVariable* pfmGV = new GlobalVariable(module, i8AT, true, GlobalValue::PrivateLinkage, 0, "pfmStr");
  pfmGV->setAlignment(1);
  Constant* strPf = ConstantDataArray::getString(context,"++++++++++icmp result:%d\n", true);
  //Constant* getPfm = ConstantExpr::getGetElementPtr(pfmGV, vec00);
  pfmGV->setInitializer(strPf);
  GetElementPtrInst* getPfm = GetElementPtrInst::CreateInBounds(i8AT, pfmGV, ar00,"", conBB);

  vector<Value*> vecPrint;
  vecPrint.push_back(getPfm);
  vecPrint.push_back(cmpLI);
  ArrayRef<Value*> arPrint(vecPrint);
  CallInst::Create(printFunc, arPrint, "", conBB);

  ICmpInst* conII = new ICmpInst(*conBB, CmpInst::ICMP_EQ, (Value*) cmpLI, ci1, "");
  BranchInst::Create(trueBB, falseBB, conII, conBB);
}

/*
 *Function Define:
 *i32** @MM(i32** %mat1, i32** %mat2, i32 %m1Height, i32 %m1Width, i32 %m2Height, i32 %m2Width)
 *
 *Another way is to write the code in c and link during compilation: 
 *(https://www.cs.cornell.edu/~asampson/blog/llvm.html)
*/
/*

Function* GenMatMulFunc(LLVMContext& context, Module& module){
  LOG(L2_DEBUG) << "GenMatMulFunc...";
  //We construct a type of 2-level int array as the type for the matrix. 

  Type* i64Type = IntegerType::getInt64Ty(context);
  Type* i32Type = IntegerType::getInt32Ty(context);

  ConstantInt* ci0 = (ConstantInt*) ConstantInt::getSigned(i64Type,0); 
  ConstantInt* ci1 = (ConstantInt*) ConstantInt::getSigned(i64Type,1); 

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
  Function* funcMM = Function::Create(funcType, GlobalValue::ExternalLinkage, *funcName, &module);

  BasicBlock* entryBB = BasicBlock::Create(context, "entry", funcMM);
  BasicBlock* ifDimCheckBB = BasicBlock::Create(context, "if_dimCheck", funcMM);
  BasicBlock* ifDimCheckEndBB = BasicBlock::Create(context, "if_dimCheckEnd", funcMM);
  BasicBlock* forCond1BB = BasicBlock::Create(context, "for_cond1", funcMM);
  BasicBlock* forBody1BB = BasicBlock::Create(context, "for_body1", funcMM);
  BasicBlock* forCond2BB = BasicBlock::Create(context, "for_cond2", funcMM);
  BasicBlock* forBody2BB = BasicBlock::Create(context, "for_body2", funcMM);
  BasicBlock* forCond3BB = BasicBlock::Create(context, "for_cond3", funcMM);
  BasicBlock* forBody3BB = BasicBlock::Create(context, "for_body3", funcMM);
  BasicBlock* forInc1BB = BasicBlock::Create(context, "for_inc1", funcMM);
  BasicBlock* forEnd1BB = BasicBlock::Create(context, "for_end1", funcMM);
  BasicBlock* forInc2BB = BasicBlock::Create(context, "for_inc2", funcMM);
  BasicBlock* forEnd2BB = BasicBlock::Create(context, "for_end2", funcMM);
  BasicBlock* forInc3BB = BasicBlock::Create(context, "for_inc3", funcMM);
  BasicBlock* forEnd3BB = BasicBlock::Create(context, "for_end3", funcMM);
  BasicBlock* cleanupBB = BasicBlock::Create(context, "cleanup", funcMM);
  IRBuilder<> builder(entryBB);

  //Get the two matrix from arguments
  Value* mat1;
  Value* mat2;
  Value* mat1Height;
  Value* mat1Width;
  Value* mat2Height;
  Value* mat2Width;
  int argId = 0;
  for (Function::arg_iterator arg = funcMM->arg_begin(); arg!=funcMM->arg_end(); ++arg){
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
  AllocaInst* iAI = new AllocaInst(i64Type,"i", entryBB);
  AllocaInst* jAI = new AllocaInst(i64Type,"j", entryBB);
  AllocaInst* eleAllocaInst = new AllocaInst(i64Type,"ele", entryBB);
  AllocaInst* kAI = new AllocaInst(i64Type,"k", entryBB);

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

  StoreInst* iSI = new StoreInst((Value*) ci0, (Value*) iAI, ifDimCheckEndBB);
  BranchInst::Create(forCond1BB,ifDimCheckEndBB);

  //BasicBlock* for_cond1
  LoadInst* iLI = new LoadInst((Value*) iAI, "", forCond1BB);
  //LoadInst* m1HeightAddrLoadInst = new LoadInst((Value*) m1HeightAddrAllocaInst, "", forCond1BB);
  ICmpInst* icmpInst1 = new ICmpInst(*forCond1BB,CmpInst::ICMP_SLT,(Value*) iLI, (Value*) m1HeightLoadInst, "");
  BranchInst::Create(forBody1BB,forEnd3BB,icmpInst1, forCond1BB);
  
  //BasicBlock* for_body1
  StoreInst* jSI = new StoreInst((Value*) ci0, (Value*) jAI, forBody1BB);
  BranchInst::Create(forCond2BB,forBody1BB);
  
  //BasicBlock* for_cond2
  LoadInst* jLI = new LoadInst((Value*) jAI, "", forCond2BB);
  ICmpInst* icmpInst2 = new ICmpInst(*forCond2BB,CmpInst::ICMP_SLT,(Value*) iLI, (Value*) m2WidthLoadInst, "");
  BranchInst::Create(forBody2BB, forEnd2BB, icmpInst2, forCond2BB);
  
  //BasicBlock* for_body2
  StoreInst* kSI = new StoreInst((Value*) ci0, (Value*) kAI, forBody2BB);
  StoreInst* eleStoreInst = new StoreInst((Value*) ci0, (Value*) eleAllocaInst, forBody2BB);
  BranchInst::Create(forCond3BB,forBody2BB);
  
  //BasicBlock* for_cond3
  LoadInst* kLI = new LoadInst((Value*) kAI, "", forCond3BB);
  ICmpInst* icmpInst3 = new ICmpInst(*forCond3BB,CmpInst::ICMP_SLT,(Value*) kLI, (Value*) m1WidthLoadInst, "");
  BranchInst::Create(forBody3BB, forEnd1BB, icmpInst3, forCond3BB);
  
  //BasicBlock* for_body3
  LoadInst* eleLoadInst = new LoadInst((Value*) eleAllocaInst, "", forBody3BB);
  //load an element from the first matrix
  LoadInst* mat1AddrLoadInst = new LoadInst((Value*) mat1AddrAllocaInst, "", forBody3BB);
  BitCastInst* bitCastInst2 = new BitCastInst((Value*) mat1AddrLoadInst, l2PtrType, "", forBody3BB);
  LoadInst* iLI2 = new LoadInst((Value*) iAI, "", forBody3BB);
  BinaryOperator* bopMul1 = BinaryOperator::CreateNUWMul((Value*) m1WidthLoadInst, (Value*) iLI2, "", forBody3BB);
  GetElementPtrInst* getEPInst = GetElementPtrInst::CreateInBounds(i64Type, (Value*) bitCastInst2, bopMul1,"", forBody3BB);
  LoadInst* kLI2 = new LoadInst((Value*) kAI, "", forBody3BB);
  GetElementPtrInst* getEPInst2 = GetElementPtrInst::CreateInBounds(i64Type, (Value*) getEPInst, kLI2,"", forBody3BB);
  LoadInst* eLoadInst31 = new LoadInst((Value*) getEPInst2, "", forBody3BB);

  //load an element from the first matrix
  LoadInst* mat2AddrLoadInst = new LoadInst((Value*) mat2AddrAllocaInst, "", forBody3BB);
  BitCastInst* bitCastInst3 = new BitCastInst((Value*) mat2AddrLoadInst, l2PtrType, "", forBody3BB);
  LoadInst* kLI3 = new LoadInst((Value*) kAI, "", forBody3BB);
  BinaryOperator* bopMul3 = BinaryOperator::CreateNUWMul((Value*) m2WidthLoadInst, (Value*) kLI2, "", forBody3BB);
  GetElementPtrInst* getEPInst31 = GetElementPtrInst::CreateInBounds(i64Type, (Value*) bitCastInst3, bopMul3,"", forBody3BB);
  LoadInst* jLI3 = new LoadInst((Value*) jAI, "", forBody3BB);
  GetElementPtrInst* getEPInst32 = GetElementPtrInst::CreateInBounds(i64Type, (Value*) getEPInst31, jLI3,"", forBody3BB);
  LoadInst* eLoadInst32 = new LoadInst((Value*) getEPInst32, "", forBody3BB);
  BinaryOperator* bopMul32 = BinaryOperator::CreateNSWMul((Value*) eLoadInst31, (Value*) eLoadInst32, "", forBody3BB);
  BinaryOperator* bopAdd31 = BinaryOperator::CreateNSWAdd((Value*) bopMul32, (Value*) eleLoadInst, "", forBody3BB);
  StoreInst* eleStoreInst3 = new StoreInst((Value*) bopAdd31, (Value*) eleAllocaInst, forBody3BB);
  BranchInst::Create(forInc1BB,forBody3BB);
  
  //BasicBlock* for_inc1
  LoadInst* kLIInc1 = new LoadInst((Value*) kAI, "", forInc1BB);
  BinaryOperator* bopAddInc1 = BinaryOperator::CreateNSWAdd((Value*) kLIInc1, (Value*) ci1, "", forInc1BB);
  StoreInst* kSIInc1 = new StoreInst((Value*) bopAddInc1, (Value*) kAI, forInc1BB);
  BranchInst::Create(forCond3BB,forInc1BB);
  
  //BasicBlock* for_end1
  LoadInst* eleLoadInstEnd1 = new LoadInst((Value*) eleAllocaInst, "", forEnd1BB);
  LoadInst* jLIEnd1 = new LoadInst((Value*) jAI, "", forEnd1BB);
  LoadInst* iLIEnd1 = new LoadInst((Value*) iAI, "", forEnd1BB);
  BinaryOperator* bopMulEnd1 = BinaryOperator::CreateNSWMul((Value*) iLIEnd1, (Value*) m2WidthLoadInst, "", forEnd1BB);
  GetElementPtrInst* getEPInstEnd1 = GetElementPtrInst::CreateInBounds(i64Type, (Value*) vlaAllocaInst, bopMulEnd1,"", forEnd1BB);
  GetElementPtrInst* getEPInstEnd2 = GetElementPtrInst::CreateInBounds(i64Type, (Value*) getEPInstEnd1, jLIEnd1,"", forEnd1BB);
  StoreInst* eleStoreInstEnd1 = new StoreInst((Value*) eleLoadInstEnd1, (Value*) getEPInstEnd2, forEnd1BB);
  BranchInst::Create(forInc2BB,forEnd1BB);
  
  //BasicBlock* for_inc2
  LoadInst* jLIInc2 = new LoadInst((Value*) jAI, "", forInc2BB);
  BinaryOperator* bopAddInc2 = BinaryOperator::CreateNSWAdd((Value*) jLIInc2, (Value*) ci1, "", forInc2BB);
  StoreInst* jSIInc2 = new StoreInst((Value*) bopAddInc2, (Value*) jAI, forInc2BB);
  BranchInst::Create(forCond2BB,forInc2BB);
  
  //BasicBlock* for_end2
  BranchInst::Create(forInc3BB,forEnd2BB);
  
  //BasicBlock* for_inc3
  LoadInst* iLIInc3 = new LoadInst((Value*) iAI, "", forInc3BB);
  BinaryOperator* bopAddInc3 = BinaryOperator::CreateNSWAdd((Value*) iLIInc3, (Value*) ci1, "", forInc3BB);
  StoreInst* iSIInc3 = new StoreInst((Value*) bopAddInc3, (Value*) iAI, forInc3BB);
  BranchInst::Create(forCond1BB,forInc3BB);
  
  //BasicBlock* for_end3
  BitCastInst* bitCastInstEnd3 = new BitCastInst((Value*) vlaAllocaInst, l1PtrType, "", forEnd3BB);
  StoreInst* storeInstEnd3 = new StoreInst((Value*) bitCastInstEnd3, (Value*) retAllocaInst, forEnd3BB);
  BranchInst::Create(cleanupBB,forEnd3BB);

  //BasicBlock* cleanup
  LoadInst* retLoadInst= new LoadInst((Value*) retAllocaInst,"",cleanupBB); 
  ReturnInst* retInst = ReturnInst::Create(context, retLoadInst, cleanupBB);

  return funcMM;
}

*/
