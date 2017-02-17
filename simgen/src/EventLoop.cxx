#include "EventLoop.h"

//Remember to source env.sh before running GenerateEvents
//Projects to do: make this able to run multiple jobs simulatneously and also be able to have multiple events per file

ClassImp(EventLoop)

EventLoop::EventLoop(Int_t nEvents, Float_t sqrts)
{
  ////////instatiations
  pythia = new TPythia6;
  events = new TTree("events","events");

  TString simhome=gSystem->Getenv("SIMHOME");  
  macbase=simhome + "/geant/fmsu/macros/macro";
  JPsiAnalysis=false; //Only Look at e+e- from JPsi decay
  Pi0Analysis=false;
  JustTriggeredPi0=false; //Only create macros with the pi0s that pass cuts, not additional particles
  JustTriggeredOther=false;
  evt_num=0;
  nev=nEvents;

  if(Pi0Analysis && JPsiAnalysis) { std::cout<<"BOTH ANALYSIS ARE SET TO TRUE. EXITING"<<std::endl; exit; }


  std::cout<<"Deleting macros of form "<<macbase<<"*.mac"<<std::endl;
  Int_t touchmacro=system("touch " + macbase + "fake.mac");
  Int_t clearmacros=system("rm " + macbase + "*.mac");
  if(clearmacros) std::cout<<"Error in clearing macros" <<std::endl;


  ////////////Limits
  //Generation
  etamingen=2.5;
  etamaxgen=4.0;//Note these are technicaly rapdity not psudeorapdity. Negative means infinite
  ptmingen=2; //pt cut for JPsi happens before initialization
  ptmaxgen=-1; //Note negative means infinite

  //Cuts
  Emincuts=30; //pi0
  Emaxcuts=sqrts;
  ptmincuts=2;
  ptmaxcuts=sqrts;
  etamincuts=etamingen;
  etamaxcuts=etamaxgen;

  //  events_arr = new TObjArray();

  particles = (TClonesArray*) pythia->GetListOfParticles();
  events->Branch("event",&particles);

  ///////PYTHIA initialization
  pythia->SetMDCY(pythia->Pycomp(111),1,0); //make pi0 stable
  pythia->SetCKIN(7, etamingen); //Minimum allowed rapidity (note not psuedorapidity of scattering subsystem (look into this) to understand what it means by scattering subsystem
  pythia->SetCKIN(8, etamaxgen); //Maximum allowed rapidity


  //  pythia->Pytune(320); //BS perugia 0 tune i believe. Doesn't recognize it. My version of root doesn't have this function, but it may be useful later

  if(Pi0Analysis)
  {
    std::cout<<"Pi0 Analysis"<<std::endl;
    pythia->SetMSEL(0); //Turn off all processes
    //Hard QCD processes
    pythia->SetMSUB(11,1);
    pythia->SetMSUB(12,1);
    pythia->SetMSUB(13,1);
    pythia->SetMSUB(28,1);
    pythia->SetMSUB(53,1);
    pythia->SetMSUB(68,1);
  }
  else if(JPsiAnalysis)
  {
    std::cout<<"JPSI Analysis"<<std::endl;
    std::cout<<"Turning off processes"<<std::endl;
    pythia->SetMSEL(0); //Turn off minimum bias
    pythia->SetMSUB(86,1); //gg->JPsi + g
    pythia->SetMSUB(106,1); //gg=>JPsi + photon

    pythia->SetCKIN(3,ptmingen); // minimum pt, cut happens at generation
    pythia->SetCKIN(4,-1); //maximum pt cut; negative means infinite
    pythia->SetCKIN(5,ptmingen);//lower pt limit on processes that are singular in limit pt->0. This is in addition to ckin 3. Seems JPSI stuff is this
    //

    //  pythia->Pylist(12); //Prints out all decays availible. Useful for turning off decays you want
    //  Turning off all decays for JPsi except e+ e-
    pythia->SetMDME(859,1,0); //Turn off Jpsi->mu- mu+
    pythia->SetMDME(860,1,0); //Turn off Jspi->rndmflav rndmflavbar


  }
  else
  {
    std::cout<<"MINIMUM BIAS"<<std::endl;
    //Put default behavior here if you want something other than min bias
  }


  pythia->Initialize("CMS","p","p",sqrts);

  srand(time(NULL)); //seeds a random number generator based on the time
  pythia->SetMRPY(2,0);//BS Tells the program to  prepare for a new Random seed. NOTE: when I switched to non-decaying pi0's I kept getting the same events over and over, even with new MRPY(1), so I added this line
  pythia->SetMRPY(1,rand()%900000000);
  std::cout<<"PYTHIA MRPY(1) = "<<pythia->GetMRPY(1)<<std::endl;
  std::cout<<"THE NUMBER OF EVENTS TO GENERATE IS "<<nEvents<<std::endl;

}

EventLoop::~EventLoop()
{
  //delete any allocated memory 
  //
  if(pythia) delete pythia;
  if(events) delete events;

}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void EventLoop::horse_ramp() {

  TFile * horse_glue = TFile::Open("~/simulations/geant/fmsu/simgen/outgen/events.root","READ");
  TTree * fudd = (TTree*) horse_glue->Get("events");
  TMCParticle parttmp;

  int dummyEventNumber=1; //for use in the naming of macro files. iterates when match to conditions is found

  fudd->GetBranch("event.fKF")->SetAddress(&KF_arr); // make these arrays
  fudd->GetBranch("event.fVx")->SetAddress(&vx_arr);
  fudd->GetBranch("event.fVy")->SetAddress(&vy_arr);
  fudd->GetBranch("event.fVz")->SetAddress(&vz_arr);
  fudd->GetBranch("event.fPx")->SetAddress(&px_arr);
  fudd->GetBranch("event.fPy")->SetAddress(&py_arr);
  fudd->GetBranch("event.fPz")->SetAddress(&pz_arr);
  fudd->GetBranch("event.fEnergy")->SetAddress(&E_arr);
  fudd->GetBranch("event.fMass")->SetAddress(&m_arr);

  //fudd->GetBranch("event.fEta")->SetAddress(&eta_arr);
  //fudd->GetBranch("event.fPmag")->SetAddress(&pmag_arr);
  //fudd->GetBranch("event.fPt")->SetAddress(&pt_arr);
  fudd->GetBranch("event.fKS")->SetAddress(&KS_arr);
  //fudd->GetBranch("event.fKLineFirst")->SetAddress(&KLineFirst_arr);
  Bool_t keep=0;
  KLineFirst=0;
  
  if(!JPsiAnalysis)
    {
      
      evt_num++;
      outstring=gSystem->Getenv("SIMHOME");
      outstring+=Form("/geant/fmsu/macros/macro%d.mac",evt_num);
      myfile.open(outstring);
      if (myfile.is_open())
	{
	  myfile<<"/gps/source/multiplevertex true\n";
	  myfile<<"/gps/source/clear\n";
	}
    }

  for (Int_t j = 0; j < fudd->GetEntries(); j++)
    {
      fudd->GetEntry(j);
      px=px_arr[j];
      py=py_arr[j];
      pz=pz_arr[j];
      E=E_arr[j];
      m=m_arr[j];
      vx=vx_arr[j];
      vy=vx_arr[j];
      vz=vx_arr[j];
      KS=KS_arr[j];
      KF=KF_arr[j];
      
      pt=sqrt(pow(px,2)+pow(py,2));
      pmag=sqrt(pow(px,2)+pow(py,2)+pow(pz,2));
      eta=-log(tan(.5*(acos(pz/pmag))));
      
      //	if(j==pythia->GetNumberOfParticles()-1) std::cout<<"j last term E="<<E<<" and KF:KS="<<KF<<":"<<KS<<" and m="<<m<<std::endl;
      
      Double_t vmag=sqrt(pow(vx,2)+pow(vy,2)+pow(vz,2));
      
      if(eta<etamincuts || eta>etamaxcuts || (KS>10 && !(KF==443 && KS==11)) )
	{
	  particles->RemoveAt(j); //Remove Particles from list that are not in the range of the fms or are not final state particles (except for JPsi at time of decay)
	  continue;
	}
      else if(!JPsiAnalysis)
	{
	  






	  if(JustTriggeredPi0 && !(KF==111 && E>Emincuts && E<Emaxcuts && pt>ptmincuts && pt<ptmaxcuts ) )
	    {
	      continue;
	    }
	  else if(JustTriggeredOther && !(KF==211 && E>Emincuts && E<Emaxcuts && pt>ptmincuts && pt<ptmaxcuts ) ){continue;}
	  
	  else 
	    {
	      parttmp.SetKF(KF);
	      strcpy(PName,parttmp.GetName());
	      if(strcmp(PName,"n0") == 0) {strcpy(PName,"neutron");} //geant calls nuetron "nuetron" while pythia calls it "n0"
	      else if(strcmp(PName,"nbar0") == 0) {strcpy(PName,"anti_neutron");} 
	      else if(strcmp(PName,"K+") == 0) {strcpy(PName,"kaon+");}
	      else if(strcmp(PName,"K-") == 0) {strcpy(PName,"kaon-");}
	      else if(strcmp(PName,"K_L0") == 0) {strcpy(PName,"kaon+");}
	      else if(strcmp(PName,"p+") == 0) {strcpy(PName,"proton");} 
	      else if(strcmp(PName,"pbar-") == 0) {strcpy(PName,"anti_proton");}
	      else if(strcmp(PName,"pi0") == 0 || strcmp(PName,"pi-") ==0 || strcmp(PName, "pi+") == 0 || strcmp(PName, "gamma") == 0 ) {} // These names are already the same as geant's
	      else
		{
		  printf("Particle with name %s skipped. \n",PName);
		  continue;
		};
	      
	      //BS, may need to account for time differences here later. Note that PYTHIA distances are mm by default;
	      
	      if(myfile.is_open())
		{
		  myfile<<"/gps/source/add 1\n";
		  myfile<<Form("/gps/particle %s\n",PName);
		  myfile<<Form("/gps/energy %f GeV\n",E);
		  myfile<<Form("/gps/direction %f %f %F\n",px,py,pz);
		  myfile<<Form("/gps/position %f %f %f mm\n", vx, vy, vz-7000);
		}
	    }
	}
    }
  if(!JPsiAnalysis)
    {
      if(myfile.is_open())
	{
	  myfile<<"/run/beamOn 1\n";
	  myfile<<"exit";
	}
      myfile.close();
    }
  
  particles->Compress();
  // if(particles->GetEntriesFast()!=0)events->Fill();
  
  
  std::cout<<"Number of passed events="<<evt_num<<std::endl;
  return;
  
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void EventLoop::Start()
{
  TMCParticle parttmp;
  
  int dummyEventNumber=1; //for use in the naming of macro files. iterates when match to conditions is found
  for(Int_t i=0; i<nev; i++)
    {
      Bool_t keep=0;
      KLineFirst=0;
      
      
      pythia->GenerateEvent();
      if((i)%500==0 )
	{
	  printf("%d events generated\n",i+1);
	  std::cout<<"Cross section is "<< pythia->GetPARI(1)<<" mb"<<std::endl; //I belive in milibarns
	}
      
      
      for(Int_t k=0; k<pythia->GetNumberOfParticles(); k++)
	{
	  px=pythia->GetP(k+1,1);
	  py=pythia->GetP(k+1,2);
	  pz=pythia->GetP(k+1,3);
	  E=pythia->GetP(k+1,4);
	  m=pythia->GetP(k+1,5);
	  vx=pythia->GetV(k+1,1);
	  vy=pythia->GetV(k+1,2);
	  vz=pythia->GetV(k+1,3);
	  KS=pythia->GetK(k+1,1);
	  KF=pythia->GetK(k+1,2);
	  KLineFirst=pythia->GetK(k+1,4);
	  
	  pt=sqrt(pow(px,2)+pow(py,2));
	  pmag=sqrt(pow(px,2)+pow(py,2)+pow(pz,2));
	  eta=-log(tan(.5*(acos(pz/pmag))));
	  
	  
	  if(eta<etamincuts || eta>etamaxcuts) continue;
	  else if(JPsiAnalysis)
	    {
	      if( !(KF==443 && KS==11) ) continue; //Looking for decayed JPsi
	      Int_t KLineSecond=KLineFirst+1;
	      Double_t pxFirst=pythia->GetP(KLineFirst,1);
	      Double_t pyFirst=pythia->GetP(KLineFirst,2);
	      Double_t pzFirst=pythia->GetP(KLineFirst,3);
	      Double_t EFirst=pythia->GetP(KLineFirst,4);
	      Double_t mFirst=pythia->GetP(KLineFirst,5);
	      Double_t vxFirst=pythia->GetV(KLineFirst,1);
	      Double_t vyFirst=pythia->GetV(KLineFirst,2);
	      Double_t vzFirst=pythia->GetV(KLineFirst,3);
	      Int_t KSFirst=pythia->GetK(KLineFirst,1);
	      Int_t KFFirst=pythia->GetK(KLineFirst,2);
	      Double_t ptFirst=sqrt(pow(px,2)+pow(py,2));
	      Double_t pmagFirst=sqrt(pow(pxFirst,2)+pow(pyFirst,2)+pow(pzFirst,2));
	      Double_t etaFirst=-log(tan(.5*(acos(pzFirst/pmagFirst))));
	      
	      Double_t pxSecond=pythia->GetP(KLineSecond,1);
	      Double_t pySecond=pythia->GetP(KLineSecond,2);
	      Double_t pzSecond=pythia->GetP(KLineSecond,3);
	      Double_t ESecond=pythia->GetP(KLineSecond,4);
	      Double_t mSecond=pythia->GetP(KLineSecond,5);
	      Double_t vxSecond=pythia->GetV(KLineSecond,1);
	      Double_t vySecond=pythia->GetV(KLineSecond,2);
	      Double_t vzSecond=pythia->GetV(KLineSecond,3);
	      Int_t KSSecond=pythia->GetK(KLineSecond,1);
	      Int_t KFSecond=pythia->GetK(KLineSecond,2);
	      Double_t ptSecond=sqrt(pow(px,2)+pow(py,2));
	      Double_t pmagSecond=sqrt(pow(pxSecond,2)+pow(pySecond,2)+pow(pzSecond,2));
	      Double_t etaSecond=-log(tan(.5*(acos(pzSecond/pmagSecond))));
	      Double_t Z= fabs(EFirst-ESecond)/(EFirst+ESecond);
	      
	      if(etaFirst>=etamincuts && etaSecond>= etamincuts && etaFirst<=etamaxcuts && etaSecond<=etamaxcuts)
		{
		  outstring=gSystem->Getenv("SIMHOME");
		  evt_num++;
		  outstring+=Form("/geant/fmsu/macros/macro%d.mac",evt_num);
		  myfile.open(outstring);
		  
		  if(myfile.is_open())
		    {
		      myfile<<"/gps/source/multiplevertex true\n";
		      myfile<<"/gps/source/clear\n";
		      myfile<<"/gps/source/add 1\n";
		      if(KFFirst==11) myfile<<"/gps/particle e-\n";
		      else if(KFFirst==-11) myfile<<"/gps/particle e+\n";
		      else {myfile.close(); break;}
		      myfile<<Form("/gps/energy %f GeV\n",EFirst);
		      myfile<<Form("/gps/direction %f %f %f\n",pxFirst, pyFirst, pzFirst);
		      myfile<<Form("/gps/position %f %f %f mm\n", vxFirst, vyFirst, vzFirst-7000);
		      myfile<<"/gps/source/add 1\n";
		      if(KFSecond==11) myfile<<"/gps/particle e-\n";
		      else if(KFSecond==-11) myfile<<"/gps/particle e+\n";
		      else {myfile.close(); break;}
		      myfile<<Form("/gps/energy %f\n",ESecond);
		      myfile<<Form("/gps/direction %f %f %f\n",pxSecond, pySecond, pzSecond);
		      myfile<<Form("/gps/position %f %f %f mm\n", vxSecond, vySecond, vzSecond-7000);
		      myfile<<"/run/beamOn 1\n";
		      myfile<<"exit";
		      myfile.close();
		    }
		  keep=1;
		  break;
		}
	    }
	  else if(Pi0Analysis)
	    {
	      if(KF==111 && KS<10 && pt>ptmincuts && pt<ptmaxcuts && E>Emincuts && E<Emaxcuts)
		{
		  keep=1;//cuts on events here
		  break;//No need to test the rest of the particles
		}
	    }
	  else
	    {
	      //Minimum bias cuts
	      if(KF==211 && KS<10 && pt>ptmincuts && pt<ptmaxcuts && E>Emincuts && E<Emaxcuts) keep=1;
	    }
	}
      
      if(!keep)
	{
	  particles->Clear();
	  continue; //move on to next event
	}
      else
	{
	  if(!JPsiAnalysis)
	    {
	      
	      evt_num++;
	      outstring=gSystem->Getenv("SIMHOME");
	      outstring+=Form("/geant/fmsu/macros/macro%d.mac",evt_num);
	      myfile.open(outstring);
	      if (myfile.is_open())
		{
		  myfile<<"/gps/source/multiplevertex true\n";
		  myfile<<"/gps/source/clear\n";
		}
	    }
	  
	  for(Int_t j=0; j<pythia->GetNumberOfParticles();j++)
	    {
	      px=pythia->GetP(j+1,1);
	      py=pythia->GetP(j+1,2);
	      pz=pythia->GetP(j+1,3);
	      E=pythia->GetP(j+1,4);
	      m=pythia->GetP(j+1,5);
	      vx=pythia->GetV(j+1,1);
	      vy=pythia->GetV(j+1,2);
	      vz=pythia->GetV(j+1,3);
	      KS=pythia->GetK(j+1,1);
	      KF=pythia->GetK(j+1,2);
	      KLineFirst=pythia->GetK(j+1,4);
	      
	      pt=sqrt(pow(px,2)+pow(py,2));
	      pmag=sqrt(pow(px,2)+pow(py,2)+pow(pz,2));
	      eta=-log(tan(.5*(acos(pz/pmag))));
	      
	      //	if(j==pythia->GetNumberOfParticles()-1) std::cout<<"j last term E="<<E<<" and KF:KS="<<KF<<":"<<KS<<" and m="<<m<<std::endl;
	      
	      Double_t vmag=sqrt(pow(vx,2)+pow(vy,2)+pow(vz,2));
	      
	      if(eta<etamincuts || eta>etamaxcuts || (KS>10 && !(KF==443 && KS==11)) )
		{
		  particles->RemoveAt(j); //Remove Particles from list that are not in the range of the fms or are not final state particles (except for JPsi at time of decay)
		  continue;
		}
	      else if(!JPsiAnalysis)
		{
		  
		  if(JustTriggeredPi0 && !(KF==111 && E>Emincuts && E<Emaxcuts && pt>ptmincuts && pt<ptmaxcuts ) )
		    {
		      continue;
		    }
		  else if(JustTriggeredOther && !(KF==211 && E>Emincuts && E<Emaxcuts && pt>ptmincuts && pt<ptmaxcuts ) ){continue;}
		  
		  else 
		    {
		      parttmp.SetKF(KF);
		      strcpy(PName,parttmp.GetName());
		      if(strcmp(PName,"n0") == 0) {strcpy(PName,"neutron");} //geant calls nuetron "nuetron" while pythia calls it "n0"
		      else if(strcmp(PName,"nbar0") == 0) {strcpy(PName,"anti_neutron");} 
		      else if(strcmp(PName,"K+") == 0) {strcpy(PName,"kaon+");}
		      else if(strcmp(PName,"K-") == 0) {strcpy(PName,"kaon-");}
		      else if(strcmp(PName,"K_L0") == 0) {strcpy(PName,"kaon+");}
		      else if(strcmp(PName,"p+") == 0) {strcpy(PName,"proton");} 
		      else if(strcmp(PName,"pbar-") == 0) {strcpy(PName,"anti_proton");}
		      else if(strcmp(PName,"pi0") == 0 || strcmp(PName,"pi-") ==0 || strcmp(PName, "pi+") == 0 || strcmp(PName, "gamma") == 0 ) {} // These names are already the same as geant's
		      else
			{
			  printf("Particle with name %s skipped. \n",PName);
			  continue;
			};
		      
		      //BS, may need to account for time differences here later. Note that PYTHIA distances are mm by default;
		      
		      if(myfile.is_open())
			{
			  myfile<<"/gps/source/add 1\n";
			  myfile<<Form("/gps/particle %s\n",PName);
			  myfile<<Form("/gps/energy %f GeV\n",E);
			  myfile<<Form("/gps/direction %f %f %F\n",px,py,pz);
			  myfile<<Form("/gps/position %f %f %f mm\n", vx, vy, vz-7000);
			}
		    }
		}
	    }
	  if(!JPsiAnalysis)
	    {
	      if(myfile.is_open())
		{
		  myfile<<"/run/beamOn 1\n";
		  myfile<<"exit";
		}
	      myfile.close();
	    }
	  
	  particles->Compress();
	  if(particles->GetEntriesFast()!=0)events->Fill();
	}
    }
  std::cout<<"Number of passed events="<<evt_num<<std::endl;
  return;
}
 
