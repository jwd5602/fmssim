//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: FMSlarge.cc,v 1.9 2006/06/29 17:48:27 gunter Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "FMSlarge.hh"
#include "FMSlargeHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include <stdio.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  FMSlarge::FMSlarge(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="FMSlargeColl");
  HCID = -1;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

FMSlarge::~FMSlarge()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FMSlarge::Initialize(G4HCofThisEvent* HCE)
{
  hitsCollection = new FMSlargeHitsCollection (SensitiveDetectorName,collectionName[0]);
  if(HCID<0)
  {
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection);
  };
  HCE->AddHitsCollection(HCID,hitsCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool FMSlarge::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{ 
  TrackInformation * info= (TrackInformation*) (aStep->GetTrack()->GetUserInformation());
  G4bool Optical=(aStep->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition());  
  G4bool keepDep=false;
  G4bool keepPhoto=false;
  G4bool isHit=true;

  if((!Optical) &&(aStep->GetTrack()->GetVolume()->GetName()=="physiCellL"))
  {
    keepDep=true;
  }
  else
  {
    if(Optical && (aStep->GetTrack()->GetVolume()->GetName()=="physiPhotoCL"))
    {
      keepPhoto=true;
    }
    else
    {
      keepPhoto=true;
      isHit=false;
      //return false; // comment out to keep ALL photons
    };
  };

  G4TouchableHandle theTouchable =aStep->GetPreStepPoint()->GetTouchableHandle();
//  G4int copyNo = theTouchable->GetCopyNumber();
  G4int motherCopyNo = theTouchable->GetCopyNumber(2);
//  G4int gmotherCopyNo= theTouchable->GetCopyNumber(3);


  G4StepPoint* 	sPoint=aStep->GetPreStepPoint();
//  G4VPhysicalVolume * pVol= sPoint->GetPhysicalVolume();
//  G4LogicalVolume* mVol= pVol->GetMotherLogical();

  // fix for optical photons only being detected in cell
  // with copy number 0 (original is commented out)
  //G4int ecopy=gmotherCopyNo;
  G4int ecopy=motherCopyNo;
  if(keepDep)ecopy=motherCopyNo;
  // end fix


  const G4DynamicParticle* DParticle=aStep->GetTrack()->GetDynamicParticle();
  G4int PDGcode=DParticle->GetPDGcode(); 
  G4double TrackKEnergy=aStep->GetTrack()->GetKineticEnergy();
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4double t0=aStep->GetPreStepPoint()->GetGlobalTime()*ns;
  G4double z0=aStep->GetPreStepPoint()->GetPosition().z()*cm;
  G4double z1=aStep->GetPostStepPoint()->GetPosition().z()*cm;
  G4double dt1=aStep->GetPostStepPoint()->GetGlobalTime()*ns-t0;
  G4int stepnum=aStep->GetTrack()->GetCurrentStepNumber();
 

//BS, not sure if this does anything at current moment
  aStep->GetTrack()->SetBelowThresholdFlag();

  /*
  G4double TrackTotEnergy=aStep->GetTrack()->GetTotalEnergy();
  G4ThreeVector TrackMom=aStep->GetTrack()->GetMomentum();
  G4double TrackVelocity=aStep->GetTrack()->GetVelocity();
  */


//  const G4VProcess* creator=aStep->GetTrack()->GetCreatorProcess();
  //G4ProcessType proctype=creator->GetProcessType(); 

//BS, next lines count number of optical photons produced

  const G4TrackVector* secondaries=aStep->GetSecondary(); //Gives secondaries produced in this step geant4.8 and beyond

  size_t nSeco = secondaries->size(); 
  G4int nSeco_Cut=0;
  for(size_t i=0;i<nSeco;i++)
  {
    G4Track* trSec=(*secondaries)[i];
    //      G4cout << trSec->GetDefinition()->GetParticleName()<<G4endl;
    if(
        trSec->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()
        && trSec->GetMomentum().cosTheta()>-1.
      )
      { 
      nSeco_Cut++;
      };
  };

  FMSlargeHit* newHit = new FMSlargeHit();
  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());

  newHit->SetCellNb(ecopy);                                           
  newHit->SetIsOptical(Optical);
  newHit->SetIsHit(isHit);
  newHit->SetTrackNo(aStep->GetTrack()->GetTrackID());


  newHit->SetGlobalTime(sPoint->GetGlobalTime());
  newHit->SetLocalTime(sPoint->GetLocalTime());
  newHit->SetKinE(TrackKEnergy);
  newHit->SetTotalTrackLength( aStep->GetTrack()->GetTrackLength() /cm );
  if(keepPhoto)
  {
    newHit->SetTdep(aStep->GetTrack()->GetTotalEnergy());
    newHit->SetEdep(aStep->GetTotalEnergyDeposit());
    newHit->SetKinE(aStep->GetPreStepPoint()->GetKineticEnergy());
    newHit->SetnCerenkov(0); //BS, don't think the nCerenkov is being used for anything but a matrix in Postsim, which is not used
    newHit->SetPos(aStep->GetTrack()->GetVertexPosition());
//    newHit->SetMom(aStep->GetTrack()->GetVertexMomentumDirection());
    newHit->SetMom(aStep->GetTrack()->GetMomentum());
    newHit->SetName(aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
    aStep->GetTrack()->SetGoodForTrackingFlag(false);
    newHit->SetCosTh(aStep->GetTrack()->GetVertexMomentumDirection().cosTheta());

  }
  else
  {
    newHit->SetnCerenkov(nSeco_Cut);
    G4double edep = aStep->GetTotalEnergyDeposit();

    newHit->SetEdep(edep);
    newHit->SetTdep(aStep->GetTrack()->GetTotalEnergy());
    newHit->SetKinE(aStep->GetTrack()->GetKineticEnergy());
    newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
    newHit->SetMom(aStep->GetPostStepPoint()->GetMomentumDirection());
    newHit->SetMom(aStep->GetTrack()->GetMomentum());
    newHit->SetName(aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
    newHit->SetCosTh(aStep->GetTrack()->GetMomentumDirection().cosTheta()); 
  };

  if(info)
  { 
    newHit->SetOriginalID(info->GetOriginalTrackID());

    //newHit->SetPhotonID(info->GetPrimaryPhotonID());

    newHit->SetAncestors(info->particle_ances);
  }
  

  hitsCollection->insert( newHit );

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void FMSlarge::EndOfEvent(G4HCofThisEvent*)
{
  if (verboseLevel>0 || true) { 
    G4int NbHits = hitsCollection->entries();
    G4cout << "\n-------->Hits Collection: in this event they are " << NbHits 
      << " hits in the large FMS cells: " << G4endl;
    //for (G4int i=0;i<NbHits;i++) (*hitsCollection)[i]->Print(); //++
  } 
}

//....oooOO0OOooo........oooOO0OOxsooo........oooOO0OOooo........oooOO0OOooo......

