#include <stdio.h>
#include <stdint.h>

int64_t** MM(const int64_t** mat1, const int64_t** mat2, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width){
  int64_t restMat[m1Height][m2Width]; 
  if(m1Width != m2Height)
    return restMat;

  for(int64_t i=0; i < m1Height; i++){
    for(int64_t j=0; j < m2Width; j++){
	  int64_t ele = 0;
	  for(int64_t k=0; k < m1Width; k++){
		ele = ele + *((int64_t*)mat1 + i*m1Width + k) * *((int64_t*)mat2+ k*m2Width + j);
	  }
	  restMat[i][j] = ele;
	}
  }
  return restMat;
}
