#include <stdio.h>
#include <math.h>
#include <stdint.h>

int64_t** MultIntMatrix(int64_t** mat1, int64_t** mat2, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width, int64_t mod){
  printf("Entering MatrixMult: perform maltiplication...\n");
  int64_t retMat[m1Height][m2Width];
  if(m1Width != m2Height)
    return retMat;

  for(int64_t i=0; i < m1Height; i++){
    for(int64_t j=0; j < m2Width; j++){
	  int64_t ele = 0;
	  for(int64_t k=0; k < m1Width; k++){
		ele = (ele + *((int64_t*)(mat1 + i*m1Width + k)) * *((int64_t*) (mat2+ k*m2Width + j)))%mod;
	  }
	  retMat[i][j] = ele;
	  printf("%d,",ele);
	}
	printf("\n");
  }
  printf("*********************\n");
  return retMat;
}

double** MultFpMatrix(double** mat1, double** mat2, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width, int64_t mod){
  printf("Entering MatrixMult: perform maltiplication...\n");
  double retMat[m1Height][m2Width];
  if(m1Width != m2Height)
    return retMat;

  for(int64_t i=0; i < m1Height; i++){
    for(int64_t j=0; j < m2Width; j++){
	  double ele = 0;
	  for(int64_t k=0; k < m1Width; k++){
		ele = fmod(ele + *((double*)(mat1 + i*m1Width + k)) * *((double*) (mat2+ k*m2Width + j)), mod);
		//ele = ele + mat1[i][k] * mat2[k][j];
	  }
	  retMat[i][j] = ele;
	  printf("%.2f,",ele);
	}
	printf("\n");
  }
  printf("*********************\n");
  return retMat;
}
