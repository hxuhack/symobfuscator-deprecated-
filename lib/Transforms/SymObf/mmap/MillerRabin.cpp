#include "MillerRabin.h"

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
