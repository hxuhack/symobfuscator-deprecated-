#include "MathUtils.h"

class MillerRabin{
public: MillerRabin();
private: int iter;
public: bool testcomposite(__int128,__int128); // composite testing of a number | eg:- testcomposite(235,3) would result to true/1 which states that 3 is witness for 235 being composite
		bool testprime(__int128);               // Tests if a number is prime | eg:- testprime(19) would result in true/1
		void printprime(__int128 start, __int128 stop); // Prints prime from start to stop | eg:- printprime(2,9) would result in 2 3 5 7
		int factorize2(__int128);                        // Computes the power of 2 as a factor of the input number
		__int128  modulo(__int128  a,__int128  ,__int128  c); // Computes the modulus a^b%c
		__int128 mod(__int128 a,__int128  b, __int128  c);    // Computes the modulus a^b%c but is very slow for large numbers because its O(b)
		__int128* randgen(__int128 max, int num);               // Utility Function randgen computes num distinct random numbers from 1 to max-1 
		bool search(__int128 *arr, int num);                     // Utility Function used by randgen to compute distinct random numbers by searching arr, if it contains num or not
		void setiter(int);                                        // Sets the value of private variable iter
};
MillerRabin::MillerRabin()
{
    // seed the random number generator
	srand (time(NULL));
	// set default value of iter as 20
	iter=20;
}


bool MillerRabin::testprime(__int128 num)
{
	if(num<=1)
		return false;
	if(num%2==0)
		return false;
    
	// randarr :- array to store iter distinct random numbers ranging from 1 : num-1
	__int128 *randarr=new __int128[iter];
	randarr=randgen(num,iter);

	for(int i=0;i<iter;i++)
	{
		if(testcomposite(num,randarr[i])==1)
			return false;
	}
	return true;
}

bool MillerRabin::testcomposite(__int128 num,__int128 witness)
{
	int factor=factorize2(num-1); // factor 2 of the number num
	
	__int128 rem=(__int128)(num-1)/(int)pow(2.0,factor); // rem:- remainder after (num-1)/2^factor
	
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


void MillerRabin::printprime(__int128 start, __int128 stop)
{

 if(start==stop)
 {
	 //if(testprime(start))
     //std::cout<<start;
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

int MillerRabin::factorize2(__int128 num)
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

__int128  MillerRabin::modulo(__int128  a,__int128  b,__int128  c){
    __int128 p=1,r=a; 
    while(b > 0){
        if(b%2 == 1){
            p=(p*r)%c;
        }
        r = (r*r)%c; 
        b /= 2;
    }
    return p%c;
}

__int128 MillerRabin::mod(__int128 a, __int128 b, __int128 c)
{
	__int128 mod=a%c;
	for(int i=1;i<b;i++)
	{
        mod=(mod*a%c)%c;
	}
	return mod;
}

__int128 * MillerRabin::randgen(__int128 max,int num)
{
	__int128 *arr=new __int128[num];
	int ind=0;
	while(ind<num)
	{
		__int128 a=rand()%(max-1)+1;
		if(!search(arr,a))
		{
			arr[ind]=a;
			ind++;
		}

	}

	return arr;
}

bool MillerRabin::search(__int128 *arr, int num)
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
  int64_t t32 = 4294967295;//2^32-1
  int64_t start = (rand() * t32 + rand()) % mod; 
  MillerRabin mr;
  while(1){
    for(int64_t i=start; i < mod; i++){
      if(mr.testprime(i)){
	    return i;
	  }
    }
  }
}

int64_t InvMod(int64_t p, int64_t mod) {
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
