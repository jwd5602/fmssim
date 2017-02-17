#include "HashKF.h"
using namespace std;

ClassImp(HashKF)

const Int_t HASH_ORDER=9;
const Int_t TABLE_SIZE=pow(2,HASH_ORDER);

HashKF::HashKF(const char * codesfile)
{
  // intialise and zero arrays
  PDGtable = new PDG[TABLE_SIZE];
  nHashProbes = new Int_t[TABLE_SIZE];
  for(Int_t i=0; i<TABLE_SIZE; i++) 
    nHashProbes[i]=-1;
  step=1; // linear probe step size
  

  // read KF codes file and hash 
  FILE * file = fopen(codesfile,"r");
  if(file!=NULL)
  {
    printf("codesfile opened\n");
    char line[512];
    while(fgets(line,sizeof(line),file)!=NULL)
    {
      sscanf(line,"%ld %d %s %s %d %d %d %f %f %f %f %d",
       &(PDGread.kf),&(PDGread.kc),
       (char*) &(PDGread.particle), (char*) &(PDGread.antiparticle),
       &(PDGread.chg),&(PDGread.col),&(PDGread.anti),
       &(PDGread.mass),&(PDGread.width),&(PDGread.wcut),&(PDGread.lifetime),
       &(PDGread.decay));
      
      key = Put(PDGread.kf);

      PDGtable[key].kf = PDGread.kf;
      PDGtable[key].kc = PDGread.kc;
      strcpy(PDGtable[key].particle,PDGread.particle);
      strcpy(PDGtable[key].antiparticle,PDGread.antiparticle);
      PDGtable[key].chg = PDGread.chg;
      PDGtable[key].col = PDGread.col;
      PDGtable[key].anti = PDGread.anti;
      PDGtable[key].mass = PDGread.mass;
      PDGtable[key].width = PDGread.width;
      PDGtable[key].wcut = PDGread.wcut;
      PDGtable[key].lifetime = PDGread.lifetime;
      PDGtable[key].decay = PDGread.decay;
      
      printf("KF=%ld key=%d\n",PDGread.kf,key);
    }
    fclose(file);
  }
  else
  {
    fprintf(stderr,"Error: %s not found\n",codesfile);
  }
  
}


HashKF::~HashKF()
{
  if(PDGtable) delete PDGtable;
};


// knuth multiplicative hash
Int_t HashKF::Hash(Long_t kf)
{
  return (Long_t) (kf*1.618*pow(2,HASH_ORDER)) % (Long_t) pow(2,HASH_ORDER);
}

// hash with linear probe
Int_t HashKF::Put(Long_t kf)
{
  initkey = Hash(kf);
  returnkey = initkey;
  while(nHashProbes[returnkey]>=0) 
  { 
    returnkey = (returnkey+step) % TABLE_SIZE;
    nHashProbes[initkey]++;
  }
  //if(initkey!=returnkey) nHashProbes[initkey]++;
  nHashProbes[returnkey]++;
  return returnkey;
}

Int_t HashKF::Get(Long_t kf)
{
  initkey = Hash(kf);
  returnkey = initkey;
  if(nHashProbes[initkey]==0) return initkey;
  else if(nHashProbes[initkey]>0)
  {
    for(Int_t i=0; i<=nHashProbes[initkey]; i++)
    {
      if(PDGtable[(initkey+i) % (Int_t) pow(2,HASH_ORDER)].kf==kf) return returnkey;
      returnkey = (returnkey+step) % TABLE_SIZE;
    }
    return 0;
  }
  else return 0;
}
  

// PDG accessor
struct HashKF::PDG * HashKF::GetParticle(Long_t kf)
{
  Int_t getkey = Get(kf);
  return &(PDGtable[getkey]);
}


// print particle info
void HashKF::PrintParticleInfo(Long_t kf)
{
  Int_t getkey = Get(kf);
  printf("getkey = %d\n",getkey);
  printf("Particle: %s\n",PDGtable[getkey].particle);
  printf("Antiparticle: %s\n",PDGtable[getkey].antiparticle);
  printf("KF = %ld\n",PDGtable[getkey].kf);
  printf("KC = %d\n",PDGtable[getkey].kc);
  printf("Electric Charge = %d\n",PDGtable[getkey].chg);
  printf("Colour Charge = %d\n",PDGtable[getkey].col);
  printf("Anticolour Charge = %d\n",PDGtable[getkey].anti);
  printf("Mass = %f\n",PDGtable[getkey].mass);
  printf("Width = %f\n",PDGtable[getkey].width);
  printf("Width Cut = %f\n",PDGtable[getkey].wcut);
  printf("Lifetime = %f\n",PDGtable[getkey].lifetime);
  printf("Decay = %d\n",PDGtable[getkey].decay);
}


// print hash table + nprobes
void HashKF::PrintHashTable()
{
  printf("key  kf  nprobes\n");
  for(Int_t i=0; i<TABLE_SIZE; i++)
  {
    printf("%d  %ld  %d\n",i,PDGtable[i].kf,nHashProbes[i]);
  }
}


// check hash table
void HashKF::CheckHashTable()
{
  printf("error checking hash table...\n");
  Int_t correctpercent=0;
  Int_t getkey;
  Long_t getkf;
  for(Int_t i=0; i<TABLE_SIZE; i++)
  {
    printf("checking key %i - ",i);
    getkf = PDGtable[i].kf;
    getkey = Get(getkf);
    if(i!=getkey && getkf) 
      printf("key mismatch: hash(%ld)=%d, but associated with key %d\n",getkf,getkey,i);
    else if(!getkf)
    {
      printf("not used\n");
      correctpercent++;
    }
    else 
    {
      printf("correct\n");
      correctpercent++;
    }
  }
  printf("Hash table is %.1f%% correct\n",(float)correctpercent*100/TABLE_SIZE);
}


