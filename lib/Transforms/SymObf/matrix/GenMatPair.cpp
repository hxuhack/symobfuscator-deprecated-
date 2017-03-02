#include <stdio.h>
#include <stdlib.h>	//srand
#include <time.h>	//time
#include <math.h>	//fabs

int InvMat(double** matrix,  double** inverse,  int size)
{
	double** clone = (double**)malloc(sizeof(double*) * size);

	for (int i = 0; i < size; i++)
	{
		clone[i] = (double*)malloc(sizeof(double) * size);
		for (int j = 0; j < size; j++)
		{
			clone[i][j] = matrix[i][j];
		}
	}

	for (int i = 0; i < size; i++)
	{		
		for (int j = i+1; j < size; j++)
		{
			if (clone[i][i] == 0 && clone[j][i] != 0)
			{
				double* swap = clone[i];
				clone[i] = clone[j];
				clone[j] = swap;

				swap = inverse[i];
				inverse[i] = inverse[j];
				inverse[j] = swap;
			}

			if (clone[i][i] != 1 )
			{
				double cons = clone[i][i];
				
				for (int k = 0; k < size; k++)
				{
					clone[i][k] = clone[i][k] / cons;
					inverse[i][k] = inverse[i][k] / cons;
				}
			}

			if (clone[i][i] == 1 && clone[j][i] != 0 )
			{
				double cons = clone[i][i] * fabs(clone[j][i]);
				
				if (0 < clone[j][i])
					cons = -1 * cons;
				
				for (int k = 0; k < size; k++)
				{	
					clone[j][k] = clone[j][k] + (clone[i][k] * cons);
					inverse[j][k] = inverse[j][k] + (inverse[i][k] * cons);	
				}				
			}
		}		

		if (clone[i][i] == 0)
		{
			return 0;
		}

		if (i == size-1)
		{
			double cons = clone[i][i];

			for (int k = 0; k < size; k++)
			{
				clone[i][k] = clone[i][k] / cons;
				inverse[i][k] = inverse[i][k] / cons;
			}
		}


		for (int j = 0; j < i; j++)
		{
			if (clone[j][i] != 0)
			{
				double cons = (clone[i][i] * fabs(clone[j][i]));
				if (0 < clone[j][i])
					cons = -1 * cons;

				for (int k = 0; k<size; k++)
				{
					clone[j][k] = clone[j][k] + (clone[i][k] * cons);
					inverse[j][k] = inverse[j][k] + (inverse[i][k] * cons);
				}

			}
		}
		
	}


	for (int i = 0; i < size; i++)
	{
		free(clone[i]);
	}
	free(clone);

	return 1;
}

void GenNonSinMat(double** matrix, double** identity, int size)
{
	srand((unsigned)time(NULL));

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			matrix[i][j] = rand() % 100;

			identity[i][j] = 0;
			if (i == j)
			{
				identity[i][j] = 1;
			}
		}
	}
}


int GenMatPair(double** mat, double** matInv, int dim)
{
	while (1)
	{
		GenNonSinMat(mat, matInv, dim); //generate S

		if (InvMat(mat, matInv, dim) != 0) //calcurate S inverse
			break;
	}

	return 0;
}
