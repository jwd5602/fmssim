#ifndef CellGeo_H
#define CellGeo_H 1

#include "globals.hh"

class CellGeo
{
  public:
    CellGeo();
    ~CellGeo();
    void SetCoord(G4int copyNo, G4int nstb, G4int row, G4int col);
    // det = 1 <--> large
    // det = 0 <--> small
    G4int GetNstb(G4int copyNo, G4int det);
    G4int GetRow(G4int copyNo, G4int det);
    G4int GetCol(G4int copyNo, G4int det);
    G4double GetZdist() { return 31.1; }
    //BS. Origional has no arguments
    void SetGains(G4String txtString="", G4String gainString="", G4String corrString="");
    G4float GetGain(G4int nstb, G4int row, G4int col);
    G4float GetCorr(G4int nstb, G4int row, G4int col);
  
  private:
    G4int coordL[1000][3];
    G4int coordS[1000][3];
    G4float gain[1000][4];
    G4float corr[1000][4];
};

extern CellGeo * cellgeo;

#endif
