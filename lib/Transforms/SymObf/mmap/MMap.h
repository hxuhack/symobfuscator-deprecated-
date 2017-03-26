#ifndef _MMAP_H_
#define _MMAP_H_
#include <stdio.h>
#include <stdlib.h>	//srand
#include <assert.h>
#include <time.h>	//time
#include <vector>	//time
#include <cmath>	//time
//#include "llvm/Transforms/SymObf/Logger.h"
//#include "llvm/Transforms/SymObf/Utils.h"

struct secparam{
  int Z; //set num
  int N; //multilinear level
  int setnum;
  int lambda;
  int64_t q; //public number = p1 * p2 *... * pN
  int64_t pzt; //zero test parameter
  int64_t* z; //small random value
  int64_t* h; //small random value
  int64_t* g; //small prime
  int64_t* ginv; //inverse of g mod p
  int64_t* p; //large prime
  //std::vector<int64_t>* setid; 
};

void MMapInitParam(int z, int n, int setnum);
int64_t MMapEnc(int m, int mid, int setid);
int64_t MMapEncDefault(int m, int setid);
int64_t MMapAdd(int64_t u1, int64_t u2, int mid);
int64_t MMapMult(int64_t u1, int64_t u2, int mid);
int64_t MMapIsZero(int64_t u);

int64_t GetPzt();
int64_t GetQ();
int64_t GetExp();

#endif
