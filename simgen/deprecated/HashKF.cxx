#include "HashKF.h"

const int TABLE_SIZE=2048;

HashKF::HashKF(const char * codesfile)
{
  //printf("codesfile=%s\n",codesfile);


  pdgtable = new pdg[TABLE_SIZE];

  for(int i=0; i<TABLE_SIZE; i++) pdgtable[key].kf=0;

  FILE * file = fopen(codesfile,"r");
  if(file!=NULL)
  {
    char line[512];
    while(fgets(line,sizeof(line),file)!=NULL)
    {
      sscanf(line,"%d %d %s %s %d %d %d %f %f %f %f %d",
       &(pdgread.kf),&(pdgread.kc),
       (char*) &(pdgread.particle), (char*) &(pdgread.antiparticle),
       &(pdgread.chg),&(pdgread.col),&(pdgread.anti),
       &(pdgread.mass),&(pdgread.width),&(pdgread.wcut),&(pdgread.lifetime),
       &(pdgread.decay));
      
      key = Hash(pdgread.kf);
      pdgtable[key].kf = pdgread.kf;
      pdgtable[key].kc = pdgread.kc;
      //pdgtable[key].particle = pdgread.particle;
      //pdgtable[key].antiparticle = pdgread.antiparticle;
      strcpy(pdgtable[key].particle,pdgread.particle);
      strcpy(pdgtable[key].antiparticle,pdgread.antiparticle);
      pdgtable[key].chg = pdgread.chg;
      pdgtable[key].col = pdgread.col;
      pdgtable[key].anti = pdgread.anti;
      pdgtable[key].mass = pdgread.mass;
      pdgtable[key].width = pdgread.width;
      pdgtable[key].wcut = pdgread.wcut;
      pdgtable[key].lifetime = pdgread.lifetime;
      pdgtable[key].decay = pdgread.decay;
      
      //printf("kf=%d key=%d\n",pdgread.kf,key);
      printf("%d %d\n",pdgread.kf,key);
    }
    fclose(file);
  }
  else
  {
    fprintf(stderr,"Error: %s not found\n",codesfile);
  }
  
}

HashKF::~HashKF()
{}

int HashKF::Hash(int kf)
{
  int testkey = (int) (kf*1.618*pow(2,11)) % (int) pow(2,11);
  //printf("\nkf=%d\n",kf);
  return CheckKey(testkey);
}

int HashKF::CheckKey(int check)
{
  // recursively check for valid key
  //pdg[checkkey].kf should be zero

  //printf("checking key %d\n",check);
  //printf("stored kf is %d\n",pdgtable[check].kf);

  if(pdgtable[check].kf)
  {
    check++;
    if(check>TABLE_SIZE) check=0;
    return CheckKey(check);
  }
  else 
    return check;
}

int main()
{
  HashKF * hash = new HashKF("codes.dat");
  return 0;
} 
  


