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
// $Id: FMSsmallHit.hh,v 1.8 2006/06/29 17:47:53 gunter Exp $
// GEANT4 tag $Name: geant4-09-03-patch-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef FMSsmallHit_h
#define FMSsmallHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "TrackInformation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class FMSsmallHit : public G4VHit
{
  public:

    FMSsmallHit();
    ~FMSsmallHit();
    FMSsmallHit(const FMSsmallHit&);
    const FMSsmallHit& operator=(const FMSsmallHit&);
    G4int operator==(const FMSsmallHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    void Draw();
    void Print();

  public:

    void SetTrackID  (G4int track)      { trackID = track; };
    void SetCellNb(G4int cell)      { CellNb = cell; };  
    void SetEdep     (G4double de)      { edep = de; };
    void SetTdep(G4double te) { tdep = te; };
    void SetKinE(G4double ke) { kine = ke; };
    void SetnCerenkov     (G4int nC)      { nCerenkov = nC; };
    void SetPos      (G4ThreeVector xyz){ pos = xyz; };
    void SetMom(G4ThreeVector pp) { mom = pp; };
    void SetName(G4String nm) { name = nm; };
    //G4int SetGlobalTime(G4double time) {GlobalTime=time ;};
    void SetGlobalTime(G4double time) {GlobalTime=time ;}; //++
    //G4double SetLocalTime(G4double time) {LocalTime=time ;};
    void SetLocalTime(G4double time) {LocalTime=time ;}; //++

    G4int GetTrackID()    { return trackID; };
    G4int GetCellNb()  { return CellNb; };
    G4double GetEdep()    { return edep; };      
    G4double GetTdep() { return tdep; };
    G4double GetKinE() { return kine; };
    G4ThreeVector GetPos(){ return pos; };
    G4ThreeVector GetMom(){ return mom; };
    G4String GetName(){ return name; };
    G4int GetnCerenkov()  { return nCerenkov; };
    G4bool IsOptical() {return isOptical;};
    G4bool IsHit() {return isHit;};
    void SetIsOptical(G4bool io) {isOptical = io;};
    void SetIsHit(G4bool io) {isHit = io; };
    void SetTrackNo(G4int trN) {trackNo = trN;};
    void SetCosTh(G4double ct) {cosTh = ct;};
    G4int GetTrackNo() {return trackNo; };
    G4double GetCosTh() {return cosTh; };
    G4int GetGlobalTime() {return GlobalTime; };
    G4double GetLocalTime() {return LocalTime; };

	std::vector<int> GetAncestors(){ return ancestors; };
	void SetAncestors(std::vector<int> anc) {ancestors = anc;};
    void SetOriginalID(G4int id) {originalID = id;};
    G4int GetOriginalID() { return originalID;};

    //BS Radiation damage testing
    void SetTotalTrackLength(G4double length) { TotalTrackLength=length;}
    G4double GetTotalTrackLength() { return TotalTrackLength;}

  private:

    G4double TotalTrackLength;

	std::vector<int> ancestors;

    G4int originalID;
    G4int         trackID;
    G4int         CellNb;
    G4double      edep;
    G4double      tdep;
    G4double      kine;
    G4ThreeVector pos;
    G4ThreeVector mom;
    G4String      name;
    G4int         nCerenkov;
    G4bool        isOptical;   
    G4bool        isHit;
    G4int         trackNo;
    G4double      cosTh;
    G4double      GlobalTime;
    G4double      LocalTime;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<FMSsmallHit> FMSsmallHitsCollection;

extern G4Allocator<FMSsmallHit> FMSsmallHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* FMSsmallHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) FMSsmallHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void FMSsmallHit::operator delete(void *aHit)
{
  FMSsmallHitAllocator.FreeSingle((FMSsmallHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
