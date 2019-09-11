#ifndef StackingAction_h
#define StackingAction_h 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class StackingAction : public G4UserStackingAction
{
public:
  StackingAction();
  ~StackingAction();
  
public:
  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  void NewStage();
  void PrepareNewEvent();
  inline G4int GetTotPhotNum(){return gammaCounter;}

private:
  G4int gammaCounter;
};
#endif
