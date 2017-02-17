// call tppmc to generate events
// if nEvents=0, just show information
// on events.root

void generateEvents(Int_t nEvents=0, Int_t beam_en=200)
{
  const Double_t PI = 3.14159265;

  // run tppmc
  //gSystem->Load("libtppmc.so");
  char callTPPMC[200];
  char fname[50];
  sprintf(fname,"events.root");
  if(nEvents)
  {
    if(beam_en==200 || beam_en==500)
      sprintf(callTPPMC,"tppmc --rhic%d -e %d -o %s",beam_en,nEvents,fname);
    else sprintf(callTPPMC,"tppmc -e %d -o %s",nEvents,fname);
    printf("generator: %s\n",callTPPMC);
    system(callTPPMC);
  }

  // eta-phi
  TFile * ff = new TFile(fname);
  TTree * ev = (TTree*) ff->Get("events");

  TH2F * pxy_hist = new TH2F("pxy_hist","hadron p_{x}-p_{y}",40,-4,4,40,-4,4);
  TH1F * pz_hist = new TH1F("pz_hist","hadron p_{z}",90,0,60);
  TH1F * e_hist = new TH1F("e_hist","hadron E",80,0,80);
  TH2F * eta_phi = new TH2F("eta_phi","#eta-#phi plot",30,2.5,4,30,-PI,PI);
  TH1F * name_hist = new TH1F();

  TBranch * hadron_br = ev->GetBranch("mHadrons");
  TLeaf * px_le = ev->GetLeaf("mHadrons.px");
  TLeaf * py_le = ev->GetLeaf("mHadrons.py");
  TLeaf * pz_le = ev->GetLeaf("mHadrons.pz");
  TLeaf * type_le = ev->GetLeaf("mHadrons.type");
  Double_t px,py,pz,p,theta,phi,eta,mass,energy;
  Int_t type;
  char name[30];

  TFile * fileout = new TFile("evout.root","recreate");
  TTree * evout = new TTree("evout","evout");
  evout->Branch("name",&name,"name/C");
  evout->Branch("mass",&mass,"mass/D");
  evout->Branch("px",&px,"px/D");
  evout->Branch("py",&py,"py/D");
  evout->Branch("pz",&pz,"pz/D");
  evout->Branch("E",&energy,"energy/D");
  evout->Branch("eta",&eta,"eta/D");
  evout->Branch("phi",&phi,"phi/D");

  TPolyLine3D * polyline = new TPolyLine3D();
  polyline->SetPoint(0,0,0,0);

  for(Int_t i=0; i<hadron_br->GetEntries(); i++)
  {
    hadron_br->GetEntry(i);
    for(Int_t j=0; j<px_le->GetLen(); j++)
    {
      px = px_le->GetValue(j);
      py = py_le->GetValue(j);
      pz = pz_le->GetValue(j);
      type = type_le->GetValue(j);

      switch(type)
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
          fprintf(stderr,"Error: Unknown Particle Type %d encountered\n",type);
          exit();
      };

      p = sqrt(pow(px,2)+pow(py,2)+pow(pz,2));
      energy=sqrt(pow(mass,2)+pow(p,2));
      e_hist->Fill(energy);

      phi = atan2(py,px);

      pxy_hist->Fill(px,py);
      pz_hist->Fill(pz);
      polyline->SetNextPoint(pz,px,py);
      polyline->SetNextPoint(0,0,0);

      theta = acos(pz/p);
      eta = -log(tan(theta/2));
      //if(pz<0) eta=-eta;
      //printf("pz=%f eta=%f phi=%f\n",pz,eta,phi);
      eta_phi->Fill(eta,phi);
      name_hist->Fill(name,1);

      evout->Fill();
    };
  };

  TAxis3D * p_axis = new TAxis3D();
  p_axis->SetXTitle("pz");
  p_axis->SetYTitle("px");
  p_axis->SetZTitle("py");
  p_axis->SetTitleOffset(1.3);


  pxy_hist->SetStats(0);
  pxy_hist->GetYaxis()->SetTitle("p_{y} (GeV)");
  pxy_hist->GetXaxis()->SetTitle("p_{x} (GeV)");
  pz_hist->SetStats(0);
  pz_hist->GetXaxis()->SetTitle("p_{z} (GeV)");
  e_hist->SetStats(0);
  e_hist->GetXaxis()->SetTitle("E (GeV)");
  eta_phi->SetStats(0);
  eta_phi->GetYaxis()->SetTitle("#phi");
  eta_phi->GetXaxis()->SetTitle("#eta");
  name_hist->SetStats(0);
  name_hist->SetTitle("hadron types");
  name_hist->GetXaxis()->SetTitle("type");
  name_hist->SetBarWidth(0.4);
  name_hist->SetBarOffset(0.55);
  name_hist->SetFillColor(50);

  TCanvas * cc = new TCanvas("cc","cc",1200,800);
  cc->Divide(3,2);
  cc->cd(1);
  pxy_hist->Draw("colz");
  cc->cd(2);
  pz_hist->Draw();
  cc->cd(3);
  e_hist->Draw();
  cc->cd(4);
  eta_phi->Draw("colz");
  cc->cd(5);
  name_hist->Draw("bar2");
  cc->cd(6); 
  TView3D * p_view = TView::CreateView(1);
  p_view->SetRange(-20,-10,-10,20,10,10);
  p_view->SetPerspective();
  polyline->Draw();
  p_axis->Draw();
  
  cc->Write();
  evout->Write();
};
