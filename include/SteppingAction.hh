#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingMessenger.hh"
#include "RunAction.hh"
#include "globals.hh"

#include "TString.h"

#include <iostream>
#include <fstream>
using namespace std;

class EventAction;
class RunAction;

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(RunAction*, PrimaryGeneratorAction*);
  ~SteppingAction();

  void UserSteppingAction(const G4Step*);

  void Reset();
  void ResetPerEvent();

  G4double  EnergyTop, EnergyBot;
  G4double  TrackLTop, TrackLBot;

  void SetProb1(G4double);
  void SetProb2(G4double);
  G4double GetProb1()   {return _probOfReflection1;}
  G4double GetProb2()   {return _probOfReflection2;}

  static const G4int _nPhotMax = 20000;

private:
  void InternalReflectionProbability(G4double, G4double&);
  void InternalReflectionProbability(G4double, G4double&, G4int);
  PrimaryGeneratorAction* _genAction;
  SteppingMessenger* fSteppingMessenger;

private:

  EventAction*  eventaction;
  RunAction*    runAction;
  //Trk information  
  G4int _particleID;
  G4int _SecID;
  G4double probOfReflection;
  G4double _probOfReflection1;
  G4double _probOfReflection2;
  G4double _trkMomX;
  G4double _trkMomY;
  G4double _trkMomZ;
  G4double _trkPosX;
  G4double _trkPosY;
  G4double _trkPosZ;
  G4double _trkT;
  G4double _trkLength;
  G4double _trkCharge;
  G4int _reflectionNum[_nPhotMax];
  G4int _reflectionNum1[_nPhotMax];
  G4int _reflectionNum2[_nPhotMax];

  //photon information
  G4int _chID;
  //G4int nKillPhot;
  //G4double  _chX;
  //G4double  _chY;
  //G4double  _chZ;

  // G4int _particleID;
  //G4double _totalPath;
  //G4double _barPath;
  //G4double _sobPath;
  //G4int _nBounceMirror1;
  //G4int _nBounceMirror2;
  //G4int _nBounceEndMirror;
  //G4int _nWedgeSide;
  //G4int _nWedgeTop;
  //G4int _nWedgeBottom;
  //G4int _nFBlockSide;
  //G4int _bar;
  //G4int _barFirst;
  //G4double _barX;
  //G4double _barY;
  //G4double _barZ;
  //G4double _barThetaX;
  //G4double _barThetaY;
  //G4double _timeLeftBar;
  //G4double _probOfReflection;
  //G4int _numberOfBounces;

  ofstream myfileB;

};

#endif
