#ifndef CellParameterisation_H
#define CellParameterisation_H 1

#include "globals.hh"
#include "G4VPVParameterisation.hh"
#include "CellGeo.hh"

//BS
#include "TH1D.h"

class G4VPhysicalVolume;
class G4Box;

// Dummy declarations to get rid of warnings ...
class G4Trd;
class G4Trap;
class G4Cons;
class G4Orb;
class G4Sphere;
class G4Torus;
class G4Para;
class G4Hype;
class G4Tubs;
class G4Polycone;
class G4Polyhedra;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class CellParameterisation : public G4VPVParameterisation
{ 
  public:
  
    CellParameterisation(  G4int    NoCells, 
				G4int    NoXCells, // Number of cells in X
				G4double startX,  //  X of center of first 
				G4double startY,  //  Y of center of first 
				G4double startZ,  //  Z of center of first 
				G4double spacingX,//  X spacing of centers
				G4double spacingY,//  X spacing of centers
				G4double widthCell,
				G4double lengthCell);
  
    virtual				 
   ~CellParameterisation();
   
    void ComputeTransformation (const G4int copyNo,
                                G4VPhysicalVolume* physVol) const;
    
    void ComputeDimensions (G4Box & FMSLayer, const G4int copyNo,
                            const G4VPhysicalVolume* physVol) const;

    //BS
    TH1D *CellEtaL;
    TH1D *CellEtaS;

  private:  // Dummy declarations to get rid of warnings ...

    void ComputeDimensions (G4Trd&,const G4int,const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Trap&,const G4int,const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Cons&,const G4int,const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Sphere&,const G4int,const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Orb&,const G4int,const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Torus&,const G4int,const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Para&,const G4int,const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Hype&,const G4int,const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Tubs&,const G4int,const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Polycone&,const G4int,const G4VPhysicalVolume*) const {}
    void ComputeDimensions (G4Polyhedra&,const G4int,const G4VPhysicalVolume*) const {}
  private:

    G4int    fNoCells;  
    G4int    fNoCellsX;  

    G4double fstartX;          //  X of center of first 
    G4double fstartY;          //  Y of center of first 
    G4double fstartZ;          //  Z of center of first 
    G4double fspacingX;        //  X spacing of centers
    G4double fwidthCell;
    G4double flengthCell;


  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


