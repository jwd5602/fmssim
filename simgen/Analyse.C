void Analyse(const char * fname="outgen/events.root")
{
  TFile * ff = new TFile(fname,"READ");
  TTree * tr = (TTree*) ff->Get("events");
  //tr->Print();
  
  // ---- CUTS ---- //
  char eta_cut_l[128];
  char eta_cut_h[128];
  char eta_def[128];
  char eta_cut[256];
  char pz_cut[128];
  strcpy(eta_def,
    "-log(tan(1/2*acos(event.fPz/sqrt(pow(event.fPx,2)+pow(event.fPy,2)+pow(event.fPz,2)))))");
  sprintf(eta_cut_l,"%s>%f",eta_def,2.5);
  sprintf(eta_cut_h,"%s<%f",eta_def,4.0);
  sprintf(eta_cut,"%s && %s",eta_cut_l,eta_cut_h);
  strcpy(pz_cut,"event.fPz>0");

  char pi0_cut[512];
  char pip_cut[512];
  char pim_cut[512];
  char gamma_cut[512];
  char etap_cut[512];
  sprintf(pi0_cut,"%s && %s && event.fKF==111",eta_cut,pz_cut);
  sprintf(pip_cut,"%s && %s && event.fKF==211",eta_cut,pz_cut);
  sprintf(pim_cut,"%s && %s && event.fKF==-211",eta_cut,pz_cut);
  sprintf(gamma_cut,"%s && %s && event.fKF==22",eta_cut,pz_cut);
  sprintf(etap_cut,"%s && %s && event.fKF==221",eta_cut,pz_cut);
  



  // ----- ANALYSIS ----- //
  // pt spectra
  const Int_t PT_NBINS = 150;
  const Int_t EN_NBINS = 200;
  TH1F * pi0_pt = new TH1F("pi0_pt","normalised p_{#perp}  spectra (2.5<#eta<4.0)",PT_NBINS,0,5);
  TH1F * pip_pt = new TH1F("pip_pt","normalised p_{#perp}  spectra (2.5<#eta<4.0)",PT_NBINS,0,5);
  TH1F * pim_pt = new TH1F("pim_pt","normalised p_{#perp}  spectra (2.5<#eta<4.0)",PT_NBINS,0,5);
  TH1F * gamma_pt = new TH1F("gamma_pt","normalised p_{#perp}  spectra (2.5<#eta<4.0)",PT_NBINS,0,5);
  TH1F * etap_pt = new TH1F("eta_pt","normalised p_{#perp}  spectra (2.5<#eta<4.0)",PT_NBINS,0,5);
  // energy spectra
  TH1F * pi0_en = new TH1F("pi0_en","normalised E_{inc} spectra (2.5<#eta<4.0)",EN_NBINS,0,100);
  TH1F * pip_en = new TH1F("pip_en","normalised E_{inc} spectra (2.5<#eta<4.0)",EN_NBINS,0,100);
  TH1F * pim_en = new TH1F("pim_en","normalised E_{inc} spectra (2.5<#eta<4.0)",EN_NBINS,0,100);
  TH1F * gamma_en = new TH1F("gamma_en","normalised E_{inc} spectra (2.5<#eta<4.0)",EN_NBINS,0,100);
  TH1F * etap_en = new TH1F("etap_en","normalised E_{inc} spectra (2.5<#eta<4.0)",EN_NBINS,0,100);

  pi0_pt->SetXTitle("p_{#perp} (GeV)");
  pip_pt->SetXTitle("p_{#perp} (GeV)");
  pim_pt->SetXTitle("p_{#perp} (GeV)");
  gamma_pt->SetXTitle("p_{#perp} (GeV)");
  etap_pt->SetXTitle("p_{#perp} (GeV)");

  pi0_en->SetXTitle("E_{inc} (GeV)");
  pip_en->SetXTitle("E_{inc} (GeV)");
  pim_en->SetXTitle("E_{inc} (GeV)");
  gamma_en->SetXTitle("E_{inc} (GeV)");
  etap_en->SetXTitle("E_{inc} (GeV)");


  pi0_pt->SetYTitle("dN/dp_{#perp}");
  pip_pt->SetYTitle("dN/dp_{#perp}");
  pim_pt->SetYTitle("dN/dp_{#perp}");
  gamma_pt->SetYTitle("dN/dp_{#perp}");
  etap_pt->SetYTitle("dN/dp_{#perp}");

  pi0_en->SetYTitle("dN/dE_{inc}");
  pip_en->SetYTitle("dN/dE_{inc}");
  pim_en->SetYTitle("dN/dE_{inc}");
  gamma_en->SetYTitle("dN/dE_{inc}");
  etap_en->SetYTitle("dN/dE_{inc}");
  
  pi0_pt->SetLineColor(kBlue);
  pim_pt->SetLineColor(kRed);
  pip_pt->SetLineColor(kGreen);
  gamma_pt->SetLineColor(kMagenta);
  etap_pt->SetLineColor(kBlue);

  pi0_en->SetLineColor(kBlue);
  pim_en->SetLineColor(kRed);
  pip_en->SetLineColor(kGreen);
  gamma_en->SetLineColor(kMagenta);
  etap_en->SetLineColor(kBlue);

  printf("projecting pt dists...\n");
  tr->Project("pi0_pt","sqrt(pow(event.fPx,2)+pow(event.fPy,2))",pi0_cut);
  tr->Project("pip_pt","sqrt(pow(event.fPx,2)+pow(event.fPy,2))",pim_cut);
  tr->Project("pim_pt","sqrt(pow(event.fPx,2)+pow(event.fPy,2))",pip_cut);
  tr->Project("gamma_pt","sqrt(pow(event.fPx,2)+pow(event.fPy,2))",gamma_cut);
  tr->Project("etap_pt","sqrt(pow(event.fPx,2)+pow(event.fPy,2))",etap_cut);

  printf("projecting energy dists...\n");
  tr->Project("pi0_en","event.fEnergy",pi0_cut);
  tr->Project("pip_en","event.fEnergy",pip_cut);
  tr->Project("pim_en","event.fEnergy",pim_cut);
  tr->Project("gamma_en","event.fEnergy",gamma_cut);
  tr->Project("etap_en","event.fEnergy",etap_cut);

  pi0_pt->Sumw2();
  pip_pt->Sumw2();
  pim_pt->Sumw2();
  gamma_pt->Sumw2();
  etap_pt->Sumw2();

  pi0_en->Sumw2();
  pip_en->Sumw2();
  pim_en->Sumw2();
  gamma_en->Sumw2();
  etap_en->Sumw2();

  pi0_pt->Scale((Float_t)PT_NBINS/5./pi0_pt->Integral());
  pip_pt->Scale((Float_t)PT_NBINS/5./pip_pt->Integral());
  pim_pt->Scale((Float_t)PT_NBINS/5./pim_pt->Integral());
  gamma_pt->Scale((Float_t)PT_NBINS/5./gamma_pt->Integral());
  etap_pt->Scale((Float_t)PT_NBINS/5./etap_pt->Integral());
  
  pi0_en->Scale((Float_t)EN_NBINS/100./pi0_en->Integral());
  pip_en->Scale((Float_t)EN_NBINS/100./pip_en->Integral());
  pim_en->Scale((Float_t)EN_NBINS/100./pim_en->Integral());
  gamma_en->Scale((Float_t)EN_NBINS/100./gamma_en->Integral());
  etap_en->Scale((Float_t)EN_NBINS/100./etap_en->Integral());

  TCanvas * c_pt = new TCanvas("c_pt","c_pt",800,600);
  c_pt->SetLogy();
  pip_pt->Draw("e1");
  pim_pt->Draw("e1,same");
  gamma_pt->Draw("e1,same");
  etap_pt->Draw("e1,same");

  TLegend * pt_leg = new TLegend(0.8,0.4,0.9,0.7);
  pt_leg->AddEntry(pim_pt,"#pi^{-} p_{#perp}","EL");
  pt_leg->AddEntry(pip_pt,"#pi^{+} p_{#perp}","EL");
  pt_leg->AddEntry(gamma_pt,"#gamma p_{#perp}","EL");
  pt_leg->AddEntry(etap_pt,"#eta p_{#perp}","EL");
  pt_leg->SetTextSize(0.04);
  pt_leg->Draw();

  TCanvas * c_en = new TCanvas("c_en","c_en",800,600);
  c_en->SetLogy();
  pip_en->Draw("e1");
  pim_en->Draw("e1,same");
  gamma_en->Draw("e1,same");
  etap_en->Draw("e1,same");

  TLegend * en_leg = new TLegend(0.8,0.4,0.9,0.7);
  en_leg->AddEntry(pim_en,"#pi^{-} E_{inc}","EL");
  en_leg->AddEntry(pip_en,"#pi^{+} E_{inc}","EL");
  en_leg->AddEntry(gamma_en,"#gamma E_{inc}","EL");
  en_leg->AddEntry(etap_en,"#eta E_{inc}","EL");
  en_leg->SetTextSize(0.04);
  en_leg->Draw();


  // particle bar chart
  TH1F * name_hist = new TH1F();
  TH1F * other_kf_hist = new TH1F();
  Int_t KF;
  Float_t ppx,ppy,ppz;
  Float_t mass;
  Float_t theta,eta,pmag;
  char name[32];

  TLeaf * KF_le = tr->GetLeaf("event.fKF");
  TLeaf * ppx_le = tr->GetLeaf("event.fPx");
  TLeaf * ppy_le = tr->GetLeaf("event.fPy");
  TLeaf * ppz_le = tr->GetLeaf("event.fPz");
  //system("touch unknown_particles.txt; rm unknown_particles.txt");

  for(Int_t i=1; i<tr->GetEntries(); i++)
  {
    tr->GetEntry(i);
    for(Int_t j=0; j<KF_le->GetLen(); j++)
    {
      KF=KF_le->GetValue(j);
      ppx = ppx_le->GetValue(j);
      ppy = ppy_le->GetValue(j);
      ppz = ppz_le->GetValue(j);

      pmag = sqrt(pow(ppx,2)+pow(ppy,2)+pow(ppz,2));
      //printf("ppz=%f pmag=%f\n",ppz,pmag);
      theta = acos(ppz/pmag);
      //printf("theta=%f\n",theta);
      if(theta<0.0001 || fabs(theta-3.141592)<0.01) eta = 1000000; // prevent infininty
      else eta = -log(tan(theta/2));
      
      if(ppz>0 && eta>2.5 && eta<4.0)
      {
        switch(KF)
        {
          case 22: // gamma
            mass=0;
            strcpy(name,"gamma");
            break;
          case 111: // pi0
            mass=0.1349766;
            strcpy(name,"pi0");
            break;
          case 211: // pi+
            mass=0.13957018;
            strcpy(name,"pi+");
            break;
          case -211: // pi-
            mass=0.13957018;
            strcpy(name,"pi-");
            break;
          case 221: // eta
            mass=0.54751;
            strcpy(name,"eta");
            break;
          case 311: // kaon0
            mass=0.497648;
            strcpy(name,"kaon0");
            break;
          case -311: // anti_kaon0
            mass=0.497648;
            strcpy(name,"anti_kaon0");
            break;
          case 321: // kaon+
            mass=0.493677;
            strcpy(name,"kaon+");
            break;
          case -321: // kaon-
            mass=0.493677;
            strcpy(name,"kaon-");
            break;
          case 2112: // neutron
            mass=0.93956536;
            strcpy(name,"neutron");
            break;
          case -2112: // anti_neutron
            mass=0.93956536;
            strcpy(name,"anti_neutron");
            break;
          case 2212: // proton
            mass=0.93827203;
            strcpy(name,"proton");
            break;
          case -2212: // anti_proton
            mass=0.93827203;
            strcpy(name,"anti_proton");
            break;
          default:
            mass=0;
            strcpy(name,"OTHER");
            other_kf_hist->Fill(KF);
            /*
            gSystem->RedirectOutput("unknown_particles.txt");
            printf("%d\n",KF);
            gSystem->RedirectOutput(0);
            */
            break;
        }; // eo switch
        name_hist->Fill(name,1);
      }; // eo if eta and pz cut
    }; // eo for j
  }; // eo for i

  // unknown particles list
  //system(relist); // deprecated

  name_hist->SetStats(0);
  name_hist->SetTitle("produced particles (2.5<#eta<4.0)");
  name_hist->GetXaxis()->SetTitle("type");
  name_hist->SetBarWidth(0.4);
  name_hist->SetBarOffset(0.55);
  name_hist->SetFillColor(50);

  TCanvas * c_name = new TCanvas("c_name","c_name",800,600);
  name_hist->Draw("bar2");

  TFile * outroot  = new TFile("outgen/plots.root","RECREATE");
  pi0_pt->Write("pi0_pt");
  pip_pt->Write("pip_pt");
  pim_pt->Write("pim_pt");
  gamma_pt->Write("gamma_pt");
  etap_pt->Write("etap_pt");
  pi0_en->Write("pi0_en");
  pip_en->Write("pip_en");
  pim_en->Write("pim_en");
  gamma_en->Write("gamma_en");
  etap_en->Write("etap_en");
  c_pt->Write("c_pt");
  c_en->Write("c_en");



  //TCanvas * c_other_kf = new TCanvas("c_other_kf","c_other_kf",800,600);
  //other_kf_hist->Scan();




  

  

};

