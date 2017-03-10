#include <stdio.h>
#include <stdlib.h>	//srand
#include <time.h>	//time
#include <math.h>	//fabs


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
	}
  }
  return 0;
}

int64_t PrintStep(int64_t** mat, int64_t** matInv, int64_t dim, int64_t step){
  printf("====Step: %d===========\n", step);
  for (int64_t m = 0; m < dim; m++){
    for (int64_t n = 0; n < dim; n++){
	  printf("%d,",mat[m][n]);
	}
	printf(" || ");
    for (int64_t n = 0; n < dim; n++){
      printf("%d,",matInv[m][n]);
    }
    printf("\n");
  }
  return 0;
}

int64_t GenIntMatPair(int64_t** mat, int64_t** matInv, int64_t** iMat, int64_t dim, int64_t mod)
{
  srand((unsigned)time(NULL));
  int64_t t[dim];
  int64_t rowId[dim];
  for(int64_t i=0; i<dim; i++){
    t[i] = rand() % 10 + 1; 
	if(i!=0){
      rowId[i] = rand()%i;
	}
  }
  rowId[0] = rand()%dim;

  for (int64_t i = 0; i < dim; i++){
	for (int64_t j = 0; j < dim; j++){
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
	  for (int64_t j = 0; j < dim; j++){
	    mat[i][j] = (mat[i][j] + mat[rowId[i]][j] * t[i]) % mod;
		iMat[i][j] = mat[i][j];
	  }
	}
  }
  for (int64_t j = 0; j < dim; j++){
    mat[0][j] = (mat[0][j] + mat[rowId[0]][j] * t[0]) % mod;
	iMat[0][j] = mat[0][j];
  }
  for(int64_t j = 0; j < dim; j++){
    iMat[0][j] = (iMat[0][j] - iMat[rowId[0]][j]*t[0]) % mod;
    matInv[0][j] = (matInv[0][j] - matInv[rowId[0]][j]*t[0]) % mod;
    if(iMat[0][j] < 0){
  	  iMat[0][j] = iMat[0][j] + mod;
	}
	if(matInv[0][j] < 0){
	  matInv[0][j] = matInv[0][j] + mod;
	}
  }
  PrintStep(mat,matInv,dim,0);
  for(int64_t i = dim-1; i>0; i--){
    for(int64_t j = 0; j < dim; j++){
	  iMat[i][j] = (iMat[i][j] - iMat[rowId[i]][j]*t[i]) % mod;
	  matInv[i][j] = (matInv[i][j] - matInv[rowId[i]][j]*t[i]) % mod;
	  if(iMat[i][j] < 0){
		iMat[i][j] = iMat[i][j] + mod;
	  }
	  if(matInv[i][j] < 0){
		matInv[i][j] = matInv[i][j] + mod;
	  }
	}
    PrintStep(iMat,matInv,dim,dim-i);
  }
  for (int64_t i = dim-1; i >0; i--){
    for(int64_t j = 0; j<i; j++){
	int64_t tmp = iMat[j][i];
      for(int64_t k = 0; k < dim; k++){
	    iMat[j][k] = (iMat[j][k] - iMat[i][k] * tmp) % mod;
	    matInv[j][k] = (matInv[j][k] - matInv[i][k] * tmp) % mod;
	    if(iMat[i][j] < 0){
		  iMat[i][j] = iMat[i][j] + mod;
	    }
	    if(matInv[j][k] < 0){
		  matInv[j][k] = matInv[j][k] + mod;
	    }
	  }
	}
    PrintStep(iMat,matInv,dim,2*dim-i-1);
  }
  return 0;
}

int main(int argc, char** argv)
{
	int64_t dim = 0;
	printf("dim : ");	scanf("%d", &dim);
	printf("mat %d \n", dim);
	int64_t** mat = (int64_t**)malloc(sizeof(int64_t*) * dim);
	int64_t** matInv = (int64_t**)malloc(sizeof(int64_t*) * dim);
	int64_t** iMat = (int64_t**)malloc(sizeof(int64_t*) * dim);

	for (int64_t i = 0; i < dim; i++)
	{
		mat[i] = (int64_t*)malloc(sizeof(int64_t) * dim);
		matInv[i] = (int64_t*)malloc(sizeof(int64_t) * dim);
		iMat[i] = (int64_t*)malloc(sizeof(int64_t) * dim);
	}

	int64_t mod = 16;
	GenIntMatPair(mat,matInv,iMat, dim, mod);

	MultIntMatrix(mat,matInv,iMat,dim,dim,dim,dim, mod);
    printf("===========Identity Mat========\n");
    for (int64_t i = 0; i < dim; i++){
	  for (int64_t j = 0; j < dim; j++){
		  printf("%d,",iMat[i][j]);
	  }
	  printf("\n");
	}
	for (int64_t i = 0; i < dim; i++)
	{
		free(matInv[i]);
		free(mat[i]);
	}
	free(matInv);
	free(mat);

	return 0;
}
