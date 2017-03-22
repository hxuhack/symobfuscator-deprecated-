#ifndef _MMAP_H_
#define _MMAP_H_
#include <stdio.h>
#include <stdlib.h>	//srand
#include <assert.h>
#include <time.h>	//time
#include <vector>	//time
#include <cmath>	//time
#include "llvm/Transforms/SymObf/Logger.h"
#include "llvm/Transforms/SymObf/Utils.h"

struct secparam{
  int64_t Z; //set num
  int64_t N; //multilinear level
  int64_t setnum;
  int64_t lambda;
  int64_t q; //public number = p1 * p2 *... * pN
  int64_t pzt; //zero test parameter
  int64_t* z; //small random value
  int64_t* h; //small random value
  int64_t* g; //small prime
  int64_t* ginv; //inverse of g mod p
  int64_t* p; //large prime
  std::vector<int64_t>* setid; 
};

int64_t MMapInitParam(int64_t z, int64_t n, int64_t setnum);
int64_t MMapEnc(int64_t m, int64_t mid, int64_t setid);
int64_t MMapEncDefault(int64_t m);
int64_t MMapAdd(int64_t u1, int64_t u2, int64_t mid);
int64_t MMapMult(int64_t u1, int64_t u2, int64_t mid);
int64_t MMapIsZero(int64_t u);

int64_t GetPzt();
int64_t GetQ();
int64_t GetExp();

#endif
