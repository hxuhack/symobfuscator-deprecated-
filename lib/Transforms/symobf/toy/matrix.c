#include <stdio.h>


int** MM(const int** mat1, const int** mat2, int m1Height, int m1Width, int m2Height, int m2Width){
  int restMat[m1Height][m2Width]; 
  if(m1Width != m2Height)
    return restMat;

  for(int i=0; i < m1Height; i++){
    for(int j=0; j < m2Width; j++){
	  int ele = 0;
	  for(int k=0; k < m1Width; k++){
		ele = ele + *((int*)mat1 + i*m1Width + k) * *((int*)mat2+ k*m2Width + j);
	  }
	  restMat[i][j] = ele;
	  printf("%d\n",ele);
	}
  }
  return restMat;
}

int main(int argc, char** argv){
  int inp[2];
  inp[0] = argv[1][0]%2;
  inp[1] = argv[1][1]%2;
  int len = 2;

  int mat[2][2][2][2]; 

  mat[0][1][0][0] = 0;
  mat[0][1][0][1] = 1;
  mat[0][1][1][1] = 1;
  mat[0][1][1][0] = 0;
  

  mat[0][0][0][0] = 0;
  mat[0][0][0][1] = 1;
  mat[0][0][1][1] = 0;
  mat[0][0][1][0] = 0;

  mat[1][1][0][0] = 0;
  mat[1][1][0][1] = 1;
  mat[1][1][1][1] = 1;
  mat[1][1][1][0] = 0;
  

  mat[1][0][0][0] = 0;
  mat[1][0][0][1] = 1;
  mat[1][0][1][0] = 0;
  mat[1][0][1][1] = 0;

  int** selMat1;
  int** selMat2;
  int initMat[1][2] = {1,0};
  int** interMat = MM(initMat, mat[0][inp[0]], 1,2,2,2);
  for(int i = 1; i<len;i++){
      interMat = MM(interMat, mat[i][inp[i]], 1,2,2,2);
  }
}
