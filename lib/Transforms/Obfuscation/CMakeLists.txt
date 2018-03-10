add_llvm_library(LLVMObfuscation
	BogusControlFlow.cpp
	Utils.cpp
	CryptoUtils.cpp
	)

add_dependencies(LLVMObfuscation intrinsics_gen)
