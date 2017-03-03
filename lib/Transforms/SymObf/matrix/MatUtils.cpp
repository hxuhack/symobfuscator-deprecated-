#include <stdio.h>
#include <stdint.h>
#include "llvm/Transforms/SymObf/Utils.h"

using namespace llvm;

int PrintMat(double** mat, int height, int width){
  for(int i=0; i<height; i++){
	for(int j=0; j<width; j++){
	  char tmpStr[8] = {0}; 
	  sprintf(tmpStr, "%.2f,", mat[i][j]);
	  errs()<< tmpStr;
	}
    errs()<<"\n";
  }
}

int CopyMat(double** src, double** dst, int height, int width){
  for(int i=0; i<height; i++){
	for(int j=0; j<width; j++){
	  dst[i][j] = src[i][j];
	}
  }

}

int MatrixMult(double** mat1, double** mat2, double** retMat, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width){
  if(m1Width != m2Height)
    return -1;

  /*
  printf("Mat1#################################\n");
  for(int64_t i=0; i < m1Height; i++){
    for(int64_t j=0; j < m1Width; j++){
	  printf("%d", *(mat1 + i * m1Width + j));
	}
	printf("\n");
  }
  printf("####################################\n");

  printf("Mat2#################################\n");
  for(int64_t i=0; i < m2Height; i++){
    for(int64_t j=0; j < m2Width; j++){
	  printf("%d", *(mat2 + i * m2Width + j));
	}
	printf("\n");
  }
  printf("####################################\n");
*/

  for(int64_t i=0; i < m1Height; i++){
    for(int64_t j=0; j < m2Width; j++){
	  double ele = 0;
	  for(int64_t k=0; k < m1Width; k++){
		//ele = ele + *((double)(mat1 + i*m1Width + k)) * *((double*) (mat2+ k*m2Width + j));
		ele = ele + mat1[i][k] * mat2[k][j];
	  }
	  retMat[i][j] = ele;
	  //printf("%.3f",ele);
	}
	//printf("\n");
  }
  //printf("*********************\n");
  return 0;
}
