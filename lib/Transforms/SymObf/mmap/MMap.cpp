#include "MMap.h"
#include "MathUtils.h"

using namespace std;

secparam sp;
int64_t MMapInitParam(int64_t z, int64_t n, int64_t setnum){
  //LOG(L_INFO) << "Initializing MMap parameters...";
  sp.Z = z;
  sp.N = n;
  sp.setnum = setnum;
  sp.z = (int64_t *) malloc (sizeof(int64_t) * sp.Z); 
  sp.h = (int64_t *) malloc (sizeof(int64_t) * sp.N); 
  sp.g = (int64_t *) malloc (sizeof(int64_t) * sp.N); 
  sp.ginv = (int64_t *) malloc (sizeof(int64_t) * sp.N); 
  sp.p = (int64_t *) malloc (sizeof(int64_t) * sp.N); 
  sp.setid = (vector<int64_t> *) malloc (sizeof(vector<int64_t>) * sp.setnum); 

  int64_t zLen, hLen, pLen, gLen;
  zLen = 2;
  hLen = 8;
  gLen = 8;
  pLen = 64;
  int64_t zMax = pow(2, zLen); 
  int64_t hMax = pow(2, hLen); 
  int64_t gMax = pow(2, gLen); 
  int64_t pMax = pow(2, pLen); 
  sp.lambda = 8;

  srand((unsigned)time(NULL));

  for(int64_t i =0; i< sp.Z; i++){
	sp.z[i] = rand() % zMax + 1;
  }

  sp.q = 1;
  for(int64_t i =0; i< sp.N; i++){
	sp.g[i] = GetPrime(gMax); 
	sp.p[i] = GetPrime(pMax); 
	sp.ginv[i] = InvMod(sp.g[i], sp.p[i]); 
	sp.h[i] = rand() % hMax + 1; //h[1] cannot be 0; 
	sp.q = sp.q * sp.p[i];
  }

  sp.pzt = 0;
  for(int64_t i=0; i<sp.N; i++){
	int64_t mid = 1;
	int64_t tail = 1;
    for(int64_t j=0; j<sp.Z; j++){
	  mid = (mid * sp.ginv[i] * sp.z[j]) % sp.p[i] % sp.q;
	}
    for(int64_t k=0; k<sp.N; k++){
	  if (k!=i){
	    tail = tail * sp.p[k] % sp.q;
	  }
	}
    sp.pzt = (sp.pzt + sp.h[i] * mid * tail) % sp.q; 
  }

  //TODO:To implement more secure set mechanism
  for(int64_t i =0; i< sp.setnum; i++){
    sp.setid[i].push_back(i);
  }
  /*
  printf("Finish MMap parameter initialization...\n");
  cout<<"pzt = " << sp.pzt<< endl;
  for(int64_t i =0; i< sp.N; i++){
    cout << "g[" <<i<<"]="<< sp.g[i]<<endl;
  }
  for(int64_t i =0; i< sp.N; i++){
    cout << "p[" <<i<<"]="<< sp.p[i]<<endl;
  }
  cout<<"q = "<<sp.q<<endl;
  for(int64_t i =0; i< sp.Z; i++){
    cout << "z[" <<i<<"]="<< sp.z[i]<<endl;
  }
  */
  return 0;
}

int64_t GetPzt(){
  return sp.pzt;
}

int64_t GetQ(){
  return sp.q;
}

int64_t GetExp(){
  return sp.lambda * -1;
}

int64_t MMapIsZero(int64_t u){
  int64_t left, right;
  left = sp.pzt * u % sp.q;
  right = sp.q * pow(2, sp.lambda * -1);
  cout << "left = "<<left<<", right = " << right <<endl;
  if(left < right){
    cout << "Zero test returns true!" <<endl;
	return 0;
  }
  return 1;
}


int64_t MMapEnc(int64_t m, int64_t mid, int64_t setid){
  int64_t set = 1;
  for(int i=0; i<sp.setid[setid].size(); i++){
	int64_t zinv = InvMod(sp.z[sp.setid[setid][i]], sp.p[i]); 
    set = set * zinv;
  }
  int tmpRi = rand() % 16 + 1;
  int result = (tmpRi * sp.g[mid] + m) * set % sp.p[mid];
  return result;
}

int64_t MMapEncDefault(int64_t m){
  return MMapEnc(m, 0, 0);
}
int64_t MMapAdd(int64_t u1, int64_t u2, int64_t mid){
  int64_t result = (u1 + u2) % sp.p[mid];
  return result;
}

int64_t MMapMult(int64_t u1, int64_t u2, int64_t mid){
  int64_t result = (u1 * u2) % sp.p[mid];
  return result;
}
