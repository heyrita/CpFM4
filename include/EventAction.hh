#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "DetectorConstruction.hh"
#include "SteppingAction.hh"

class G4Event;
class RunAction;
class SteppingAction;
class StackingAction;
class PrimaryGeneratorAction;
class DetectorConstruction;

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction*, SteppingAction*);
  ~EventAction();

public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

  void SetStackingAction(StackingAction* sta){_stackingAction = sta;}
  void SetPrimGenerator(PrimaryGeneratorAction *gen){_primGenerator = gen;}

  //void AddTrigTop(G4double de, G4double dl) {EnergyTop += de; TrackLTop += dl;};
  //void AddTrigBot(G4double de, G4double dl) {EnergyBot += de; TrackLBot += dl;};

private:

  RunAction* runAction;
  SteppingAction* _steppingAction;
  G4int printModulo;
  G4int thePhotonCollectionID;
  
  StackingAction* _stackingAction; 
  PrimaryGeneratorAction* _primGenerator;
  DetectorConstruction* _detector;
};
#endif
