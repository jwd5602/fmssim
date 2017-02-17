# include "Analysis.h"
using namespace std;

ClassImp(Analysis)

const Float_t PI=3.14159265;

Analysis::Analysis(TTree * events,Float_t sqrts_)
{
  printf("HELLO EVERYBODY FROM ANALYSIS\n");
  evtr = events;
  sqrts = sqrts_;

  printf("TMC PARTICLE INSTANCE\n");
  parttmp = new TMCParticle();

  //TString datfile="";
  printf("SIMGENDIR CREATED HERE FROM gSystem\n");
  SIMGENDIR = gSystem->ExpandPathName("${SIMGEN}");
  //datfile=SIMGENDIR+"/dat/codes.dat";
  //cout << "datfile=" << datfile << endl;

  // hash = new HashKF(datfile);
  printf("LEAVES ON THE TTREE STUFF!!!\n");

  KF_le = evtr->GetLeaf("event.fKF");
  KS_le = evtr->GetLeaf("event.fKS");
  Energy_le = evtr->GetLeaf("event.fEnergy");
  px_le = evtr->GetLeaf("event.fPx");
  py_le = evtr->GetLeaf("event.fPy");
  pz_le = evtr->GetLeaf("event.fPz");
  parent_le = evtr->GetLeaf("event.fParent");
  fchild_le = evtr->GetLeaf("event.fFirstChild");
  lchild_le = evtr->GetLeaf("event.fLastChild");
}



void Analysis::Analyse()
{
  printf("Analysing hadron kf codes...\n");

  // define binning
  Int_t nbins_en = (Int_t) sqrts*3/4;
  Int_t maxbin_en = nbins_en;
  Int_t nbins_pt = (Int_t) sqrts*3/4;
  Int_t maxbin_pt = nbins_pt/30;
  Int_t nbins_pz = (Int_t) sqrts*3/4;
  Int_t maxbin_pz = nbins_pz;
  
  // initialise histograms
  kaon_en = new TH1F("kaon_en","E spectra",nbins_en,0,maxbin_en);
  nucleon_en = new TH1F("nucleon_en","E spectra",nbins_en,0,maxbin_en);
  photon_en = new TH1F("photon_en","E spectra",nbins_en,0,maxbin_en);
  pion_en = new TH1F("pion_en","E spectra",nbins_en,0,maxbin_en);
  lepton_en = new TH1F("lepton_en","E spectra",nbins_en,0,maxbin_en);
  neutrino_en = new TH1F("neutrino_en","E spectra",nbins_en,0,maxbin_en);

  kaon_pt = new TH1F("kaon_pt","p_{#perp}  spectra",nbins_pt,0,maxbin_pt);
  nucleon_pt = new TH1F("nucleon_pt","p_{#perp}  spectra",nbins_pt,0,maxbin_pt);
  photon_pt = new TH1F("photon_pt","p_{#perp}  spectra",nbins_pt,0,maxbin_pt);
  pion_pt = new TH1F("pion_pt","p_{#perp}  spectra",nbins_pt,0,maxbin_pt);
  lepton_pt = new TH1F("lepton_pt","p_{#perp}  spectra",nbins_pt,0,maxbin_pt);
  neutrino_pt = new TH1F("neutrino_pt","p_{#perp}  spectra",nbins_pt,0,maxbin_pt);

  kaon_pz = new TH1F("kaon_pz","p_{z}  spectra",nbins_pz,0,maxbin_pz);
  nucleon_pz = new TH1F("nucleon_pz","p_{z}  spectra",nbins_pz,0,maxbin_pz);
  photon_pz = new TH1F("photon_pz","p_{z}  spectra",nbins_pz,0,maxbin_pz);
  pion_pz = new TH1F("pion_pz","p_{z}  spectra",nbins_pz,0,maxbin_pz);
  lepton_pz = new TH1F("lepton_pz","p_{z}  spectra",nbins_pz,0,maxbin_pz);
  neutrino_pz = new TH1F("neutrino_pz","p_{z}  spectra",nbins_pz,0,maxbin_pz);

  pion_ep = new TH2F("pion_ep","#eta-#phi plot",30,2.5,4,30,-PI,PI);
  nucleon_ep = new TH2F("nucleon_ep","#eta-#phi plot",30,2.5,4,30,-PI,PI);
  photon_ep = new TH2F("photon_ep","#eta-#phi plot",30,2.5,4,30,-PI,PI);
  kaon_ep = new TH2F("kaon_ep","#eta-#phi plot",30,2.5,4,30,-PI,PI);
  lepton_ep = new TH2F("lepton_ep","#eta-#phi plot",30,2.5,4,30,-PI,PI);
  neutrino_ep = new TH2F("neutrino_ep","#eta-#phi plot",30,2.5,4,30,-PI,PI);
  all_ep = new TH2F("all_ep","#eta-#phi plot",30,2.5,4,30,-PI,PI);
  
  //name_hist = new TH1F();
  name_hist_full = new TH1F();

  // fill loop
  printf("filling histograms...\n");
  for(Int_t i=0; i<evtr->GetEntries(); i++)
  {
    evtr->GetEntry(i);
    KF_le_len = KF_le->GetLen();
    for(Int_t j=0; j<KF_le_len; j++)
    {
      // get leaf values
      KF = KF_le->GetValue(j);
      KS = KS_le->GetValue(j);
      Energy = Energy_le->GetValue(j);
      px = px_le->GetValue(j);
      py = py_le->GetValue(j);
      pz = pz_le->GetValue(j);
      parent = parent_le->GetValue(j);
      fchild = fchild_le->GetValue(j);
      lchild = lchild_le->GetValue(j);

      pt = sqrt(pow(px,2)+pow(py,2));
      pmag = sqrt(pow(px,2)+pow(py,2)+pow(pz,2));
      theta = acos(pz/pmag);
      phi = atan2(py,px);
      eta = -log(tan(theta/2));

      // final state filter
      outParticle = FinalStateFilter(KF,KS,parent,KF_le);

      if(outParticle)
      {
        // strcpy(PName,hash->GetParticle(KF)->GetPname());
        parttmp->SetKF(KF);
        strcpy(PName,parttmp->GetName());

        name_hist_full->Fill(PName,1);
        all_ep->Fill(eta,phi);

        if(strstr(PName,"K")!=NULL) 
        {
          kaon_en->Fill(Energy);
          kaon_pt->Fill(pt);
          kaon_pz->Fill(pz);
          kaon_ep->Fill(eta,phi);
        }
        else if(strstr(PName,"p+")!=NULL ||
                strstr(PName,"n0")!=NULL ||
                strstr(PName,"pbar")!=NULL ||
                strstr(PName,"nbar")!=NULL
               )
        {
          nucleon_en->Fill(Energy);
          nucleon_pt->Fill(pt);
          nucleon_pz->Fill(pz);
          nucleon_ep->Fill(eta,phi);
        }
        else if(strstr(PName,"gamma")!=NULL)
        {
          photon_en->Fill(Energy);
          photon_pt->Fill(pt);
          photon_pz->Fill(pz);
          photon_ep->Fill(eta,phi);
        }
        else if(strstr(PName,"pi")!=NULL)
        {
          pion_en->Fill(Energy);
          pion_pt->Fill(pt);
          pion_pz->Fill(pz);
          pion_ep->Fill(eta,phi);
        }
        else if(strstr(PName,"nu")==NULL &&
                 ( strstr(PName,"e+")!=NULL ||
                   strstr(PName,"e-")!=NULL ||
                   strstr(PName,"mu")!=NULL ||
                   strstr(PName,"tau")!=NULL
                 )
               )
        {
          lepton_en->Fill(Energy); // NOTE - lepton plots exclude neutrinos!
          lepton_pt->Fill(pt);
          lepton_pz->Fill(pz);
          lepton_ep->Fill(eta,phi);
        }
        else if(strstr(PName,"nu")!=NULL)
        {
          neutrino_en->Fill(Energy);
          neutrino_pt->Fill(pt);
          neutrino_pz->Fill(pz);
          neutrino_ep->Fill(eta,phi);
        }
        else printf("particle %s not plotted\n",PName);

        //name_hist->Fill(PName,1);
      } // if ks
    } // leaf loop
  } // branch loop

  /* ----- PLOT PROPERTIES ----- */
  // name histogram properties
  /*
  name_hist->SetStats(0);
  name_hist->SetTitle("produced particles");
  name_hist->SetName("name_hist");
  name_hist->GetXaxis()->SetTitle("type");
  name_hist->SetBarWidth(0.4);
  name_hist->SetBarOffset(0.55);
  name_hist->SetFillColor(50);
  name_hist->LabelsOption(">","X");
  */
  
  name_hist_full->SetStats(0);
  name_hist_full->SetTitle("produced particles");
  name_hist_full->SetName("name_hist_full");
  name_hist_full->GetXaxis()->SetTitle("type");
  name_hist_full->SetBarWidth(0.4);
  name_hist_full->SetBarOffset(0.55);
  name_hist_full->SetFillColor(50);
  name_hist_full->LabelsOption(">","X");

  // energy spectra properties
  kaon_en->SetXTitle("E (GeV)");
  nucleon_en->SetXTitle("E (GeV)");
  photon_en->SetXTitle("E (GeV)");
  pion_en->SetXTitle("E (GeV)");
  lepton_en->SetXTitle("E (GeV)");
  neutrino_en->SetXTitle("E (GeV)");

  kaon_en->SetYTitle("dN/dE");
  nucleon_en->SetYTitle("dN/dE");
  photon_en->SetYTitle("dN/dE");
  pion_en->SetYTitle("dN/dE");
  lepton_en->SetYTitle("dN/dE");
  neutrino_en->SetYTitle("dN/dE");

  kaon_en->SetLineColor(kBlack);
  nucleon_en->SetLineColor(kBlue);
  photon_en->SetLineColor(kRed);
  pion_en->SetLineColor(kGreen);
  lepton_en->SetLineColor(kMagenta);
  neutrino_en->SetLineColor(kOrange);
  
  kaon_en->Sumw2();
  nucleon_en->Sumw2();
  photon_en->Sumw2();
  pion_en->Sumw2();
  lepton_en->Sumw2();
  neutrino_en->Sumw2();

  kaon_en->Scale((Float_t)nbins_en/(Float_t)maxbin_en/kaon_en->Integral());
  nucleon_en->Scale((Float_t)nbins_en/(Float_t)maxbin_en/nucleon_en->Integral());
  photon_en->Scale((Float_t)nbins_en/(Float_t)maxbin_en/photon_en->Integral());
  pion_en->Scale((Float_t)nbins_en/(Float_t)maxbin_en/pion_en->Integral());
  lepton_en->Scale((Float_t)nbins_en/(Float_t)maxbin_en/lepton_en->Integral());
  neutrino_en->Scale((Float_t)nbins_en/(Float_t)maxbin_en/neutrino_en->Integral());

  // pt spectra properties
  kaon_pt->SetXTitle("p_{#perp}  (GeV)");
  nucleon_pt->SetXTitle("p_{#perp}  (GeV)");
  photon_pt->SetXTitle("p_{#perp}  (GeV)");
  pion_pt->SetXTitle("p_{#perp}  (GeV)");
  lepton_pt->SetXTitle("p_{#perp}  (GeV)");
  neutrino_pt->SetXTitle("p_{#perp}  (GeV)");

  kaon_pt->SetYTitle("dN/dp_{#perp}");
  nucleon_pt->SetYTitle("dN/dp_{#perp}");
  photon_pt->SetYTitle("dN/dp_{#perp}");
  pion_pt->SetYTitle("dN/dp_{#perp}");
  lepton_pt->SetYTitle("dN/dp_{#perp}");
  neutrino_pt->SetYTitle("dN/dp_{#perp}");

  kaon_pt->SetLineColor(kBlack);
  nucleon_pt->SetLineColor(kBlue);
  photon_pt->SetLineColor(kRed);
  pion_pt->SetLineColor(kGreen);
  lepton_pt->SetLineColor(kMagenta);
  neutrino_pt->SetLineColor(kOrange);
  
  kaon_pt->Sumw2();
  nucleon_pt->Sumw2();
  photon_pt->Sumw2();
  pion_pt->Sumw2();
  lepton_pt->Sumw2();
  neutrino_pt->Sumw2();

  kaon_pt->Scale((Float_t)nbins_pt/(Float_t)maxbin_pt/kaon_pt->Integral());
  nucleon_pt->Scale((Float_t)nbins_pt/(Float_t)maxbin_pt/nucleon_pt->Integral());
  photon_pt->Scale((Float_t)nbins_pt/(Float_t)maxbin_pt/photon_pt->Integral());
  pion_pt->Scale((Float_t)nbins_pt/(Float_t)maxbin_pt/pion_pt->Integral());
  lepton_pt->Scale((Float_t)nbins_pt/(Float_t)maxbin_pt/lepton_pt->Integral());
  neutrino_pt->Scale((Float_t)nbins_pt/(Float_t)maxbin_pt/neutrino_pt->Integral());

  // pz spectra properties
  kaon_pz->SetXTitle("p_{z}  (GeV)");
  nucleon_pz->SetXTitle("p_{z}  (GeV)");
  photon_pz->SetXTitle("p_{z}  (GeV)");
  pion_pz->SetXTitle("p_{z}  (GeV)");
  lepton_pz->SetXTitle("p_{z}  (GeV)");
  neutrino_pz->SetXTitle("p_{z}  (GeV)");

  kaon_pz->SetYTitle("dN/dp_{z}");
  nucleon_pz->SetYTitle("dN/dp_{z}");
  photon_pz->SetYTitle("dN/dp_{z}");
  pion_pz->SetYTitle("dN/dp_{z}");
  lepton_pz->SetYTitle("dN/dp_{z}");
  neutrino_pz->SetYTitle("dN/dp_{z}");

  kaon_pz->SetLineColor(kBlack);
  nucleon_pz->SetLineColor(kBlue);
  photon_pz->SetLineColor(kRed);
  pion_pz->SetLineColor(kGreen);
  lepton_pz->SetLineColor(kMagenta);
  neutrino_pz->SetLineColor(kOrange);
  
  kaon_pz->Sumw2();
  nucleon_pz->Sumw2();
  photon_pz->Sumw2();
  pion_pz->Sumw2();
  lepton_pz->Sumw2();
  neutrino_pz->Sumw2();

  kaon_pz->Scale((Float_t)nbins_pz/(Float_t)maxbin_pz/kaon_pz->Integral());
  nucleon_pz->Scale((Float_t)nbins_pz/(Float_t)maxbin_pz/nucleon_pz->Integral());
  photon_pz->Scale((Float_t)nbins_pz/(Float_t)maxbin_pz/photon_pz->Integral());
  pion_pz->Scale((Float_t)nbins_pz/(Float_t)maxbin_pz/pion_pz->Integral());
  lepton_pz->Scale((Float_t)nbins_pz/(Float_t)maxbin_pz/lepton_pz->Integral());
  neutrino_pz->Scale((Float_t)nbins_pz/(Float_t)maxbin_pz/neutrino_pz->Integral());

  // eta phi properties
  kaon_ep->SetStats(0);
  pion_ep->SetStats(0);
  photon_ep->SetStats(0);
  nucleon_ep->SetStats(0);
  lepton_ep->SetStats(0);
  neutrino_ep->SetStats(0);
  all_ep->SetStats(0);

  kaon_ep->GetYaxis()->SetTitle("#phi");
  pion_ep->GetYaxis()->SetTitle("#phi");
  photon_ep->GetYaxis()->SetTitle("#phi");
  nucleon_ep->GetYaxis()->SetTitle("#phi");
  lepton_ep->GetYaxis()->SetTitle("#phi");
  neutrino_ep->GetYaxis()->SetTitle("#phi");
  all_ep->GetYaxis()->SetTitle("#phi");

  kaon_ep->GetXaxis()->SetTitle("#eta");
  pion_ep->GetXaxis()->SetTitle("#eta");
  photon_ep->GetXaxis()->SetTitle("#eta");
  nucleon_ep->GetXaxis()->SetTitle("#eta");
  lepton_ep->GetXaxis()->SetTitle("#eta");
  neutrino_ep->GetXaxis()->SetTitle("#eta");
  all_ep->GetXaxis()->SetTitle("#eta");


  /* ----- DRAW OUTPUT ----- */
  // bar charts draw
  //c_name = new TCanvas("c_name","c_name",800,600);
  //name_hist->Draw("bar2");
  c_name_full = new TCanvas("c_name_full","c_name_full",800,600);
  name_hist_full->Draw("bar2");

  // energy spectra draw
  c_en = new TCanvas("c_en","c_en",800,600);
  c_en->SetLogy();
  kaon_en->Draw("e1");
  nucleon_en->Draw("e1,same");
  photon_en->Draw("e1,same");
  pion_en->Draw("e1,same");
  lepton_en->Draw("e1,same");
  neutrino_en->Draw("e1,same");

  TLegend * en_leg = new TLegend(0.8,0.5,0.9,0.7);
  en_leg->AddEntry(kaon_en,"kaons","EL");
  en_leg->AddEntry(nucleon_en,"nucleons","EL");
  en_leg->AddEntry(photon_en,"photons","EL");
  en_leg->AddEntry(pion_en,"pions","EL");
  en_leg->AddEntry(lepton_en,"leptons","EL");
  en_leg->AddEntry(neutrino_en,"neutrinos","EL");
  en_leg->SetTextSize(0.04);
  en_leg->Draw();

  // pt spectra draw
  c_pt = new TCanvas("c_pt","c_pt",800,600);
  c_pt->SetLogy();
  kaon_pt->Draw("e1");
  nucleon_pt->Draw("e1,same");
  photon_pt->Draw("e1,same");
  pion_pt->Draw("e1,same");
  lepton_pt->Draw("e1,same");
  neutrino_pt->Draw("e1,same");

  TLegend * pt_leg = new TLegend(0.8,0.5,0.9,0.7);
  pt_leg->AddEntry(kaon_pt,"kaons","EL");
  pt_leg->AddEntry(nucleon_pt,"nucleons","EL");
  pt_leg->AddEntry(photon_pt,"photons","EL");
  pt_leg->AddEntry(pion_pt,"pions","EL");
  pt_leg->AddEntry(lepton_pt,"leptons","EL");
  pt_leg->AddEntry(neutrino_pt,"neutrinos","EL");
  pt_leg->SetTextSize(0.04);
  pt_leg->Draw();

  // pz spectra draw
  c_pz = new TCanvas("c_pz","c_pz",800,600);
  c_pz->SetLogy();
  kaon_pz->Draw("e1");
  nucleon_pz->Draw("e1,same");
  photon_pz->Draw("e1,same");
  pion_pz->Draw("e1,same");
  lepton_pz->Draw("e1,same");
  neutrino_pz->Draw("e1,same");

  TLegend * pz_leg = new TLegend(0.8,0.5,0.9,0.7);
  pz_leg->AddEntry(kaon_pz,"kaons","EL");
  pz_leg->AddEntry(nucleon_pz,"nucleons","EL");
  pz_leg->AddEntry(photon_pz,"photons","EL");
  pz_leg->AddEntry(pion_pz,"pions","EL");
  pz_leg->AddEntry(lepton_pz,"leptons","EL");
  pz_leg->AddEntry(neutrino_pz,"neutrinos","EL");
  pz_leg->SetTextSize(0.04);
  pz_leg->Draw();

  // eta phi draw
  c_ep = new TCanvas("c_ep","c_ep",800,600);
  all_ep->Draw("colz");
}

TObjArray * Analysis::GetHistograms()
{
  TObjArray * toa = new TObjArray();

  toa->AddLast(pion_en);
  toa->AddLast(nucleon_en);
  toa->AddLast(photon_en);
  toa->AddLast(kaon_en);
  toa->AddLast(lepton_en);
  toa->AddLast(neutrino_en);

  toa->AddLast(pion_pt);
  toa->AddLast(nucleon_pt);
  toa->AddLast(photon_pt);
  toa->AddLast(kaon_pt);
  toa->AddLast(lepton_pt);
  toa->AddLast(neutrino_pt);

  toa->AddLast(pion_pz);
  toa->AddLast(nucleon_pz);
  toa->AddLast(photon_pz);
  toa->AddLast(kaon_pz);
  toa->AddLast(lepton_pz);
  toa->AddLast(neutrino_pz);

  toa->AddLast(pion_ep);
  toa->AddLast(nucleon_ep);
  toa->AddLast(photon_ep);
  toa->AddLast(kaon_ep);
  toa->AddLast(lepton_ep);
  toa->AddLast(neutrino_ep);
  toa->AddLast(all_ep);

  return toa;
}

TObjArray * Analysis::GetCanvases()
{
  TObjArray * toa = new TObjArray();

  //toa->AddLast(c_name);
  toa->AddLast(c_name_full);
  toa->AddLast(c_en);
  toa->AddLast(c_pt);
  toa->AddLast(c_pz);
  toa->AddLast(c_ep);

  return toa;
}


void Analysis::ListParents(Int_t lb=0, Int_t ub=0)
{
  Int_t shift=1;

  for(Int_t i=lb; i<=ub; i++)
  {
    evtr->GetEntry(i);
    KF_le_len = KF_le->GetLen();
    for(Int_t j=0; j<KF_le_len; j++)
    {
      KF = KF_le->GetValue(j);
      KS = KS_le->GetValue(j);
      Energy = Energy_le->GetValue(j);
      px = px_le->GetValue(j);
      py = py_le->GetValue(j);
      pz = pz_le->GetValue(j);
      parent = parent_le->GetValue(j);
      fchild = fchild_le->GetValue(j);
      lchild = lchild_le->GetValue(j);

      outParticle = FinalStateFilter(KF,KS,parent,KF_le);

      if(outParticle)
      {

        parttmp->SetKF(KF);


        printf("particle(%d): %s",j+1,parttmp->GetName());
        printf("\t\t");

        if(parent<KF_le_len && parent)
        {
          KF = KF_le->GetValue(parent-shift);
          parttmp->SetKF(KF);
          printf(" parent(%d): %s",parent,parttmp->GetName());
        }
        else if(parent==0)
          printf(" parent(0): none");
        else
          printf(" parent(0): unknown");
        printf("\t\t");

        if(fchild<KF_le_len && fchild)
        {
          KF = KF_le->GetValue(fchild-shift);
          parttmp->SetKF(KF);
          printf(" fchild(%d): %s",fchild,parttmp->GetName());
        }
        else if(fchild==0)
          printf(" fchild(0): none");
        else
          printf(" fchild(0): unknown");
        printf("\t\t");

        if(lchild<KF_le_len && lchild)
        {
          KF = KF_le->GetValue(lchild-shift);
          parttmp->SetKF(KF);
          printf(" lchild(%d): %s",lchild,parttmp->GetName());
        }
        else if(lchild==0)
          printf(" lchild(0): none");
        else
          printf(" lchild(0): unknown");

        printf("\n");
      } // if kf
    } // leaf loop
    printf("-------------------\n");
  } // branch loop
}

Bool_t Analysis::FinalStateFilter(Int_t KF0, Int_t KS0, Int_t parent0, TLeaf * KF_le0)
{
  Bool_t returnbool = false;
  if(KS0<11) returnbool = true; // final status
    
  // mark pi0s as final state and unmark daugters
  if(parent0 && parent0-1<KF_le0->GetLen())
  {
    if(KF_le0->GetValue(parent0-1)==111) returnbool=false;
    if(KF0==111) returnbool=true;
  }

  return returnbool;
}

void Analysis::BuildArgfile(Int_t macro_size=50,Bool_t randomEnergy=false,Float_t maxRandE=100.)
{
  Int_t macro_count=0, batch_number=0, hadron_count=1;
  TString ARGFILE=SIMGENDIR+"/outgen/argfile";
  char purgeArgFile[256];
  sprintf(purgeArgFile,"touch %s ; rm %s",ARGFILE.Data(),ARGFILE.Data());
  Int_t success = system(purgeArgFile);

/*
BS, remenant of me not realizing that particle data is in coordtr, may have possible use later to compare analyzed data to simulation
  TFile *argfile=new TFile("/home/branden/geant4testing/fmsu/simgen/outgen/argtree.root","RECREATE");
  TTree *partdata=new TTree("partdata","partdata");
  Int_t event_num=0, particle_num=0;
  partdata->Branch("Energy",&Energy);
  partdata->Branch("eta",&eta);
  partdata->Branch("phi",&phi);
  partdata->Branch("KF",&KF);
  partdata->Branch("px",&px);
  partdata->Branch("py",&py);
  partdata->Branch("event_num",&event_num);
  partdata->Branch("particle_num",&particle_num);
*/ 

  /*
   //BS, testing out a new way of generating argfile
 TFile* pythiaout=new TFile("outgen/pythiaout.root","READ");
 

   */


  for(Int_t i=0; i<evtr->GetEntries(); i++)
  {
    evtr->GetEntry(i);
    KF_le_len = KF_le->GetLen();
    for(Int_t j=0; j<KF_le_len; j++)
    {
      KF = KF_le->GetValue(j);
      KS = KS_le->GetValue(j);
      if(randomEnergy) Energy = (rand()%10000+1)/maxRandE;
      else Energy = Energy_le->GetValue(j);
      px = px_le->GetValue(j);
      py = py_le->GetValue(j);
      pz = pz_le->GetValue(j);
      parent = parent_le->GetValue(j);
      fchild = fchild_le->GetValue(j);
      lchild = lchild_le->GetValue(j);

      outParticle = FinalStateFilter(KF,KS,parent,KF_le);

      pmag = sqrt(pow(px,2)+pow(py,2)+pow(pz,2));
      pt = sqrt( pow(px,2) + pow(py,2) );
      theta = acos(pz/pmag);
      phi = atan2(py,px);
      eta = -log(tan(theta/2));


      // argfile cut ----------------------------
      // pions(111,211,-211),eta(221),gammas(22)
      // all
      /*
      if(outParticle && 
         (KF==111 || KF==211 || KF==-211 || KF==221 || KF==22) &&
         pz>0 && 
         eta>2.5 && eta<4
        )
        */
      // charged pions
      /*
      if(outParticle && 
         (KF==211 || KF==-211) &&
         pz>0 && 
         eta>2.5 && eta<4
        )
        */
      // gammas
/*
      if(outParticle && 
         KF==22 &&
         pz>0 && 
         eta>2.5 && eta<4
        )
*/
      //pi0
      if(outParticle && KF==111 && eta>2.5 && eta<4.0)
      {
        parttmp->SetKF(KF);
        strcpy(PName,parttmp->GetName());
        if(hadron_count%macro_size==1)
        {
          batch_number=1;
          macro_count++;
        }
        else if(hadron_count%macro_size==0) batch_number=3;
        else batch_number=2;
        gSystem->RedirectOutput(ARGFILE);
        /*
        printf("-i %d -b %d -t %s -n 1 -p -x %f -y %f -z %f\n",
          macro_count,batch_number,PName,px,py,pz); // energy specified via momentum
        */
        printf("-i %d -b %d -t %s -n 1 -e %f -x %f -y %f -z %f\n",
          macro_count,batch_number,PName,Energy,px,py,pz); // energy explicitly specified
        gSystem->RedirectOutput(0);
        hadron_count++;
      } // if outParticle
    } // leaf loop
  } // branch loop
  gSystem->RedirectOutput(ARGFILE);
  printf("-i %d -b %d\n",macro_count,4);
  gSystem->RedirectOutput(0);
  printf("arguments stored in %s\n", ARGFILE.Data());
  printf("# of primary particles to simulate: %d\n",hadron_count);
}
