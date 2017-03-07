#include <stdio.h>
#include <stdint.h>
#include "llvm/Transforms/SymObf/Utils.h"

using namespace llvm;

int PrintFpMat(double** mat, int height, int width){
  for(int i=0; i<height; i++){
	for(int j=0; j<width; j++){
	  char tmpStr[8] = {0}; 
	  sprintf(tmpStr, "%.2f,", mat[i][j]);
	  errs()<< tmpStr;
	}
    errs()<<"\n";
  }
}

int PrintIntMat(int64_t** mat, int height, int width){
  for(int i=0; i<height; i++){
	for(int j=0; j<width; j++){
	  char tmpStr[8] = {0}; 
	  sprintf(tmpStr, "%d,", mat[i][j]);
	  errs()<< tmpStr;
	}
    errs()<<"\n";
  }
  errs()<<"---------------------\n";
}

int CopyFpMat(double** src, double** dst, int height, int width){
  for(int i=0; i<height; i++){
	for(int j=0; j<width; j++){
	  dst[i][j] = src[i][j];
	}
  }
}

int CopyIntMat(int64_t** src, int64_t** dst, int height, int width){
  for(int i=0; i<height; i++){
	for(int j=0; j<width; j++){
	  dst[i][j] = src[i][j];
	}
  }
}

int64_t MultIntMatrix(int64_t** mat1, int64_t** mat2, int64_t** retMat, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width){
  if(m1Width != m2Height)
    return -1;

  for(int64_t i=0; i < m1Height; i++){
    for(int64_t j=0; j < m2Width; j++){
	  int64_t ele = 0;
	  for(int64_t k=0; k < m1Width; k++){
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

int MultFpMatrix(double** mat1, double** mat2, double** retMat, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width){
  if(m1Width != m2Height)
    return -1;

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
