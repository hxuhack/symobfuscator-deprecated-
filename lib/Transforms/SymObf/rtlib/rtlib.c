#include <stdio.h>
#include <math.h>
#include <stdint.h>

int64_t MultArMatrix(int64_t** mat1, int64_t** mat2, int64_t** retMat, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width, int64_t mod){
  if(m1Width != m2Height)
    return -1;

  printf("********Mat1*************\n");
  for(int64_t i=0; i < m1Height; i++){
    for(int64_t j=0; j < m1Width; j++){
	  int64_t ele = *(int64_t*) (mat1 + i * m1Width + j);
	  printf("%d,", ele);
	}
	printf("\n");
  }
  printf("********Mat2*************\n");
  for(int64_t i=0; i < m2Height; i++){
    for(int64_t j=0; j < m2Width; j++){
	  int64_t ele = *(int64_t*) (mat2 + i * m2Width + j);
	  printf("%d,", ele);
	}
	printf("\n");
  }

  printf("*******Ret Mat**************\n");
  for(int64_t i=0; i < m1Height; i++){
    for(int64_t j=0; j < m2Width; j++){
	  int64_t ele = 0;
	  for(int64_t k=0; k < m1Width; k++){
		ele = (ele + *(int64_t*) (mat1 + i * m1Height + k) * *(int64_t*) (mat2 + k * m2Width + j)) % mod;
	  }
	  retMat[i][j] = ele;
	  printf("%d,",ele);
	}
	printf("\n");
  }
  printf("*********************\n");
  return 0;
}

int64_t MultIntMatrix(int64_t** mat1, int64_t** mat2, int64_t** retMat, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width, int64_t mod){
  if(m1Width != m2Height)
    return -1;

  printf("********Mat1*************\n");
  for(int64_t i=0; i < m1Height; i++){
    for(int64_t j=0; j < m1Width; j++){
	  printf("%d,", mat1[i][j]);
	}
	printf("\n");
  }
  printf("********Mat2*************\n");
  for(int64_t i=0; i < m2Height; i++){
    for(int64_t j=0; j < m2Width; j++){
	  int64_t ele = *(int64_t*) (mat2 + i * m2Width + j);
	  printf("%d,", ele);
	}
	printf("\n");
  }
  printf("**********RetMat***********\n");
  for(int64_t i=0; i < m1Height; i++){
    for(int64_t j=0; j < m2Width; j++){
	  int64_t ele = 0;
	  for(int64_t k=0; k < m1Width; k++){
		ele = (ele + mat1[i][k] * *(int64_t*)(mat2+m2Width*k+j)) % mod;
	  }
	  retMat[i][j] = ele;
	  printf("%d,",ele);
	}
	printf("\n");
  }
  return 0;
}
