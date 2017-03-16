#ifndef _MMAP_H_
#define _MMAP_H_
#include <stdio.h>
#include <stdlib.h>	//srand
#include <time.h>	//time
#include <vector>	//time
#include <cmath>	//time

struct secparam{
  int64_t* z; //small random value
  int64_t* h; //small random value
  int64_t* g; //small prime
  int64_t* ginv; //inverse of g mod p
  int64_t* p; //large prime
  int64_t Z; //set num
  int64_t N; //multilinear level
  int64_t q = 1; //public number = p1 * p2 *... * pN
  int64_t setnum;
  std::vector<int64_t>* setid; 
  int64_t alphalen = 8;
};

extern struct secparam sp;

int64_t MMapInitParam(int64_t z, int64_t n, int64_t setnum);
double MMapEnc(int64_t m, int64_t mid, int64_t setid);
double MMapAdd(double u1, double u2, int64_t mid);
double MMapMult(double u1, double u2, int64_t mid);
int64_t MMapIsZero(double u);
#endif
