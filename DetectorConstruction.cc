#include "DetectorConstruction.hh"
#include "CellParameterisation.hh"
#include "FMSlarge.hh"
#include "FMSsmall.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SDManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4NistManager.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4FileUtilities.hh"
#include "G4EmSaturation.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction():
solidWorld(0),
logicWorld(0),
physiWorld(0),
solidTarget(0),
logicTarget(0),
physiTarget(0), 
solidFMSL(0),
solidFMSS(0),
logicFMSL(0),
logicFMSS(0),
physiFMSL(0),
physiFMSS(0),
solidCellL(0),
solidCellS(0),
logicCellL(0),
logicCellS(0),
physiCellL(0), 
physiCellS(0), 
solidPhotoCL(0),
solidPhotoCS(0),
logicPhotoCL(0),
logicPhotoCS(0),
physiPhotoCL(0), 
physiPhotoCS(0), 
solidCookieL(0),
solidCookieS(0),
logicCookieL(0),
logicCookieS(0),
physiCookieL(0),
physiCookieS(0),
solidWrapperL(0),
solidWrapperS(0),
logicWrapperL(0),
logicWrapperS(0),
physiWrapperL(0),
physiWrapperS(0),
TargetMater(0),
CellMaterL(0),
CellMaterS(0),
CellParamL(0),
CellParamS(0),
stepLimitL(0),
stepLimitS(0),
fWorldLength(0.),
fTargetLength(0.),
fFMSLengthL(0.),
fFMSLengthS(0.),
fFMSWidthL(0.),
fFMSWidthS(0.),
NbOfCellsL(0) ,
NbOfCellsS(0) ,
CellWidthL(0.),
CellWidthS(0.),
CellSpacingL(0.),
CellSpacingS(0.),
CellLengthL(0.),
CellLengthS(0.)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete stepLimitL;
  delete stepLimitS;
  delete CellParamL;
  delete CellParamS;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // ---------------------------------------------- MATERIALS DEFINITION
  G4NistManager* man = G4NistManager::Instance();
  G4double a, z;
  G4double density;
  G4int nel;

  G4VSensitiveDetector * FMSlarge_sd;
  G4VSensitiveDetector * FMSsmall_sd;

  // Elements 
  G4Element* N = new G4Element("Nitrogen", "N", z=7., a= 14.01*g/mole);
  G4Element* O = new G4Element("Oxygen", "O", z=8., a= 16.00*g/mole);
  G4Element* K = new G4Element("Potassium", "K", z=19., a= 39.102*g/mole);
  G4Element* Si = new G4Element("Silicon", "Si", z=14., a= 28.088*g/mole);
  G4Element* Na = new G4Element("Sodium", "Na", z=11., a= 22.99*g/mole);
  G4Element* As = new G4Element("Arsenic", "As", z=33., a= 74.922*g/mole);
  G4Element* PbE = new G4Element("Lead", "Pb", z=82., a= 207.19*g/mole);
  G4Material* Al = man->FindOrBuildMaterial("G4_Al");
  G4Material* Pb = new G4Material("Lead", z=82., a= 207.19*g/mole, density= 11.35*g/cm3);

  // Air
  G4Material* Air = new G4Material("Air", density= 1.29*mg/cm3, nel=2);
  Air->AddElement(N, 70*perCent);
  Air->AddElement(O, 30*perCent);

  // Pb Glass - Photocathode
  G4Material* PbGl0 = man->FindOrBuildMaterial("G4_GLASS_LEAD");

  // Pb Glass - Protvino Small Cell
  G4Material* PbgS = new G4Material("PbGlass386", density= 3.86*g/cm3, nel=5);
  G4double fudge=65.4/60.712;
  G4double fudge2=(100.-65.4)/(100.-60.712);
  fudge=1.;
  fudge2=1.;
  PbgS->AddElement(PbE,fudge*60.712*perCent);
  PbgS->AddElement(K,2.324*perCent*fudge2);
  PbgS->AddElement(Si,14.771*perCent*fudge2);
  PbgS->AddElement(O,22.041*perCent*fudge2);
  PbgS->AddElement(As,.152*perCent*fudge2);

  // Pb Glass - Schott Large Cell
  G4Material * PbgL = new G4Material("PbGlass361", density = 3.61*g/cm3, nel=6);
  G4double correction = 0.25/6;
  PbgL->AddElement(PbE,(42.2+correction)*perCent);
  PbgL->AddElement(Si,(21.4+correction)*perCent);
  PbgL->AddElement(O,(29.5+correction)*perCent);
  PbgL->AddElement(K,(4.2+correction)*perCent);
  PbgL->AddElement(Na,(2.3+correction)*perCent);
  PbgL->AddElement(As,(0.15+correction)*perCent);


  
  // Plexiglass
  G4Material *Plexi = man->FindOrBuildMaterial("G4_PLEXIGLASS");
  

  // Material Properties Table - LEAD GLASS
  const G4int nEntries = 32;

  G4double PhotonEnergy[nEntries] =
  { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
    2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
    2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
    2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
    2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
    3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
    3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
    3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };
  G4double RefractiveIndex1[nEntries] =
  {1.600, 1.604, 1.607, 1.610, 1.614, 1.617, 1.620, 
    1.623, 1.626, 1.630, 1.633, 1.636, 1.638, 1.641, 
    1.643, 1.646, 1.648, 1.651, 1.654, 1.656, 1.659, 1.662, 
    1.664, 1.666, 1.667, 1.669, 1.671, 1.673, 1.675, 1.676, 
    1.678, 1.680};
  G4double Absorption1[nEntries] =
  {300.000, 300.000, 300.000, 300.000, 300.000, 
    300.000, 300.000, 300.000, 300.000, 290.175, 
    270.081, 250.346, 238.118, 226.060, 214.059, 
    202.138, 190.121, 170.092, 150.263, 129.186, 
    107.196, 85.092, 50.373, 33.450, 22.057, 
    16.064, 10.030, 8.213, 6.406, 4.613, 2.814, 1.006};

  for(int jj=0;jj<nEntries;jj++)Absorption1[jj]=Absorption1[jj]*cm;//*1000000; //BS, delete large number later
  G4double Absorption0[nEntries];
  for(int jj=0;jj<nEntries;jj++)Absorption0[jj]=.00001*cm;//absorb all

  G4double ScintilFast[nEntries] =
  { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00 };
  G4double ScintilSlow[nEntries] =
  { 0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00,
    7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 4.00,
    3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00,
    4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
    7.00, 6.00, 5.00, 4.00 };

  /* begin Schott F2 glass properties */
  G4double RefractiveIndexF2[nEntries] = 
  { 1.618, 1.619, 1.620, 1.621, 1.622, 1.623, 1.624, 1.625,
    1.626, 1.627, 1.629, 1.630, 1.632, 1.633, 1.635, 1.637,
    1.639, 1.641, 1.644, 1.646, 1.649, 1.652, 1.656, 1.660,
    1.664, 1.669, 1.675, 1.681, 1.688, 1.696, 1.705, 1.714 };
  G4double AbsorptionF2[nEntries] = 
  { 1000, 1000, 1000, 1000,
    1000, 1000, 1000, 1000,
    1000, 1000, 966.226, 877.112,
    795.994, 747.750, 668.305, 620.495,
    579.686, 542.412, 504.795, 460.945,
    407.259, 373.124, 298.049, 185.811,
    113.594, 49.936, 16.095, 4.733,
    1.154, 1, 1, 1 };
  /* end Schott F2 glass properties */
  for(int jj=0;jj<nEntries;jj++)AbsorptionF2[jj]=AbsorptionF2[jj]*cm;//*1000000; //BS, delete large number later


  // protvino pb-glass material tables 
  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();
  myMPT1->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex1, nEntries);
  myMPT1->AddProperty("ABSLENGTH", PhotonEnergy, Absorption1, nEntries);
  myMPT1->AddProperty("FASTCOMPONENT", PhotonEnergy, ScintilFast, nEntries);
  myMPT1->AddProperty("SLOWCOMPONENT", PhotonEnergy, ScintilSlow, nEntries);
  myMPT1->AddConstProperty("SCINTILLATIONYIELD",1./MeV);
  myMPT1->AddConstProperty("RESOLUTIONSCALE",1.0);
  myMPT1->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
  myMPT1->AddConstProperty("SLOWTIMECONSTANT",10.*ns);
  myMPT1->AddConstProperty("YIELDRATIO",0.8);
  PbgS->SetMaterialPropertiesTable(myMPT1);

  // schott pb-glass material tables
  G4MaterialPropertiesTable* myMPT3 = new G4MaterialPropertiesTable();
  myMPT3->AddProperty("RINDEX", PhotonEnergy, RefractiveIndexF2, nEntries);
  myMPT3->AddProperty("ABSLENGTH", PhotonEnergy, AbsorptionF2, nEntries);
  myMPT3->AddProperty("FASTCOMPONENT", PhotonEnergy, ScintilFast, nEntries);
  myMPT3->AddProperty("SLOWCOMPONENT", PhotonEnergy, ScintilSlow, nEntries);
  myMPT3->AddConstProperty("SCINTILLATIONYIELD",1./MeV);
  myMPT3->AddConstProperty("RESOLUTIONSCALE",1.0);
  myMPT3->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
  myMPT3->AddConstProperty("SLOWTIMECONSTANT",10.*ns);
  myMPT3->AddConstProperty("YIELDRATIO",0.8);
  PbgL->SetMaterialPropertiesTable(myMPT3);

  // photocathode material tables
  G4MaterialPropertiesTable* myMPT0 = new G4MaterialPropertiesTable();
  myMPT0->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex1, nEntries);
  myMPT0->AddProperty("ABSLENGTH", PhotonEnergy, Absorption0, nEntries);
  myMPT0->AddProperty("FASTCOMPONENT", PhotonEnergy, ScintilFast, nEntries);
  myMPT0->AddProperty("SLOWCOMPONENT", PhotonEnergy, ScintilSlow, nEntries);
  myMPT0->AddConstProperty("SCINTILLATIONYIELD",100./MeV);
  myMPT0->AddConstProperty("RESOLUTIONSCALE",1.0);
  myMPT0->AddConstProperty("FASTTIMECONSTANT", 1.*ns);
  myMPT0->AddConstProperty("SLOWTIMECONSTANT",10.*ns);
  myMPT0->AddConstProperty("YIELDRATIO",0.8);
  PbGl0->SetMaterialPropertiesTable(myMPT0);


  // Material Properties Table - AIR
  G4double RefractiveIndex2[nEntries] =
  { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00 };
  G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex2, nEntries);
  Air->SetMaterialPropertiesTable(myMPT2);
  G4double RefractiveIndex8[nEntries] =
  { 1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50, 1.50, 1.50, 1.50,
    1.50, 1.50, 1.50, 1.50 };
  G4MaterialPropertiesTable* myMPT8 = new G4MaterialPropertiesTable();
  myMPT8->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex8, nEntries);
  //myMPT8->AddConstProperty("RINDEX",1.0);
  Plexi->SetMaterialPropertiesTable(myMPT8);
  G4double CookieLength=0.0*mm;
  
  // Dimensions
  fWorldLength= 1500.*cm; // world length
  G4double HalfWorldLength = 0.5*fWorldLength; // half world length


  G4double PhotoCRadiusL,PhotoCRadiusS;
  CellWidthL = 5.80*cm;     // large cell width
  CellSpacingL = 5.81*cm;   // large cell spacing
  CellLengthL = 60.*cm;    // large cell length
  PhotoCRadiusL = 1.675*cm; // photocathode radius
  CellWidthS = 3.81*cm;   // small cell width
  CellSpacingS = 3.82*cm; // small cell spacing
  CellLengthS = 45.*cm;   // small cell length
  PhotoCRadiusS = 1.4*cm; // photocathode radius
  
  fFMSLengthL = CellLengthL+1.*cm;     // FMS length
  fFMSLengthS = CellLengthS+1.*cm;     // FMS length
  fFMSWidthL = CellSpacingL*34;
  fFMSWidthS = CellSpacingS*24;
  //fFMSWidth=1.1*CellSpacingS*25+1.1*CellSpacingL*35; // FMS width
  //G4double FMSSize = 0.5*fFMSLength;   // half FMS length


  G4double PhotoCLength=.05*cm; // photocathode length

  G4double Alwidth=.002*cm;
  G4double Airgapwidth=.002*cm;
  G4double AlAirgap=Alwidth+Airgapwidth; // air gaps

  TargetMater  = Pb;
  CellMaterL = PbgL;
  CellMaterS = PbgS;
  
  fTargetLength  = .001 * cm;   // target length (deprecated)
  //G4double targetSize  = 0.5*fTargetLength;  // half target length (deprecated)


  // World
  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(fWorldLength);

  solidWorld= new G4Box("solidWorld",
    300.*cm,
    300.*cm,
    HalfWorldLength);
  logicWorld= new G4LogicalVolume(solidWorld, Air, "logicWorld");
  physiWorld = new G4PVPlacement(0,
    G4ThreeVector(),
    logicWorld,
    "physiWorld",
    0,
    false,
    0);


  // FMS
  G4ThreeVector positionFMSL = G4ThreeVector(0,0,0);
  G4ThreeVector positionFMSS = G4ThreeVector(0,0,-7.5*cm);
  solidFMSLtmp = new G4Box("solidFMSLtmp",
    fFMSWidthL/2.,
    fFMSWidthL/2.,
    fFMSLengthL/2.+CookieLength+2*PhotoCLength);
  solidFMSStmp = new G4Box("solidFMSStmp",
    fFMSWidthS/2.,
    fFMSWidthS/2.,
    fFMSLengthL/2.+CookieLength+2*PhotoCLength);

  solidFMSS = new G4Box("solidFMSS",
    fFMSWidthS/2.,
    fFMSWidthS/2.,
    fFMSLengthS/2.+CookieLength+2*PhotoCLength);
  logicFMSS = new G4LogicalVolume(solidFMSS, Air, "logicFMSS");  
  physiFMSS = new G4PVPlacement(0,
    positionFMSS,
    logicFMSS,
    "physiFMSS",
    logicWorld,
    false,
    0);

  solidFMSL = new G4SubtractionSolid("solidFMSL",solidFMSLtmp,solidFMSStmp);
  logicFMSL = new G4LogicalVolume(solidFMSL, Air, "logicFMSL");  
  physiFMSL = new G4PVPlacement(0,
    positionFMSL,
    logicFMSL,
    "physiFMSL",
    logicWorld,
    false,
    0);

  // Logcal AlCell Volume (parameterised physi volume)
  G4ThreeVector positionAlCellL = G4ThreeVector(0,0,0);
  G4ThreeVector positionCellL = G4ThreeVector(0,0,-PhotoCLength-CookieLength/2);
  solidAlCellL = new G4Box("solidCellAlL",
    CellWidthL/2,
    CellWidthL/2,
    CellLengthL/2+PhotoCLength+CookieLength/2);
  logicAlCellL = new G4LogicalVolume(solidAlCellL,Al,"logicAlCellL");
  G4ThreeVector positionAlCellS = G4ThreeVector(0,0,0);
  G4ThreeVector positionCellS = G4ThreeVector(0,0,-PhotoCLength-CookieLength/2);
  solidAlCellS = new G4Box("solidCellAlS",
    CellWidthS/2,
    CellWidthS/2,
    CellLengthS/2+PhotoCLength+CookieLength/2);
  logicAlCellS = new G4LogicalVolume(solidAlCellS,Al,"logicAlCellS");

  // Air Gap
  airgapCellL = new G4Box("solidAirCellL",
    CellWidthL/2-Alwidth,
    CellWidthL/2-Alwidth,
    CellLengthL/2-Alwidth+PhotoCLength+CookieLength/2);
  logicAirCellL = new G4LogicalVolume(airgapCellL,CellMaterL,"logicAirCellL");
  physiAirCellL= new G4PVPlacement(0,
      positionAlCellL,
      logicAirCellL,
      "physiAirCellL",
      logicAlCellL,
      false,
      0);
  airgapCellS = new G4Box("solidAirCellS",
    CellWidthS/2-Alwidth,
    CellWidthS/2-Alwidth,
    CellLengthS/2-Alwidth+PhotoCLength+CookieLength/2);
  logicAirCellS = new G4LogicalVolume(airgapCellS,CellMaterS,"logicAirCellS");
  physiAirCellS= new G4PVPlacement(0,
      positionAlCellS,
      logicAirCellS,
      "physiAirCellS",
      logicAlCellS,
      false,
      0);

  // Solid Cell
  solidCellL = new G4Box("solidCellL",
    CellWidthL/2-AlAirgap,
    CellWidthL/2-AlAirgap,
    CellLengthL/2-AlAirgap);
  logicCellL = new G4LogicalVolume(solidCellL,CellMaterL,"logicCellL");
  physiCellL= new G4PVPlacement(0,
      positionCellL,
      logicCellL,
      "physiCellL",
      logicAirCellL,
      false,
      0);
  solidCellS = new G4Box("solidCellS",
    CellWidthS/2-AlAirgap,
    CellWidthS/2-AlAirgap,
    CellLengthS/2-AlAirgap);
  logicCellS = new G4LogicalVolume(solidCellS,CellMaterS,"logicCellS");
  physiCellS= new G4PVPlacement(0,
      positionCellS,
      logicCellS,
      "physiCellS",
      logicAirCellS,
      false,
      0);

  //AlAirgap = 0; //+++++ (0 for airgap, comment out for no airgap)

  // Photocathode
  G4ThreeVector positionPhotoCL = G4ThreeVector(0,0,CellLengthL/2+CookieLength/2-AlAirgap);
  solidPhotoCL = new G4Tubs("solidPhotoCL",0.,PhotoCRadiusL,PhotoCLength,0.,2*3.1415926);
  logicPhotoCL = new G4LogicalVolume(solidPhotoCL,PbGl0,"logicPhotoCL",0,0,0);
  physiPhotoCL= new G4PVPlacement(0,
      positionPhotoCL,
      logicPhotoCL,
      "physiPhotoCL",
      logicAirCellL,
      false,
      0);
  G4ThreeVector positionPhotoCS = G4ThreeVector(0,0,CellLengthS/2+CookieLength/2-AlAirgap);
  solidPhotoCS = new G4Tubs("solidPhotoCS",0.,PhotoCRadiusS,PhotoCLength,0.,2*3.1415926);
  logicPhotoCS = new G4LogicalVolume(solidPhotoCS,PbGl0,"logicPhotoCS",0,0,0);
  physiPhotoCS= new G4PVPlacement(0,
      positionPhotoCS,
      logicPhotoCS,
      "physiPhotoCS",
      logicAirCellS,
      false,
      0);



  // Cell parameterisation
  G4double startXL=-16.5*CellSpacingL;  //  X of center of first 
  G4double startYL=-16.5*CellSpacingL;  //  Y of center of first 
  G4double startZL=0;  //  Z of center of first 
  G4double spacingXL=CellSpacingL;//  X spacing of centers
  G4double spacingYL=CellSpacingL;//  Y spacing of centers
  G4double widthCellL=CellWidthL;
  G4double lengthCellL=CellLengthL+2*PhotoCLength+CookieLength; // twice width of solidAlCell
  NbOfCellsL = 788;     // number of large cells (16x16 hole)
  G4double startXS=-11.5*CellSpacingS;  //  X of center of first 
  G4double startYS=-11.5*CellSpacingS;  //  Y of center of first 
  G4double startZS=0;  //  Z of center of first 
  G4double spacingXS=CellSpacingS;//  X spacing of centers
  G4double spacingYS=CellSpacingS;//  Y spacing of centers
  G4double widthCellS=CellWidthS;
  G4double lengthCellS=CellLengthS+2*PhotoCLength+CookieLength; // twice width of solidAlCell
  NbOfCellsS = 476;      // number of small cells (10x10 hole)

  CellParamL = new CellParameterisation( NbOfCellsL,
      NbOfCellsL, // Number of cells in X
      startXL,  //  X of center of first 
      startYL,  //  Y of center of first 
      startZL,  //  Z of center of first 
      spacingXL, //  X spacing of centers
      spacingYL, //  Y spacing of centers
      widthCellL,
      lengthCellL);
  CellParamS = new CellParameterisation( NbOfCellsS,
      NbOfCellsS, // Number of cells in X
      startXS,  //  X of center of first 
      startYS,  //  Y of center of first 
      startZS,  //  Z of center of first 
      spacingXS, //  X spacing of centers
      spacingYS, //  Y spacing of centers
      widthCellS,
      lengthCellS);

  // use test*2.mac for large->small
  // use test*3.mac for small->large (and change NbOfCellS)
  // normally just change name & logical vol
  physiAlCellL = new G4PVParameterised(
      "physiAlCellL",    // their name
      logicAlCellL, // their logical volume
      logicFMSL,    // Mother logical volume
      kXAxis,      // Are placed along this axis 
      NbOfCellsL,   // Number of chambers
      CellParamL);  // The parametrisation
  physiAlCellS = new G4PVParameterised(
      "physiAlCellS",    // their name
      logicAlCellS, // their logical volume
      logicFMSS,    // Mother logical volume
      kXAxis,      // Are placed along this axis 
      NbOfCellsS,   // Number of chambers
      CellParamS);  // The parametrisation
//  G4cout << "AlCell built" << G4endl;

G4cout<<"CALLING cellgeo->SetGains() to read correction and gain file"<<G4endl;
  cellgeo->SetGains();

  G4double unity[nEntries] =
  { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00 }; 
  // Mylar Surface
  G4int NUM=nEntries;
  G4double* ppp=PhotonEnergy;
  G4OpticalSurface* OpSurfaceL = new G4OpticalSurface("mcoat");
  G4OpticalSurface* OpSurfaceS = new G4OpticalSurface("mcoat");
  G4LogicalBorderSurface* SurfaceL = new G4LogicalBorderSurface("mylarL",physiAirCellL,physiAlCellL,OpSurfaceL);
  G4LogicalBorderSurface* SurfaceS = new G4LogicalBorderSurface("mylarS",physiAirCellS,physiAlCellS,OpSurfaceS);
  OpSurfaceL -> SetType(dielectric_metal);
  OpSurfaceL -> SetFinish(ground);
  OpSurfaceL -> SetModel(glisur);
  OpSurfaceS -> SetType(dielectric_metal);
  OpSurfaceS -> SetFinish(ground);
  OpSurfaceS -> SetModel(glisur);
  G4double polish = 0.8;
  G4double reflectivity[nEntries]=
  {0.909,0.908,0.906,0.905,0.903,0.901,0.900,
    0.898,0.896,0.894,0.893,0.891,0.889,0.887,
    0.886,0.884,0.882,0.880,0.877,0.875,0.872,
    0.870,0.868,0.866,0.863,0.861,0.859,0.857,
    0.855,0.852,0.850,0.848};
  G4double* efficiency=unity;
  /*
    for(int j=0;j<nEntries;j++)reflectivity[j]=.999;
    G4OpticalSurface* OpSurfaceC = new G4OpticalSurface("connect");
    G4LogicalBorderSurface* SurfasbceC = new G4LogicalBorderSurface("phC",physiCell,physiPhotoC,OpSurfaceC);
    OpSurfaceC -> SetType(dielectric_dielectric);
    OpSurfaceC -> SetFinish(ground);
    OpSurfaceC -> SetModel(glisur);
  */
  G4MaterialPropertiesTable *OpSurfacePropertyL = new G4MaterialPropertiesTable();
  OpSurfacePropertyL -> AddProperty("REFLECTIVITY",ppp,reflectivity,NUM);
  OpSurfacePropertyL -> AddProperty("EFFICIENCY",ppp,efficiency,NUM);
  OpSurfaceL -> SetMaterialPropertiesTable(OpSurfacePropertyL);
  G4MaterialPropertiesTable *OpSurfacePropertyS = new G4MaterialPropertiesTable();
  OpSurfacePropertyS -> AddProperty("REFLECTIVITY",ppp,reflectivity,NUM);
  OpSurfacePropertyS -> AddProperty("EFFICIENCY",ppp,efficiency,NUM);
  OpSurfaceS -> SetMaterialPropertiesTable(OpSurfacePropertyS);



  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String SDname;

  FMSlarge_sd = new FMSlarge(SDname="FMSlarge");
  SDman->AddNewDetector(FMSlarge_sd);
  logicCellL->SetSensitiveDetector(FMSlarge_sd);
  logicPhotoCL->SetSensitiveDetector(FMSlarge_sd);

  FMSsmall_sd = new FMSsmall(SDname="FMSsmall");
  SDman->AddNewDetector(FMSsmall_sd);
  logicCellS->SetSensitiveDetector(FMSsmall_sd);
  logicPhotoCS->SetSensitiveDetector(FMSsmall_sd);


//////--------- Visualization attributes -------------------------------

  G4VisAttributes* BoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));

  logicWorld->SetVisAttributes(BoxVisAtt);  //BS. was a space
  logicFMSL->SetVisAttributes(BoxVisAtt);
  logicFMSS->SetVisAttributes(BoxVisAtt);
  G4VisAttributes* CellVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  logicCellL->SetVisAttributes(CellVisAtt);
  logicCellS->SetVisAttributes(CellVisAtt);

////// Sets a max Step length in the tracker region, with G4StepLimiter
  //

  G4double maxStepL = 0.5*fFMSWidthL;
  G4double maxStepS = 0.5*fFMSWidthS;
  stepLimitL = new G4UserLimits(maxStepL);
  stepLimitS = new G4UserLimits(maxStepS);

  logicFMSL->SetUserLimits(stepLimitL);
  logicFMSS->SetUserLimits(stepLimitS);

  return physiWorld;
}

