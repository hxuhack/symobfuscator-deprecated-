# symobfuscator
The project has been moved to: https://github.com/zzrcxb/fusor. 

## Introduction
See the [DSN2018](https://dsn2018.uni.lu/main-track-accepted-papers/) paper *Manufacturing Resilient Bi-Opaque Predicates against Symbolic Execution* for more details.  
Authors: Hui Xu (CUHK); Yangfan Zhou, Yu Kang (Fudan University); Fengzhi Tu, Michael R. Lyu (CUHK)
## Installation
```
cd symobfuscator
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
make -j7
```
## Available Compiler Options
- `-mllvm -bcf`: Activates control flow flattening
- `-mllvm -boguscf-prob`: Choose the probability each basic blocks will be obfuscated by the -bcf pass
- `-mllvm -OpqType`: Choose the opaque predicate type
- `-mllvm -OpqNum`: Set the number of opaque predicates
- `-mllvm -boguscf-loop`: Choose how many time the -bcf pass loop on a function

## Example
```
# Under the build folder
# Type 1: Symbolic memory
./bin/clang demo.c -o demo -mllvm -bcf -mllvm -boguscf-prob=20 -mllvm -OpqType=1 -mllvm -OpqNum=1

# Type 2: Floating-point number
./bin/clang demo.c -o demo -mllvm -bcf -mllvm -boguscf-prob=20 -mllvm -OpqType=2 -mllvm -OpqNum=1

# Type 3: Covert symbolic propagation
./bin/clang -c rtlib.c
./bin/clang -c demo.c -mllvm -bcf -mllvm -boguscf-prob=50 -mllvm -OpqType=3 -mllvm -OpqNum=1
./bin/clang demo.o rtlib.o -lpthread -o demo

# Type 4: Parallel programming (Thread)
./bin/clang -c rtlib.c
./bin/clang -c demo.c -mllvm -bcf -mllvm -boguscf-prob=50 -mllvm -OpqType=4 -mllvm -OpqNum=1
./bin/clang demo.o rtlib.o -lpthread -o demo

# Type 5: Parallel programming (Fork)
./bin/clang -c rtlib.c
./bin/clang -c demo.c -mllvm -bcf -mllvm -boguscf-prob=50 -mllvm -OpqType=5 -mllvm -OpqNum=1
./bin/clang demo.o rtlib.o -lpthread -o demo
```
