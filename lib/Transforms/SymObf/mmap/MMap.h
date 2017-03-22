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
  __int128 q; //public number = p1 * p2 *... * pN
  __int128 pzt; //zero test parameter
  int* z; //small random value
  int* h; //small random value
  int* g; //small prime
  __int128* ginv; //inverse of g mod p
  __int128* p; //large prime
  //std::vector<__int128>* setid; 
};

void MMapInitParam(int z, int n, int setnum);
__int128 MMapEnc(int m, int mid, int setid);
__int128 MMapEncDefault(int m, int setid);
__int128 MMapAdd(__int128 u1, __int128 u2, int mid);
__int128 MMapMult(__int128 u1, __int128 u2, int mid);
__int128 MMapIsZero(__int128 u);

__int128 GetPzt();
__int128 GetQ();
__int128 GetExp();

#endif
