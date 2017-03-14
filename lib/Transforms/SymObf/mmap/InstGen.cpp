#include "MMap.h" 

int InitSecParam(int64_t z, int64_t n){
  sp.Z = z;
  sp.N = n;
  sp->z = (int64_t *) malloc (sizeof(int64_t) * sp.Z); 
  sp->g = (int64_t *) malloc (sizeof(int64_t) * sp.N); 
  sp->p = (int64_t *) malloc (sizeof(int64_t) * sp.N); 

  srand((unsigned)time(NULL));
  for(int i =0; i< sp.Z; i++){
    sp->z[i] = rand() % zMax; 
  }
  for(int i =0; i< sp.N; i++){
    sp->g[i] = GetPrime(gMax); 
    sp->p[i] = GetPrime(pMax); 
    sp.q = sp.q * sp->p[i]; 
  }
}
