#ifndef EventLoop_
#define EventLoop_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

#include "TObject.h"
#include "TFile.h"
#include "TTree.h"
#include "TPythia6.h"
#include "TClonesArray.h"
#include "TMCParticle.h"
#include "TSystem.h"

class EventLoop : public TObject
{
  public:
    EventLoop(Int_t nEvents, Float_t sqrts);
    ~EventLoop();
    void Start();
	void make_macros();
	bool pass_cuts();
	void write_macro_info();
    TTree * GetEvents() { return events; };
    TClonesArray * GetParticles() { return particles; };

double vx_arr[];
double vy_arr[];
double vz_arr[];
double px_arr[];
double py_arr[];
double pz_arr[];
double E_arr[];
double m_arr[];
double pmag_arr[];
int KS_arr[];
int KF_arr[];
    
    TPythia6 * pythia;
    TClonesArray * particles;
    TTree * events;
    Double_t vx,vy,vz; //Location of particle
    Double_t px; // P(I,1) - x momentum (GeV/c)
    Double_t py; // P(I,2) - y momentum (GeV/c)
    Double_t pz; // P(I,3) - z momentum (GeV/c)
    Double_t E; // P(I,4) - energy (GeV)
    Double_t m;
    Double_t eta;
    Double_t pmag; //magnitude of momentum
    Double_t pt;
    Int_t KS; //Status code >10 means particle does not exist at end
    Int_t KF; //Particle ID code.
    char PName[64]; //holds name of particle temporarily
    Int_t evt_num; //dummy variable to iterate in particle name
    Int_t nev; //number of events

    TString macbase; //directory with macros + "macro"
    TString macrofile; //combination of macbase + evt_num + .mac
    TString eventstring;
    Double_t etamingen, etamaxgen, ptmingen, ptmaxgen; //Used for gen
    Double_t etamincuts, etamaxcuts, ptmincuts, ptmaxcuts,Emincuts,Emaxcuts; //Used for cuts


    Int_t KLineFirst; //BS. Using For JPsi this will give the first daughter, which for Jpsi will be eithier e+ or e-, with the other on the next line
    Bool_t JPsiAnalysis, Pi0Analysis, JustTriggeredPi0, JustTriggeredOther;
    ofstream myfile;
    TString outstring;


  public:
    ClassDef(EventLoop,1);
};

#endif

