#include "MatUtils.h"

using namespace llvm;

int64_t PrintIntMat(int64_t** mat, int64_t height, int64_t width){
  for(int64_t i=0; i<height; i++){
	for(int64_t j=0; j<width; j++){
	  char tmpStr[8] = {0}; 
	  sprintf(tmpStr, "%d,", mat[i][j]);
	  errs()<< tmpStr;
	}
    errs()<<"\n";
  }
  errs()<<"---------------------\n";
}

int64_t CopyIntMat(int64_t** dst, int64_t** src,  int64_t height, int64_t width){
  for(int64_t i=0; i<height; i++){
	for(int64_t j=0; j<width; j++){
	  dst[i][j] = src[i][j];
	}
  }
}

int64_t MultIntMatrix(int64_t** mat1, int64_t** mat2, int64_t** retMat, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width, int64_t mod){
  if(m1Width != m2Height)
    return -1;

  for(int64_t i=0; i < m1Height; i++){
    for(int64_t j=0; j < m2Width; j++){
	  int64_t ele = 0;
	  for(int64_t k=0; k < m1Width; k++){
		ele = (ele + mat1[i][k] * mat2[k][j]) % mod;
	  }
	  retMat[i][j] = ele;
	  //printf("%df",ele);
	}
	//printf("\n");
  }
  //printf("*********************\n");
  return 0;
}

int64_t EncMatrix(int64_t** mat, int64_t height, int64_t width, int64_t setid){
  for(int64_t i=0; i < height; i++){
    for(int64_t j=0; j < width; j++){
	  MMapEncDefault(mat[i][j], mat[i][j], setid);
	}
  }
  return 0;
}
