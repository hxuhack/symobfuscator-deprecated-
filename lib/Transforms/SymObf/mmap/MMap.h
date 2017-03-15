#include <stdio.h>
#include <stdlib.h>	//srand
#include <time.h>	//time
#include <vector>	//time

int64_t zMax = 128;
int64_t hMax = 128;
int64_t gMax = 65535;
int64_t pMax = 2147483647;
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
} *sp;

int64_t MMapInitParam(int64_t z, int64_t n);
int64_t MMapEnc(int64_t m, int64_t mid, int64_t setid);
int64_t MMapAdd(int64_t m, int64_t mid, int64_t setid);
int64_t MMapMult(int64_t m, int64_t mid, int64_t set2id, int64_t set1id);
