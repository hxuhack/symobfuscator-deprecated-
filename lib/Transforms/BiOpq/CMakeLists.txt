#add_llvm_loadable_module(BiOpq 

add_library(BiOpq MODULE 
  Utils.cpp
  BiOpq.cpp
)

target_compile_features(BiOpq PRIVATE 
  cxx_range_for 
  cxx_auto_type
)

set_target_properties(BiOpq PROPERTIES
  COMPILE_FLAGS "-fno-rtti"
)

add_dependencies(BiOpq intrinsics_gen)
