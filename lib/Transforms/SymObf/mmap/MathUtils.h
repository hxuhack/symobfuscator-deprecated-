#ifndef _MILLERRABIN_H_
#define _MILLERRABIN_H_
#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <cstdlib>

int InvMod(int p, __int128 mod);//calculate modular multiplicative inverse
int64_t GetPrime(int64_t mod);
int GetPrime(int mod);
__int128 GetPrime(__int128 mod);
	
#endif
