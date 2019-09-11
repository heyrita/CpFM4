#include "TrackingAction.hh"
#include "TrackInformation.hh"
#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "G4TrackingManager.hh"

TrackingAction::TrackingAction()
{;}

TrackingAction::~TrackingAction()
{;}

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  if (aTrack->GetParentID() == 0 && aTrack->GetUserInformation() == 0) {
    TrackInformation* anInfo = new TrackInformation(aTrack);
    G4Track* theTrack = (G4Track*)aTrack;
    theTrack->SetUserInformation(anInfo);
  }
}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  if (secondaries) {
    TrackInformation* info = 
      (TrackInformation*)(aTrack->GetUserInformation());
    size_t nSeco = secondaries->size();
    if (nSeco > 0) {
      for (size_t i = 0; i < nSeco; i++) {
	TrackInformation* infoNew = new TrackInformation(info);
	infoNew->SetMyOriginalPosition((*secondaries)[i]->GetPosition());
	infoNew->SetMyOriginalMomentum((*secondaries)[i]->GetMomentum());
	infoNew->SetMyOriginalEnergy((*secondaries)[i]->GetTotalEnergy());
	infoNew->SetMyOriginalTime((*secondaries)[i]->GetGlobalTime());
	(*secondaries)[i]->SetUserInformation(infoNew);
      }
    }
  }
}
