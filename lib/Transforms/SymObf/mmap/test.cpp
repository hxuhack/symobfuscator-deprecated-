#include "MMap.h"

extern secparam sp;

int main(int argc, char** argv){
  int num = atoi(argv[1]);
  MMapInitParam(num,1,num);
  int64_t result = 1;
  for(int i=0;i<num;i++){
	int64_t u;
	MMapEnc(u,i,0,i);
    MMapMult(result, result, u);
  }
  printf("%d\n",MMapIsZero(result));

  result = 1;
  for(int i=1;i<num;i++){
	int64_t u;
	MMapEnc(u,i,0,i);
    MMapMult(result, result, u);
  }
  printf("%d\n",MMapIsZero(result));
  return 0;
}
