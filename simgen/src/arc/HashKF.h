#ifndef HashKF_
#define HashKF_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <sstream>

#include "TObject.h"

#include "Particle.h"

extern const Int_t TABLE_SIZE;
extern const Int_t HASH_ORDER;


class HashKF : public TObject
{

  public:
    struct PDG
    {
      Long_t kf;
      Int_t kc;
      char particle[64];
      char antiparticle[64];
      Int_t chg;
      Int_t col;
      Int_t anti;
      Float_t mass;
      Float_t width;
      Float_t wcut;
      Float_t lifetime;
      Int_t decay;
    };

    Int_t key;
    Int_t * nHashProbes;

    Int_t returnkey;
    Int_t initkey;
    Int_t step;

    PDG * PDGtable;
    PDG PDGread;

    HashKF(const char * codesfile);
    ~HashKF();
    Int_t Hash(Long_t kf);
    Int_t Put(Long_t kf);
    Int_t Get(Long_t kf);
    HashKF::PDG * GetParticle(Long_t kf);
    void PrintParticleInfo(Long_t kf);
    void PrintHashTable();
    void CheckHashTable();



    ClassDef(HashKF,1);
};

#endif
