#include "MMap.h"

int main(int argc, char** argv){
  MMapInitParam(1,1,1);
  int64_t a = atoi(argv[1]);
  double ea = MMapEnc(a,0,0);
  printf("%d\n",MMapIsZero(ea));
  return 0;
}
