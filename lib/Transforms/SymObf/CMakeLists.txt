add_llvm_loadable_module(LLVMSymObf 
  Utils.cpp
  BranchProg.cpp
  MatrixBranchProg.cpp
  BoolProg.cpp
  SymObf.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
)
