add_llvm_loadable_module(symobf 
  #TaintEngine.cpp
  Utils.cpp
  BranchProg.cpp
  MatrixBranchProg.cpp
  BoolProg.cpp
  SymObf.cpp
)
add_dependencies(symobf intrinsics_gen)
