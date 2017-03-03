#include "llvm/Transforms/SymObf/MatrixBranchProg.h"
#include "matrix/MatUtils.h"

using namespace llvm;
using namespace std;

class MatrixInIR{

private:
  int width = 0;
  int height = 0;
  BasicBlock* bb;
  LLVMContext& context;
  Module& module;
  AllocaInst* matAI;

public:

  MatrixInIR(Module& module, LLVMContext& context, BasicBlock* bb, double** mat, const int height, const int width)
	  :module(module), context(context), bb(bb), height(height), width(width){
    LOG(L2_DEBUG) << "Constructing MatrixInIR...";
	ArrayType* fpAT = ArrayType::get(doubleType, width);
	ArrayType* arAT = ArrayType::get(fpAT, height);

	matAI = new AllocaInst(arAT,"", bb);
	for(int i=0; i<height; i++){
      ConstantInt* cii = (ConstantInt*) ConstantInt::getSigned(i64Type, i);
	  vector<Value*> ciVec;
	  ciVec.push_back(ci0);
	  ciVec.push_back(cii);
	  ArrayRef<Value*> ar0i(ciVec);
	  GetElementPtrInst* getEPInst = GetElementPtrInst::CreateInBounds(arAT, (Value*) matAI, ar0i,"", bb);

	  for(int j=0; j<width; j++){
        ConstantInt* cij = (ConstantInt*) ConstantInt::getSigned(i64Type,j);
	    vector<Value*> l2IdxVec;
	    l2IdxVec.push_back(ci0);
	    l2IdxVec.push_back(cij);
	    ArrayRef<Value*> ar0j(l2IdxVec);
	    GetElementPtrInst* l2GetPEInst = GetElementPtrInst::CreateInBounds(fpAT, (Value*) getEPInst, ar0j,"", bb);
		ConstantFP* fpTmp = (ConstantFP*) ConstantFP::get(doubleType, mat[i][j]);
		StoreInst* storeInst = new StoreInst((Value *) fpTmp, (Value *) l2GetPEInst, bb);

		const char strArg1[] = "%.3f,";
		PrintInIR(module, bb, strArg1, sizeof(strArg1), fpTmp);
	  }
 	  const char strArg1[] = "\n";
	  PrintInIR(module, bb, strArg1, sizeof(strArg1), nullptr);
	}
 	const char strArg1[] = "---------------------------\n";
	PrintInIR(module, bb, strArg1, sizeof(strArg1), nullptr);
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

  void GradEncode(){
    //TODO: to be implement
  }

  ~MatrixInIR(){
	/*
	for(int i=0; i<height; i++){
	  free(ciMat[i]);
	}
	if(ciMat != nullptr){
	  free(ciMat);
	}
	*/
  }

};


void ConvertIcmp2Mbp(Module& module, ICmpInst *icmpInst, Function* funcMM){
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
  //list<MatrixInIR*> matListInp0;
  //list<MatrixInIR*> matListInp1;

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

  int len = ciCmpObj->getBitWidth();
  int dim = len + 2;
  LOG(L_INFO) << "LEN:"<<len;

  ArrayType* fpAT = ArrayType::get(doubleType, dim);
  ArrayType* arAT = ArrayType::get(fpAT, dim);

  ArrayType* l2HeadMat = ArrayType::get(doubleType, dim);
  ArrayType* l1HeadMat = ArrayType::get(l2HeadMat, 1);
  
  ArrayType* l2TailMat = ArrayType::get(doubleType, 1);
  ArrayType* l1TailMat = ArrayType::get(l2TailMat, dim);
  //We create a matrix that points to ptrPT 
  ArrayType* ptrPtrAT = ArrayType::get(ptrPT, len);
  ArrayType* ptrPtrArAT = ArrayType::get(ptrPtrAT, 2);

  AllocaInst* matAI = new AllocaInst(ptrPtrArAT,"matrix", pBB);

  ConstantInt* ciDim = (ConstantInt*) ConstantInt::getSigned(i64Type,dim);
  ConstantInt* ciLen = (ConstantInt*) ConstantInt::getSigned(i64Type,len);

  //Get the pointers of matrix for input bit 0
  GetElementPtrInst* mat0PtrEPI = GetElementPtrInst::CreateInBounds(ptrPtrArAT, (Value*) matAI, ar00,"", pBB);
  //Get the pointers of matrix for input bit 1 
  GetElementPtrInst* mat1PtrEPI = GetElementPtrInst::CreateInBounds(ptrPtrArAT, (Value*) matAI, ar01,"", pBB);

  const char strArg_1[] = "ICmp constant: %.3f\n";
  PrintInIR(module, pBB, strArg_1, sizeof(strArg_1), ciCmpObj);

  //Generate the matrix and save to matAI
  double** headMat = (double**) malloc (sizeof(double*) * 1);
  double** headMatRand = (double**) malloc (sizeof(double*) * 1);
  double** tailMat = (double**) malloc (sizeof(double*) * dim);
  double** tailMatRand = (double**) malloc (sizeof(double*) * dim);
  double** midMat0 = (double**) malloc (sizeof(double*) * dim);
  double** midMat0Rand = (double**) malloc (sizeof(double*) * dim);
  double** midMat1 = (double**) malloc (sizeof(double*) * dim);
  double** midMat1Rand = (double**) malloc (sizeof(double*) * dim);

  headMat[0] = (double*) malloc (sizeof(double) * dim);
  headMatRand[0] = (double*) malloc (sizeof(double) * dim);
  for(int i=0; i<dim; i++){
    tailMat[i] = (double*) malloc (sizeof(double) * 1);
    tailMatRand[i] = (double*) malloc (sizeof(double) * 1);
	midMat0[i] = (double*) malloc (sizeof(double) * dim);
	midMat0Rand[i] = (double*) malloc (sizeof(double) * dim);
	midMat1[i] = (double*) malloc (sizeof(double) * dim);
	midMat1Rand[i] = (double*) malloc (sizeof(double) * dim);
  }

  CreateMat(headMat, 1, dim, 0, 0);
  CreateMat(tailMat, dim, 1, dim-1, 0);

  double** randMat = (double**) malloc (sizeof(double*) * dim);
  double** randMatInv = (double**) malloc (sizeof(double*) * dim);
  double** randMat2 = (double**) malloc (sizeof(double*) * dim);
  double** randMatInv2 = (double**) malloc (sizeof(double*) * dim);
  for (int i = 0; i < dim; i++)
  {
	randMat[i] = (double*) malloc (sizeof(double) * dim);
	randMatInv[i] = (double*) malloc (sizeof(double) * dim);
	randMat2[i] = (double*) malloc (sizeof(double) * dim);
	randMatInv2[i] = (double*) malloc (sizeof(double) * dim);
  }
  GenRandMatPair(randMat, randMatInv, dim, 1024);
  MatrixMult(headMat, randMat, headMatRand, 1, dim, dim, dim);
  PrintMat(headMatRand, 1, dim);
  
  MatrixInIR* headMatIR = new MatrixInIR(module, context, pBB, headMatRand, 1, dim); //~bit

  GenRandMatPair(randMat2, randMatInv2, dim, 1024);

  int iRow;
  int iCol4T;
  int iCol4F = len;
  for(int i=0; i<len; i++){
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
	  CreateMat(midMat0,dim,dim,iRow,iCol4F);
	  CreateMat(midMat1,dim,dim,iRow,iCol4T);
	}
	else{
	  CreateMat(midMat0,dim,dim,iRow,iCol4T);
	  CreateMat(midMat1,dim,dim,iRow,iCol4F);
	}

	if(i==0){
	  MatrixMult(randMatInv, midMat0, midMat0Rand, dim, dim, dim, dim);
	  MatrixMult(randMatInv, midMat1, midMat1Rand, dim, dim, dim, dim);
	}
	else{
	  MatrixMult(randMatInv2, midMat0, midMat0Rand, dim, dim, dim, dim);
	  MatrixMult(randMatInv2, midMat1, midMat1Rand, dim, dim, dim, dim);
	}

    CopyMat(midMat0Rand, midMat0, dim, dim); 
    CopyMat(midMat1Rand, midMat1, dim, dim); 

    GenRandMatPair(randMat2, randMatInv2, dim, 1024);
    MatrixMult(midMat0, randMat2, midMat0Rand, dim, dim, dim, dim);
    MatrixMult(midMat1, randMat2, midMat1Rand, dim, dim, dim, dim);

	MatrixInIR*  mat0 = new MatrixInIR(module, context, pBB, midMat0Rand, dim,dim); //bit
	MatrixInIR*  mat1 = new MatrixInIR(module, context, pBB, midMat1Rand, dim,dim); //~bit

    ConstantInt* cii = (ConstantInt*) ConstantInt::getSigned(i64Type,i);
    vector<Value*> vec0i;
    vec0i.push_back(ci0);
    vec0i.push_back(cii);
    ArrayRef<Value*> ar0i(vec0i);
    GetElementPtrInst* mat0EPI = GetElementPtrInst::CreateInBounds(ptrPtrAT, (Value*) mat0PtrEPI, ar0i,"", pBB);
    GetElementPtrInst* mat1EPI = GetElementPtrInst::CreateInBounds(ptrPtrAT, (Value*) mat1PtrEPI, ar0i,"", pBB);

    GetElementPtrInst* mat0iEPI = GetElementPtrInst::CreateInBounds(arAT, (Value*) mat0->getMatAI(), ar00,"", pBB);
    GetElementPtrInst* mat1iEPI = GetElementPtrInst::CreateInBounds(arAT, (Value*) mat1->getMatAI(), ar00,"", pBB);

    BitCastInst* mat0BCI = new BitCastInst((Value*) mat0iEPI, ptrPT, "", pBB);
    BitCastInst* mat1BCI = new BitCastInst((Value*) mat1iEPI, ptrPT, "", pBB);

    StoreInst* mat0StoreInst = new StoreInst(mat0BCI, (Value *) mat0EPI, pBB);
    StoreInst* mat1StoreInst = new StoreInst(mat1BCI, (Value *) mat1EPI, pBB);
  }

  MatrixMult(randMatInv2, tailMat, tailMatRand, dim, dim, dim, 1);
  MatrixInIR* tailMatIR = new MatrixInIR(module, context, pBB, tailMatRand, dim, 1); //bit

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
  
  const char strArg_10[] = "Inp: %d\n";
  PrintInIR(module, pBB, strArg_10, sizeof(strArg_10), matIdLI);

  vector<Value*> vec0li;
  vec0li.push_back(ci0);
  vec0li.push_back(matIdLI);
  ArrayRef<Value*> ar0li(vec0li);
  GetElementPtrInst* getBinEPI = GetElementPtrInst::CreateInBounds(ptrPtrArAT, (Value*) matAI, ar0li,"", pBB);
  GetElementPtrInst* getLenEPI = GetElementPtrInst::CreateInBounds(ptrPtrAT, (Value*) getBinEPI, ar00,"", pBB);
  LoadInst* ldMatLI = new LoadInst(getLenEPI,"",pBB);

  GetElementPtrInst* headMatEPI = GetElementPtrInst::CreateInBounds(l1HeadMat, (Value*) headMatIR->getMatAI(), ar00,"", pBB);
  BitCastInst* headMatBI = new BitCastInst((Value*) headMatEPI, ptrPT, "", pBB);
  AllocaInst* headMatAI = new AllocaInst(ptrPT,"", pBB);
  StoreInst* headMatSI = new StoreInst(headMatBI, (Value *) headMatAI, pBB);
  LoadInst* headMatLI = new LoadInst(headMatAI,"", pBB);

  AllocaInst* interMatAI = new AllocaInst(ptrPT,"interMat",pBB);
  
  vector<Value*> vecMM;
  vecMM.push_back(headMatLI);
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
  BinaryOperator* shlFbBO = BinaryOperator::Create(Instruction::Shl, (Value*) ci1, iFbLI, "", forBodyBB);
  BinaryOperator* andFbBO = BinaryOperator::Create(Instruction::And, (Value*) inp64, shlFbBO, "", forBodyBB);
  BinaryOperator* ashrFbBO = BinaryOperator::Create(Instruction::AShr, (Value*) andFbBO, iFbLI, "", forBodyBB);
  StoreInst* matIdFbSI = new StoreInst((Value *) ashrFbBO, matIdAI, forBodyBB);

  LoadInst* matIdFbLI = new LoadInst((Value *) matIdAI, "", forBodyBB);
  LoadInst* iFbLI02 = new LoadInst((Value*) iAI, "", forBodyBB);

  //const char strArg_11[] = "Inp: %.3f\n";
  //PrintInIR(module, forBodyBB, strArg_11, sizeof(strArg_11), matIdFbLI);

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

  GetElementPtrInst* tailMatEPI = GetElementPtrInst::CreateInBounds(l1TailMat, (Value*) tailMatIR->getMatAI(), ar00,"", conBB);
  BitCastInst* tailMatBI = new BitCastInst((Value*) tailMatEPI, ptrPT, "", conBB);
  AllocaInst* tailMatAI = new AllocaInst(ptrPT,"", conBB);
  StoreInst* tailMatSI = new StoreInst(tailMatBI, (Value *) tailMatAI, conBB);
  LoadInst* tailMatLI = new LoadInst(tailMatAI,"", conBB);

  LoadInst* interMatConLI = new LoadInst(interMatAI,"",conBB);

  vector<Value*> vecConMM;
  vecConMM.push_back(interMatConLI);
  vecConMM.push_back(tailMatLI);
  vecConMM.push_back(ci1);
  vecConMM.push_back(ciDim);
  vecConMM.push_back(ciDim);
  vecConMM.push_back(ci1);
  ArrayRef<Value*> arConMM(vecConMM);
  CallInst* mmConCI = CallInst::Create(funcMM, arConMM, "", conBB);
  StoreInst* interMatConSI = new StoreInst((Value*) mmConCI, interMatAI, "", conBB);
  
  AllocaInst* cmpAI = new AllocaInst(i64Type,"cmpAI", conBB);
  LoadInst* interMatConLI2 = new LoadInst(interMatAI,"",conBB);
  BitCastInst* cmpCI = new BitCastInst((Value*) interMatConLI2, fpPT, "", conBB);
  GetElementPtrInst* getCmpL1EPI = GetElementPtrInst::CreateInBounds(doubleType, (Value*) cmpCI, ci0,"", conBB);
  GetElementPtrInst* getCmpEPI = GetElementPtrInst::CreateInBounds(doubleType, (Value*) getCmpL1EPI, ci0,"", conBB);
  LoadInst* getCmpLI = new LoadInst(getCmpEPI,"",conBB);
  FPToSIInst* fp2SI = new FPToSIInst(getCmpLI, i64Type, "", conBB);
  StoreInst* cmpSI = new StoreInst((Value*) fp2SI, cmpAI, "", conBB);
  LoadInst* cmpLI = new LoadInst(cmpAI,"",conBB);

  const char strArg1[] = "ICmp Reuslt: %.3f\n";
  PrintInIR(module, conBB, strArg1, sizeof(strArg1), cmpLI);

  ICmpInst* conII = new ICmpInst(*conBB, CmpInst::ICMP_EQ, (Value*) cmpLI, ci1, "");
  BranchInst::Create(trueBB, falseBB, conII, conBB);
}
