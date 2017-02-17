// DEPRECATED

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
    Int_t key;
    Int_t * nHashProbes;

    Int_t returnkey;
    Int_t initkey;

    Bool_t useAnti;
    Particle * PDGtable;
    Particle * antiPDGtable;

    HashKF(const char * codesfile);
    ~HashKF();
    Int_t Hash(Long_t kf);
    Int_t Put(Long_t kf);
    Int_t Get(Long_t kf);
    Particle * GetParticle(Long_t kf);
    void PrintParticleInfo(Long_t kf);
    void PrintHashTable();
    void CheckHashTable();

    Long_t KF_;
    Int_t KC_;
    char pname_[64];
    char aname_[64];
    Int_t chg_,col_,anticol_;
    Float_t mass_,width_,wcut_,lifetime_;
    Int_t decay_;



    ClassDef(HashKF,1);
};

#endif
