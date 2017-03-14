#include "MillerRabin.h"
#include <stdio.h> 

using namespace std;

int64_t GetPrime(int64_t mod){
  srand((unsigned)time(NULL));
  int64_t start = rand() % mod; 
  MillerRabin mr;
  for(int64_t i=start; i < mod; i++){
    if(mr.testprime(i)){
	  return i;
	}
  }
  for(int64_t i=start; i > 0; i--){
    if(mr.testprime(i)){
	  return i;
	}
  }
}


int main()
{
  printf("prime: %ld\n", GetPrime(65535));
  return 0;
}
