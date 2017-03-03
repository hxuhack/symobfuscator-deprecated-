#ifndef _MATUTILS_H_
#define _MATUTILS_H_

#include <stdio.h>
#include <stdlib.h>	//srand
#include <time.h>	//time
#include <math.h>	//fabs
#include "llvm/Transforms/SymObf/Logger.h"

int CreateMat(double** mat, int height, int width, int rowId, int colId);
int GenRandMatPair(double** mat, double** matInv, int dim, int mod);
int MatrixMult(double** mat1, double** mat2, double** retMat, int64_t m1Height, int64_t m1Width, int64_t m2Height, int64_t m2Width);
int PrintMat(double** mat, int height, int width);
int CopyMat(double** src, double** dst, int height, int width);

#endif

