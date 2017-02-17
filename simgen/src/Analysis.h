#ifndef Analysis_
#define Analysis_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <sstream>


#include "TObject.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TString.h"
#include "TObjArray.h"
#include "TMCParticle.h"

//#include "HashKF.h"

extern const Float_t PI;

class Analysis : public TObject
{
  public:
    Analysis(TTree * events, Float_t sqrts_);
    //void AnalysePT();
    void Analyse();
    TObjArray * GetHistograms();
    TObjArray * GetCanvases();
    void ListParents(Int_t lb, Int_t ub);
    Bool_t FinalStateFilter(Int_t KF0, Int_t KS0, Int_t parent0, TLeaf * KF_le0);
    void BuildArgfile(Int_t macro_size,Bool_t randomEnergy,Float_t maxRandE);

    TString SIMGENDIR;

    TTree * evtr;
    Float_t sqrts;
    TMCParticle * parttmp;
    // HashKF * hash;
    // TSystem * gSystem;
    
    //TH1F * name_hist;
    TH1F * name_hist_full;

    // energy spectra
    TH1F * pion_en;
    TH1F * nucleon_en;
    TH1F * photon_en;
    TH1F * kaon_en;
    TH1F * lepton_en;
    TH1F * neutrino_en;

    // transverse momentum spectra
    TH1F * pion_pt;
    TH1F * nucleon_pt;
    TH1F * photon_pt;
    TH1F * kaon_pt;
    TH1F * lepton_pt;
    TH1F * neutrino_pt;

    // longitudinal momentum spectra
    TH1F * pion_pz;
    TH1F * nucleon_pz;
    TH1F * photon_pz;
    TH1F * kaon_pz;
    TH1F * lepton_pz;
    TH1F * neutrino_pz;

    // eta-phi
    TH2F * pion_ep;
    TH2F * nucleon_ep;
    TH2F * photon_ep;
    TH2F * kaon_ep;
    TH2F * lepton_ep;
    TH2F * neutrino_ep;
    TH2F * all_ep;

    // canvases
    TCanvas * c_en;
    TCanvas * c_pt;
    TCanvas * c_pz;
    TCanvas * c_ep;
    TCanvas * c_name;
    TCanvas * c_name_full;

    TLeaf * KF_le;
    TLeaf * KS_le;
    TLeaf * Energy_le;
    TLeaf * px_le;
    TLeaf * py_le;
    TLeaf * pz_le;
    TLeaf * parent_le;
    TLeaf * fchild_le;
    TLeaf * lchild_le;

    Int_t KF_le_len;
    Int_t part_num;
    Long_t KF;
    Int_t KS;
    Float_t Energy;
    char PName[64];
    Float_t px,py,pz,pt,pmag;
    Float_t theta,eta,phi;
    Int_t parent,fchild,lchild;
    Bool_t outParticle;

    ClassDef(Analysis,1);
};

#endif
