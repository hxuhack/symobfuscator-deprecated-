#include "MMap.h"
#include "MathUtils.h"

using namespace std;

int64_t zMax = 8;
int64_t hMax = 16;
int64_t gMax = 2048;
int64_t pMax = 2147483647;

secparam sp;

int64_t MMapInitParam(int64_t z, int64_t n, int64_t setnum){
  printf("Initializing MMap parameters...\n");
  sp.Z = z;
  sp.N = n;
  sp.setnum = setnum;
  sp.z = (int64_t *) malloc (sizeof(int64_t) * sp.Z); 
  sp.h = (int64_t *) malloc (sizeof(int64_t) * sp.N); 
  sp.g = (int64_t *) malloc (sizeof(int64_t) * sp.N); 
  sp.ginv = (int64_t *) malloc (sizeof(int64_t) * sp.N); 
  sp.p = (int64_t *) malloc (sizeof(int64_t) * sp.N); 
  sp.setid = (vector<int64_t> *) malloc (sizeof(vector<int64_t>) * sp.setnum); 

  srand((unsigned)time(NULL));
  for(int64_t i =0; i< sp.Z; i++){
    sp.z[i] = rand() % zMax + 1; //z cannot be 0; 
  }

  for(int64_t i =0; i< sp.N; i++){
    sp.g[i] = GetPrime(gMax); 
    sp.p[i] = GetPrime(pMax); 
    sp.ginv[i] = InvMod(sp.g[i], sp.p[i]); 
    sp.h[i] = rand() % hMax + 1; //h[1] cannot be 0; 
    sp.q = sp.q * sp.p[i]; 
  }
  //TODO:To implement more secure set mechanism
  for(int64_t i =0; i< sp.setnum; i++){
    sp.setid[i].push_back(sp.z[i]);
  }
  printf("Finish MMap parameter initialization...\n");
  for(int64_t i =0; i< sp.N; i++){
    printf("g[%d] = %d\n", i, sp.g[i]);
  }
  for(int64_t i =0; i< sp.N; i++){
    printf("p[%d] = %d\n", i, sp.p[i]);
  }
  printf("q = %d\n", sp.q);
  for(int64_t i =0; i< sp.Z; i++){
    printf("z[%d] = %d\n", i, sp.z[i]);
  }
  return 0;
}

int64_t MMapIsZero(double u){
  double pzt = 0;
  for(int64_t i=0; i<sp.N; i++){
	int mid = 1;
	int64_t tail = 1;
    for(int64_t j=0; j<sp.Z; j++){
	  if(sp.p[i]==0){
	    printf("!!!p[i]=0. Denominator cannot be 0!\n");
	  }
	  mid = (mid * sp.ginv[i] * sp.z[j]) % sp.p[i];
	}
    for(int64_t k=0; k<sp.N; k++){
	  if (k!=i){
	    tail = tail * sp.p[k] % sp.q;
	  }
	}
    pzt = pzt + sp.h[i] * mid * tail; 
  }
  if(sp.q==0){
    printf("!!!q=0. Denominator cannot be 0!\n");
  }
  pzt = fmod((pzt * u), sp.q);
  printf("pzt = %.5f\n",pzt);
  if(abs(pzt) < sp.q * pow(2,-2-sp.N-sp.alphalen))
	return 1;
  return 0;
}

double MMapEnc(int64_t m, int64_t mid, int64_t setid){
  int64_t set = 1;
  for(int i=0; i<sp.setid[setid].size(); i++){
	int64_t z = sp.setid[setid][i];
    set = set * z;
  }
  printf("hi = %d, gi = %d, m = %d, set = %d, p = %d\n", sp.h[mid], sp.g[mid], m, set, sp.p[mid]);
  double rest = fmod((sp.h[mid] * sp.g[mid] + m) / set, sp.p[mid]);
  return rest;
}

double MMapAdd(double u1, double u2, int64_t mid){
  double rest = fmod(u1+u2, sp.p[mid]);
  return rest;
}

double MMapMult(double u1, double u2, int64_t mid){
  double rest = fmod(u1*u2, sp.p[mid]);
  return rest;
}
