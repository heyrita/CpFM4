#ifndef SteppingMessenger_h
#define SteppingMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class SteppingAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADouble;

class SteppingMessenger: public G4UImessenger
{
  public:
  
    SteppingMessenger(SteppingAction*);
   ~SteppingMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
    SteppingAction*             fStepping;
    G4UIcmdWithADouble*  _prob1_Cmd;
    G4UIcmdWithADouble*  _prob2_Cmd;
};

#endif

