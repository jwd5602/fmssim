#include <stdlib.h>
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"
#include "G4ios.hh"

#include "CellGeo.hh"

#include "StackingAction.hh"

#include "SteppingAction.hh"
#include "SteppingVerbose.hh"
#include "StepTree.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "stdio.h"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif
#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include "Qt.h"
#include "FilesSet.h"
//#include "/home/demaio/simulations/geant/fmsu/myRootClass/G4QT.h"


////////BS
#include <stdio.h>
#include <stdlib.h>

#include "TCanvas.h"
#include "CellParameterisation.hh"
#include "Randomize.hh"
#include "time.h"



FILE* fp;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());

  G4long seed = time(NULL);
  CLHEP::HepRandom::setTheSeed(seed);


////////////////////////////////////////
// I/O INFORMATION
  TFile* pFile=0; 
  // Not sure what point of if !fp part is. fopen will automatically create the file if it is not there already. Not sure how fp can = 0 /Branden
  G4cout<<"argc="<<argc<<G4endl;
  TString gStep;
  if(argc>2)  //You have declared an output file
  {
    G4cout<<argv[2]<<G4endl;
    fp=fopen(argv[2],"w");
    TString ggFile=argv[2];
    ggFile=ggFile+".root";
    gStep=argv[2];
    gStep=gStep+"_step.root";
    pFile=new TFile(ggFile,"RECREATE");
    if(!fp)
    {
      G4Exception("tmp file name cannot be opened","NotApplicable",
        FatalException,"Not Applicable");
    };
  }
  else
  {
    fp=fopen("tmp.txt","w");
    pFile=new TFile("gFile.root","recreate");
    gStep="gFile_step.root";
  };

  Int_t eventNumber=1;
  if(argc>2)
  {
    TString dumString=argv[2];
    if( dumString.BeginsWith("gFile") )
    {
      TString dumStringShort=dumString.Replace(0,5,"");
      if( dumString.BeginsWith("s/gFile") ) {dumStringShort=dumString.Replace(0,7,"");}
      if ( dumStringShort.IsFloat() )
      {
	eventNumber= dumStringShort.Atoi(); //BS, way to make event numbers for jobs run in large parrell batchs. Extracts number in name of file
      }
    }
  }

  std::cout<<"THE EVENT NUMBER IS "<<eventNumber<<std::endl;



//////////////////////////////////////////
// INITIALIZING ALL OUR GEANT CLASSES

  // User Verbose output class
  //
  //t G4VSteppingVerbose* verbosity = new SteppingVerbose;
  //t G4VSteppingVerbose::SetInstance(verbosity);

  // Run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // User Initialization classes (mandatory)
  //
  DetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);
  //
  //Note only those particles that are listed in PhysicsList.cc will be included, attempting to call otherwise will result in fatal error
  G4VUserPhysicsList* physics = new PhysicsList;
  runManager->SetUserInitialization(physics);

  runManager->SetUserAction(new PrimaryGeneratorAction(detector));  //Create particle gun and initilize default values
  runManager->SetUserAction(new RunAction);  
  pFile->cd();

  TTree *geotr = new TTree("geotr","geotr");
  EventAction* evact=  new EventAction(fp,pFile,geotr,eventNumber); //Creates the Postsim class and sets branchs on the geotr. In general the Postsim is responsible for most output (except coordtr). Postsim also creates an instance of Qt.

  runManager->SetUserAction(evact);

  TrackingAction *trackingaction = new TrackingAction;
  StackingAction * stackaction = new StackingAction(); //coordtr created here
  runManager->SetUserAction(stackaction);
  runManager->SetUserAction(trackingaction);
  runManager->SetUserAction(new SteppingAction); //Creates a ttree called sttr, with position, momentum, and number of reflections
  TTree * coordtr = stackaction->GetCoordTr();


///////////////////////////////////
// Initialize G4 kernel, creates the detector and physics processes, preps for run
  
  runManager->Initialize(); // Calls Construct method of DetectorConstruction. Physics list is more complicated, calling G4VUserPhysicsLists as well as it's manager.

////////////////////////////////////////////////

//Visualization, use if you want to picture the dector
//BS. NOTE: Currently I'm having issues with this. I think that Heprap visualization could work, however it has issues with OpenJDK. May neeed to use JAVA Oracle JDK

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  G4cout<<"VISUALIZATION MANGAGER INITIALIZING"<<G4endl;
  visManager->Initialize();
#endif    

  // Get the pointer to the User Interface manager
  //
  G4UImanager * UImanager = G4UImanager::GetUIpointer();  

  if (argc!=1)   // batch mode  
  {
    G4String fileName = argv[1];
    if(fileName == "/home/demaio/simulations/fmsu/simgen/outgen/pythiaout.root")
    {

    }

    else
    {
    G4String command = "/control/execute ";
    G4cout << "command=" << command << " fileName=" << fileName << G4endl;
    UImanager->ApplyCommand(command+fileName);
    }
  }
  else           // interactive mode : define UI session
  { 
#ifdef G4UI_USE
G4cout<<"G4UI_USE IS DEFINED"<<G4endl;
    G4UIExecutive * ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute vis.mac");     
#endif
    ui->SessionStart();
    delete ui;
#endif

#ifdef G4VIS_USE
    delete visManager;
#endif     
  }


  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !
  //
  //delete verbosity;

  printf("writing data...\n");
  //gtrl->Write();
  //gtrs->Write();
  TCanvas *c2 = new TCanvas("c2");
//  trackingaction->startphotonZ->Draw();
//  c2->Print("Testing.pdf");
  evact->testtree->Write();
//  evact->PhotonWavelengthS->Write();
//  evact->PhotonWavelengthL->Write();
 // evact->OrigPhotonWavelengthS->Write();
 // evact->OrigPhotonWavelengthL->Write();
//  evact->EnergyDepositShapeS->Write();
//  evact->EnergyDepositShapeL->Write();

// ( (CellParameterisation*) (detector->CellParamL) )->CellEtaL->Write();

  //fclose(fp);
  delete runManager;
  pFile->Close();

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
