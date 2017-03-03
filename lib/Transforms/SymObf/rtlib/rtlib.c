#include <stdio.h>
#include <stdint.h>

double** MatrixMult(double** mat1, double** mat2, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width){
  printf("Entering MatrixMult: perform maltiplication...\n");
  double retMat[m1Height][m2Width];
  if(m1Width != m2Height)
    return retMat;

  for(int64_t i=0; i < m1Height; i++){
    for(int64_t j=0; j < m2Width; j++){
	  double ele = 0;
	  for(int64_t k=0; k < m1Width; k++){
		ele = ele + *((double*)(mat1 + i*m1Width + k)) * *((double*) (mat2+ k*m2Width + j));
		//ele = ele + mat1[i][k] * mat2[k][j];
	  }
	  retMat[i][j] = ele;
	  printf("%.3f",ele);
	}
	printf("\n");
  }
  printf("*********************\n");
  return retMat;
}
