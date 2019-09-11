#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;

class DetectorMessenger: public G4UImessenger
{
  public:
  
    DetectorMessenger(DetectorConstruction*);
   ~DetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
  
    DetectorConstruction*       fDetector;
    G4UIcmdWithAnInteger*       _src_conf_id_Cmd;
    G4UIcmdWithADoubleAndUnit*  _src_shiftX_Cmd;
    G4UIcmdWithADoubleAndUnit*  _src_shiftY_Cmd;
    G4UIcmdWithADoubleAndUnit*  _src_shiftZ_Cmd;
    G4UIcmdWithADoubleAndUnit*  _abs_width1_Cmd;
    G4UIcmdWithADoubleAndUnit*  _abs_width2_Cmd;
    G4UIcmdWithoutParameter*    _modify_Cmd;
};

#endif

