void EventLoop(Int_t nEvents=1000, Float_t sqrts=200.) 
{
  TFile * fout = new TFile("events.root","RECREATE");
  TTree * events = new TTree("events","events");

  // initialise pythia
  TPythia6 * pythia = new TPythia6;
  pythia->Initialize("CMS","p","p",sqrts);
  TClonesArray * particles = (TClonesArray*) pythia->GetListOfParticles();
  events->Branch("event",&particles);

  
  // ----- EVENT LOOP ----- //
  for(Int_t i=0; i<nEvents; i++)
  {
    if(i%100==0) printf("Event # %d\n",i);
    pythia->GenerateEvent();
    events->Fill();
  };

  events->Write();

  exit();
};


