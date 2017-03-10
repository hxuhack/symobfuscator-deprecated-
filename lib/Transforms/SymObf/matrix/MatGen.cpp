#include <stdio.h>
#include <stdlib.h>	//srand
#include <time.h>	//time
#include <math.h>	//
#include "llvm/Transforms/SymObf/Utils.h"

using namespace llvm;

int64_t CreateIntMat(int64_t** mat, int64_t height, int64_t width, int64_t rowId, int64_t colId){
  for (int64_t i=0; i< height; i++){
    for (int64_t j=0; j< width; j++){
	  mat[i][j] = 0;
	  if((i==j && width !=1 && height != 1)||(i==rowId && j==colId)){
	    mat[i][j] = 1;  
	  }
	}
  }
}

int64_t CreateFpMat(double** mat, int64_t height, int64_t width, int64_t rowId, int64_t colId){
  for (int64_t i=0; i< height; i++){
    for (int64_t j=0; j< width; j++){
	  mat[i][j] = 0;
	  if((i==j && width !=1 && height != 1)||(i==rowId && j==colId)){
	    mat[i][j] = 1;  
	  }
	}
  }
}


int64_t GenIntMatPair(int64_t** mat, int64_t** matInv, int64_t dim, int64_t mod)
{
  srand((unsigned)time(NULL));
  int64_t iMat[dim][dim];
  int64_t t[dim];
  int64_t rowId[dim];
  for(int64_t i=0; i<dim-1; i++){
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
	    mat[i][j] = (mat[i][j] + mat[rowId[i]][j] * t[i])%mod; 
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
  }
  for (int64_t i = dim-1; i >0; i--){
    for(int64_t j = 0; j<i; j++){
	int64_t tmp = iMat[j][i];
      for(int64_t k = 0; k < dim; k++){
	    iMat[j][k] = (iMat[j][k] - iMat[i][k] * tmp) % mod;
	    matInv[j][k] = (matInv[j][k] - matInv[i][k] * tmp) % mod;
	    if(iMat[j][k] < 0){
	      iMat[j][k] = iMat[j][k] + mod;
	    }
	    if(matInv[j][k] < 0){
	      matInv[j][k] = matInv[j][k] + mod;
	    }
	  }
	}
  }
  errs()<<"Identity Matrix Verification\n";
  for(int64_t i=0; i < dim; i++){
    for(int64_t j=0; j < dim; j++){
	  int64_t ele = 0;
	  for(int64_t k=0; k < dim; k++){
		ele = (ele + mat[i][k] * matInv[k][j]) % mod;
	  }
	  errs()<<","<<ele;
	}
	errs()<<"\n";
  }
  errs()<<"-----------------\n";
  return 0;
}

