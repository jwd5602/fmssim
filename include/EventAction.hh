#ifndef EventAction_h
#define EventAction_h 1
#include "TString.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "G4UserEventAction.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "FMSlargeHit.hh"
#include "FMSsmallHit.hh"
#include "CellGeo.hh"
#include "StackingAction.hh"
#include "PostSim.hh"
#include "Qt.h"
#include "Trajectory.hh"
#include <stdio.h>
#include "/home/demaio/simulations/geant/fmsu/myRootClass/G4QT.h"
#include <vector>
#include <math.h>

#include "TMath.h"

class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
  public:
  EventAction(FILE* fp0,TFile* p0file,TTree* geotr, Int_t eventNum);
  ~EventAction();

  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
    FILE* fp;

    TFile* pFile;
    TTree* pgeotr;
    int evnum_;
    int rowg;
    int colg;
    int nstbg;
    float gaing;
    float corrg;

    TH1D *PhotonWavelengthS;
    TH1D *PhotonWavelengthL;
    TH1D *OrigPhotonWavelengthS;
    TH1D *OrigPhotonWavelengthL;
    TH2D *EnergyDepositShapeS;
    TH2D *EnergyDepositShapeL;

    PostSim pss;

    TTree *testtree;
    Int_t eventNumber;
    

    //BS
    Int_t nsaved_traj;
    UInt_t nhits;
    Int_t parentID[40];
    Int_t trackID[40];
    Float_t mass[40];
    Float_t energy[40];
    Float_t vx[40];
    Float_t vy[40];
    Float_t vz[40];
    Float_t px[40];
    Float_t py[40];
    Float_t pz[40];
    Float_t pt[40];
    Float_t eta[40];
    Int_t nstb[40];
    Int_t row[40];
    Int_t col[40];

    //BS Radiation Diagnostics
    Int_t nPhotonsNormL, nPhotonsRadL,nPhotonsNormS, nPhotonsRadS ; //number of detected photons before and after radaition damage
    Double_t zPhotonsNormL, zPhotonsNormS, zPhotonsRadL, zPhotonsRadS; //depth from photocathode of photon, holds total z to later be divided by number of photons
    Double_t zAvgNormL, zAvgNormS, zAvgRadL, zAvgRadS;
    Double_t lPhotonsNormL, lPhotonsNormS, lPhotonsRadL, lPhotonsRadS; //Total Path Length traveled by Photons, holds total l to later be divided by number of photons
    Double_t lAvgNormL, lAvgNormS, lAvgRadL, lAvgRadS;
    Double_t EdeptotL, EdeptotS; //total deposited energy via energy deposition



    Trajectory *trj;
    G4QT test;
    std::vector<G4QT> myvectest;
    std::vector<std::string> name;
    UInt_t nPrimaries;

//    UInt_t Qtenergy[3700];
//    UInt_t nqtEnergy;
//    UInt_t *pnqtEnergy;
    G4QT energyG4QT;
    std::vector<G4QT> energyQTvec;

    G4QT EnergyModG4QT;
    std::vector<G4QT> EnergyModVec;
//    UInt_t QtEnergyMod[3700];
//    UInt_t nqtEnergyMod;
//    UInt_t *pnqtEnergyMod;

    G4QT RadDamageG4QT;
    std::vector<G4QT> RadDamageVec;


    //BS
    G4QT CellGeo1G4QT;
    std::vector<G4QT> CellGeo1Vec;
    //End BS

  private:
	int max;
    G4int collectionIDL;
    G4int collectionIDS;
    G4int verboseLevel;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
