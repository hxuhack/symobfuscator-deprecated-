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
  int len = 2;


  int mat01[2][2], mat00[2][2], mat11[2][2], mat10[2][2];
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

  int** mat[2][2] = {{mat00, mat01}, {mat10,  mat11}}; 

  int initMat[1][2] = {1,0};
  int inp = (argc & 1);

  int** interMat = MM(initMat, mat[inp][0], 1,2,2,2);
  for(int i = 1; i<len;i++){
	  //To get the k-th bit of n: (n & ( 1 << k )) >> k;
	  inp = (argc & (1<<i)) >> i;
      interMat = MM(interMat, mat[inp][i], 1,2,2,2);
  }
}
