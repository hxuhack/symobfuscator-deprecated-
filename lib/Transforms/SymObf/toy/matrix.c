#include <stdio.h>
#include <stdint.h>


int64_t** MM(const int64_t** mat1, const int64_t** mat2, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width){
  int64_t retMat[m1Height][m2Width];
	/*
  int64_t** retMat = (int64_t**) malloc (sizeof(int64_t*) * m1Height);
  for(int64_t i=0; i<m1Height; i++){
    retMat[i] = (int64_t*) malloc (sizeof(int64_t) * m2Width);
  }
  */
  if(m1Width != m2Height)
    return retMat;

  for(int64_t i=0; i < m1Height; i++){
    for(int64_t j=0; j < m2Width; j++){
	  int64_t ele = 0;
	  for(int64_t k=0; k < m1Width; k++){
		ele = ele + *((int64_t*)mat1 + i*m1Width + k) * *((int64_t*)mat2+ k*m2Width + j);
	  }
	  retMat[i][j] = ele;
	  printf("%d\n",ele);
	}
  }
  return retMat;
}

int main(int argc, char** argv){
  int64_t len = 2;

  int64_t mat01[2][2], mat00[2][2], mat11[2][2], mat10[2][2];
  mat01[0][0] = 0;
  mat01[0][1] = 1;
  mat01[1][1] = 1;
  mat01[1][0] = 0;
  

  mat00[0][0] = 0;
  mat00[0][1] = 2;
  mat00[1][1] = 3;
  mat00[1][0] = 0;

  mat11[0][0] = 0;
  mat11[0][1] = 1;
  mat11[1][1] = 1;
  mat11[1][0] = 0;
  

  mat10[0][0] = 0;
  mat10[0][1] = 1;
  mat10[1][0] = 0;
  mat10[1][1] = 5;

  int64_t** mat[2][2] = {{mat00, mat01}, {mat10,  mat11}}; 

  int64_t initMat[1][2] = {1,0};
  int64_t inp = (argc & 1);

  int64_t** int64_terMat = MM(initMat, mat[inp][0], 1,2,2,2);
  for(int64_t i = 1; i<len;i++){
	  //To get the k-th bit of n: (n & ( 1 << k )) >> k;
	  inp = (argc & (1<<i)) >> i;
      int64_terMat = MM(int64_terMat, mat[inp][i], 1,2,2,2);
  }
}
