#include <stdio.h>
#include <stdlib.h>	//srand
#include <time.h>	//time
#include <math.h>	//fabs


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
	}
  }
  return 0;
}

int64_t PrintStep(int64_t** mat, int64_t** matInv, int dim, int step){
  printf("====Step: %d===========\n", step);
  for (int m = 0; m < dim; m++){
    for (int n = 0; n < dim; n++){
	  printf("%d,",mat[m][n]);
	}
	printf(" || ");
    for (int n = 0; n < dim; n++){
      printf("%d,",matInv[m][n]);
    }
    printf("\n");
  }
  return 0;
}

int GenIntMatPair(int64_t** mat, int64_t** matInv, int64_t** iMat, int dim, int mod)
{
  srand((unsigned)time(NULL));
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
  PrintStep(mat,matInv,dim,0);
  for(int i = dim-1; i>0; i--){
    for(int j = 0; j < dim; j++){
	  iMat[i][j] = iMat[i][j] - iMat[rowId[i-1]][j]*t[i-1];
	  matInv[i][j] = matInv[i][j] - matInv[rowId[i-1]][j]*t[i-1];
	}
    PrintStep(iMat,matInv,dim,dim-i);
  }
  for (int i = dim-1; i >0; i--){
    for(int j = 0; j<i; j++){
	int tmp = iMat[j][i];
      for(int k = 0; k < dim; k++){
	    iMat[j][k] = iMat[j][k] - iMat[i][k] * tmp;
	    matInv[j][k] = matInv[j][k] - matInv[i][k] * tmp;
	  }
	}
    PrintStep(iMat,matInv,dim,2*dim-i-1);
  }
  return 0;
}

int main(int argc, char** argv)
{
	int dim = 0;
	printf("dim : ");	scanf("%d", &dim);
	printf("mat %d \n", dim);
	int64_t** mat = (int64_t**)malloc(sizeof(int64_t*) * dim);
	int64_t** matInv = (int64_t**)malloc(sizeof(int64_t*) * dim);
	int64_t** iMat = (int64_t**)malloc(sizeof(int64_t*) * dim);

	for (int i = 0; i < dim; i++)
	{
		mat[i] = (int64_t*)malloc(sizeof(int64_t) * dim);
		matInv[i] = (int64_t*)malloc(sizeof(int64_t) * dim);
		iMat[i] = (int64_t*)malloc(sizeof(int64_t) * dim);
	}

	GenIntMatPair(mat,matInv,iMat, dim,4);

	MultIntMatrix(mat,matInv,iMat,dim,dim,dim,dim);
    printf("===========Identity Mat========\n");
    for (int i = 0; i < dim; i++){
	  for (int j = 0; j < dim; j++){
		  printf("%d,",iMat[i][j]);
	  }
	  printf("\n");
	}
	for (int i = 0; i < dim; i++)
	{
		free(matInv[i]);
		free(mat[i]);
	}
	free(matInv);
	free(mat);

	return 0;
}
