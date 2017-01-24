#include "StepTree.hh"

StepTree * steptree;

StepTree::StepTree()
{
  sttr = new TTree();
  sttr->Branch("x",&x,"x/F"); // vertex coordinates
  sttr->Branch("y",&y,"y/F");
  sttr->Branch("z",&z,"z/F");
  sttr->Branch("px",&px,"px/F"); // normalised vertex momentum
  sttr->Branch("py",&py,"py/F");
  sttr->Branch("pz",&pz,"pz/F"); 
  sttr->Branch("nref",&nref,"nref/I"); // number of reflections
}

StepTree::~StepTree()
{}

void StepTree::AddEntry(
  G4float x_, G4float y_, G4float z_,
  G4float px_, G4float py_, G4float pz_, G4int nref_)
{
  x=x_;
  y=y_;
  z=z_;
  px=px_;
  py=py_;
  pz=pz_;
  nref=nref_;
  sttr->Fill();
}

void StepTree::WriteTree(TString fname)
{
  TFile * ffile = new TFile(fname,"RECREATE");
  sttr->Write("sttr");
}
