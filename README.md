# symobfuscator

#Changes made to LLVM
Create passes in lib/Transform

#Test
opt -load ~/llvm-3.9.0.src/build/lib/symobf.so -mat -sym toy/pf.ll -print-after sym
