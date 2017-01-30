
add_llvm_loadable_module( LLVMsymbof
  extract_syminsts.cpp

  DEPENDS
  intrinsics_gen
  PLUGIN_TOOL
  opt
  )
