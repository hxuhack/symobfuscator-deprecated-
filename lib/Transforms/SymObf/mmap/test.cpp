#include "MMap.h"

int main(int argc, char** argv){
  MMapInitParam(1,1,1);
  int64_t a = atoi(argv[1]);
  mpz_t em;
  mpz_init(em);
  MMapEnc(em,a,0,0);
  printf("%d\n",MMapIsZero(em));
  return 0;
}
