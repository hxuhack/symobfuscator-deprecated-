#include "MMap.h"

int main(int argc, char** argv){
  int64_t num = atoi(argv[1]);
  MMapInitParam(num,1,num);
  mpz_t result;
  mpz_init_set_ui(result,1);
  for(int64_t i=0; i<num; i++){
    mpz_t ei;
    mpz_init(ei);
    MMapEnc(ei,i,0,i);
    mpz_mul(result, ei, result);
	mpz_mod(result,result,sp.q);
  }
  printf("%d\n",MMapIsZero(result));
  return 0;
}
