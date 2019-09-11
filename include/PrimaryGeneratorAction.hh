#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4String.hh"

class G4ParticleGun;
class G4Event;
class DetectorConstruction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction();

private:
  G4int _src_conf_id;
  G4double _src_shiftX;
  G4double _src_shiftY;
  G4double _src_shiftZ;

  void GeneratePrimaries(G4Event* anEvent);

private:
  G4ParticleGun*        _particleGun;
  DetectorConstruction* _detector;
};

#endif
