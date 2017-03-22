#ifndef _MMAP_H_
#define _MMAP_H_
#include <stdio.h>
#include <stdlib.h>	//srand
#include <assert.h>
#include <time.h>	//time
#include <vector>	//time
#include <cmath>	//time
#include "gmp.h"

struct secparam{
  int64_t Z; //set num
  int64_t N; //multilinear level
  int64_t setnum;
  int64_t lambda;
  mpz_t q; //public number = p1 * p2 *... * pN
  mpz_t pzt; //zero test parameter
  mpz_t* z; //small random value
  mpz_t* h; //small random value
  mpz_t* g; //small prime
  mpz_t* ginv; //inverse of g mod p
  mpz_t* p; //large prime
  std::vector<int64_t>* setid; 
};

extern secparam sp;

int64_t MMapInitParam(int64_t z, int64_t n, int64_t setnum);
void MMapEnc(mpz_t result, int64_t m, int64_t mid, int64_t setid);
void MMapAdd(mpz_t result, mpz_t u1, mpz_t u2, int64_t mid);
void MMapMult(mpz_t result, mpz_t u1, mpz_t u2, int64_t mid);
int64_t MMapIsZero(mpz_t u);
#endif
