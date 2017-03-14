#ifndef _MILLERRABIN_H_
#define _MILLERRABIN_H_
#include <iostream>
#include <time.h>
#include <math.h>
#include <cstdlib>

class MillerRabin{

public: MillerRabin();

private: int iter;

public: bool testcomposite(long long,long long); // composite testing of a number | eg:- testcomposite(235,3) would result to true/1 which states that 3 is witness for 235 being composite
		bool testprime(long long);               // Tests if a number is prime | eg:- testprime(19) would result in true/1
		void printprime(long long start, long long stop); // Prints prime from start to stop | eg:- printprime(2,9) would result in 2 3 5 7
		int factorize2(long long);                        // Computes the power of 2 as a factor of the input number
		long long  modulo(long long  a,long long  ,long long  c); // Computes the modulus a^b%c
		long long mod(long long a,long long  b, long long  c);    // Computes the modulus a^b%c but is very slow for large numbers because its O(b)
		long long* randgen(long long max, int num);               // Utility Function randgen computes num distinct random numbers from 1 to max-1 
		bool search(long long *arr, int num);                     // Utility Function used by randgen to compute distinct random numbers by searching arr, if it contains num or not
		void setiter(int);                                        // Sets the value of private variable iter
};
#endif
