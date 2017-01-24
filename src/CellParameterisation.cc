#include "CellParameterisation.hh"
#include "globals.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"

//BS
#include "TVector3.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CellParameterisation::CellParameterisation(  
    G4int    NoCells, 
    G4int    NoCellsX,
    G4double startX,          //  X of center of first 
    G4double startY,          //  Y of center of first 
    G4double startZ,          //  Z of center of first 
    G4double spacingX,        //  X spacing of centers
    G4double spacingY,        //  Y spacing of centers
    G4double widthCell,
    G4double lengthCell)
{
  fNoCells =  NoCells; 
  fNoCellsX=NoCellsX;
  fstartX=startX;
  fstartY=startY;
  fstartZ=startZ;
  spacingY=spacingX; 
  fspacingX=spacingX;
  fwidthCell=widthCell;
  flengthCell=lengthCell;

  if( NoCells > 0 ){
    if (fspacingX < widthCell) {
      G4Exception("CellParameterisation construction: Width>Spacing",
        "NotApplicable",FatalException,"Not Applicable");
    }
  }

  cellgeo = new CellGeo();

//  CellEtaL = new TH1D ("CellEtaL", "Cell Eta Position, Large",101,2.5,3.5);
//  CellEtaS = new TH1D ("CellEtaS", "Cell Eta Position, Small",101,3.,4.4);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CellParameterisation::~CellParameterisation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CellParameterisation::ComputeTransformation
(const G4int copyNo, G4VPhysicalVolume* physVol) const
{
  G4double xp,yp,shift,leftNo,rowNo;
  G4int nstb,row,col;
  if(!strcmp(physVol->GetName(),"physiAlCellL"))
  { 
    // bottom
    if(copyNo>=0 && copyNo<=19) { shift=7; leftNo=0; rowNo=0; }
    else if(copyNo>=20 && copyNo<=41) { shift=6; leftNo=20; rowNo=1; }
    else if(copyNo>=42 && copyNo<=65) { shift=5; leftNo=42; rowNo=2; }
    else if(copyNo>=66 && copyNo<=91) { shift=4; leftNo=66; rowNo=3; }
    else if(copyNo>=92 && copyNo<=119) { shift=3; leftNo=92; rowNo=4; }
    else if(copyNo>=120 && copyNo<=149) { shift=2; leftNo=120; rowNo=5; }
    else if(copyNo>=150 && copyNo<=181) { shift=1; leftNo=150; rowNo=6; }
    else if(copyNo>=182 && copyNo<=215) { shift=0; leftNo=182; rowNo=7; }
    else if(copyNo>=216 && copyNo<=249) { shift=0; leftNo=216; rowNo=8; }

    // middle top
    else if(copyNo>=250 && copyNo<=258) { shift=0; leftNo=250; rowNo=9; }
    else if(copyNo>=259 && copyNo<=267) { shift=25; leftNo=259; rowNo=9; }
    else if(copyNo>=268 && copyNo<=276) { shift=0; leftNo=268; rowNo=10; }
    else if(copyNo>=277 && copyNo<=285) { shift=25; leftNo=277; rowNo=10; }
    else if(copyNo>=286 && copyNo<=294) { shift=0; leftNo=286; rowNo=11; }
    else if(copyNo>=295 && copyNo<=303) { shift=25; leftNo=295; rowNo=11; }
    else if(copyNo>=304 && copyNo<=312) { shift=0; leftNo=304; rowNo=12; }
    else if(copyNo>=313 && copyNo<=321) { shift=25; leftNo=313; rowNo=12; }
    else if(copyNo>=322 && copyNo<=330) { shift=0; leftNo=322; rowNo=13; }
    else if(copyNo>=331 && copyNo<=339) { shift=25; leftNo=331; rowNo=13; }
    else if(copyNo>=340 && copyNo<=348) { shift=0; leftNo=340; rowNo=14; }
    else if(copyNo>=349 && copyNo<=357) { shift=25; leftNo=349; rowNo=14; }
    else if(copyNo>=358 && copyNo<=366) { shift=0; leftNo=358; rowNo=15; }
    else if(copyNo>=367 && copyNo<=375) { shift=25; leftNo=367; rowNo=15; }
    else if(copyNo>=376 && copyNo<=384) { shift=0; leftNo=376; rowNo=16; }
    else if(copyNo>=385 && copyNo<=393) { shift=25; leftNo=385; rowNo=16; }

    // middle top
    else if(copyNo>=394 && copyNo<=402) { shift=0; leftNo=394; rowNo=17; }
    else if(copyNo>=403 && copyNo<=411) { shift=25; leftNo=403; rowNo=17; }
    else if(copyNo>=412 && copyNo<=420) { shift=0; leftNo=412; rowNo=18; }
    else if(copyNo>=421 && copyNo<=429) { shift=25; leftNo=421; rowNo=18; }
    else if(copyNo>=430 && copyNo<=438) { shift=0; leftNo=430; rowNo=19; }
    else if(copyNo>=439 && copyNo<=447) { shift=25; leftNo=439; rowNo=19; }
    else if(copyNo>=448 && copyNo<=456) { shift=0; leftNo=448; rowNo=20; }
    else if(copyNo>=457 && copyNo<=465) { shift=25; leftNo=457; rowNo=20; }
    else if(copyNo>=466 && copyNo<=474) { shift=0; leftNo=466; rowNo=21; }
    else if(copyNo>=475 && copyNo<=483) { shift=25; leftNo=475; rowNo=21; }
    else if(copyNo>=484 && copyNo<=492) { shift=0; leftNo=484; rowNo=22; }
    else if(copyNo>=493 && copyNo<=501) { shift=25; leftNo=493; rowNo=22; }
    else if(copyNo>=502 && copyNo<=510) { shift=0; leftNo=502; rowNo=23; }
    else if(copyNo>=511 && copyNo<=519) { shift=25; leftNo=511; rowNo=23; }
    else if(copyNo>=520 && copyNo<=528) { shift=0; leftNo=520; rowNo=24; }
    else if(copyNo>=529 && copyNo<=537) { shift=25; leftNo=529; rowNo=24; }

    // top
    else if(copyNo>=538 && copyNo<=571) { shift=0; leftNo=538; rowNo=25; }
    else if(copyNo>=572 && copyNo<=605) { shift=0; leftNo=572; rowNo=26; }
    else if(copyNo>=606 && copyNo<=637) { shift=1; leftNo=606; rowNo=27; }
    else if(copyNo>=638 && copyNo<=667) { shift=2; leftNo=638; rowNo=28; }
    else if(copyNo>=668 && copyNo<=695) { shift=3; leftNo=668; rowNo=29; }
    else if(copyNo>=696 && copyNo<=721) { shift=4; leftNo=696; rowNo=30; }
    else if(copyNo>=722 && copyNo<=745) { shift=5; leftNo=722; rowNo=31; }
    else if(copyNo>=746 && copyNo<=767) { shift=6; leftNo=746; rowNo=32; }
    else if(copyNo>=768 && copyNo<=787) { shift=7; leftNo=768; rowNo=33; }

    xp = fstartX+(copyNo-leftNo+shift)*fspacingX;
    yp = fstartY+rowNo*fspacingX;
//    printf("large cell %d: (%f,%f)\n",copyNo,xp,yp);

    if((copyNo-leftNo+shift+1)<=17) nstb = 1; // south large
    else nstb = 2; // north large

    row = 33-rowNo;
    if(nstb==1) col = 16-(copyNo-leftNo+shift);
    else col = copyNo-leftNo+shift-17;

    cellgeo->SetCoord(copyNo,nstb,row,col);

    G4ThreeVector origin(xp,yp,0);
    physVol->SetTranslation(origin);
    physVol->SetRotation(0);


    TVector3 myvecbig( fabs(xp / cm) + 5.8/2., fabs(yp / cm) + 5.8/2, 700);
    TVector3 myvecsmall( fabs(xp / cm) - 5.8/2., fabs(yp / cm) - 5.8/2,700);

    /*
    Int_t EtaStartBin= CellEtaL->FindBin(myvecbig.Eta() ); //Note eta increases as distance to z axis decreases for constant z
    Int_t EtaEndBin= CellEtaL->FindBin(myvecsmall.Eta() );

    for(Int_t i=EtaStartBin; i<=EtaEndBin; i++) { CellEtaL->AddBinContent(i); }
    */

//    std::cout<<"Large Eta max =" << myvecsmall.Eta() << " and Eta min ="<< myvecbig.Eta() <<std::endl;

  }
  else if(!strcmp(physVol->GetName(),"physiAlCellS"))
  {
    // bottom
    if(copyNo>=0 && copyNo<=23) { shift=0; leftNo=0; rowNo=0; }
    else if(copyNo>=24 && copyNo<=47) { shift=0; leftNo=24; rowNo=1; }
    else if(copyNo>=48 && copyNo<=71) { shift=0; leftNo=48; rowNo=2; }
    else if(copyNo>=72 && copyNo<=95) { shift=0; leftNo=72; rowNo=3; }
    else if(copyNo>=96 && copyNo<=119) { shift=0; leftNo=96; rowNo=4; }
    else if(copyNo>=120 && copyNo<=143) { shift=0; leftNo=120; rowNo=5; }
    else if(copyNo>=144 && copyNo<=167) { shift=0; leftNo=144; rowNo=6; }

    // middle bottom
    else if(copyNo>=168 && copyNo<=174) { shift=0; leftNo=168; rowNo=7; }
    else if(copyNo>=175 && copyNo<=181) { shift=17; leftNo=175; rowNo=7; }
    else if(copyNo>=182 && copyNo<=188) { shift=0; leftNo=182; rowNo=8; }
    else if(copyNo>=189 && copyNo<=195) { shift=17; leftNo=189; rowNo=8; }
    else if(copyNo>=196 && copyNo<=202) { shift=0; leftNo=196; rowNo=9; }
    else if(copyNo>=203 && copyNo<=209) { shift=17; leftNo=203; rowNo=9; }
    else if(copyNo>=210 && copyNo<=216) { shift=0; leftNo=210; rowNo=10; }
    else if(copyNo>=217 && copyNo<=223) { shift=17; leftNo=217; rowNo=10; }
    else if(copyNo>=224 && copyNo<=230) { shift=0; leftNo=224; rowNo=11; }
    else if(copyNo>=231 && copyNo<=237) { shift=17; leftNo=231; rowNo=11; }
    
    // middle top
    else if(copyNo>=238 && copyNo<=244) { shift=0; leftNo=238; rowNo=12; }
    else if(copyNo>=245 && copyNo<=251) { shift=17; leftNo=245; rowNo=12; }
    else if(copyNo>=252 && copyNo<=258) { shift=0; leftNo=252; rowNo=13; }
    else if(copyNo>=259 && copyNo<=265) { shift=17; leftNo=259; rowNo=13; }
    else if(copyNo>=266 && copyNo<=272) { shift=0; leftNo=266; rowNo=14; }
    else if(copyNo>=273 && copyNo<=279) { shift=17; leftNo=273; rowNo=14; }
    else if(copyNo>=280 && copyNo<=286) { shift=0; leftNo=280; rowNo=15; }
    else if(copyNo>=287 && copyNo<=293) { shift=17; leftNo=287; rowNo=15; }
    else if(copyNo>=294 && copyNo<=300) { shift=0; leftNo=294; rowNo=16; }
    else if(copyNo>=301 && copyNo<=307) { shift=17; leftNo=301; rowNo=16; }

    // top
    else if(copyNo>=308 && copyNo<=331) { shift=0; leftNo=308; rowNo=17; }
    else if(copyNo>=332 && copyNo<=355) { shift=0; leftNo=332; rowNo=18; }
    else if(copyNo>=356 && copyNo<=379) { shift=0; leftNo=356; rowNo=19; }
    else if(copyNo>=380 && copyNo<=403) { shift=0; leftNo=380; rowNo=20; }
    else if(copyNo>=404 && copyNo<=427) { shift=0; leftNo=404; rowNo=21; }
    else if(copyNo>=428 && copyNo<=451) { shift=0; leftNo=428; rowNo=22; }
    else if(copyNo>=452 && copyNo<=475) { shift=0; leftNo=452; rowNo=23; }

    xp = fstartX+(copyNo-leftNo+shift)*fspacingX;
    yp = fstartY+rowNo*fspacingX;
   // printf("small cell %d: (%f,%f)\n",copyNo,xp,yp);
    
    if((copyNo-leftNo+shift+1)<=12) nstb = 3; // south small
    else nstb = 4; // north small

    row = 23-rowNo;
    if(nstb==3) col=11-(copyNo-leftNo+shift);
    else col = copyNo-leftNo+shift-12;

    cellgeo->SetCoord(copyNo,nstb,row,col);

    G4ThreeVector origin(xp,yp,0);
    physVol->SetTranslation(origin);
    physVol->SetRotation(0);

    TVector3 myvecbig( fabs(xp / cm) + 3.81/2., fabs(yp / cm) + 3.81/2, 700);
    TVector3 myvecsmall( fabs(xp / cm) - 3.81/2., fabs(yp / cm) - 3.81/2,700);


    /*
    Int_t EtaStartBin= CellEtaS->FindBin(myvecbig.Eta() ); //Note eta increases as distance to z axis decreases for constant z
    Int_t EtaEndBin= CellEtaS->FindBin(myvecsmall.Eta() );

    for(Int_t i=EtaStartBin; i<=EtaEndBin; i++) { CellEtaS->AddBinContent(i); }
    */

  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CellParameterisation::ComputeDimensions
(G4Box& FMSCell, const G4int copyNo, const G4VPhysicalVolume* physVol) const
{
  /*
  static int calledFirst = 1;
  if(calledFirst)
  {
    printf("<<<>>> CellParamaterisation::ComputeDimensions called\n");
    calledFirst = 0;
  };
  */
  if(copyNo<-1)
  {

  };
  G4double  halfLength= flengthCell/2;
  G4double  halfWidth= fwidthCell/2;
  FMSCell.SetXHalfLength(halfWidth);
  FMSCell.SetYHalfLength(halfWidth);
  FMSCell.SetZHalfLength(halfLength);
  // G4VPhysicalVolume*  physiPhotoC=physVol->GetLogicalVolume()->GetDaughter(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
