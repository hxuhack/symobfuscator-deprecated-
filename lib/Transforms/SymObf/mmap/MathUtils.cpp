#include "MathUtils.h"

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
MillerRabin::MillerRabin()
{
    // seed the random number generator
	srand (time(NULL));
	// set default value of iter as 20
	iter=20;
}


bool MillerRabin::testprime(long long num)
{
	if(num<=1)
		return false;
	if(num%2==0)
		return false;
    
	// randarr :- array to store iter distinct random numbers ranging from 1 : num-1
	long long *randarr=new long long[iter];
	randarr=randgen(num,iter);

	for(int i=0;i<iter;i++)
	{
		if(testcomposite(num,randarr[i])==1)
			return false;
	}
	return true;
}

bool MillerRabin::testcomposite(long long num,long long witness)
{
	int factor=factorize2(num-1); // factor 2 of the number num
	
	long long rem=(long long)(num-1)/(int)pow(2.0,factor); // rem:- remainder after (num-1)/2^factor
	
	double *remain;
	
	    remain=new double[factor];

		int runs;
		(factor==0)?runs=1:runs=factor; // if factor is 0 then runs is 1 otherwise runs is factor

	    for(int i=0;i<runs;i++)
	  {
		remain[i]=modulo(witness,rem*pow(2.0,i),num);
	  }

		// Check for Compositness
	   if(remain[0]!=1)
	  {
        for(int i=0;i<factor;i++)
		{
			if(remain[i]!=(num-1))
			{

			}
			else
				return false;
		}
		return true;
	  }
	   return false;
	
}


void MillerRabin::printprime(long long start, long long stop)
{

 if(start==stop)
 {
	 if(testprime(start))
     std::cout<<start;
 }
 else
  {
	if(start>stop||start<=0)
		std::cout<<"Please Enter Correct Values";
	else
	{
	    int startind;

		(start%6==0) ? startind=start : startind=start+6-start%6;

		if(start<=2)
			std::cout<<2<<std::endl;
		if(stop>=3&&start<=3 )
			std::cout<<3<<std::endl;

		for(int i=startind;i<=stop+1;i+=6)
		{
			if(testprime(i-1)&&(i-1>=start))
				std::cout<<i-1<<std::endl;
			if(testprime(i+1)&&(i+1<=stop))
				std::cout<<i+1<<std::endl;
		}
	}
 }
}

int MillerRabin::factorize2(long long num)
{
	int count=0;

	while(1)
	{
		if(num%2==0&&num!=0)
		{
			num=num/2;
			count++;
		}
		else
			break;
	}
	return count;
}

long long  MillerRabin::modulo(long long  a,long long  b,long long  c){
    long long p=1,r=a; 
    while(b > 0){
        if(b%2 == 1){
            p=(p*r)%c;
        }
        r = (r*r)%c; 
        b /= 2;
    }
    return p%c;
}

long long MillerRabin::mod(long long a, long long b, long long c)
{
	long long mod=a%c;
	for(int i=1;i<b;i++)
	{
        mod=(mod*a%c)%c;
	}
	return mod;
}

long long * MillerRabin::randgen(long long max,int num)
{
	long long *arr=new long long[num];
	int ind=0;
	while(ind<num)
	{
		long long a=rand()%(max-1)+1;
		if(!search(arr,a))
		{
			arr[ind]=a;
			ind++;
		}

	}

	return arr;
}

bool MillerRabin::search(long long *arr, int num)
{
	if(arr[0]==NULL)
		return false;
	int size=sizeof(arr)/sizeof(arr[0]);

	for(int i=0;i<size;i++)
	{
		if(arr[i]==num)
			return true;
	}
	return false;
}

void MillerRabin::setiter(int num)
{
	iter=num;
}

int64_t GetPrime(int64_t mod){
  srand((unsigned)time(NULL));
  int64_t start = rand() % mod; 
  MillerRabin mr;
  for(int64_t i=start; i < mod; i++){
    if(mr.testprime(i)){
	  return i;
	}
  }
  for(int64_t i=start; i > 0; i--){
    if(mr.testprime(i)){
	  return i;
	}
  }
}

int GetPrime(int mod){
  srand((unsigned)time(NULL));
  int start = rand() % mod; 
  MillerRabin mr;
  for(int i=start; i < mod; i++){
    if(mr.testprime(i)){
	  return i;
	}
  }
  for(int i=start; i > 0; i--){
    if(mr.testprime(i)){
	  return i;
	}
  }
}

__int128 GetPrime(__int128 mod){
  srand((unsigned)time(NULL));
  __int128 start = rand() % mod; 
  MillerRabin mr;
  for(__int128 i=start; i < mod; i++){
    if(mr.testprime(i)){
	  return i;
	}
  }
  for(__int128 i=start; i > 0; i--){
    if(mr.testprime(i)){
	  return i;
	}
  }
}

int InvMod(int p, __int128 mod) {
	std::vector<__int128> r;
	std::vector<__int128> s;
	std::vector<__int128> t;
	std::vector<__int128> q;
	int size = 2;
	int i = 1;
	
	r.resize(size);
	s.resize(size);
	t.resize(size);
	q.resize(size);
	
	s[0] = 1;
	t[0] = 0;
	s[1] = 0;
	t[1] = 1;
	
	//input from the arguments
	r[0] = p;
	r[1] = mod;
	
	//change order of input numbers, higher number comes first
	if (r[0] < r[1]) {
		__int128 swp;
		swp = r[0];
		r[0] = r[1];	
		r[1] = swp;
	}

	//execute the extended euclidean algorithm
	do {
		i = i+1;
		
		//resize the vector and add one element per vector
		size++;
		r.resize(size);
		s.resize(size);
		t.resize(size);
		q.resize(size);
		
		r[i] = r[i-2] % r[i-1];
		q[i-1] = (r[i-2]-r[i]) / r[i-1];
		s[i] = s[i-2] - (q[i-1] * s[i-1]);
		t[i] = t[i-2] - (q[i-1] * t[i-1]);
	} while (r[i] != 0);
	//check if inverse is < 0 and if so, calculate mod n to display positive inverse
	if(s[i-1] < 0){
		s[i-1] = s[i-1] + r[0];
	}
	if(t[i-1] < 0){
		t[i-1] = t[i-1] + r[0];
	}

	return t[i-1];
}
