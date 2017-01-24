#ifndef StepTree_H
#define StepTree_H 1

#include "globals.hh"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

class StepTree
{
  public:
    StepTree();
    ~StepTree();
    void AddEntry(G4float x_, G4float y_, G4float z_, G4float px_, G4float py_, G4float pz_, G4int nref_);
    void WriteTree(TString fname);

    TTree * sttr;
    Float_t x,y,z;
    Float_t px,py,pz;
    Int_t nref;
};

extern StepTree * steptree;

#endif
