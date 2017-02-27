# symobfuscator

#Changes made to LLVM
Create passes in lib/Transform

#Test the effect
First compile the source code into IR codes:  
./bin/clang -emit-llvm -S toy/pf.c  
##Output modified IR codes.
opt -load ~/llvm-3.9.0.src/build/lib/libSymObf.so -symobf toy/pf.ll -print-after-all  

##Straight forward
###Compile code:
clang -Xclang -load -Xclang lib/libSymObf.so pf.c
opt -load lib/libSymObf.so -symobf <pf.ll> pf_result.bc  
###Execute IR:  
.lli pf_result.bc  
###Compile to Binary Code
clang pf_result.bc   

##With IR from C code
clang -c rtlib.c
clang -Xclang -load -Xclang lib/libSymObf.so -c pf.c
clang rtlib.o pf.bc
