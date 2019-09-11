//My
#include "StackingAction.hh"

//G4
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"


StackingAction::StackingAction() : gammaCounter(0)
{}

StackingAction::~StackingAction()
{}

G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
  if (aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()
      && aTrack->GetParentID() > 0) {
    gammaCounter++;
    // Get direction
    //    const G4ThreeVector& momDir(aTrack->GetMomentumDirection());
  }

  return fUrgent;
}

void StackingAction::NewStage()
{
  //G4cout << "Number of optical photons in this event : " << gammaCounter
  //<< G4endl;
}

void StackingAction::PrepareNewEvent()
{
  gammaCounter = 0;
}
