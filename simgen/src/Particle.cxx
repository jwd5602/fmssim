#include "Particle.h"
using namespace std;

ClassImp(Particle)

Particle::Particle()
{
  kf=0;
  kc=0;
  strcpy(pname,"");
  strcpy(aname,"");
  chg=0;
  col=0;
  anticol=0;
  mass=0;
  width=0;
  wcut=0;
  lifetime=0;
  decay=0;
}

Particle::Particle(Long_t kf_, Int_t kc_,
      const char * pname_, const char * aname_,
      Int_t chg_, Int_t col_, Int_t anticol_,
      Float_t mass_, Float_t width_, Float_t wcut_,
      Float_t lifetime_, Int_t decay_)
{
  kf=kf_;
  kc=kc_;
  strcpy(pname,pname_);
  strcpy(aname,aname_);
  chg=chg_;
  col=col_;
  anticol=anticol_;
  mass=mass_;
  width=width_;
  wcut=wcut_;
  lifetime=lifetime_;
  decay=decay_;
}

void Particle::SetVals(Long_t kf_, Int_t kc_,
      const char * pname_, const char * aname_,
      Int_t chg_, Int_t col_, Int_t anticol_,
      Float_t mass_, Float_t width_, Float_t wcut_,
      Float_t lifetime_, Int_t decay_)
{
  kf=kf_;
  kc=kc_;
  strcpy(pname,pname_);
  strcpy(aname,aname_);
  chg=chg_;
  col=col_;
  anticol=anticol_;
  mass=mass_;
  width=width_;
  wcut=wcut_;
  lifetime=lifetime_;
  decay=decay_;
}
