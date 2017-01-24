// GEANT4 tag $Name: geant4-09-03-patch-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef StackingAction_H
#define StackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"
#include "TTree.h"
#include <math.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class StackingAction : public G4UserStackingAction
{
  public:
    StackingAction();
   ~StackingAction();

  public:
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    void NewStage();
    void PrepareNewEvent();
    TTree * GetCoordTr() { return coordtr; }

  private:
    G4int gammaCounter;
    G4int otherCounter;
    G4int statCounter;
    G4int stage;
    G4int cnt;
    G4double PcathEff[32];
    G4double Esteps[32];

    TTree * coordtr;
    char name_[32];
    char volume_[32]; //++
    //char material_[32]; //++
    int evnum_; 
    int generation_;
    int track_id_; //++
    float energy_;
    float kinetic_energy_; //++
    float mapx_;
    float mapy_;
    float px_;
    float py_;
    float pz_;
    float posx_;
    float posy_;
    float posz_;
    float eta_;
    float phi_;
    float pt_;
};

extern StackingAction * stackaction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

