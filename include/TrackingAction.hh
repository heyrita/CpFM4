#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class TrackingAction : public G4UserTrackingAction
{
public:
  TrackingAction();
  ~TrackingAction();

  void PreUserTrackingAction(const G4Track*);
  void PostUserTrackingAction(const G4Track*);

private:

};
#endif
