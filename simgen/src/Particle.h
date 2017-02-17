#ifndef Particle_
#define Particle_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TObject.h"

class Particle : public TObject
{
  public:
    Long_t kf; // pdg kf code
    Int_t kc;  // compressed kf code
    char pname[64]; // particle name
    char aname[64]; // antiparticle name
    Int_t chg; // em charge
    Int_t col; // colour charge
    Int_t anticol; // anticolour charge
    Float_t mass; // mass
    Float_t width;
    Float_t wcut;
    Float_t lifetime;
    Int_t decay;

    Particle();
    Particle(Long_t kf_, Int_t kc_,
      const char * pname_, const char * aname_,
      Int_t chg_, Int_t col_, Int_t anticol_,
      Float_t mass_, Float_t width_, Float_t wcut_,
      Float_t lifetime_, Int_t decay_);
    void SetVals(Long_t kf_, Int_t kc_,
      const char * pname_, const char * aname_,
      Int_t chg_, Int_t col_, Int_t anticol_,
      Float_t mass_, Float_t width_, Float_t wcut_,
      Float_t lifetime_, Int_t decay_);

    // accessors
    Long_t GetKF() { return kf; };
    Int_t GetKC() { return kc; };
    char * GetPname() { return pname; };
    char * GetAname() { return aname; };
    Int_t GetChg() { return chg; };
    Int_t GetCol() { return col; };
    Int_t GetAnticol() { return anticol; };
    Float_t GetMass() { return mass; };
    Float_t GetWidth() { return width; };
    Float_t GetWcut() { return wcut; };
    Float_t GetLifetime() { return lifetime; };
    Int_t GetDecay() { return decay; };

    // modifiers
    void SetKF(Long_t kf_) { kf=kf_; return; };
    void SetKC(Int_t kc_) { kc=kc_; return; };
    void SetPname(const char * pname_) { strcpy(pname,pname_); return; };
    void SetAname(const char * aname_) { strcpy(aname,aname_); return; };
    void SetChg(Int_t chg_) { chg=chg_; return; };
    void SetCol(Int_t col_) { col=col_; return; };
    void SetAnticol(Int_t anticol_) { anticol=anticol_; return; };
    void SetMass(Float_t mass_) { mass=mass_; return; };
    void SetWidth(Float_t width_) { width=width_; return; };
    void SetWcut(Float_t wcut_) { wcut=wcut_; return; };
    void SetLifetime(Float_t lifetime_) { lifetime=lifetime_; return; };
    void SetDecay(Int_t decay_) { decay=decay_; return; };


    ClassDef(Particle,1);
};

#endif
