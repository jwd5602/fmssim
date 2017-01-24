
#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "TH1D.h"
#include "G4ParticleTypes.hh"


class TrackingAction : public G4UserTrackingAction {

  public:
    TrackingAction();
    virtual ~TrackingAction(){};
   
    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

    TH1D *startphotonZ, endphotonZ;


};

#endif
