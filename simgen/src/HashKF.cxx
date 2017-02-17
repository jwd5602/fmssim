// DEPRECATED

#include "HashKF.h"
using namespace std;

ClassImp(HashKF)

const Int_t HASH_ORDER=9;
const Int_t TABLE_SIZE=pow(2,HASH_ORDER);

HashKF::HashKF(const char * codesfile)
{
  // intialise and zero arrays
  PDGtable = new Particle[TABLE_SIZE];
  antiPDGtable = new Particle[TABLE_SIZE];
  nHashProbes = new Int_t[TABLE_SIZE];
  for(Int_t i=0; i<TABLE_SIZE; i++) 
    nHashProbes[i]=-1;
  

  // read KF codes file and hash 
  FILE * file = fopen(codesfile,"r");
  if(file!=NULL)
  {
    printf("codesfile opened\n");
    char line[512];
    while(fgets(line,sizeof(line),file)!=NULL)
    {
      sscanf(line,"%ld %d %s %s %d %d %d %f %f %f %f %d",
        &KF_,&KC_,pname_,aname_,&chg_,&col_,&anticol_,&mass_,&width_,&wcut_,&lifetime_,&decay_);

      key = Put(KF_);

      PDGtable[key].SetVals(KF_,KC_,
        pname_,aname_,
        chg_,col_,anticol_,
        mass_,width_,wcut_,
        lifetime_,decay_);
      antiPDGtable[key].SetVals(-KF_,KC_,
        aname_,pname_,
        -chg_,anticol_,col_,
        mass_,width_,wcut_,
        lifetime_,decay_);
      
      printf("KF=%ld key=%d\n",PDGtable[key].GetKF(),key);
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
    returnkey = (returnkey+1) % TABLE_SIZE;
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
      //printf("initkey=%d i=%d max=%d\n",initkey,i,nHashProbes[initkey]);
      if(PDGtable[(initkey+i) % TABLE_SIZE].GetKF()==kf) return returnkey;
      returnkey = (returnkey+1) % TABLE_SIZE;
    }
    return 0;
  }
  else return 0;
}
  

// PDG accessor
Particle * HashKF::GetParticle(Long_t kf)
{
  useAnti=0;
  if(kf<0) useAnti=1;
  Int_t getkey = Get(abs(kf));
  if(useAnti) return &(antiPDGtable[getkey]);
  else return &(PDGtable[getkey]);
}


// print particle info
void HashKF::PrintParticleInfo(Long_t kf)
{
  Particle * returnparticle = GetParticle(kf);
  
  printf("Particle: %s\n",returnparticle->GetPname());
  printf("Antiparticle: %s\n",returnparticle->GetAname());
  printf("KF = %ld\n",returnparticle->GetKF());
  printf("KC = %d\n",returnparticle->GetKC());
  printf("Electric Charge = %d\n",returnparticle->GetChg());
  printf("Colour Charge = %d\n",returnparticle->GetCol());
  printf("Anticolour Charge = %d\n",returnparticle->GetAnticol());
  printf("Mass = %f\n",returnparticle->GetMass());
  printf("Width = %f\n",returnparticle->GetWidth());
  printf("Width Cut = %f\n",returnparticle->GetWcut());
  printf("Lifetime = %f\n",returnparticle->GetLifetime());
  printf("Decay = %d\n",returnparticle->GetDecay());
}


// print hash table + nprobes
void HashKF::PrintHashTable()
{
  printf("key  kf  antikf  nprobes\n");
  for(Int_t i=0; i<TABLE_SIZE; i++)
  {
    if(nHashProbes[i]>=0)
      printf("%d  %ld  %ld  %d\n",i,
        PDGtable[i].GetKF(),antiPDGtable[i].GetKF(),nHashProbes[i]);
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
    getkf = PDGtable[i].GetKF();
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


