#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4SubtractionSolid.hh"
#include "CellGeo.hh"

class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VPVParameterisation;
class G4UserLimits;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
    ~DetectorConstruction();

  public:

    G4VPhysicalVolume* Construct();

    const 
    G4VPhysicalVolume* GetFMS() {return physiFMSL;}; //++
    G4double GetFMSFullLength() {return fFMSLengthL;};
    G4double GetTargetFullLength()  {return fFMSLengthL;};
    G4double GetWorldFullLength()   {return fWorldLength;}; 

    //BS. Moved from private to facilitate getting cell position vs eta
    G4VPVParameterisation* CellParamL; // pointer to small cell parameterisation
    G4VPVParameterisation* CellParamS; // pointer to small cell parameterisation

  private:

    G4Box*             solidWorld;    // pointer to the solid envelope 
    G4LogicalVolume*   logicWorld;    // pointer to the logical envelope
    G4VPhysicalVolume* physiWorld;    // pointer to the physical envelope

    G4Box*             solidTarget;   // pointer to the solid Target
    G4LogicalVolume*   logicTarget;   // pointer to the logical Target
    G4VPhysicalVolume* physiTarget;   // pointer to the physical Target

    G4Box*             solidFMSLtmp;  // pointer to the solid Tracker
    G4Box*             solidFMSStmp;
    G4SubtractionSolid*  solidFMSL;
    G4Box*             solidFMSS;  // pointer to the solid Tracker
    G4LogicalVolume*   logicFMSL;  // pointer to the logical Tracker
    G4LogicalVolume*   logicFMSS;  // pointer to the logical Tracker
    G4VPhysicalVolume* physiFMSL;  // pointer to the physical Tracker
    G4VPhysicalVolume* physiFMSS;  // pointer to the physical Tracker

    G4Box*             frontCell;  // pointer to the solid Cell
    G4LogicalVolume*   logicfCell;  // pointer to the logical Cell
    G4VPhysicalVolume* physifCell;  // pointer to the physical Cell

    G4Box*             solidCellL;  // pointer to the solid Cell
    G4Box*             solidCellS;  // pointer to the solid Cell
    G4LogicalVolume*   logicCellL;  // pointer to the logical Cell
    G4LogicalVolume*   logicCellS;  // pointer to the logical Cell
    G4VPhysicalVolume* physiCellL;  // pointer to the physical Cell
    G4VPhysicalVolume* physiCellS;  // pointer to the physical Cell

    G4Tubs*  solidPhotoCL;
    G4Tubs*  solidPhotoCS;
    G4LogicalVolume*   logicPhotoCL;
    G4LogicalVolume*   logicPhotoCS;
    G4VPhysicalVolume* physiPhotoCL;
    G4VPhysicalVolume* physiPhotoCS;

    G4Tubs* solidCookieL;
    G4Tubs* solidCookieS;
    G4LogicalVolume* logicCookieL;
    G4LogicalVolume* logicCookieS;
    G4VPhysicalVolume* physiCookieL;
    G4VPhysicalVolume* physiCookieS;

    G4Tubs* solidWrapperL;
    G4Tubs* solidWrapperS;
    G4LogicalVolume* logicWrapperL;
    G4LogicalVolume* logicWrapperS;
    G4VPhysicalVolume* physiWrapperL;
    G4VPhysicalVolume* physiWrapperS;

    G4Material*         TargetMater;  // pointer to the target  material
    G4Material*         CellMaterL; // pointer to the large cell material
    G4Material*         CellMaterS; // pointer to the small cell material

//    G4VPVParameterisation* CellParamL; // pointer to small cell parameterisation
 //   G4VPVParameterisation* CellParamS; // pointer to small cell parameterisation

    G4UserLimits* stepLimitL;             // pointer to user step limits
    G4UserLimits* stepLimitS;             // pointer to user step limits

    G4double fWorldLength;            // Full length of the world volume
    G4double fTargetLength;           // Full length of Target
    G4double fFMSLengthL;          // Full length of FMS
    G4double fFMSLengthS;          // Full length of FMS
    G4double fFMSWidthL;          // Full width of FMS
    G4double fFMSWidthS;          // Full width of FMS
    G4int    NbOfCellsL;            // Nb of cells in the FMS region
    G4int    NbOfCellsS;            // Nb of cells in the FMS region
    G4double CellWidthL;            // width of the cells
    G4double CellWidthS;            // width of the cells
    G4double CellSpacingL;	       // distance between cells
    G4double CellSpacingS;	       // distance between cells
    G4double CellLengthL;	       // distance between cells
    G4double CellLengthS;	       // distance between cells

    G4Box*             airgapCellL;  // pointer to the solid Cell
    G4Box*             airgapCellS;  // pointer to the solid Cell
    G4Box*             solidAlCellL;  // pointer to the solid Cell
    G4Box*             solidAlCellS;  // pointer to the solid Cell


    G4LogicalVolume*   logicAlCellL;  // pointer to the logical Cell
    G4LogicalVolume*   logicAlCellS;  // pointer to the logical Cell
    G4VPhysicalVolume* physiAlCellL;  // pointer to the physical Cell
    G4VPhysicalVolume* physiAlCellS;  // pointer to the physical Cell

    G4LogicalVolume*   logicAirCellL;  // pointer to the logical Cell
    G4LogicalVolume*   logicAirCellS;  // pointer to the logical Cell
    G4VPhysicalVolume* physiAirCellL;  // pointer to the physical Cell
    G4VPhysicalVolume* physiAirCellS;  // pointer to the physical Cell
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
