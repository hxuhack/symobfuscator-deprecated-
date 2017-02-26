# symobfuscator

#Changes made to LLVM
Create passes in lib/Transform

#Test the effect
First compile the source code into IR codes: \\
./bin/clang -emit-llvm -S toy/pf.c
##Output modified IR codes.
opt -load ~/llvm-3.9.0.src/build/lib/libSymObf.so -symobf toy/pf.ll -print-after-all

##Compile code:

./bin/opt -load lib/libSymObf.so -symobf <toy/pf.ll> toy/pf_result.bc
./bin/llc toy/pf_result.bc 
