#include "MMap.h"
#include "MathUtils.h"

using namespace std;

int64_t MMapInitParam(int64_t z, int64_t n, int64_t setnum){
  sp->Z = z;
  sp->N = n;
  sp->setnum = setnum;
  sp->z = (int64_t *) malloc (sizeof(int64_t) * sp->Z); 
  sp->h = (int64_t *) malloc (sizeof(int64_t) * sp->N); 
  sp->g = (int64_t *) malloc (sizeof(int64_t) * sp->N); 
  sp->ginv = (int64_t *) malloc (sizeof(int64_t) * sp->N); 
  sp->p = (int64_t *) malloc (sizeof(int64_t) * sp->N); 
  sp->setid = (vector<int64_t> *) malloc (sizeof(vector<int64_t>) * sp->setnum); 

  srand((unsigned)time(NULL));
  for(int64_t i =0; i< sp->Z; i++){
    sp->z[i] = rand() % zMax; 
  }

  for(int64_t i =0; i< sp->N; i++){
    sp->g[i] = GetPrime(gMax); 
    sp->p[i] = GetPrime(pMax); 
    sp->ginv[i] = InvMod(sp->g[i], sp->p[i]); 
    sp->h[i] = rand() % hMax; 
    sp->q = sp->q * sp->p[i]; 
  }
  //TODO:To implement more secure set mechanism
  for(int64_t i =0; i< sp->setnum; i++){
    sp->setid[i].push_back(sp->z[i]);
  }
}


int64_t MMapIsZero(int64_t u){
  double pzt = 0;
  for(int64_t i=0; i<sp->N; i++){
	double mid = 1;
	int64_t tail = 1;
    for(int64_t j=0; j<sp->Z; j++){
	  mid = fmod((mid * sp->ginv[i] / sp->z[j]),sp->p[i]);
	}
    for(int64_t k=0; k<sp->N; k++){
	  if (k!=i){
	    tail = tail * sp->p[k] % sp->q;
	  }
	}
    pzt = pzt + sp->h[i] * mid * tail; 
  }
  int64_t ret = (int64_t) fmod((pzt * u), sp->q);
}

int64_t MMapEnc(int64_t m, int64_t mid, int64_t setid){
  int64_t set = 1;
  for(int i=0; i<sp->setid[setid].size(); i++){
	int64_t zid = sp->setid[setid][i];
    set = set * sp->z[zid];
  }
  int64_t rest = (sp->h[mid] * sp->g[mid] + m) * set % sp->p[mid];
  return rest;
}

int64_t MMapAdd(int64_t m, int64_t mid, int64_t setid){

}

int64_t MMapMult(int64_t m, int64_t mid, int64_t set1id, int64_t set2id){

}
