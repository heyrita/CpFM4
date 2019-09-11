#ifndef HitDataStructure_h
#define HitDataStructure_h 1

#include "globals.hh"

struct HitData
{
  G4int TrackID;
  G4int ParentID;
  G4double Energy;
  G4double Wavelength;
  G4double GlobalTime;
  G4double LocalTime;
  G4double photPathLen;
  G4int SecID;
  G4int chID;
  G4double PosX;
  G4double PosY;
  G4double PosZ;
  G4double MomX;
  G4double MomY;
  G4double MomZ;
  G4double MomDirX;
  G4double MomDirY;
  G4double MomDirZ;
  G4double trkMomX;
  G4double trkMomY;
  G4double trkMomZ;
  G4double trkPosX;
  G4double trkPosY;
  G4double trkPosZ;
  G4double trkT;
  G4double trkLength;
  G4double trkCharge;
  G4double reflectionNum;
  G4double reflectionNum1;
  G4double reflectionNum2;
};

#endif
