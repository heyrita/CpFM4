#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "HitMy.hh"
#include "HitDataStructure.hh"

class G4HCofThisEvent;
class G4Step;
class G4TouchableHistory;

class SensitiveDetector : public G4VSensitiveDetector
{
public:
  SensitiveDetector(G4String);
  ~SensitiveDetector();

  void Initialize(G4HCofThisEvent*);
  
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  G4bool ProcessHits_fTOF(const G4Step*, G4TouchableHistory*, HitData hitInfo);

  void EndOfEvent(G4HCofThisEvent*);
private:

  HitsCollection* OpticalPhotonCollection;

};

#endif
