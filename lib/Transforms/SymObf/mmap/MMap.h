#include <stdio.h>
#include <stdlib.h>	//srand
#include <time.h>	//time

int64_t zMax = 128;
int64_t gMax = 65535;
int64_t pMax = 2147483647;
struct secparam{
  int64_t* z; //small random value
  int64_t* g; //small prime
  int64_t* p; //large prime
  int64_t Z; //set num
  int64_t N; //multilinear level
  int64_t q = 1; //public number = p1 * p2 *... * pN
} sp;

int64_t InitSecParam(int64_t z, int64_t n);
