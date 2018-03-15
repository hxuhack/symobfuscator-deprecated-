add_llvm_library(LLVMObfuscation
  CryptoUtils.cpp
  BogusControlFlow.cpp
  Utils.cpp
  )

add_dependencies(LLVMObfuscation intrinsics_gen)