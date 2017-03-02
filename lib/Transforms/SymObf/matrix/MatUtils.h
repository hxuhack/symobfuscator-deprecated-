#ifndef _MATUTILS_H_
#define _MATUTILS_H_

#include <stdio.h>
#include <stdlib.h>	//srand
#include <time.h>	//time
#include <math.h>	//fabs
#include "llvm/Transforms/SymObf/Logger.h"

int GenMatPair(double** mat, double** matInv, int dim);

#endif

