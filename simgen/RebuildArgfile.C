// rebuilds argfile from a given events.root file
// - see "argfile cut" in src/Analysis.cxx
//
// randomEnergy=true will randomize the energy in range [0,maxRandE]
// instead of using distribution of energy from pythia; this is typically
// used as an ad hoc method to generate high energy events

void RebuildArgfile(const char *evfile="outgen/events.root", 
                      Float_t sqrts=200,
                      Int_t macro_size=10,
                      Bool_t randomEnergy=false,
                      Float_t maxRandE=100.
                     )
{
  TFile *infile = new TFile(evfile,"READ");
  TTree *evtr = (TTree*) infile->Get("events");

  gROOT->LoadMacro("LoadLibs.C");
  if(!LoadLibs()) { exit(); };

  Analysis *as = new Analysis(evtr,sqrts);
  as->BuildArgfile(macro_size,randomEnergy,maxRandE);
};
