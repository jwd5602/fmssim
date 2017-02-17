//iter was added by branden to be able to run mkLargedata.sh, set to negative number to run as usual

#include "TObject.h"
#include "TTree.h"
#include "TClonesArray.h"
void GenerateEvents(Bool_t bb = false, Int_t nevents=1000, Float_t sqrts=200, Int_t iter=-1)
{
  gROOT->LoadMacro("LoadLibs.C");
  if(!LoadLibs()) { exit(); };
  string filenamestr;
char filename[200];
  if(iter < 0)
  { 
    filenamestr="outgen/events.root";
    sprintf(filename,"outgen/events.root");
  }
  else
  {
    filenamestr=Form("outgen/temp%d.root",iter);
    sprintf(filename,"outgen/temp%i.root",iter);
  }

  // event loop
  EventLoop evloop(nevents,sqrts);
  std::cout<<"\nohs nos\n";
  if(bb){evloop.Start();}
  //std::cout<<"\nohs yes\n";
  else{evloop.horse_ramp();}
  std::cout<<"\nohs ma hoes\n";
  TTree *evtr= evloop.GetEvents();
  std::cout<<"\nohs boes\n";
//  EventLoop * evl = new EventLoop(nevents,sqrts);
//  evl->Start();
//  TTree * evtr = evl->GetEvents();
//  TObjArray *testarr=evl->GetEventsArr();
//  evtr->Branch("testarr",testarr);

  // analysis
//  Analysis * anal = new Analysis(evtr,sqrts);
  //anal->AnalysePT();
//  printf("ANALYZING DATA\n");
//  anal->Analyse();
//Note that the next line (buildargfile) was origionally later. However placing it after the evtr->Write command causes only a blank argfile to be created
//  anal->BuildArgfile(500,false,100.); //(number of events, ignore pythia and use random energy, max random energy)
  //anal->ListParents(0,10);
  
  // write output
//  TFile * ff = new TFile(filename,"RECREATE");
  std::cout<<"About to write"<<std::endl;
  TFile outfile(filenamestr.c_str(),"RECREATE");
  evtr->Write("events");
//  anal->GetCanvases()->Write();
//  anal->GetHistograms()->Write();
//
  std::cout<<"About to return"<<std::endl;

  return;
};
