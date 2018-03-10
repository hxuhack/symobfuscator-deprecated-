#include "llvm/Transforms/Obfuscation/BogusControlFlow.h"
#include "llvm/Transforms/Obfuscation/Utils.h"
#include "llvm/Transforms/Obfuscation/Logger.h"

// Stats
#define DEBUG_TYPE "BogusControlFlow"
STATISTIC(NumFunction,  "a. Number of functions in this module");
STATISTIC(NumTimesOnFunctions, "b. Number of times we run on each function");
STATISTIC(InitNumBasicBlocks,  "c. Initial number of basic blocks in this module");
STATISTIC(NumModifiedBasicBlocks,  "d. Number of modified basic blocks");
STATISTIC(NumAddedBasicBlocks,  "e. Number of added basic blocks in this module");
STATISTIC(FinalNumBasicBlocks,  "f. Final number of basic blocks in this module");

// Options for the pass
const int defaultObfRate = 3, defaultObfTime = 1;

IntegerType *i1Type, *i8Type, * i16Type, *i32Type, *i64Type, *boolType;
Type *voidType, *floatType, *doubleType;
StructType *ioMarkerType, *ioFileType;
PointerType *i8PT, *ioMarkerPT, *ioFilePT, *fpPT, *i32PT, *i64PT, *l2Ptr_64, *l3Ptr_64, *l4Ptr_64;
Constant *atoiFunc, *fopenFunc, *mcpyFunc, *fprintfFunc, *fcloseFunc, *fscanfFunc, *printFunc, *mallocFunc, *pipeFunc, *forkFunc, *sprintfFunc, *closeFunc, *readFunc, *writeFunc, *exitFunc;
Constant *multMatFunc, *threadpropFunc, *forkpropFunc, *covertpropFunc;
ConstantInt *ci0_64, *ci1_64, *ci2_64, *ci3_64, *ci4_64, *ci5_64, *ci6_64, *ci7_64, *ci8_64, *ci9_64, *ci10_64, *ci11_64, *bFalse;
ConstantInt *ci0_32, *ci1_32, *ci2_32, *ci3_32, *ci4_32, *ci5_32, *ci6_32, *ci7_32, *ci8_32, *ci9_32, *ci10_32, *ci11_32;
GlobalVariable *fileGV, *attrGV, *attrGV2, *attrGV3;
vector<Value*> vec00_32,vec01_32;
vector<Value*> vec00_64,vec01_64;

loglevel_e loglevel = L3_DEBUG;
int complex_control = 0;
int obf_control = 0;

//set the type of opaque predicate; for evaluation purpose
static cl::opt<int>
OpqType("OpqType", cl::desc("Choose the opaque predicate type"), cl::value_desc("opaque predicate type"), cl::init(0), cl::Optional);

static cl::opt<int>
OpqNum("OpqNum", cl::desc("Set the number of opaque predicates"), cl::value_desc("opaque predicate number"), cl::init(0), cl::Optional);

static cl::opt<int>
ObfProbRate("boguscf-prob", cl::desc("Choose the probability [%] each basic blocks will be obfuscated by the -bcf pass"), cl::value_desc("probability rate"), cl::init(defaultObfRate), cl::Optional);

static cl::opt<int>
ObfTimes("boguscf-loop", cl::desc("Choose how many time the -bcf pass loop on a function"), cl::value_desc("number of times"), cl::init(defaultObfTime), cl::Optional);

namespace {
	struct BogusControlFlow : public FunctionPass {
		static char ID; // Pass identification
		bool flag;
		BogusControlFlow() : FunctionPass(ID) {}
		BogusControlFlow(bool flag) : FunctionPass(ID) {this->flag = flag; BogusControlFlow();}

		/* runOnFunction
		 *
		 * Overwrite FunctionPass method to apply the transformation
		 * to the function. See header for more details.
		 */
		virtual bool runOnFunction(Function &F){
			// Check if the percentage is correct

			Module& M = *(F.getParent());
			LLVMContext& context = M.getContext();

			if (ObfTimes <= 0) {
				context.emitError(Twine ("BogusControlFlow application number -boguscf-loop=x must be x > 0"));
			}

			// Check if the number of applications is correct
			if ( !((ObfProbRate > 0) && (ObfProbRate <= 100)) ) {
				context.emitError(Twine ("BogusControlFlow application basic blocks percentage -boguscf-prob=x must be 0 < x <= 100"));
			}

			voidType = Type::getVoidTy(context);
			i1Type = IntegerType::get(context, 1);
			i8Type = IntegerType::getInt8Ty(context);
			i16Type = IntegerType::getInt16Ty(context);
			i32Type = IntegerType::getInt32Ty(context);
			i64Type = IntegerType::getInt64Ty(context);
			i8PT = PointerType::getUnqual(i8Type);
			i32PT = PointerType::getUnqual(i32Type);
			i64PT = PointerType::getUnqual(i64Type);
			l2Ptr_64 = PointerType::getUnqual(i64PT);
			l3Ptr_64 = PointerType::getUnqual(l2Ptr_64);
			l4Ptr_64 = PointerType::getUnqual(l3Ptr_64);
			floatType = Type::getFloatTy(context);
			doubleType = Type::getDoubleTy(context);

			ci0_32 = (ConstantInt*) ConstantInt::getSigned(i32Type, 0);
			ci1_32 = (ConstantInt*) ConstantInt::getSigned(i32Type, 1);
			ci2_32 = (ConstantInt*) ConstantInt::getSigned(i32Type, 2);
			ci3_32 = (ConstantInt*) ConstantInt::getSigned(i32Type, 3);
			ci4_32 = (ConstantInt*) ConstantInt::getSigned(i32Type, 4);
			ci5_32 = (ConstantInt*) ConstantInt::getSigned(i32Type, 5);
			ci6_32 = (ConstantInt*) ConstantInt::getSigned(i32Type, 6);
			ci7_32 = (ConstantInt*) ConstantInt::getSigned(i32Type, 7);
			ci8_32 = (ConstantInt*) ConstantInt::getSigned(i32Type, 8);
			ci9_32 = (ConstantInt*) ConstantInt::getSigned(i32Type, 9);
			ci10_32 = (ConstantInt*) ConstantInt::getSigned(i32Type, 10);
			ci11_32 = (ConstantInt*) ConstantInt::getSigned(i32Type, 11);
			bFalse = (ConstantInt*) ConstantInt::getSigned(i1Type, 0);
			ci0_64 = (ConstantInt*) ConstantInt::getSigned(i64Type, 0);
			ci1_64 = (ConstantInt*) ConstantInt::getSigned(i64Type, 1);
			ci2_64 = (ConstantInt*) ConstantInt::getSigned(i64Type, 2);
			ci3_64 = (ConstantInt*) ConstantInt::getSigned(i64Type, 3);
			ci4_64 = (ConstantInt*) ConstantInt::getSigned(i64Type, 4);
			ci5_64 = (ConstantInt*) ConstantInt::getSigned(i64Type, 5);
			ci6_64 = (ConstantInt*) ConstantInt::getSigned(i64Type, 6);
			ci7_64 = (ConstantInt*) ConstantInt::getSigned(i64Type, 7);
			ci8_64 = (ConstantInt*) ConstantInt::getSigned(i64Type, 8);
			ci9_64 = (ConstantInt*) ConstantInt::getSigned(i64Type, 9);
			ci10_64 = (ConstantInt*) ConstantInt::getSigned(i64Type, 10);
			ci11_64 = (ConstantInt*) ConstantInt::getSigned(i64Type, 11);

			vec00_64.push_back(ci0_64);
			vec00_64.push_back(ci0_64);
			vec01_64.push_back(ci0_64);
			vec01_64.push_back(ci1_64);

			vec00_32.push_back(ci0_32);
			vec00_32.push_back(ci0_32);
			ArrayRef<Value*> ar00(vec00_32);
			vec01_32.push_back(ci0_32);
			vec01_32.push_back(ci1_32);
			ArrayRef<Value*> ar01(vec01_32);

			Constant* fileVal = ConstantDataArray::getString(context, "tmp.covpro\00", true);
			Constant* attrVal = ConstantDataArray::getString(context, "w+\00", true);
			Constant* attrVal2 = ConstantDataArray::getString(context, "r\00", true);
			Constant* attrVal3 = ConstantDataArray::getString(context, "%d\00", true);
			fileGV = new GlobalVariable(M, fileVal->getType(), false, GlobalValue::PrivateLinkage, 0, "file.name");
			attrGV = new GlobalVariable(M, attrVal->getType(), false, GlobalValue::PrivateLinkage, 0, "attribute_rw");
			attrGV2 = new GlobalVariable(M, attrVal2->getType(), false, GlobalValue::PrivateLinkage, 0, "attribute_r");
			attrGV3 = new GlobalVariable(M, attrVal3->getType(), false, GlobalValue::PrivateLinkage, 0, "attribute_%d");
			fileGV->setAlignment(1);
			attrGV->setAlignment(1);
			attrGV2->setAlignment(1);
			attrGV3->setAlignment(1);
			fileGV->setUnnamedAddr(GlobalValue::UnnamedAddr::Global);
			attrGV->setUnnamedAddr(GlobalValue::UnnamedAddr::Global);
			attrGV2->setUnnamedAddr(GlobalValue::UnnamedAddr::Global);
			attrGV3->setUnnamedAddr(GlobalValue::UnnamedAddr::Global);
			fileGV->setConstant(true);
			attrGV->setConstant(true);
			attrGV2->setConstant(true);
			attrGV3->setConstant(true);
			fileGV->setInitializer(fileVal);
			attrGV->setInitializer(attrVal);
			attrGV2->setInitializer(attrVal2);
			attrGV3->setInitializer(attrVal3);

			ioFileType = M.getTypeByName("struct._IO_FILE");
			if(!ioFileType){
				ioFileType = StructType::create(context, "struct._IO_FILE");
			}
			ioMarkerType = M.getTypeByName("struct._IO_marker");
			if(!ioMarkerType){
				ioMarkerType = StructType::create(context, "struct._IO_marker");
			}
			ioMarkerPT = PointerType::getUnqual(ioMarkerType);
			ioFilePT = PointerType::getUnqual(ioFileType);

			ArrayType* i8L1AT = ArrayType::get(i8Type, 1);
			ArrayType* i8L20AT = ArrayType::get(i8Type, 20);
			vector<Type*> vcFile, vcMarker;
			vcFile.push_back(i32Type);
			vcFile.push_back(i8PT);
			vcFile.push_back(i8PT);
			vcFile.push_back(i8PT);
			vcFile.push_back(i8PT);
			vcFile.push_back(i8PT);
			vcFile.push_back(i8PT);
			vcFile.push_back(i8PT);
			vcFile.push_back(i8PT);
			vcFile.push_back(i8PT);
			vcFile.push_back(i8PT);
			vcFile.push_back(i8PT);
			vcFile.push_back(ioMarkerPT);
			vcFile.push_back(ioFilePT);
			vcFile.push_back(i32Type);
			vcFile.push_back(i32Type);
			vcFile.push_back(i64Type);
			vcFile.push_back(i16Type);
			vcFile.push_back(i8Type);
			vcFile.push_back(i8L1AT);
			vcFile.push_back(i8PT);
			vcFile.push_back(i64Type);
			vcFile.push_back(i8PT);
			vcFile.push_back(i8PT);
			vcFile.push_back(i8PT);
			vcFile.push_back(i8PT);
			vcFile.push_back(i64Type);
			vcFile.push_back(i32Type);
			vcFile.push_back(i8L20AT);
			ArrayRef<Type*> arFile(vcFile);
			vcMarker.push_back(ioMarkerPT);
			vcMarker.push_back(ioFilePT);
			vcMarker.push_back(i32Type);
			ArrayRef<Type*> arMarker(vcMarker);

			if(ioFileType->isOpaque()){
				ioFileType->setBody(arFile, false);
			}
			if(ioMarkerType->isOpaque()){
				ioMarkerType->setBody(arMarker, false);
			}

			printFunc = M.getFunction("printf"); 
			mallocFunc = M.getFunction("malloc"); 

			vector<Type*> paramVec;
			paramVec.push_back((Type *) l2Ptr_64); //mat1
			paramVec.push_back((Type *) l2Ptr_64); //mat2
			paramVec.push_back((Type *) l2Ptr_64); //result
			paramVec.push_back(i64Type);		//m1Height
			paramVec.push_back(i64Type);		//m1Width
			paramVec.push_back(i64Type);		//m2Height
			paramVec.push_back(i64Type);		//m2Width
			paramVec.push_back(i64Type);		//mod
			ArrayRef<Type*> paramArrayType(paramVec);
			FunctionType* funcType = FunctionType::get(i64Type, paramArrayType, false);
			multMatFunc = M.getOrInsertFunction("MultIntMatrix", funcType); 

			vector<Type*> forkPropVec;
			forkPropVec.push_back(i64Type);
			ArrayRef<Type*> arForkProp(forkPropVec);
			funcType = FunctionType::get(i64Type, arForkProp, false);
			forkpropFunc = M.getOrInsertFunction("ForkProp", funcType); 

			vector<Type*> tpropVec;
			tpropVec.push_back(i64Type);
			ArrayRef<Type*> arTprop(tpropVec);
			funcType = FunctionType::get(i64Type, arTprop, false);
			threadpropFunc = M.getOrInsertFunction("ThreadProp", funcType); 

			vector<Type*> covertPropVec;
			covertPropVec.push_back(i64Type);
			ArrayRef<Type*> covertProp(covertPropVec);
			funcType = FunctionType::get(i64Type, covertProp, false);
			covertpropFunc = M.getOrInsertFunction("CovertProp", funcType);

			fopenFunc = M.getFunction("fopen");
			if(!fopenFunc){
				vector<Type*> fopenPV;
				fopenPV.push_back(i8PT);
				fopenPV.push_back(i8PT);
				ArrayRef<Type*> fopenAR(fopenPV);
				FunctionType* fopenFT = FunctionType::get(ioFilePT, fopenAR, false);
				fopenFunc = M.getOrInsertFunction("fopen",fopenFT);
			}
			mcpyFunc = M.getFunction("llvm.memcpy.p0i8.p0i8.i64");
			if(!mcpyFunc){
				vector<Type*> mcpyPV;
				mcpyPV.push_back(i8PT);
				mcpyPV.push_back(i8PT);
				mcpyPV.push_back(i64Type);
				mcpyPV.push_back(i32Type);
				mcpyPV.push_back(i1Type);
				ArrayRef<Type*> mcpyAR(mcpyPV);
				FunctionType* mcpyFT = FunctionType::get(Type::getVoidTy(context), mcpyAR, false);
				mcpyFunc =M.getOrInsertFunction("llvm.memcpy.p0i8.p0i8.i64", mcpyFT);
			}
			fprintfFunc = M.getFunction("fprintf");
			if(!fprintfFunc){
				vector<Type*> fprintfVec;
				fprintfVec.push_back(ioFilePT);
				fprintfVec.push_back(i8PT);
				ArrayRef<Type*> fprintfAR(fprintfVec);
				FunctionType* fprintfFT = FunctionType::get(i32Type, fprintfAR, true);
				fprintfFunc = M.getOrInsertFunction("fprintf", fprintfFT);
			}

			fscanfFunc = M.getFunction("fscanf");
			if(!fscanfFunc){
				vector<Type*> fscanfVec;
				fscanfVec.push_back(ioFilePT);
				fscanfVec.push_back(i8PT);
				ArrayRef<Type*> fscanfAR(fscanfVec);
				FunctionType* fscanfFT = FunctionType::get(i32Type, fscanfAR, true);
				fscanfFunc = M.getOrInsertFunction("fscanf", fscanfFT);
			}

			fcloseFunc = M.getFunction("fclose");
			if(!fcloseFunc){
				vector<Type*> fcloseVec;
				fcloseVec.push_back(ioFilePT);
				ArrayRef<Type*> fcloseAR(fcloseVec);
				FunctionType* fcloseFT = FunctionType::get(i32Type, fcloseAR, false);
				fcloseFunc = M.getOrInsertFunction("fclose", fcloseFT);
			}

			forkFunc = M.getFunction("fork");
			if(!forkFunc){
				vector<Type*> forkVec;
				FunctionType* forkFT = FunctionType::get(i32Type, forkVec, false);
				forkFunc = M.getOrInsertFunction("fork", forkFT);
			}

			pipeFunc = M.getFunction("pipe");
			if(!pipeFunc){
				vector<Type*> pipeVec;
				pipeVec.push_back(i32PT);
				FunctionType* pipeFT = FunctionType::get(i32Type, pipeVec, false);
				pipeFunc = M.getOrInsertFunction("pipe", pipeFT);
			}

			atoiFunc = M.getFunction("atoi");
			if(!atoiFunc){
				vector<Type*> atoiVec;
				FunctionType* atoiFT = FunctionType::get(i32Type, atoiVec, true);
				atoiFunc = M.getOrInsertFunction("atoi", atoiFT);
			}

			closeFunc = M.getFunction("close");
			if(!closeFunc){
				vector<Type*> closeVec;
				closeVec.push_back(i32Type);
				FunctionType* closeFT = FunctionType::get(i32Type, closeVec, false);
				closeFunc = M.getOrInsertFunction("close", closeFT);
			}

			sprintfFunc = M.getFunction("sprintf");
			if(!sprintfFunc){
				vector<Type*> sprintfVec;
				sprintfVec.push_back(i8PT);
				sprintfVec.push_back(i8PT);
				FunctionType* sprintfFT = FunctionType::get(i32Type, sprintfVec, true);
				sprintfFunc = M.getOrInsertFunction("sprintf", sprintfFT);
			}

			readFunc = M.getFunction("read");
			if(!readFunc){
				vector<Type*> readVec;
				readVec.push_back(i32Type);
				readVec.push_back(i8PT);
				readVec.push_back(i64Type);
				FunctionType* readFT = FunctionType::get(i64Type, readVec, false);
				readFunc = M.getOrInsertFunction("read", readFT);
			}

			writeFunc = M.getFunction("write");
			if(!writeFunc){
				vector<Type*> writeVec;
				writeVec.push_back(i32Type);
				writeVec.push_back(i8PT);
				writeVec.push_back(i64Type);
				FunctionType* writeFT = FunctionType::get(i64Type, writeVec, false);
				writeFunc = M.getOrInsertFunction("write", writeFT);
			}

			exitFunc = M.getFunction("exit");
			if(!exitFunc){
				vector<Type*> exitVec;
				exitVec.push_back(i32Type);
				FunctionType* exitFT = FunctionType::get(voidType, exitVec, false);
				exitFunc = M.getOrInsertFunction("exit", exitFT);
			}
			// If fla annotations
			if(toObfuscate(flag, &F, "bcf")) 
			{
				errs() << "----------------------------------------";
				errs() << F;
				bogus(F);
				errs() << "----------------------------------------";
				errs() << F;
				doF(F);
				errs() << "----------------------------------------";
				errs() << F;
				return true;
			}
			return false;
		} // end of runOnFunction()

		void bogus(Function &F) {
			// For statistics and debug
			++NumFunction;
			int NumBasicBlocks = 0;
			bool firstTime = true; // First time we do the loop in this function
			bool hasBeenModified = false;
			DEBUG_WITH_TYPE("opt", errs() << "bcf: Started on function " << F.getName() << "\n");
			DEBUG_WITH_TYPE("opt", errs() << "bcf: Probability rate: "<< ObfProbRate<< "\n");
			if(ObfProbRate < 0 || ObfProbRate > 100){
				DEBUG_WITH_TYPE("opt", errs() << "bcf: Incorrect value,"
							<< " probability rate set to default value: "
							<< defaultObfRate <<" \n");
				ObfProbRate = defaultObfRate;
			}
			DEBUG_WITH_TYPE("opt", errs() << "bcf: How many times: "<< ObfTimes<< "\n");
			if(ObfTimes <= 0){
				DEBUG_WITH_TYPE("opt", errs() << "bcf: Incorrect value,"
							<< " must be greater than 1. Set to default: "
							<< defaultObfTime <<" \n");
				ObfTimes = defaultObfTime;
			}
			NumTimesOnFunctions = ObfTimes;
			int NumObfTimes = ObfTimes;

			// Real begining of the pass
			// Loop for the number of time we run the pass on the function
			do{
				DEBUG_WITH_TYPE("cfg", errs() << "bcf: Function " << F.getName()
							<<", before the pass:\n");
				DEBUG_WITH_TYPE("cfg", F.viewCFG());
				// Put all the function's block in a list
				std::list<BasicBlock *> basicBlocks;
				for (Function::iterator i=F.begin();i!=F.end();++i) {
					basicBlocks.push_back(&*i);
				}
				DEBUG_WITH_TYPE("gen", errs() << "bcf: Iterating on the Function's Basic Blocks\n");

				while(!basicBlocks.empty()){
					NumBasicBlocks ++;
					// Basic Blocks' selection
					if(obf_control++ % 10 < ObfProbRate){
						
						DEBUG_WITH_TYPE("opt", errs() << "bcf: Block "
									<< NumBasicBlocks <<" selected. \n");
						hasBeenModified = true;
						++NumModifiedBasicBlocks;
						NumAddedBasicBlocks += 3;
						FinalNumBasicBlocks += 3;
						// Add bogus flow to the given Basic Block (see description)
						BasicBlock *basicBlock = basicBlocks.front();
						addBogusFlow(basicBlock, F);
					}
					else{
						DEBUG_WITH_TYPE("opt", errs() << "bcf: Block "
									<< NumBasicBlocks <<" not selected.\n");
					}
					// remove the block from the list
					basicBlocks.pop_front();

					if(firstTime){ // first time we iterate on this function
						++InitNumBasicBlocks;
						++FinalNumBasicBlocks;
					}
				} // end of while(!basicBlocks.empty())
				DEBUG_WITH_TYPE("gen", errs() << "bcf: End of function " << F.getName() << "\n");
				if(hasBeenModified){ // if the function has been modified
					DEBUG_WITH_TYPE("cfg", errs() << "bcf: Function " << F.getName()
								<<", after the pass: \n");
					DEBUG_WITH_TYPE("cfg", F.viewCFG());
				}
				else{
					DEBUG_WITH_TYPE("cfg", errs() << "bcf: Function's not been modified \n");
				}
				firstTime = false;
			}while(--NumObfTimes > 0);
		}

		/* addBogusFlow
		 *
		 * Add bogus flow to a given basic block, according to the header's description
		 */
		virtual void addBogusFlow(BasicBlock * basicBlock, Function &F)
		{
			// Split the block: first part with only the phi nodes and debug info and terminator
			//                  created by splitBasicBlock. (-> No instruction)
			//                  Second part with every instructions from the original block
			// We do this way, so we don't have to adjust all the phi nodes, metadatas and so on
			// for the first block. We have to let the phi nodes in the first part, because they
			// actually are updated in the second part according to them.
			Instruction *i1 = &*basicBlock->begin(); // ?????
			if(basicBlock->getFirstNonPHIOrDbgOrLifetime())
			  i1 = basicBlock->getFirstNonPHIOrDbgOrLifetime();
			Twine *var;
			var = new Twine("originalBB");
			BasicBlock *originalBB = basicBlock->splitBasicBlock(i1, *var);
			DEBUG_WITH_TYPE("gen", errs() << "bcf: First and original basic blocks: ok\n");

			// Creating the altered basic block on which the first basicBlock will jump
			Twine * var3 = new Twine("alteredBB");
			BasicBlock *alteredBB = createAlteredBasicBlock(originalBB, *var3, &F);
			DEBUG_WITH_TYPE("gen", errs() << "bcf: Altered basic block: ok\n");

			// Now that all the blocks are created,
			// we modify the terminators to adjust the control flow.

			alteredBB->getTerminator()->eraseFromParent();
			basicBlock->getTerminator()->eraseFromParent();
			DEBUG_WITH_TYPE("gen", errs() << "bcf: Terminator removed from the altered" <<" and first basic blocks\n");

			// Preparing a condition..
			// For now, the condition is an always true comparaison between 2 float
			// This will be complicated after the pass (in doFinalization())
			Value * LHS = ConstantFP::get(Type::getFloatTy(F.getContext()), 1.0);
			Value * RHS = ConstantFP::get(Type::getFloatTy(F.getContext()), 1.0);
			DEBUG_WITH_TYPE("gen", errs() << "bcf: Value LHS and RHS created\n");


			// The always true condition. End of the first block
			Twine * var4 = new Twine("condition");
			FCmpInst * condition = new FCmpInst(*basicBlock, FCmpInst::FCMP_TRUE , LHS, RHS, *var4);
			DEBUG_WITH_TYPE("gen", errs() << "bcf: Always true condition created\n");

			// Jump to the original basic block if the condition is true or
			// to the altered block if false.
			BranchInst::Create(originalBB, alteredBB, (Value *)condition, basicBlock);
			DEBUG_WITH_TYPE("gen", errs() << "bcf: Terminator instruction in first basic block: ok\n");

			// The altered block loop back on the original one.
			BranchInst::Create(originalBB, alteredBB);
			DEBUG_WITH_TYPE("gen", errs() << "bcf: Terminator instruction in altered block: ok\n");


			// The end of the originalBB is modified to give the impression that sometimes
			// it continues in the loop, and sometimes it return the desired value
			// (of course it's always true, so it always use the original terminator..
			//  but this will be obfuscated too;) )

			// iterate on instruction just before the terminator of the originalBB
			BasicBlock::iterator i = originalBB->end();

			// Split at this point (we only want the terminator in the second part)
			Twine * var5 = new Twine("originalBBpart2");
			BasicBlock * originalBBpart2 = originalBB->splitBasicBlock(--i , *var5);
			DEBUG_WITH_TYPE("gen", errs() << "bcf: Terminator part of the original basic block" << " is isolated\n");
			// the first part go either on the return statement or on the begining
			// of the altered block.. So we erase the terminator created when splitting.
			originalBB->getTerminator()->eraseFromParent();
			// We add at the end a new always true condition
			Twine * var6 = new Twine("condition2");
			FCmpInst * condition2 = new FCmpInst(*originalBB, CmpInst::FCMP_TRUE , LHS, RHS, *var6);
			BranchInst::Create(originalBBpart2, alteredBB, (Value *)condition2, originalBB);
			DEBUG_WITH_TYPE("gen", errs() << "bcf: Terminator original basic block: ok\n");
			DEBUG_WITH_TYPE("gen", errs() << "bcf: End of addBogusFlow().\n");

		} // end of addBogusFlow()

		/* createAlteredBasicBlock
		 *
		 * This function return a basic block similar to a given one.
		 * It's inserted just after the given basic block.
		 * The instructions are similar but junk instructions are added between
		 * the cloned one. The cloned instructions' phi nodes, metadatas, uses and
		 * debug locations are adjusted to fit in the cloned basic block and
		 * behave nicely.
		 */
		virtual BasicBlock* createAlteredBasicBlock(BasicBlock * basicBlock, const Twine &  Name = "gen", Function * F = 0)
		{
			// Useful to remap the informations concerning instructions.
			ValueToValueMapTy VMap;
			BasicBlock * alteredBB = llvm::CloneBasicBlock (basicBlock, VMap, Name, F);
			DEBUG_WITH_TYPE("gen", errs() << "bcf: Original basic block cloned\n");
			// Remap operands.
			BasicBlock::iterator ji = basicBlock->begin();
			for (BasicBlock::iterator i = alteredBB->begin(), e = alteredBB->end() ; i != e; ++i)
			{
				// Loop over the operands of the instruction
				for(User::op_iterator opi = i->op_begin (), ope = i->op_end(); opi != ope; ++opi)
				{
					// get the value for the operand
					Value *v = MapValue(*opi, VMap,  RF_None, 0);
					if (v != 0)
					{
						*opi = v;
						DEBUG_WITH_TYPE("gen", errs() << "bcf: Value's operand has been setted\n");
					}
				}
				DEBUG_WITH_TYPE("gen", errs() << "bcf: Operands remapped\n");
				// Remap phi nodes' incoming blocks.
				if (PHINode *pn = dyn_cast<PHINode>(i)) 
				{
					for (unsigned j = 0, e = pn->getNumIncomingValues(); j != e; ++j) 
					{
						Value *v = MapValue(pn->getIncomingBlock(j), VMap, RF_None, 0);
						if (v != 0)
						{
							pn->setIncomingBlock(j, cast<BasicBlock>(v));
						}
					}
				}
				DEBUG_WITH_TYPE("gen", errs() << "bcf: PHINodes remapped\n");
				// Remap attached metadata.
				SmallVector<std::pair<unsigned, MDNode *>, 4> MDs;
				i->getAllMetadata(MDs);
				DEBUG_WITH_TYPE("gen", errs() << "bcf: Metadatas remapped\n");
				// important for compiling with DWARF, using option -g.
				i->setDebugLoc(ji->getDebugLoc());
				ji++;
				DEBUG_WITH_TYPE("gen", errs() << "bcf: Debug information location setted\n");

			} // The instructions' informations are now all correct

			DEBUG_WITH_TYPE("gen", errs() << "bcf: The cloned basic block is now correct\n");
			DEBUG_WITH_TYPE("gen", errs() << "bcf: Starting to add junk code in the cloned bloc...\n");

			// add random instruction in the middle of the bloc. This part can be improve
			for (BasicBlock::iterator i = alteredBB->begin(), e = alteredBB->end() ; i != e; ++i)
			{
				// in the case we find binary operator, we modify slightly this part by randomly
				// insert some instructions
				if(i->isBinaryOp())
				{ // binary instructions
					unsigned opcode = i->getOpcode();
					BinaryOperator *op, *op1 = NULL;
					Twine *var = new Twine("_");
					// treat differently float or int
					// Binary int
					if(opcode == Instruction::Add || opcode == Instruction::Sub ||
								opcode == Instruction::Mul || opcode == Instruction::UDiv ||
								opcode == Instruction::SDiv || opcode == Instruction::URem ||
								opcode == Instruction::SRem || opcode == Instruction::Shl ||
								opcode == Instruction::LShr || opcode == Instruction::AShr ||
								opcode == Instruction::And || opcode == Instruction::Or ||
								opcode == Instruction::Xor){
						for(int random = (int)llvm::cryptoutils->get_range(10); random < 10; ++random){
							switch(llvm::cryptoutils->get_range(4)){ // to improve
								case 0: //do nothing
									break;
								case 1: op = BinaryOperator::CreateNeg(i->getOperand(0),*var, &*i); // ?????
										op1 = BinaryOperator::Create(Instruction::Add, op, i->getOperand(1),"gen", &*i); // ?????
										break;
								case 2: op1 = BinaryOperator::Create(Instruction::Sub, i->getOperand(0), i->getOperand(1), *var, &*i); // ?????
										op = BinaryOperator::Create(Instruction::Mul, op1, i->getOperand(1),"gen", &*i); // ?????
										break;
								case 3: op = BinaryOperator::Create(Instruction::Shl, i->getOperand(0), i->getOperand(1), *var, &*i); // ?????
										break;
							}
						}
					}
					// Binary float
					if(opcode == Instruction::FAdd || opcode == Instruction::FSub ||
								opcode == Instruction::FMul || opcode == Instruction::FDiv ||
								opcode == Instruction::FRem){
						for(int random = (int)llvm::cryptoutils->get_range(10); random < 10; ++random){
							switch(llvm::cryptoutils->get_range(3)){ // can be improved
								case 0: //do nothing
									break;
								case 1: op = BinaryOperator::CreateFNeg(i->getOperand(0), *var, &*i); // ?????
										op1 = BinaryOperator::Create(Instruction::FAdd, op, i->getOperand(1),"gen", &*i); // ?????
										break;
								case 2: op = BinaryOperator::Create(Instruction::FSub, i->getOperand(0), i->getOperand(1), *var, &*i); // ?????
										op1 = BinaryOperator::Create(Instruction::FMul, op, i->getOperand(1), "gen", &*i); // ?????
										break;
							}
						}
					}
					if(opcode == Instruction::ICmp){ // Condition (with int)
						ICmpInst *currentI = (ICmpInst*)(&i);
						switch(llvm::cryptoutils->get_range(3)){ // must be improved
							case 0: //do nothing
								break;
							case 1: currentI->swapOperands();
									break;
							case 2: // randomly change the predicate
									switch(llvm::cryptoutils->get_range(10)){
										case 0: currentI->setPredicate(ICmpInst::ICMP_EQ);
												break; // equal
										case 1: currentI->setPredicate(ICmpInst::ICMP_NE);
												break; // not equal
										case 2: currentI->setPredicate(ICmpInst::ICMP_UGT);
												break; // unsigned greater than
										case 3: currentI->setPredicate(ICmpInst::ICMP_UGE);
												break; // unsigned greater or equal
										case 4: currentI->setPredicate(ICmpInst::ICMP_ULT);
												break; // unsigned less than
										case 5: currentI->setPredicate(ICmpInst::ICMP_ULE);
												break; // unsigned less or equal
										case 6: currentI->setPredicate(ICmpInst::ICMP_SGT);
												break; // signed greater than
										case 7: currentI->setPredicate(ICmpInst::ICMP_SGE);
												break; // signed greater or equal
										case 8: currentI->setPredicate(ICmpInst::ICMP_SLT);
												break; // signed less than
										case 9: currentI->setPredicate(ICmpInst::ICMP_SLE);
												break; // signed less or equal
									}
									break;
						}

					}
					if(opcode == Instruction::FCmp){ // Conditions (with float)
						FCmpInst *currentI = (FCmpInst*)(&i);
						switch(llvm::cryptoutils->get_range(3)){ // must be improved
							case 0: //do nothing
								break;
							case 1: currentI->swapOperands();
									break;
							case 2: // randomly change the predicate
									switch(llvm::cryptoutils->get_range(10)){
										case 0: currentI->setPredicate(FCmpInst::FCMP_OEQ);
												break; // ordered and equal
										case 1: currentI->setPredicate(FCmpInst::FCMP_ONE);
												break; // ordered and operands are unequal
										case 2: currentI->setPredicate(FCmpInst::FCMP_UGT);
												break; // unordered or greater than
										case 3: currentI->setPredicate(FCmpInst::FCMP_UGE);
												break; // unordered, or greater than, or equal
										case 4: currentI->setPredicate(FCmpInst::FCMP_ULT);
												break; // unordered or less than
										case 5: currentI->setPredicate(FCmpInst::FCMP_ULE);
												break; // unordered, or less than, or equal
										case 6: currentI->setPredicate(FCmpInst::FCMP_OGT);
												break; // ordered and greater than
										case 7: currentI->setPredicate(FCmpInst::FCMP_OGE);
												break; // ordered and greater than or equal
										case 8: currentI->setPredicate(FCmpInst::FCMP_OLT);
												break; // ordered and less than
										case 9: currentI->setPredicate(FCmpInst::FCMP_OLE);
												break; // ordered or less than, or equal
									}
									break;
						}
					}
				}
			}
			return alteredBB;
		} // end of createAlteredBasicBlock()


		bool InsertDefaultOpq(Module &M, Instruction* inst){
			LLVMContext& context = M.getContext();
			LoadInst * opX , * opY;
			ICmpInst * condition, * condition2;
			BinaryOperator *op,*op1 = NULL;

			Twine * varX = new Twine("x");
			Twine * varY = new Twine("y");
			Value * x1 =ConstantInt::get(Type::getInt32Ty(context), 0, false);
			Value * y1 =ConstantInt::get(Type::getInt32Ty(context), 0, false);

			GlobalVariable 	* x = new GlobalVariable(M, Type::getInt32Ty(context), false,
						GlobalValue::CommonLinkage, (Constant * )x1,
						*varX);
			GlobalVariable 	* y = new GlobalVariable(M, Type::getInt32Ty(context), false,
						GlobalValue::CommonLinkage, (Constant * )y1,
						*varY);
			opX = new LoadInst ((Value *)x, "", inst);
			opY = new LoadInst ((Value *)y, "", inst);
			op = BinaryOperator::Create(Instruction::Sub, (Value *)opX,
						ConstantInt::get(Type::getInt32Ty(context), 1,
							false), "", inst);
			op1 = BinaryOperator::Create(Instruction::Mul, (Value *)opX, op, "", inst);
			op = BinaryOperator::Create(Instruction::URem, op1,
						ConstantInt::get(Type::getInt32Ty(context), 2,
							false), "", inst);
			condition = new ICmpInst(inst, ICmpInst::ICMP_EQ, op,
						ConstantInt::get(Type::getInt32Ty(context), 0,
							false));
			condition2 = new ICmpInst(inst, ICmpInst::ICMP_SLT, opY,
						ConstantInt::get(Type::getInt32Ty(context), 10,
							false));
			op1 = BinaryOperator::Create(Instruction::Or, (Value *)condition,
						(Value *)condition2, "", inst);
			BranchInst::Create(((BranchInst*) inst)->getSuccessor(0),
						((BranchInst*) inst)->getSuccessor(1),(Value *) op1,
						((BranchInst*) inst)->getParent());
			DEBUG_WITH_TYPE("gen", errs() << "bcf: Erase branch instruction:"
						<< *((BranchInst*) inst) << "\n");
			inst->eraseFromParent(); // erase the branch
			return true;
		}
		//%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i64, i32, [20 x i8] }
		//%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }

		bool InsertFproOpq(Module &M, Instruction* inst, Value* arg)
		{
			Type* argType = arg->getType();
			if(!argType->isIntegerTy())
			{
				InsertDefaultOpq(M, inst);
				return false;
			}

			const DataLayout &DL = M.getDataLayout(); // ?????
			AllocaInst* jAI = new AllocaInst(argType, DL.getAllocaAddrSpace(), "", inst); // ?????
			StoreInst* jSI = new StoreInst(arg, jAI, inst);
			LoadInst* jLI = new LoadInst(jAI, "", inst);
			
			vector<Value*> vecProp;
			CastInst* j64CI;
			if(((IntegerType*) argType)->getBitWidth() != 64)
			{
				j64CI = new SExtInst(jLI, i64Type, "idxprom", inst);
				vecProp.push_back(j64CI);
			}
			else
			{
				vecProp.push_back(jLI);
			}
			ArrayRef<Value*> arProp(vecProp);

			CallInst* propCI = CallInst::Create(covertpropFunc, arProp, "", inst);
			ICmpInst* cmpCI;
			if(((IntegerType*) argType)->getBitWidth() != 64)
			{
				cmpCI = new ICmpInst(inst, ICmpInst::ICMP_EQ, j64CI, propCI);
			}
			else
			{
				cmpCI = new ICmpInst(inst, ICmpInst::ICMP_EQ, jLI, propCI);
			}


			BranchInst::Create(((BranchInst*) inst)->getSuccessor(0), ((BranchInst*) inst)->getSuccessor(1),(Value *) cmpCI, ((BranchInst*) inst)->getParent());
			inst->eraseFromParent();
		
			return true; // ?????
		}

		bool InsertParaOpq(Module &M, Instruction* inst, Value* arg, int type){
			Type* argType = arg->getType();
			if(!argType->isIntegerTy()){
				InsertDefaultOpq(M, inst);
				return false;
			}

			const DataLayout &DL = M.getDataLayout(); // ?????
			AllocaInst* jAI = new AllocaInst(argType, DL.getAllocaAddrSpace(), "", inst); // ?????
			StoreInst* jSI = new StoreInst(arg, jAI, inst);
			LoadInst* jLI = new LoadInst(jAI, "", inst);
			
			vector<Value*> vecProp;
			CastInst* j64CI;
			if(((IntegerType*) argType)->getBitWidth() != 64){
				j64CI = new SExtInst(jLI, i64Type, "idxprom", inst);
				vecProp.push_back(j64CI);
			}else{
				vecProp.push_back(jLI);
			}
			ArrayRef<Value*> arProp(vecProp);
			
			if (type == 1)
			{
				CallInst* propCI = CallInst::Create(threadpropFunc, arProp, "", inst);
				ICmpInst* cmpCI;
				if(((IntegerType*) argType)->getBitWidth() != 64){
					cmpCI = new ICmpInst(inst, ICmpInst::ICMP_EQ, j64CI, propCI);
				}else{
					cmpCI = new ICmpInst(inst, ICmpInst::ICMP_EQ, jLI, propCI);
				}


				BranchInst::Create(((BranchInst*) inst)->getSuccessor(0),
							((BranchInst*) inst)->getSuccessor(1),(Value *) cmpCI,
							((BranchInst*) inst)->getParent());
				inst->eraseFromParent(); // erase the branch
			}
			else if(type == 2)
			{
				CallInst* propCI = CallInst::Create(forkpropFunc, arProp, "", inst);
				ICmpInst* cmpCI;
				if(((IntegerType*) argType)->getBitWidth() != 64){
					cmpCI = new ICmpInst(inst, ICmpInst::ICMP_EQ, j64CI, propCI);
				}else{
					cmpCI = new ICmpInst(inst, ICmpInst::ICMP_EQ, jLI, propCI);
				}


				BranchInst::Create(((BranchInst*) inst)->getSuccessor(0),
							((BranchInst*) inst)->getSuccessor(1),(Value *) cmpCI,
							((BranchInst*) inst)->getParent());
				inst->eraseFromParent(); // erase the branch
			}
		}

		bool InsertArrayOpq(Module &M, Instruction* inst, Value* arg){
			Type* argType = arg->getType();
			if(!argType->isIntegerTy()){
				InsertDefaultOpq(M, inst);
				return false;
			}

			LLVMContext& context = M.getContext();

			const DataLayout &DL = M.getDataLayout(); // ?????
			AllocaInst* jAI = new AllocaInst(argType, DL.getAllocaAddrSpace(), "", inst);
			StoreInst* jSI = new StoreInst(arg, jAI, inst);
			LoadInst* jLI = new LoadInst(jAI, "", inst);

			BinaryOperator* remBO;
			if(((IntegerType*) argType)->getBitWidth() != 64)
			{
				CastInst* j64CI = new SExtInst(jLI, i64Type, "idxprom", inst);
				remBO = BinaryOperator::Create(Instruction::SRem, j64CI, ci5_64, "rem", inst);
			} 
			else 
			{
				remBO = BinaryOperator::Create(Instruction::SRem, jLI, ci5_64, "rem", inst);
			}

			ArrayType* arAT = ArrayType::get(i64Type, 5);

			AllocaInst* array1AI = new AllocaInst(arAT, DL.getAllocaAddrSpace(), "", inst); // ?????
			AllocaInst* array2AI = new AllocaInst(arAT, DL.getAllocaAddrSpace(), "", inst); // ?????

			std::vector<Value*> vec00;
			vec00.push_back(ci0_32);
			vec00.push_back(ci0_64);
			ArrayRef<Value*> ar00(vec00);
			std::vector<Value*> vec01;
			vec01.push_back(ci0_32);
			vec01.push_back(ci1_64);
			ArrayRef<Value*> ar01(vec01);
			std::vector<Value*> vec02;
			vec02.push_back(ci0_32);
			vec02.push_back(ci2_64);
			ArrayRef<Value*> ar02(vec02);
			std::vector<Value*> vec03;
			vec03.push_back(ci0_32);
			vec03.push_back(ci3_64);
			ArrayRef<Value*> ar03(vec03);
			std::vector<Value*> vec04;
			vec04.push_back(ci0_32);
			vec04.push_back(ci4_64);
			ArrayRef<Value*> ar04(vec04);
			Instruction* a10EPI = GetElementPtrInst::CreateInBounds((Value *) array1AI, ar00, "l1_arrayidx", inst);
			Instruction* a11EPI = GetElementPtrInst::CreateInBounds((Value *) array1AI, ar01, "l1_arrayidx", inst);
			Instruction* a12EPI = GetElementPtrInst::CreateInBounds((Value *) array1AI, ar02, "l1_arrayidx", inst);
			Instruction* a13EPI = GetElementPtrInst::CreateInBounds((Value *) array1AI, ar03, "l1_arrayidx", inst);
			Instruction* a14EPI = GetElementPtrInst::CreateInBounds((Value *) array1AI, ar04, "l1_arrayidx", inst);

			Instruction* a20EPI = GetElementPtrInst::CreateInBounds((Value *) array2AI, ar00, "l2_arrayidx", inst);
			Instruction* a21EPI = GetElementPtrInst::CreateInBounds((Value *) array2AI, ar01, "l2_arrayidx", inst);
			Instruction* a22EPI = GetElementPtrInst::CreateInBounds((Value *) array2AI, ar02, "l2_arrayidx", inst);
			Instruction* a23EPI = GetElementPtrInst::CreateInBounds((Value *) array2AI, ar03, "l2_arrayidx", inst);
			Instruction* a24EPI = GetElementPtrInst::CreateInBounds((Value *) array2AI, ar04, "l2_arrayidx", inst);

			StoreInst* a10SI = new StoreInst(ci0_64, a10EPI, inst);
			StoreInst* a11SI = new StoreInst(ci1_64, a11EPI, inst);
			StoreInst* a12SI = new StoreInst(ci2_64, a12EPI, inst);
			StoreInst* a13SI = new StoreInst(ci3_64, a13EPI, inst);
			StoreInst* a14SI = new StoreInst(ci4_64, a14EPI, inst);

			StoreInst* a20SI = new StoreInst(ci0_64, a20EPI, inst);
			StoreInst* a21SI = new StoreInst(ci1_64, a21EPI, inst);
			StoreInst* a22SI = new StoreInst(ci2_64, a22EPI, inst);
			StoreInst* a23SI = new StoreInst(ci3_64, a23EPI, inst);
			StoreInst* a24SI = new StoreInst(ci4_64, a24EPI, inst);

			std::vector<Value*> l1Vec;
			l1Vec.push_back(ci0_32);
			l1Vec.push_back(remBO);
			ArrayRef<Value*> l1AR(l1Vec);
			Instruction* l1EPI = GetElementPtrInst::CreateInBounds((Value *) array1AI, l1AR, "l1_arrayidx", inst);
			LoadInst* l1LI = new LoadInst(l1EPI, "", false, inst);

			std::vector<Value*> l2Vec;
			l2Vec.push_back(ci0_32);
			l2Vec.push_back(l1LI);
			ArrayRef<Value*> l2AR(l2Vec);
			Instruction* l2EPI = GetElementPtrInst::CreateInBounds((Value *) array2AI, l2AR, "l2_arrayidx", inst);
			LoadInst* l2LI = new LoadInst(l2EPI, "", false, inst);

			ICmpInst* arCI = new ICmpInst(inst, ICmpInst::ICMP_NE, l2LI, ci5_64, "cmp");
			BranchInst::Create(((BranchInst*) inst)->getSuccessor(0), ((BranchInst*) inst)->getSuccessor(1),(Value *) arCI, ((BranchInst*) inst)->getParent());
			inst->eraseFromParent(); // erase the branch
		}

		bool InsertFloatOpq(Module& M, Instruction* inst, Value* arg){
			Type* argType = arg->getType();
			if(!argType->isIntegerTy())
			  return false;

			LLVMContext& context = M.getContext();
			ConstantFP* divCFP = ConstantFP::get(context, APFloat(1.000000e+01));
			ConstantFP* conCFP = ConstantFP::get(context, APFloat(1.000000e-01));
			
			const DataLayout &DL = M.getDataLayout(); // ?????
			AllocaInst* fpAI = new AllocaInst(floatType, DL.getAllocaAddrSpace(), "", inst); // ?????
			AllocaInst* jAI = new AllocaInst(argType, DL.getAllocaAddrSpace(), "", inst); // ?????
			StoreInst* jSI = new StoreInst(arg, jAI, inst);
			LoadInst* jLI = new LoadInst(jAI, "", inst);

			CastInst* jdbCI = new SIToFPInst(jLI, doubleType, "conv", inst);
			BinaryOperator* divBO = BinaryOperator::Create(Instruction::FDiv, jdbCI, divCFP, "div", inst);
			CastInst* divBI = new FPTruncInst(divBO, floatType, "conv1", inst);
			StoreInst* divSI = new StoreInst(divBI, fpAI, false, inst);
			LoadInst* fpLI = new LoadInst(fpAI, "", inst);
			CastInst* cmpCI = new FPExtInst(fpLI, doubleType, "conv2", inst);

			FCmpInst* fpCMP = new FCmpInst(inst, FCmpInst::FCMP_OEQ, cmpCI, conCFP, "cmp");

			BranchInst::Create(((BranchInst*) inst)->getSuccessor(1),
						((BranchInst*) inst)->getSuccessor(0), fpCMP,
						((BranchInst*) inst)->getParent());
			inst->eraseFromParent(); // erase the branch
		}
		/* doFinalization
		 *
		 * Overwrite FunctionPass method to apply the transformations to the whole module.
		 * This part obfuscate all the always true predicates of the module.
		 * More precisely, the condition which predicate is FCMP_TRUE.
		 * It also remove all the functions' basic blocks' and instructions' names.
		 */
		bool doF(Function& F){
			DEBUG_WITH_TYPE("gen", errs()<<"bcf: Starting doFinalization...\n");
			Module* module = F.getParent();
			Module& M = *module;
			LLVMContext& context = M.getContext();
			//For creating symbolic opaque predicates
			Value *argValue;
			Type* argType;

			for (Function::arg_iterator argIt = F.arg_begin(); argIt != F.arg_end(); ++argIt){
				argValue = &*argIt;
				argType = argValue->getType();
				if(argType->isIntegerTy()){
					break;
				}
			}

			//LOG(L2_DEBUG) << "Argument: " << argValue->getName().str();
			//  The global values

			std::vector<Instruction*> toEdit, toDelete;
			// Looking for the conditions and branches to transform
			for(Module::iterator mi = M.begin(), me = M.end(); mi != me; ++mi){
				for(Function::iterator fi = mi->begin(), fe = mi->end(); fi != fe; ++fi){
					//fi->setName("");
					TerminatorInst * tbb= fi->getTerminator();
					if(tbb->getOpcode() == Instruction::Br){
						BranchInst * br = (BranchInst *)(tbb);
						if(br->isConditional()){
							FCmpInst * cond = (FCmpInst *)br->getCondition();
							unsigned opcode = cond->getOpcode();
							if(opcode == Instruction::FCmp){
								if (cond->getPredicate() == FCmpInst::FCMP_TRUE){
									DEBUG_WITH_TYPE("gen",
												errs()<<"bcf: an always true predicate !\n");
									toDelete.push_back(cond); // The condition
									toEdit.push_back(tbb);    // The branch using the condition
								}
							}
						}
					}
					/*
					   for (BasicBlock::iterator bi = fi->begin(), be = fi->end() ; bi != be; ++bi){
					   bi->setName(""); // setting the basic blocks' names
					   }
					   */
				}
			}


			// Replacing all the branches we found
			// We replace the original simple opaque constant with secure predicates
			srand(time(0));

			for(std::vector<Instruction*>::iterator it =toEdit.begin(); it!=toEdit.end(); ++it){
				Instruction* inst = *it;
				//int opqId = rand() % opq_type_num;

				if(argType == NULL){
					InsertDefaultOpq(M, inst);
				} else {
					//LOG(L_INFO) << "complex_control = " << complex_control;
					if(argType->isIntegerTy()){
						int opqId;
						if(complex_control < OpqNum || OpqNum == 0)
						{
							opqId = OpqType;	
							complex_control++;
						} 
						else 
						{
							opqId = 100;//the default opaque predicate	
						}
						switch(opqId){
							case 1:{
									   InsertArrayOpq(M, inst, argValue);
									   break;
								   }
							case 2:{
									   InsertFloatOpq(M, inst, argValue);
									   break;
								   }
							case 3:{
									   InsertFproOpq(M, inst, argValue);
									   break;
								   }
							case 4:{
									   InsertParaOpq(M, inst, argValue, 1);
									   break;
								   }
							case 5:{
									   InsertParaOpq(M, inst, argValue, 2);
									   break;
								   }
							default:{
										InsertDefaultOpq(M, inst);
										break;
									}
						}
					}else{
						InsertDefaultOpq(M, inst);
					}
				}
			}
			// Erase all the associated conditions we found
			for(std::vector<Instruction*>::iterator i =toDelete.begin();i!=toDelete.end();++i){
				DEBUG_WITH_TYPE("gen", errs() << "bcf: Erase condition instruction:"
							<< *((Instruction*)*i)<< "\n");
				(*i)->eraseFromParent();
			}

			// Only for debug
			DEBUG_WITH_TYPE("cfg",
						errs() << "bcf: End of the pass, here are the graphs after doFinalization\n");
			for(Module::iterator mi = M.begin(), me = M.end(); mi != me; ++mi){
				DEBUG_WITH_TYPE("cfg", errs() << "bcf: Function " << mi->getName() <<"\n");
				DEBUG_WITH_TYPE("cfg", mi->viewCFG());
			}

			return true;
		} // end of doFinalization
	}; // end of struct BogusControlFlow : public FunctionPass
}

char BogusControlFlow::ID = 0;
static RegisterPass<BogusControlFlow> X("boguscf", "inserting bogus control flow");

Pass *llvm::createBogus() {
	return new BogusControlFlow();
}

Pass *llvm::createBogus(bool flag) {
	return new BogusControlFlow(flag);
}
