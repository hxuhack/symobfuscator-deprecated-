#include "MMap.h"

int main(int argc, char** argv){
  int num = atoi(argv[1]);
  MMapInitParam(num,1,num);
  __int128 result = 1;
  for(int i=0;i<num;i++){
    result =  result * MMapEnc(i,0,i) % GetQ();
  }
  printf("%d\n",MMapIsZero(result));
  return 0;
}
