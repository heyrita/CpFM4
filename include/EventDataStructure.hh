#ifndef EventDataStructure_h
#define EventDataStructure_h 1

#include "globals.hh"

struct EventData
{
  G4int    EventID;
  G4int    BunchXID;
  G4int    NTotPhot;
  G4int    Nhits;
  G4int    primType;
  G4double primMomX;
  G4double primMomY;
  G4double primMomZ;
  G4double primPosX;
  G4double primPosY;
  G4double primPosZ;
  G4double primTime;
  G4double trigTopL;
  G4double trigTopEdep;
  G4double trigBotL;
  G4double trigBotEdep;
};

#endif
