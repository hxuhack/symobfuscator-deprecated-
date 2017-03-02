#add_llvm_loadable_module(SymObf 

add_library(SymObf MODULE 
  matrix/GenMatPair.cpp
  Utils.cpp
  BranchProg.cpp
  MatrixBranchProg.cpp
  BoolProg.cpp
  SymObf.cpp
)

target_compile_features(SymObf PRIVATE 
  cxx_range_for 
  cxx_auto_type
)

set_target_properties(SymObf PROPERTIES
  COMPILE_FLAGS "-fno-rtti"
)

add_dependencies(SymObf intrinsics_gen)
