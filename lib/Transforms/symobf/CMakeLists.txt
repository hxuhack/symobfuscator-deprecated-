
add_llvm_loadable_module(symobf 
  Taint.cpp
  TaintEngine.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )
