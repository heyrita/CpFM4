#include "SteppingMessenger.hh"
#include "SteppingAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"

SteppingMessenger::SteppingMessenger(SteppingAction * Action):G4UImessenger(), fStepping(Action)
{
    _prob1_Cmd = new G4UIcmdWithADouble("/cpfm/step/Prob1",this);
    _prob1_Cmd->SetGuidance("Probability for bar 1");
    _prob1_Cmd->SetParameterName("Prob1",false);

    _prob2_Cmd = new G4UIcmdWithADouble("/cpfm/step/Prob2",this);
    _prob2_Cmd->SetGuidance("Probability for bar 2");
    _prob2_Cmd->SetParameterName("Prob2",false);
}

SteppingMessenger::~SteppingMessenger()
{
    delete _prob1_Cmd;
    delete _prob2_Cmd;    
}

void SteppingMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    if( command == _prob1_Cmd )
    {
        fStepping->SetProb1((G4double)_prob1_Cmd->GetNewDoubleValue(newValue));
    }
    if( command == _prob2_Cmd )
    {
        fStepping->SetProb2((G4double)_prob2_Cmd->GetNewDoubleValue(newValue));
    }
}
