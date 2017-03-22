#include "llvm/Transforms/SymObf/MatrixBranchProg.h"
#include "matrix/MatUtils.h"
#include "mmap/MMap.h"

using namespace llvm;
using namespace std;

class MatrixInIR{

private:
  int64_t width = 0;
  int64_t height = 0;
  BasicBlock* bb;
  LLVMContext& context;
  Module& module;
  AllocaInst* matAI;

public:

  MatrixInIR(Module& module, LLVMContext& context, BasicBlock* bb, int64_t** mat, const int64_t height, const int64_t width)
	  :module(module), context(context), bb(bb), height(height), width(width){
    LOG(L2_DEBUG) << "Constructing MatrixInIR...";
	ArrayType* i64AT = ArrayType::get(i64Type, width);
	ArrayType* arAT = ArrayType::get(i64AT, height);

	matAI = new AllocaInst(arAT,"", bb);
	for(int64_t i=0; i<height; i++){
      ConstantInt* cii = (ConstantInt*) ConstantInt::getSigned(i64Type, i);
	  vector<Value*> ciVec;
	  ciVec.push_back(ci0);
	  ciVec.push_back(cii);
	  ArrayRef<Value*> ar0i(ciVec);
	  GetElementPtrInst* getEPInst = GetElementPtrInst::CreateInBounds(arAT, (Value*) matAI, ar0i,"", bb);

	  for(int64_t j=0; j<width; j++){
        ConstantInt* cij = (ConstantInt*) ConstantInt::getSigned(i64Type,j);
	    vector<Value*> l2IdxVec;
	    l2IdxVec.push_back(ci0);
	    l2IdxVec.push_back(cij);
	    ArrayRef<Value*> ar0j(l2IdxVec);
	    GetElementPtrInst* l2GetPEInst = GetElementPtrInst::CreateInBounds(i64AT, (Value*) getEPInst, ar0j,"", bb);
		ConstantInt* i64Tmp = (ConstantInt*) ConstantInt::get(i64Type, mat[i][j]);
		StoreInst* storeInst = new StoreInst((Value *) i64Tmp, (Value *) l2GetPEInst, bb);
	  }
	}
  }

  AllocaInst* getMatAI(){
	return matAI; 
  }

  int64_t GetWidth(){
	return width;
  }

  int64_t GetHeight(){
	return height;
  }

  ~MatrixInIR(){
	/*
	for(int64_t i=0; i<height; i++){
	  free(ciMat[i]);
	}
	if(ciMat != nullptr){
	  free(ciMat);
	}
	*/
  }

};


void ConvertIcmp2Mbp(Module& module, ICmpInst *icmpInst){
  LOG(L2_DEBUG) << "ConvertIcmp2Mbp...";
  LLVMContext& context = icmpInst->getContext();
  Function* pFunc = icmpInst->getFunction();
  BasicBlock* pBB = icmpInst->getParent();


  ArrayRef<Value*> ar00(vec00);
  ArrayRef<Value*> ar01(vec01);

  //Create the blocks
  BasicBlock* forCondBB = pBB->splitBasicBlock(icmpInst, "for_cond_loop");
  pBB->getTerminator()->eraseFromParent();
  BasicBlock* forBodyBB = forCondBB->splitBasicBlock(icmpInst, "for_body_loop");
  forCondBB->getTerminator()->eraseFromParent();
  BasicBlock* forIncBB = forBodyBB->splitBasicBlock(icmpInst, "for_inc_loop");
  forBodyBB->getTerminator()->eraseFromParent();
  BasicBlock* forEndBB = forIncBB->splitBasicBlock(icmpInst, "for_end_loop");
  forIncBB->getTerminator()->eraseFromParent();
  BasicBlock* conBB = forEndBB->splitBasicBlock(icmpInst, "entry_continue");

  Type* type = icmpInst->getType();
  Instruction::OtherOps opCode = icmpInst->getOpcode();
  CmpInst::Predicate predicate = icmpInst->getPredicate();
  Value* op0 = icmpInst->getOperand(0);
  Value* op1 = icmpInst->getOperand(1);
  if(!(isa<ConstantInt> (*op0) || isa<ConstantInt>(*op1))){
	LOG(L_INFO) << "[-PI-]:ICmpInst has two symbolic variables; Don't obfuscate!";
  }

  ConstantInt* ciCmpObj;
  IntegerType* inp;

  if(isa<ConstantInt> (*op0)){
	ciCmpObj = (ConstantInt*)op0;
	inp = (IntegerType*) op1;
  }
  else{	
	ciCmpObj = (ConstantInt*) op1;
	inp = (IntegerType*) op0;
  }

  int64_t len = ciCmpObj->getBitWidth();
  int64_t dim = len + 2;
  LOG(L_INFO) << "LEN:"<<len;

  ArrayType* i64AT = ArrayType::get(i64Type, dim);
  ArrayType* arAT = ArrayType::get(i64AT, dim);

  ArrayType* l2HeadMat = ArrayType::get(i64Type, dim);
  ArrayType* l1HeadMat = ArrayType::get(l2HeadMat, 1);
  
  ArrayType* l2TailMat = ArrayType::get(i64Type, 1);
  ArrayType* l1TailMat = ArrayType::get(l2TailMat, dim);
  //We create a matrix that points to ptrPT 
  ArrayType* ptrPtrAT = ArrayType::get(ptrPT, len);
  ArrayType* ptrPtrArAT = ArrayType::get(ptrPtrAT, 2);

  AllocaInst* matAI = new AllocaInst(ptrPtrArAT,"matrix", pBB);

  ConstantInt* ciDim = (ConstantInt*) ConstantInt::getSigned(i64Type,dim);
  ConstantInt* ciLen = (ConstantInt*) ConstantInt::getSigned(i64Type,len);
  ConstantInt* ci8 = (ConstantInt*) ConstantInt::getSigned(i64Type,8);
  ConstantInt* ci8Dim = (ConstantInt*) ConstantInt::getSigned(i64Type,8*dim);

  //Get the pointers of matrix for input bit 0
  GetElementPtrInst* mat0PtrEPI = GetElementPtrInst::CreateInBounds(ptrPtrArAT, (Value*) matAI, ar00,"", pBB);
  //Get the pointers of matrix for input bit 1 
  GetElementPtrInst* mat1PtrEPI = GetElementPtrInst::CreateInBounds(ptrPtrArAT, (Value*) matAI, ar01,"", pBB);

  const char strArg_1[] = "ICmp constant: %d\n";
  PrintInIR(module, pBB, strArg_1, sizeof(strArg_1), ciCmpObj);

  //Generate the matrix and save to matAI
  int64_t** headMat = (int64_t**) malloc (sizeof(int64_t*) * 1);
  int64_t** headMatRand = (int64_t**) malloc (sizeof(int64_t*) * 1);
  int64_t** tailMat = (int64_t**) malloc (sizeof(int64_t*) * dim);
  int64_t** tailMatRand = (int64_t**) malloc (sizeof(int64_t*) * dim);
  int64_t** midMat0 = (int64_t**) malloc (sizeof(int64_t*) * dim);
  int64_t** midMat0Rand = (int64_t**) malloc (sizeof(int64_t*) * dim);
  int64_t** midMat1 = (int64_t**) malloc (sizeof(int64_t*) * dim);
  int64_t** midMat1Rand = (int64_t**) malloc (sizeof(int64_t*) * dim);

  headMat[0] = (int64_t*) malloc (sizeof(int64_t) * dim);
  headMatRand[0] = (int64_t*) malloc (sizeof(int64_t) * dim);
  for(int64_t i=0; i<dim; i++){
    tailMat[i] = (int64_t*) malloc (sizeof(int64_t) * 1);
    tailMatRand[i] = (int64_t*) malloc (sizeof(int64_t) * 1);
	midMat0[i] = (int64_t*) malloc (sizeof(int64_t) * dim);
	midMat0Rand[i] = (int64_t*) malloc (sizeof(int64_t) * dim);
	midMat1[i] = (int64_t*) malloc (sizeof(int64_t) * dim);
	midMat1Rand[i] = (int64_t*) malloc (sizeof(int64_t) * dim);
  }

  CreateIntMat(headMat, 1, dim, 0, 0);
  
  int64_t** randMat = (int64_t**) malloc (sizeof(int64_t*) * dim);
  int64_t** randMatInv = (int64_t**) malloc (sizeof(int64_t*) * dim);
  int64_t** randMat2 = (int64_t**) malloc (sizeof(int64_t*) * dim);
  int64_t** randMatInv2 = (int64_t**) malloc (sizeof(int64_t*) * dim);
  for (int64_t i = 0; i < dim; i++)
  {
	randMat[i] = (int64_t*) malloc (sizeof(int64_t) * dim);
	randMatInv[i] = (int64_t*) malloc (sizeof(int64_t) * dim);
	randMat2[i] = (int64_t*) malloc (sizeof(int64_t) * dim);
	randMatInv2[i] = (int64_t*) malloc (sizeof(int64_t) * dim);
  }
  
  GenIntMatPair(randMat, randMatInv, dim, mod);
  MultIntMatrix(headMat, randMat, headMatRand, 1, dim, dim, dim, mod);
  EncMatrix(headMatRand,1,dim,0);

  PrintIntMat(headMat, 1, dim);

  MatrixInIR* headMatIR = new MatrixInIR(module, context, pBB, headMatRand, 1, dim); //~bit
  //MatrixInIR* headMatIR = new MatrixInIR(module, context, pBB, headMat, 1, dim); //~bit

  int64_t iRow;
  int64_t iCol4T;
  int64_t iCol4F = len;
  for(int64_t i=0; i<len; i++){
	bool inp = ciCmpObj->getValue()[i];//It starts from the lower bit.
	LOG(L1_DEBUG)<<"+++++++++++++++++++++++++++++++Inp["<<i<<"]:"<<inp;

    iRow = i;

	if(i < len-1){
	  iCol4T = i+1;
	}
	else{
	  iCol4T = i+2;
	}

	if(inp){
	  CreateIntMat(midMat0,dim,dim,iRow,iCol4F);
	  CreateIntMat(midMat1,dim,dim,iRow,iCol4T);
	} else{
	  CreateIntMat(midMat0,dim,dim,iRow,iCol4T);
	  CreateIntMat(midMat1,dim,dim,iRow,iCol4F);
	}
	if(i==0){
	  MultIntMatrix(randMatInv, midMat0, midMat0Rand, dim, dim, dim, dim, mod);
	  MultIntMatrix(randMatInv, midMat1, midMat1Rand, dim, dim, dim, dim, mod);
	}
	else{
	  MultIntMatrix(randMatInv2, midMat0, midMat0Rand, dim, dim, dim, dim, mod);
	  MultIntMatrix(randMatInv2, midMat1, midMat1Rand, dim, dim, dim, dim, mod);
	}

    CopyIntMat(midMat0, midMat0Rand, dim, dim); 
    CopyIntMat(midMat1, midMat1Rand, dim, dim); 

    GenIntMatPair(randMat2, randMatInv2, dim, mod);
    MultIntMatrix(midMat0, randMat2, midMat0Rand, dim, dim, dim, dim, mod);
    MultIntMatrix(midMat1, randMat2, midMat1Rand, dim, dim, dim, dim, mod);

    EncMatrix(midMat0Rand,dim,dim, i+1);
    EncMatrix(midMat1Rand,dim,dim, i+1);

	MatrixInIR*  mat0IR = new MatrixInIR(module, context, pBB, midMat0Rand, dim,dim); //bit
	MatrixInIR*  mat1IR = new MatrixInIR(module, context, pBB, midMat1Rand, dim,dim); //~bit

	//MatrixInIR*  mat0IR = new MatrixInIR(module, context, pBB, midMat0, dim,dim); //bit
	//MatrixInIR*  mat1IR = new MatrixInIR(module, context, pBB, midMat1, dim,dim); //~bit
    //PrintIntMat(midMat0, dim, dim);
    //PrintIntMat(midMat1, dim, dim);

    ConstantInt* cii = (ConstantInt*) ConstantInt::getSigned(i64Type,i);
    vector<Value*> vec0i;
    vec0i.push_back(ci0);
    vec0i.push_back(cii);
    ArrayRef<Value*> ar0i(vec0i);
    GetElementPtrInst* mat0EPI = GetElementPtrInst::CreateInBounds(ptrPtrAT, (Value*) mat0PtrEPI, ar0i,"", pBB);
    GetElementPtrInst* mat1EPI = GetElementPtrInst::CreateInBounds(ptrPtrAT, (Value*) mat1PtrEPI, ar0i,"", pBB);

    GetElementPtrInst* mat0iEPI = GetElementPtrInst::CreateInBounds(arAT, (Value*) mat0IR->getMatAI(), ar00,"", pBB);
    GetElementPtrInst* mat1iEPI = GetElementPtrInst::CreateInBounds(arAT, (Value*) mat1IR->getMatAI(), ar00,"", pBB);

    BitCastInst* mat0BCI = new BitCastInst((Value*) mat0iEPI, ptrPT, "", pBB);
    BitCastInst* mat1BCI = new BitCastInst((Value*) mat1iEPI, ptrPT, "", pBB);

    StoreInst* mat0StoreInst = new StoreInst(mat0BCI, (Value *) mat0EPI, pBB);
    StoreInst* mat1StoreInst = new StoreInst(mat1BCI, (Value *) mat1EPI, pBB);
  }

  CreateIntMat(tailMat, dim, 1, dim-1, 0);
  MultIntMatrix(randMatInv2, tailMat, tailMatRand, dim, dim, dim, 1, mod);
  EncMatrix(tailMatRand,dim,1,dim);
  MatrixInIR* tailMatIR = new MatrixInIR(module, context, pBB, tailMatRand, dim, 1); //bit
  //MatrixInIR* tailMatIR = new MatrixInIR(module, context, pBB, tailMat, dim, 1); //bit
  //PrintIntMat(tailMat, dim, 1);

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
  
  const char strArg_IdLI[] = "Choose mat for inp: %d\n";
  PrintInIR(module, pBB, strArg_IdLI, sizeof(strArg_IdLI), matIdLI);

  vector<Value*> vec0inp;
  vec0inp.push_back(ci0);
  vec0inp.push_back(matIdLI);
  ArrayRef<Value*> ar0inp(vec0inp);
  GetElementPtrInst* getBinEPI = GetElementPtrInst::CreateInBounds(ptrPtrArAT, (Value*) matAI, ar0inp,"", pBB);
  GetElementPtrInst* getLenEPI = GetElementPtrInst::CreateInBounds(ptrPtrAT, (Value*) getBinEPI, ar00,"", pBB);
  LoadInst* ldMatLI = new LoadInst(getLenEPI,"",pBB);

  GetElementPtrInst* headMatEPI = GetElementPtrInst::CreateInBounds(l1HeadMat, (Value*) headMatIR->getMatAI(), ar00,"", pBB);
  BitCastInst* headMatBI = new BitCastInst((Value*) headMatEPI, ptrPT, "", pBB);
  AllocaInst* headMatAI = new AllocaInst(ptrPT,"", pBB);
  StoreInst* headMatSI = new StoreInst(headMatBI, (Value *) headMatAI, pBB);
  LoadInst* headMatLI = new LoadInst(headMatAI,"", pBB);

  // Allocate two inter matrix
  AllocaInst* iMatAI = new AllocaInst(ptrPT,"iMat",pBB);
  AllocaInst* iMat2AI = new AllocaInst(ptrPT,"iMat2",pBB);

  //CreateMalloc (BasicBlock *InsertAtEnd, Type *IntPtrTy, Type *AllocTy, Value *AllocSize, Value *ArraySize=nullptr, Function *MallocF=nullptr, const Twine &Name="")
  Instruction* mallocI11 = CallInst::CreateMalloc(iMatAI, i64Type, i8Type, ci8, nullptr, (Function*) mallocFunc, "mallocCall11");
  BitCastInst* mallocBI11 = new BitCastInst(mallocI11, ptrPT, "", pBB);
  StoreInst* mallocSI11 = new StoreInst(mallocBI11, iMatAI, pBB);

  Instruction* mallocI12 = CallInst::CreateMalloc(iMatAI, i64Type, i8Type, ci8Dim, nullptr, (Function*) mallocFunc, "mallocCall12");
  BitCastInst* mallocBI12 = new BitCastInst(mallocI12, i64PT, "", pBB);
  LoadInst* iMatLI = new LoadInst(iMatAI,"", pBB);
  GetElementPtrInst* iMatEPI1 = GetElementPtrInst::CreateInBounds(i64PT, iMatLI, ci0,"", pBB);
  StoreInst* mallocSI12 = new StoreInst(mallocBI12, iMatEPI1, pBB);

  Instruction* mallocI21 = CallInst::CreateMalloc(iMat2AI, i64Type, i8Type, ci8, nullptr, (Function*) mallocFunc, "mallocCall21");
  BitCastInst* mallocBI21 = new BitCastInst(mallocI21, ptrPT, "", pBB);
  StoreInst* mallocSI21 = new StoreInst(mallocBI21, iMat2AI, pBB);

  Instruction* mallocI22 = CallInst::CreateMalloc(iMat2AI, i64Type, i8Type, ci8Dim, nullptr, (Function*) mallocFunc, "mallocCall22");
  BitCastInst* mallocBI22 = new BitCastInst(mallocI22, i64PT, "", pBB);
  LoadInst* iMat2LI = new LoadInst(iMat2AI,"", pBB);
  GetElementPtrInst* iMatEPI2 = GetElementPtrInst::CreateInBounds(i64PT, iMat2LI, ci0,"", pBB);
  StoreInst* mallocSI22 = new StoreInst(mallocBI22, iMatEPI2, pBB);

  LOG(L1_DEBUG)<<"++++++++T3++++++++++++++++";
  vector<Value*> vecMM;
  vecMM.push_back(headMatLI);
  vecMM.push_back(ldMatLI);
  vecMM.push_back(iMat2LI);
  vecMM.push_back(ci1);
  vecMM.push_back(ciDim);
  vecMM.push_back(ciDim);
  vecMM.push_back(ciDim);
  vecMM.push_back(ciMod);
  ArrayRef<Value*> arMM(vecMM);

  CallInst* mmCI = CallInst::Create(multArMatFunc, arMM, "", pBB);

  AllocaInst* swapMatAI = new AllocaInst(ptrPT,"swapMat",pBB);
  StoreInst* mallocSI1 = new StoreInst(iMat2LI, swapMatAI, pBB);
  StoreInst* mallocSI2 = new StoreInst(iMatLI, iMat2AI, pBB);
  LoadInst* swapMatLI = new LoadInst(swapMatAI,"", pBB);
  StoreInst* mallocSI3 = new StoreInst(swapMatLI, iMatAI, pBB);

  BranchInst::Create(forCondBB, pBB);

  LoadInst* iLI = new LoadInst((Value*) iAI, "", forCondBB);
  LoadInst* lenLoadInst = new LoadInst((Value*) lenAllocaInst, "", forCondBB);
  ICmpInst* forII = new ICmpInst(*forCondBB,CmpInst::ICMP_SLT,(Value*) iLI, (Value*) lenLoadInst,"");
  BranchInst::Create(forBodyBB,forEndBB, forII, forCondBB);

  //For body

  LoadInst* iFbLI = new LoadInst((Value*) iAI, "", forBodyBB);
  BinaryOperator* shlFbBO = BinaryOperator::Create(Instruction::Shl, (Value*) ci1, iFbLI, "", forBodyBB);
  BinaryOperator* andFbBO = BinaryOperator::Create(Instruction::And, (Value*) inp64, shlFbBO, "", forBodyBB);
  BinaryOperator* ashrFbBO = BinaryOperator::Create(Instruction::AShr, (Value*) andFbBO, iFbLI, "", forBodyBB);
  StoreInst* matIdFbSI = new StoreInst((Value *) ashrFbBO, matIdAI, forBodyBB);

  LoadInst* matIdFbLI = new LoadInst((Value *) matIdAI, "", forBodyBB);
  LoadInst* iFbLI02 = new LoadInst((Value*) iAI, "", forBodyBB);

  PrintInIR(module, forBodyBB, strArg_IdLI, sizeof(strArg_IdLI), matIdFbLI);

  vector<Value*> vecFb0I0;
  vecFb0I0.push_back(ci0);
  vecFb0I0.push_back(matIdFbLI);
  ArrayRef<Value*> arFb0I0(vecFb0I0);

  vector<Value*> vecFb0I1;
  vecFb0I1.push_back(ci0);
  vecFb0I1.push_back(iFbLI02);
  ArrayRef<Value*> arFb0I1(vecFb0I1);

  GetElementPtrInst* getBinFbEPI = GetElementPtrInst::CreateInBounds(ptrPtrArAT, (Value*) matAI, arFb0I0,"", forBodyBB);
  GetElementPtrInst* getLenFbEPI = GetElementPtrInst::CreateInBounds(ptrPtrAT, (Value*) getBinFbEPI, arFb0I1,"", forBodyBB);

  LoadInst* ldFbMatLI = new LoadInst(getLenFbEPI,"",forBodyBB);
  LoadInst* iMatFbLI = new LoadInst(iMatAI,"",forBodyBB);
  LoadInst* iMat2FbLI = new LoadInst(iMat2AI,"",forBodyBB);

  vector<Value*> vecFbMM;
  vecFbMM.push_back(iMatFbLI);
  vecFbMM.push_back(ldFbMatLI);
  vecFbMM.push_back(iMat2FbLI);
  vecFbMM.push_back(ci1);
  vecFbMM.push_back(ciDim);
  vecFbMM.push_back(ciDim);
  vecFbMM.push_back(ciDim);
  vecFbMM.push_back(ciMod);
  ArrayRef<Value*> arFbMM(vecFbMM);
  CallInst* mmFbCI = CallInst::Create(multMatFunc, arFbMM, "", forBodyBB);
  StoreInst* mallocFbSI1 = new StoreInst(iMat2FbLI, swapMatAI, forBodyBB);
  StoreInst* mallocFbSI2 = new StoreInst(iMatFbLI, iMat2AI, forBodyBB);
  LoadInst* swapMatFbLI = new LoadInst(swapMatAI,"", forBodyBB);
  StoreInst* mallocFbSI3 = new StoreInst(swapMatFbLI, iMatAI, forBodyBB);
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

  GetElementPtrInst* tailMatEPI = GetElementPtrInst::CreateInBounds(l1TailMat, (Value*) tailMatIR->getMatAI(), ar00,"", conBB);
  BitCastInst* tailMatBI = new BitCastInst((Value*) tailMatEPI, ptrPT, "", conBB);
  AllocaInst* tailMatAI = new AllocaInst(ptrPT,"", conBB);
  StoreInst* tailMatSI = new StoreInst(tailMatBI, (Value *) tailMatAI, conBB);
  LoadInst* tailMatLI = new LoadInst(tailMatAI,"", conBB);

  LoadInst* iMatConLI = new LoadInst(iMatAI,"",conBB);
  LoadInst* iMat2ConLI = new LoadInst(iMat2AI,"",conBB);

  vector<Value*> vecConMM;
  vecConMM.push_back(iMatConLI);
  vecConMM.push_back(tailMatLI);
  vecConMM.push_back(iMat2ConLI);
  vecConMM.push_back(ci1);
  vecConMM.push_back(ciDim);
  vecConMM.push_back(ciDim);
  vecConMM.push_back(ci1);
  vecConMM.push_back(ciMod);
  ArrayRef<Value*> arConMM(vecConMM);
  CallInst* mmConCI = CallInst::Create(multMatFunc, arConMM, "", conBB);
  
  AllocaInst* cmpAI = new AllocaInst(i64Type,"cmpAI", conBB);
  LoadInst* iMatConLI2 = new LoadInst(iMat2AI,"",conBB);
  GetElementPtrInst* getCmpL1EPI = GetElementPtrInst::CreateInBounds(i64PT, iMatConLI2, ci0,"", conBB);
  LoadInst* getCmpL1LI = new LoadInst(getCmpL1EPI,"",conBB);
  GetElementPtrInst* getCmpEPI = GetElementPtrInst::CreateInBounds(i64Type, getCmpL1LI, ci0,"", conBB);
  LoadInst* getCmpLI = new LoadInst(getCmpEPI,"",conBB);
  StoreInst* cmpSI = new StoreInst((Value*) getCmpLI, cmpAI, "", conBB);
  LoadInst* cmpLI = new LoadInst(cmpAI,"",conBB);

  ConstantInt* expCI = (ConstantInt*) ConstantInt::getSigned(i64Type, GetExp());
  AllocaInst* expAI = new AllocaInst(i64Type,"expAI", conBB);
  StoreInst* expSI = new StoreInst(expCI, expAI, "", conBB);
  LoadInst* expLI = new LoadInst(expAI,"",conBB);
  ConstantInt* qCI = (ConstantInt*) ConstantInt::getSigned(i64Type, GetQ());
  AllocaInst* qAI = new AllocaInst(i64Type,"qAI", conBB);
  StoreInst* qSI = new StoreInst(qCI, qAI, "", conBB);
  LoadInst* qLI = new LoadInst(qAI,"",conBB);
  ConstantInt* pztCI = (ConstantInt*) ConstantInt::getSigned(i64Type, GetPzt());
  AllocaInst* pztAI = new AllocaInst(i64Type,"pztAI", conBB);
  StoreInst* pztSI = new StoreInst(pztCI, pztAI, "", conBB);
  LoadInst* pztLI = new LoadInst(pztAI,"",conBB);

  vector<Value*> vecConZT;
  vecConZT.push_back(cmpLI);
  vecConZT.push_back(pztLI);
  vecConZT.push_back(qLI);
  vecConZT.push_back(expLI);
  ArrayRef<Value*> arConZT(vecConZT);
  CallInst* ztConCI = CallInst::Create(zeroTestFunc, arConZT, "", conBB);

  const char strArg1[] = "ICmp Reuslt: %d\n";
  PrintInIR(module, conBB, strArg1, sizeof(strArg1), ztConCI);

  ICmpInst* conII = new ICmpInst(*conBB, CmpInst::ICMP_EQ, (Value*) ztConCI, ci1, "");
  BranchInst::Create(trueBB, falseBB, conII, conBB);

  free(headMat[0]);
  free(headMatRand[0]);
  for(int64_t i=0; i<dim; i++){
    free(tailMat[i]);
    free(tailMatRand[i]);
	free(midMat0[i]);
	free(midMat0Rand[i]);
	free(midMat1[i]);
	free(midMat1Rand[i]);
  }
  free(headMat);
  free(headMatRand);
  free(tailMat);
  free(tailMatRand);
  free(midMat0);
  free(midMat0Rand);
  free(midMat1);
  free(midMat1Rand);
}
