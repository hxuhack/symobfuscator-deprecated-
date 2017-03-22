#include "MMap.h"
#include "MathUtils.h"

using namespace std;

secparam sp;
void MMapInitParam(int z, int n, int setnum){
  //LOG(L_INFO) << "Initializing MMap parameters...";
  if(z != setnum){
    return ;
  }
  sp.Z = z;
  sp.N = n;
  sp.setnum = setnum;
  sp.z = (int *) malloc (sizeof(int) * sp.Z); 
  sp.h = (int *) malloc (sizeof(int) * sp.N); 
  sp.g = (int *) malloc (sizeof(int) * sp.N); 
  sp.ginv = (__int128 *) malloc (sizeof(__int128) * sp.N); 
  sp.p = (__int128 *) malloc (sizeof(__int128) * sp.N); 
  //sp.setid = (vector<__int128> *) malloc (sizeof(vector<__int128>) * sp.setnum); 

  int zLen, hLen, pLen, gLen;
  zLen = 2;
  hLen = 8;
  gLen = 8;
  pLen = 96;
  __int128 zMax = pow(2, zLen); 
  __int128 hMax = pow(2, hLen); 
  __int128 gMax = pow(2, gLen); 
  __int128 pMax = pow(2, pLen); 
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
  for(__int128 i=0; i<sp.N; i++){
	__int128 mid = sp.ginv[i];
	__int128 tail = 1;
    for(int j=0; j<sp.Z; j++){
	  mid = (mid * sp.z[j]) % sp.p[i] % sp.q;
	}
    for(int k=0; k<sp.N; k++){
	  if (k!=i){
	    tail = tail * sp.p[k] % sp.q;
	  }
	}
    sp.pzt = (sp.pzt + sp.h[i] * mid * tail) % sp.q; 
  }

  //TODO:To implement more secure set mechanism
  /*
  printf("Finish MMap parameter initialization...\n");
  cout<<"pzt = " << sp.pzt<< endl;
  for(__int128 i =0; i< sp.N; i++){
    cout << "g[" <<i<<"]="<< sp.g[i]<<endl;
  }
  for(__int128 i =0; i< sp.N; i++){
    cout << "p[" <<i<<"]="<< sp.p[i]<<endl;
  }
  cout<<"q = "<<sp.q<<endl;
  for(__int128 i =0; i< sp.Z; i++){
    cout << "z[" <<i<<"]="<< sp.z[i]<<endl;
  }
  */
}

__int128 GetPzt(){
  return sp.pzt;
}

__int128 GetQ(){
  return sp.q;
}

__int128 GetExp(){
  return sp.lambda * -1;
}

__int128 MMapIsZero(__int128 u){
  __int128 left, right;
  left = sp.pzt * u % sp.q;
  right = sp.q * pow(2, sp.lambda * -1);
  //cout << "left = "<<left<<", right = " << right <<endl;
  if(left < right){
    cout << "Zero test returns true!" <<endl;
	return 1;
  }
  return 0;
}


__int128 MMapEnc(int m, int mid, int setid){
  __int128 set = 1;
  //for(int i=0; i<sp.setid[setid].size(); i++){
  __int128 zinv = InvMod(sp.z[setid], sp.p[mid]); 
  set = set * zinv;
  //}
  int tmpRi = rand() % 3 + 1;
  int result = (tmpRi * sp.g[mid] + m) * set % sp.p[mid];
  return result;
}

__int128 MMapEncDefault(int m, int setid){
  return MMapEnc(m, 0, setid);
}
__int128 MMapAdd(__int128 u1, __int128 u2, int mid){
  __int128 result = (u1 + u2) % sp.p[mid];
  return result;
}

__int128 MMapMult(__int128 u1, __int128 u2, int mid){
  __int128 result = (u1 * u2) % sp.p[mid];
  return result;
}
