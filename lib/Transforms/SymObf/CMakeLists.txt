add_llvm_loadable_module(LLVMSymObf 
  Utils.cpp
  BranchProg.cpp
  MatrixBranchProg.cpp
  BoolProg.cpp
  SymObf.cpp
)
add_dependencies(LLVMSymObf intrinsics_gen)
