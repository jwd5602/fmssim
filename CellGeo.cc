#include "CellGeo.hh"
#include "G4FileUtilities.hh"

CellGeo * cellgeo;

CellGeo::CellGeo()
{
  for(int i=0; i<1000; i++)
    for(int j=0; j<3; j++)
    {
      coordL[i][j] = 0;
      coordS[i][j] = 0;
    }
}

CellGeo::~CellGeo()
{}

void CellGeo::SetCoord
(G4int copyNo, G4int nstb, G4int row, G4int col)
{
  if(nstb<3)
  {
    coordL[copyNo][0] = nstb;
    coordL[copyNo][1] = row;
    coordL[copyNo][2] = col;
  }
  else
  {
    coordS[copyNo][0] = nstb;
    coordS[copyNo][1] = row;
    coordS[copyNo][2] = col;
  }
}

// det = 1 <--> large
// det = 0 <--> small


G4int CellGeo::GetNstb(G4int copyNo, G4int det)
{
  if(det) return coordL[copyNo][0];
  else return coordS[copyNo][0];
}

G4int CellGeo::GetRow(G4int copyNo, G4int det)
{
  if(det) return coordL[copyNo][1];
  else return coordS[copyNo][1];
}

G4int CellGeo::GetCol(G4int copyNo, G4int det)
{
  if(det) return coordL[copyNo][2];
  else return coordS[copyNo][2];
}

void CellGeo::SetGains(G4String txtString, G4String gainString, G4String corrString)
{
  G4FileUtilities * fu = new G4FileUtilities();
  G4String fmstxt = fu->GetEnv("FMSTXT");
  G4String fmsgain = fu->GetEnv("FMSGAIN");
  G4String fmscorr = fu->GetEnv("FMSCORR");

  if(txtString!="") { fmstxt = txtString; std::cout<<"USING USER TXT"<<std::endl;}
  if(gainString!="") fmsgain = gainString;
  if(corrString!="") fmscorr = corrString;

  G4String fmsgainfile = fmstxt+"/"+fmsgain;
  G4String fmscorrfile = fmstxt+"/"+fmscorr;

  char str[100];
  G4int dev,nn,tt;
  G4float gg;

  FILE * gainfile;
  gainfile = fopen(fmsgainfile,"r");
  if(gainfile == NULL)
  {
    fputs("Error: $FMSGAIN not found!",stderr);
    exit(1);
  }
  while(fgets(str,sizeof(str),gainfile))
  {
    sscanf(str,"%d %d %d %f",&dev,&nn,&tt,&gg);
    if(dev==2) gain[tt-1][nn-1] = gg;
  }
    
  FILE * corrfile;
  corrfile = fopen(fmscorrfile,"r");
  if(corrfile == NULL)
  {
    fputs("Error: $FMSCORR not found!",stderr);
    exit(1);
  }
  while(fgets(str,sizeof(str),corrfile))
  {
    sscanf(str,"%d %d %d %f",&dev,&nn,&tt,&gg);
    if(dev==2) corr[tt-1][nn-1] = gg;
  }
}

G4float CellGeo::GetGain(G4int nstb, G4int row, G4int col)
{
  G4int fac;
  if(nstb<3) fac=17;
  else fac=12;
  return gain[(col+row*fac+1)-1][nstb-1];
}

G4float CellGeo::GetCorr(G4int nstb, G4int row, G4int col)
{
  G4int fac;
  if(nstb<3) fac=17;
  else fac=12;
  return corr[(col+row*fac+1)-1][nstb-1];
}
