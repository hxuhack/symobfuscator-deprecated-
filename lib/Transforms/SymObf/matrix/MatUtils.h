#ifndef _MATUTILS_H_
#define _MATUTILS_H_

#include <stdio.h>
#include <stdlib.h>	//srand
#include <time.h>	//time
#include <math.h>	//fabs
#include "llvm/Transforms/SymObf/Logger.h"

int64_t CreateFpMat(double** mat, int64_t height, int64_t width, int64_t rowId, int64_t colId);
int64_t MultFpMatrix(double** mat1, double** mat2, double** retMat, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width, int64_t mod);
int64_t PrintFpMat(double** mat, int64_t height, int64_t width);
int64_t CopyFpMat(double** dst, double** src, int64_t height, int64_t width);

int64_t CreateIntMat(int64_t** mat, int64_t height, int64_t width, int64_t rowId, int64_t colId);
int64_t MultIntMatrix(int64_t** mat1, int64_t** mat2, int64_t** retMat, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width, int64_t mod);
int64_t PrintIntMat(int64_t** mat, int64_t height, int64_t width);
int64_t CopyIntMat(int64_t** dst, int64_t** src, int64_t height, int64_t width);
int64_t GenIntMatPair(int64_t** mat, int64_t** matInv, int64_t dim, int64_t mod);
#endif

