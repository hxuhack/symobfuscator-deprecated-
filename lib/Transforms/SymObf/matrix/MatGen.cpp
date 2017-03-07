#include <stdio.h>
#include <stdlib.h>	//srand
#include <time.h>	//time
#include <math.h>	//fabs


int CreateIntMat(int64_t** mat, int height, int width, int rowId, int colId){
  for (int i=0; i< height; i++){
    for (int j=0; j< width; j++){
	  mat[i][j] = 0;
	  if((i==j && width !=1 && height != 1)||(i==rowId && j==colId)){
	    mat[i][j] = 1;  
	  }
	}
  }
}

int CreateFpMat(double** mat, int height, int width, int rowId, int colId){
  for (int i=0; i< height; i++){
    for (int j=0; j< width; j++){
	  mat[i][j] = 0;
	  if((i==j && width !=1 && height != 1)||(i==rowId && j==colId)){
	    mat[i][j] = 1;  
	  }
	}
  }
}


int GenIntMatPair(int64_t** mat, int64_t** matInv, int dim, int mod)
{
  srand((unsigned)time(NULL));
  int64_t iMat[dim][dim];
  int64_t t[dim-1];
  int64_t rowId[dim-1];
  for(int i=0; i<dim-1; i++){
    t[i] = rand() % 10 + 1; 
    rowId[i] = rand()%(i+1); 
  }

  for (int i = 0; i < dim; i++){
	for (int j = 0; j < dim; j++){
	  matInv[i][j] = 0;
	  if (i == j){
		mat[i][j] = 1;
		matInv[i][j] = 1;
		iMat[i][j] = mat[i][j];
	  }
	  if (i < j){
	    mat[i][j] = rand() % mod;
		iMat[i][j] = mat[i][j];
	  }
	  if (i>j){
		mat[i][j] = 0;
		iMat[i][j] = mat[i][j];
	  }
	}
	if(i>0){
	  for (int j = 0; j < dim; j++){
	    mat[i][j] = mat[i][j] + mat[rowId[i-1]][j] * t[i-1]; 
		iMat[i][j] = mat[i][j];
	  }
	}
  }
  for(int i = dim-1; i>0; i--){
    for(int j = 0; j < dim; j++){
	  iMat[i][j] = iMat[i][j] - iMat[rowId[i-1]][j]*t[i-1];
	  matInv[i][j] = matInv[i][j] - matInv[rowId[i-1]][j]*t[i-1];
	}
  }
  for (int i = dim-1; i >0; i--){
    for(int j = 0; j<i; j++){
	int tmp = iMat[j][i];
      for(int k = 0; k < dim; k++){
	    iMat[j][k] = iMat[j][k] - iMat[i][k] * tmp;
	    matInv[j][k] = matInv[j][k] - matInv[i][k] * tmp;
	  }
	}
  }
  return 0;
}
