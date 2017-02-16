#include"MatrixBranchProg.hpp"

using namespace llvm;
using namespace std;


class MatrixInIR{

private:
  const int width;
  const int height;
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
//Generate the IR that multiplies two matrix
//Choice that matters the program size: 
//1). a list of constants multiplication. 
//2). using loops in IR.
//3). using function in IR.
//The overhead 1>2>3. 
//We should implement the last one, otherwise, the matrix program will collapse by constant propagation.

/*
define i32** @MM(i32** %mat1, i32** %mat2) #0 {
entry:
  %retval = alloca i32**, align 8
  %mat1.addr = alloca i32**, align 8
  %mat2.addr = alloca i32**, align 8
  %m1height = alloca i32, align 4
  %m2height = alloca i32, align 4
  %m1width = alloca i32, align 4
  %m2width = alloca i32, align 4
  %saved_stack = alloca i8*, align 8
  %cleanup.dest.slot = alloca i32
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %ele = alloca i32, align 4
  %k = alloca i32, align 4
  store i32** %mat1, i32*** %mat1.addr, align 8
  store i32** %mat2, i32*** %mat2.addr, align 8
  store i32 1, i32* %m1height, align 4
  store i32 1, i32* %m2height, align 4
  store i32 2, i32* %m1width, align 4
  store i32 2, i32* %m2width, align 4
  %0 = load i32, i32* %m1height, align 4
  %1 = zext i32 %0 to i64
  %2 = load i32, i32* %m2width, align 4
  %3 = zext i32 %2 to i64
  %4 = call i8* @llvm.stacksave()
  store i8* %4, i8** %saved_stack, align 8
  %5 = mul nuw i64 %1, %3
  %vla = alloca i32, i64 %5, align 16
  %6 = load i32, i32* %m1width, align 4
  %7 = load i32, i32* %m2height, align 4
  %cmp = icmp ne i32 %6, %7
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %8 = bitcast i32* %vla to i32**
  store i32** %8, i32*** %retval, align 8
  store i32 1, i32* %cleanup.dest.slot, align 4
  br label %cleanup

if.end:                                           ; preds = %entry
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc33, %if.end
  %9 = load i32, i32* %i, align 4
  %10 = load i32, i32* %m1height, align 4
  %cmp1 = icmp slt i32 %9, %10
  br i1 %cmp1, label %for.body, label %for.end35

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %j, align 4
  br label %for.cond2

for.cond2:                                        ; preds = %for.inc30, %for.body
  %11 = load i32, i32* %j, align 4
  %12 = load i32, i32* %m2width, align 4
  %cmp3 = icmp slt i32 %11, %12
  br i1 %cmp3, label %for.body4, label %for.end32

for.body4:                                        ; preds = %for.cond2
  store i32 0, i32* %ele, align 4
  store i32 0, i32* %k, align 4
  br label %for.cond5

for.cond5:                                        ; preds = %for.inc, %for.body4
  %13 = load i32, i32* %k, align 4
  %14 = load i32, i32* %m1width, align 4
  %cmp6 = icmp slt i32 %13, %14
  br i1 %cmp6, label %for.body7, label %for.end

for.body7:                                        ; preds = %for.cond5
  %15 = load i32, i32* %k, align 4
  %cmp8 = icmp eq i32 %15, 0
  br i1 %cmp8, label %if.then9, label %if.else

if.then9:                                         ; preds = %for.body7
  %16 = load i32, i32* %k, align 4
  %idxprom = sext i32 %16 to i64
  %17 = load i32, i32* %i, align 4
  %idxprom10 = sext i32 %17 to i64
  %18 = load i32**, i32*** %mat1.addr, align 8
  %arrayidx = getelementptr inbounds i32*, i32** %18, i64 %idxprom10
  %19 = load i32*, i32** %arrayidx, align 8
  %arrayidx11 = getelementptr inbounds i32, i32* %19, i64 %idxprom
  %20 = load i32, i32* %arrayidx11, align 4
  %21 = load i32, i32* %j, align 4
  %idxprom12 = sext i32 %21 to i64
  %22 = load i32, i32* %k, align 4
  %idxprom13 = sext i32 %22 to i64
  %23 = load i32**, i32*** %mat2.addr, align 8
  %arrayidx14 = getelementptr inbounds i32*, i32** %23, i64 %idxprom13
  %24 = load i32*, i32** %arrayidx14, align 8
  %arrayidx15 = getelementptr inbounds i32, i32* %24, i64 %idxprom12
  %25 = load i32, i32* %arrayidx15, align 4
  %mul = mul nsw i32 %20, %25
  store i32 %mul, i32* %ele, align 4
  br label %if.end25

if.else:                                          ; preds = %for.body7
  %26 = load i32, i32* %ele, align 4
  %27 = load i32, i32* %k, align 4
  %idxprom16 = sext i32 %27 to i64
  %28 = load i32, i32* %i, align 4
  %idxprom17 = sext i32 %28 to i64
  %29 = load i32**, i32*** %mat1.addr, align 8
  %arrayidx18 = getelementptr inbounds i32*, i32** %29, i64 %idxprom17
  %30 = load i32*, i32** %arrayidx18, align 8
  %arrayidx19 = getelementptr inbounds i32, i32* %30, i64 %idxprom16
  %31 = load i32, i32* %arrayidx19, align 4
  %32 = load i32, i32* %j, align 4
  %idxprom20 = sext i32 %32 to i64
  %33 = load i32, i32* %k, align 4
  %idxprom21 = sext i32 %33 to i64
  %34 = load i32**, i32*** %mat2.addr, align 8
  %arrayidx22 = getelementptr inbounds i32*, i32** %34, i64 %idxprom21
  %35 = load i32*, i32** %arrayidx22, align 8
  %arrayidx23 = getelementptr inbounds i32, i32* %35, i64 %idxprom20
  %36 = load i32, i32* %arrayidx23, align 4
  %mul24 = mul nsw i32 %31, %36
  %add = add nsw i32 %26, %mul24
  store i32 %add, i32* %ele, align 4
  br label %if.end25

if.end25:                                         ; preds = %if.else, %if.then9
  br label %for.inc

for.inc:                                          ; preds = %if.end25
  %37 = load i32, i32* %j, align 4
  %inc = add nsw i32 %37, 1
  store i32 %inc, i32* %j, align 4
  br label %for.cond5

for.end:                                          ; preds = %for.cond5
  %38 = load i32, i32* %ele, align 4
  %39 = load i32, i32* %j, align 4
  %idxprom26 = sext i32 %39 to i64
  %40 = load i32, i32* %i, align 4
  %idxprom27 = sext i32 %40 to i64
  %41 = mul nsw i64 %idxprom27, %3
  %arrayidx28 = getelementptr inbounds i32, i32* %vla, i64 %41
  %arrayidx29 = getelementptr inbounds i32, i32* %arrayidx28, i64 %idxprom26
  store i32 %38, i32* %arrayidx29, align 4
  br label %for.inc30

for.inc30:                                        ; preds = %for.end
  %42 = load i32, i32* %j, align 4
  %inc31 = add nsw i32 %42, 1
  store i32 %inc31, i32* %j, align 4
  br label %for.cond2

for.end32:                                        ; preds = %for.cond2
  br label %for.inc33

for.inc33:                                        ; preds = %for.end32
  %43 = load i32, i32* %i, align 4
  %inc34 = add nsw i32 %43, 1
  store i32 %inc34, i32* %i, align 4
  br label %for.cond

for.end35:                                        ; preds = %for.cond
  %44 = bitcast i32* %vla to i32**
  store i32** %44, i32*** %retval, align 8
  store i32 1, i32* %cleanup.dest.slot, align 4
  br label %cleanup

cleanup:                                          ; preds = %for.end35, %if.then
  %45 = load i8*, i8** %saved_stack, align 8
  call void @llvm.stackrestore(i8* %45)
  %46 = load i32**, i32*** %retval, align 8
  ret i32** %46
}

*/
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
  int argNumChecker = 0;
  for (Function::arg_iterator arg = func->arg_begin(); arg!=func->arg_end(); ++arg){
    argNumChecker++;
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
