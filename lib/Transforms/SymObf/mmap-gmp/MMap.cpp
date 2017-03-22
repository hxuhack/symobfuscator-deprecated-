#include "MMap.h"
#include "MathUtils.h"

using namespace std;

int64_t zLen, hLen, beta, gMax, alpha, pLen, gLen;

mpz_t tmpRi;

secparam sp;

void GetMpzPrime(mpz_t prime, int64_t len){
  mpz_init(prime);
  gmp_randstate_t grt;  
  gmp_randinit_default(grt); 
  gmp_randseed_ui(grt, time(NULL));
  mpz_urandomb(prime, grt, len); 
  if(mpz_even_p(prime))  
    mpz_add_ui(prime, prime, 1);
  while(!mpz_probab_prime_p(prime, 25) > 0)
    mpz_add_ui(prime, prime, 2);  
}

int64_t MMapInitParam(int64_t z, int64_t n, int64_t setnum){
  printf("Initializing MMap parameters...\n");
  sp.Z = z;
  sp.N = n;
  sp.setnum = setnum;
  sp.z = (mpz_t *) malloc (sizeof(mpz_t) * sp.Z); 
  sp.h = (mpz_t *) malloc (sizeof(mpz_t) * sp.N); 
  sp.g = (mpz_t *) malloc (sizeof(mpz_t) * sp.N); 
  sp.ginv = (mpz_t *) malloc (sizeof(mpz_t) * sp.N); 
  sp.p = (mpz_t *) malloc (sizeof(mpz_t) * sp.N); 
  sp.setid = (vector<int64_t> *) malloc (sizeof(vector<int64_t>) * sp.setnum); 

  zLen = 8;
  hLen = 64;
  gLen = 64;
  pLen = 1024;
  sp.lambda = 32;
  alpha = sp.lambda;
  beta = sp.lambda;

  gmp_randstate_t grt;  
  gmp_randinit_default(grt); 
  gmp_randseed_ui(grt, time(NULL));

  for(int64_t i =0; i< sp.Z; i++){
	mpz_init(sp.z[i]); 
    mpz_urandomb(sp.z[i], grt, zLen);//TODO:exclude 0; 
  }

  mpz_init_set_ui(sp.q, 1);
  for(int64_t i =0; i< sp.N; i++){
    //sp.g[i] = GetPrime(gMax);
	mpz_init(sp.g[i]); 
	GetMpzPrime(sp.g[i], gLen); 
	mpz_init(sp.p[i]); 
	GetMpzPrime(sp.p[i], pLen); 
    //sp.ginv[i] = InvMod(sp.g[i], sp.p[i]); 
	mpz_init(sp.ginv[i]); 
	mpz_invert(sp.ginv[i], sp.g[i], sp.p[i]);
	mpz_init(sp.h[i]);
    mpz_urandomb(sp.h[i], grt, hLen);//TODO:exclude 0; 
    //sp.q = sp.q * sp.p[i]; 
	mpz_mul(sp.q, sp.q, sp.p[i]);
  }

  for(int64_t i=0; i<sp.N; i++){
	mpz_t mid, tail;
	mpz_init_set_ui(mid,1);
	mpz_init_set_ui(tail,1);
	mpz_mul(mid,mid,sp.ginv[i]);
    for(int64_t j=0; j<sp.Z; j++){
	  //mid = (mid * sp.ginv[i] * sp.z[j]) % sp.p[i] % sp.q;
	  mpz_mul(mid,mid,sp.z[j]);
	  mpz_mod(mid,mid,sp.p[i]);
	  mpz_mod(mid,mid,sp.q);
	}
    for(int64_t k=0; k<sp.N; k++){
	  if (k!=i){
	    //tail = tail * sp.p[k] % sp.q;
	    mpz_mul(tail,tail,sp.p[k]);
	    mpz_mod(tail,tail,sp.q);
	  }
	}
    //sp.pzt = (sp.pzt + sp.h[i] * mid * tail)%sp.q; 
	mpz_mul(mid,mid,sp.h[i]);
	mpz_mul(mid,mid,tail);
	mpz_add(sp.pzt,sp.pzt,mid);
	mpz_mod(sp.pzt,sp.pzt,sp.q);
  }

  //TODO:To implement more secure set mechanism
  cout<<"pzt = " << sp.pzt<< endl;
  for(int64_t i =0; i< sp.setnum; i++){
    sp.setid[i].push_back(i);
  }
  printf("Finish MMap parameter initialization...\n");
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
  return 0;
}

int64_t MMapIsZero(mpz_t u){
  mpz_t left, right;
  //left = sp.pzt * u % sp.q;
  mpz_init(left);
  mpz_mul(left,sp.pzt,u);
  mpz_mod(left,left,sp.q);
  //right = sp.q * pow(2,- alpha - beta);
  mpz_init(right);
  mpz_t tmp,tmp2;
  mpz_init(tmp);
  mpz_init_set_ui(tmp2,2);
  mpz_pow_ui(tmp,tmp2,alpha + beta + 11);
  mpz_div(right,sp.q,tmp);

  cout << "left = "<<left<<", right = " << right <<endl;
  if(mpz_cmp(left,right) < 0){
    cout << "Zero test returns true!" <<endl;
	return 1;
  }
  return 0;
}

void MMapEnc(mpz_t result, int64_t m, int64_t mid, int64_t setid){
  mpz_t set, tmpM;
  mpz_init_set_ui(set,1);
  mpz_init_set_ui(tmpM,m);
  for(int i=0; i<sp.setid[setid].size(); i++){
	mpz_t zinv;
	mpz_init(zinv);
    mpz_invert(zinv, sp.z[sp.setid[setid][i]], sp.p[i]); 
    mpz_mul(set, set, zinv);
  }
  int tmpR = rand() % 16 + 1;
  mpz_t tmpRi;
  mpz_init_set_ui(tmpRi,tmpR);
  //result = (tmpRi * sp.g[mid] + m) * set % sp.p[mid];
  mpz_mul(result, tmpRi, sp.g[mid]);
  mpz_add(result, result, tmpM);
  mpz_mul(result, result, set);
  mpz_mod(result, result, sp.p[mid]);
}

void MMapAdd(mpz_t result, mpz_t u1, mpz_t u2, int64_t mid){
  mpz_add(result, u1, u2);
  mpz_mod(result, result, sp.p[mid]);
}

void MMapMult(mpz_t result, mpz_t u1, mpz_t u2, int64_t mid){
  mpz_mul(result, u1, u2);
  mpz_mod(result, result, sp.p[mid]);
}
