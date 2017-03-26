#include "MMap.h"
#include "MathUtils.h"

using namespace std;

secparam sp;

std::ostream& operator<<( std::ostream& dest, __int128 value)
{
    std::ostream::sentry s( dest );
    if ( s ) {
        __uint128_t tmp = value < 0 ? -value : value;
        char buffer[128];
        char* d = &buffer[127];
        do
        {
            -- d;
            *d = "0123456789"[tmp % 10];
            tmp /= 10;
        } while ( tmp != 0 );
        if ( value < 0 ) {
            -- d;
            *d = '-';
        }
        int len = &buffer[127] - d;
        if ( dest.rdbuf()->sputn( d, len ) != len ) {
            dest.setstate( std::ios_base::badbit );
        }
    }
    return dest;
}

void MMapInitParam(int z, int n, int setnum){
  //LOG(L_INFO) << "Initializing MMap parameters...";
  if(z != setnum){
    return ;
  }
  sp.Z = z;
  sp.N = n;
  sp.setnum = setnum;
  sp.z = (int64_t *) malloc (sizeof(int64_t) * sp.Z); 
  sp.h = (int64_t *) malloc (sizeof(int64_t) * sp.N); 
  sp.g = (int64_t *) malloc (sizeof(int64_t) * sp.N); 
  sp.ginv = (int64_t *) malloc (sizeof(int64_t) * sp.N); 
  sp.p = (int64_t *) malloc (sizeof(int64_t) * sp.N); 
  //sp.setid = (vector<int64_t> *) malloc (sizeof(vector<int64_t>) * sp.setnum); 

  int zLen, hLen, pLen, gLen;
  zLen = 0;
  hLen = 8;
  gLen = 8;
  pLen = 40;
  int64_t zMax = pow(2, zLen); 
  int64_t hMax = pow(2, hLen); 
  int64_t gMax = pow(2, gLen); 
  int64_t pMax = pow(2, pLen); 
  sp.lambda = gLen;

  srand((unsigned)time(NULL));

  for(int i =0; i< sp.Z; i++){
	sp.z[i] = rand() % zMax + 1;
    //sp.setid[i].push_back(i);
  }

  sp.q = 1;
  for(int i =0; i< sp.N; i++){
	sp.g[i] = GetPrime(gMax); 
	sp.p[i] = GetPrime(pMax); 
	sp.ginv[i] = InvMod(sp.g[i], sp.p[i]); 
	sp.h[i] = rand() % hMax + 1; //h[1] cannot be 0; 
	sp.q = sp.q * sp.p[i];
  }

  sp.pzt = 0;
  for(int i=0; i<sp.N; i++){
	int64_t mid = sp.ginv[i];
	int64_t tail = 1;
    for(int j=0; j<sp.Z; j++){
	  mid = (mid * sp.z[j]) % sp.p[i] % sp.q;
	}
    for(int k=0; k<sp.N; k++){
	  if (k!=i){
	    tail = tail * sp.p[k] % sp.q;
	  }
	}
    cout << "pzt = " << sp.pzt << "+" << sp.h[i] << "*" <<mid<<"*"<<tail<<"%"<<sp.q<<endl;
    sp.pzt = (sp.pzt + sp.h[i] * mid * tail) % sp.q; 
  }

  //TODO:To implement more secure set mechanism
  
  printf("Finish MMap parameter initialization...\n");
  cout << "pzt = " << sp.pzt <<endl;
  for(int i =0; i< sp.N; i++){
    cout << "g[" << i << "]=" << sp.g[i] << ", inverse =" << sp.ginv[i] << endl;
  }
  for(int i =0; i< sp.N; i++){
    cout << "p[" << i << "]=" << sp.p[i] <<endl;
  }
  cout<<"q = "<<sp.q<<endl;
  for(int i =0; i< sp.Z; i++){
    cout << "z[" << i << "]=" << sp.z[i] << endl;
  }
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
  __int128 left, right;
  left = sp.pzt * u % sp.q;
  cout << "left = "<< sp.pzt <<" * " << u <<" % "<<sp.q<<endl;
  right = sp.q * pow(2, sp.lambda * -1);
  cout << "left = "<<left<<", right = " << right <<endl;
  if(left < right){
    cout << "Zero test returns true!" <<endl;
	return 1;
  }
  return 0;
}


int64_t MMapEnc(int m, int mid, int setid){
  int64_t set = 1;
  //for(int i=0; i<sp.setid[setid].size(); i++){
  int64_t zinv = InvMod(sp.z[setid], sp.p[mid]); 
  set = set * zinv;
  //}
  int tmpRi = rand() % 3;
  int result = (tmpRi * sp.g[mid] + m) * set % sp.p[mid];
  return result;
}

int64_t MMapEncDefault(int m, int setid){
  return MMapEnc(m, 0, setid);
}
int64_t MMapAdd(int64_t u1, int64_t u2, int mid){
  int64_t result = (u1 + u2) % sp.p[mid];
  return result;
}

int64_t MMapMult(int64_t u1, int64_t u2, int mid){
  int64_t result = (u1 * u2) % sp.p[mid];
  return result;
}
