#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"

#include "globals.hh"

#ifdef G4UI_USE_TCSH
#include "G4UItcsh.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"
#include "TrackingAction.hh"
//#include "fTOF_UImessenger.hh"
#include "SteppingVerbose.hh"

#include "Randomize.hh"

#include "TString.h"

using namespace CLHEP;

int main(int argc, char** argv)
{

  // Choose the Random Engine
  //  HepRandom::setTheEngine(new RanecuEngine);
  // Seed the random number generator manually
  //

  if(argc!=4){
    G4cout<<" ERROR of the input parameters !!! "<<G4endl
      <<"      [0] - vis.mac or run.mac"<<G4endl
	  <<"      [1] - seed "<<G4endl
      <<"      [2] - output root file name"<<G4endl;
    return 0;    
  }
  else{
    G4cout<<"  Input parameters         "<<G4endl
	  <<"     mac file              "<<argv[1]<<G4endl
	  <<"     seed                  "<<argv[2]<<G4endl
      <<"     output root file name "<<argv[3]<<G4endl;
  }


  G4long myseed = 345354;
  myseed = atoi(argv[2]);
  G4cout<<" myseed - "<<myseed<<G4endl;

  CLHEP::HepRandom::setTheSeed(myseed);

  // Verbose output class
  G4VSteppingVerbose::SetInstance(new SteppingVerbose);

  // Construct the default run manager
  G4RunManager* runManager = new G4RunManager;

  // Construct mandatory initialization classes
  G4VUserDetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);
  G4VUserPhysicsList* physics = new PhysicsList;
  runManager->SetUserInitialization(physics);

  // Construct User Action Classes
  RunAction* runAction = new RunAction;
  runManager->SetUserAction(runAction);
  //
  G4String rootFileName = argv[3];
  G4cout<<rootFileName<<G4endl;
  runAction->SetOutputFileName(rootFileName);

  PrimaryGeneratorAction* genAction = 
    new PrimaryGeneratorAction();
  runManager->SetUserAction(genAction);

  SteppingAction* steppingAction = new SteppingAction(runAction,genAction);
  runManager->SetUserAction(steppingAction);

  EventAction* eventAction = new EventAction(runAction,steppingAction);
  runManager->SetUserAction(eventAction);
  StackingAction *stackingAction = new StackingAction;
  runManager->SetUserAction(stackingAction);
  runManager->SetUserAction(new TrackingAction);

  eventAction->SetStackingAction(stackingAction);
  eventAction->SetPrimGenerator(genAction);
  // Setup to be able to define some custom commands;
  //fTOF_UImessenger* messenger = new fTOF_UImessenger(runAction, genAction);

  // Initialize G4 kernel
  runManager->Initialize();

  // Get the pointer to the UI manager and set verbosities
  G4UImanager* UI = G4UImanager::GetUIpointer();
  if (argc == 1) {  // Define UI terminal for interactive mode
    UI->ApplyCommand("/run/verbose 1");
    UI->ApplyCommand("/event/verbose 1");
    UI->ApplyCommand("/tracking/verbose 1");
#if defined(G4UI_USE_TCSH)
    G4UIsession* session = new G4UIterminal(new G4UItcsh);
#else
    G4UIsession* session = new G4UIterminal;
#endif
    session->SessionStart();
    delete session;
  }
  else {

    G4String fileName = argv[1];
    if(fileName.contains("vis")){
      G4cout<<"VIS fileName "<<fileName<<G4endl;
#ifdef G4VIS_USE
      // Visualization manager
      G4VisManager* visManager = new G4VisExecutive;
      visManager->Initialize();
#endif    
      
      G4String command = "/control/execute ";
      UI->ApplyCommand(command+fileName);
      
#ifdef G4VIS_USE
      delete visManager;
#endif
    }
    else{
      G4cout<<"NO VIS fileName "<<fileName<<G4endl;
      G4String command = "/control/execute ";
      UI->ApplyCommand(command+fileName);    
    }

  }

  

  // Start a run
  //  G4int numberOfEvent = 3;
  //  runManager->BeamOn(numberOfEvent);

  // Job termination
  //
  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !
  //delete messenger;
  //delete visManager;
  delete runManager;
  
  return 0;
}


