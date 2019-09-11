#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction * Detector):G4UImessenger(),
    fDetector(Detector), _src_conf_id_Cmd(0), _src_shiftX_Cmd(0), _src_shiftY_Cmd(0), _src_shiftZ_Cmd(0), _modify_Cmd(0)
{
    _src_conf_id_Cmd = new G4UIcmdWithAnInteger("/cpfm/src/SetSrcConfID",this);
    _src_conf_id_Cmd->SetGuidance("Set source configuration ID");
    _src_conf_id_Cmd->SetParameterName("ConfID",false);
    _src_conf_id_Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    _src_shiftX_Cmd = new G4UIcmdWithADoubleAndUnit("/cpfm/src/SetSrcShiftX",this);
    _src_shiftX_Cmd->SetGuidance("Set source shift along X axis");
    _src_shiftX_Cmd->SetParameterName("X_shift",false);
    _src_shiftX_Cmd->SetUnitCategory("Length");
    _src_shiftX_Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    _src_shiftY_Cmd = new G4UIcmdWithADoubleAndUnit("/cpfm/src/SetSrcShiftY",this);
    _src_shiftY_Cmd->SetGuidance("Set source shift along Y axis");
    _src_shiftY_Cmd->SetParameterName("Y_shift",false);
    _src_shiftY_Cmd->SetUnitCategory("Length");
    _src_shiftY_Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    _src_shiftZ_Cmd = new G4UIcmdWithADoubleAndUnit("/cpfm/src/SetSrcShiftZ",this);
    _src_shiftZ_Cmd->SetGuidance("Set source shift along Z axis");
    _src_shiftZ_Cmd->SetParameterName("Z_shift",false);
    _src_shiftZ_Cmd->SetUnitCategory("Length");
    _src_shiftZ_Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    _abs_width1_Cmd = new G4UIcmdWithADoubleAndUnit("/cpfm/abs/Width1",this);
    _abs_width1_Cmd->SetGuidance("Width of the absorber for bar 1");
    _abs_width1_Cmd->SetParameterName("Abs_width1",false);
    _abs_width1_Cmd->SetUnitCategory("Length");
    _abs_width1_Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    _abs_width2_Cmd = new G4UIcmdWithADoubleAndUnit("/cpfm/abs/Width2",this);
    _abs_width2_Cmd->SetGuidance("Width of the absorber for bar 2");
    _abs_width2_Cmd->SetParameterName("Abs_width2",false);
    _abs_width2_Cmd->SetUnitCategory("Length");
    _abs_width2_Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    _modify_Cmd = new G4UIcmdWithoutParameter("/cpfm/ModifyGeometry",this);
    _modify_Cmd->SetGuidance("Modify Geometry");
    _modify_Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

DetectorMessenger::~DetectorMessenger()
{
    delete _src_conf_id_Cmd;
    delete _src_shiftX_Cmd;
    delete _src_shiftY_Cmd;
    delete _src_shiftZ_Cmd;
    delete _abs_width1_Cmd;
    delete _abs_width2_Cmd;
    delete _modify_Cmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
    if( command == _src_conf_id_Cmd )
    {
        fDetector->SetSrcConfID((G4int)_src_conf_id_Cmd->GetNewIntValue(newValue));
    }
    if( command == _src_shiftX_Cmd )
    {
        fDetector->SetSrcShiftX((G4double)_src_shiftX_Cmd->GetNewDoubleValue(newValue));
    }
    if( command == _src_shiftY_Cmd )
    {
        fDetector->SetSrcShiftY((G4double)_src_shiftY_Cmd->GetNewDoubleValue(newValue));
    }
    if( command == _src_shiftZ_Cmd )
    {
        fDetector->SetSrcShiftZ((G4double)_src_shiftZ_Cmd->GetNewDoubleValue(newValue));
    }
    if( command == _abs_width1_Cmd )
    {
        fDetector->SetAbsWidth1((G4double)_abs_width1_Cmd->GetNewDoubleValue(newValue));
    }
    if( command == _abs_width2_Cmd )
    {
        fDetector->SetAbsWidth2((G4double)_abs_width2_Cmd->GetNewDoubleValue(newValue));
    }
    if( command == _modify_Cmd )
    {
        fDetector->ModifyGeometry();
    }
}
