#ifndef _MATUTILS_H_
#define _MATUTILS_H_

#include <stdio.h>
#include <stdlib.h>	//srand
#include <time.h>	//time
#include <math.h>	//fabs
#include "llvm/Transforms/SymObf/Logger.h"

int CreateFpMat(double** mat, int height, int width, int rowId, int colId);
int MultFpMatrix(double** mat1, double** mat2, double** retMat, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width);
int PrintFpMat(double** mat, int height, int width);
int CopyFpMat(double** src, double** dst, int height, int width);

int CreateIntMat(int64_t** mat, int height, int width, int rowId, int colId);
int MultIntMatrix(int64_t** mat1, int64_t** mat2, int64_t** retMat, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width);
int PrintIntMat(int64_t** mat, int height, int width);
int CopyIntMat(int64_t** src, int64_t** dst, int height, int width);
int GenIntMatPair(int64_t** mat, int64_t** matInv, int dim, int mod);
#endif

