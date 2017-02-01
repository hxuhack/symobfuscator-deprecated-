
add_llvm_loadable_module(symobf 
  taint.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )
