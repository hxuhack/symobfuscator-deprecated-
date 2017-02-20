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
  int i0 = argv[1][0]%2;
  int i1 = argv[1][1]%2;
  printf("i0=%d,i1=%d\n",i0,i1);
  int mat1[3][2];

  mat1[0][0] = 0;
  mat1[0][1] = 1;
  mat1[1][0] = 0;
  mat1[1][1] = 1;
  mat1[2][0] = 0;
  mat1[2][1] = 1;
  

  int mat2[2][3];

  mat2[0][0] = 0;
  mat2[0][1] = 1;
  mat2[0][2] = 0;
  mat2[1][0] = 1;
  mat2[1][1] = 0;
  mat2[1][2] = 1;

  int mat01[3][2];

  mat01[0][0] = 0;
  mat01[0][1] = 0;
  mat01[1][0] = 0;
  mat01[1][1] = 0;
  mat01[2][0] = 0;
  mat01[2][1] = 0;
  

  int mat02[2][3];

  mat02[0][0] = 0;
  mat02[0][1] = 1;
  mat02[0][2] = 0;
  mat02[1][0] = 1;
  mat02[1][1] = 0;
  mat02[1][2] = 1;

  int** selMat1;
  int** selMat2;
  if(i0 == 0){
	 selMat1 = mat01;
  }
  else{
	selMat1 = mat1;
  }
  if(i1 == 0){
	 selMat2 = mat02;
  }
  else{
	selMat2 = mat2;
  }
  int** mat3 = MM(selMat1,selMat2,3,2,2,3);

}
