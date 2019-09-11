//My
#include "RunAction.hh"

//G4
#include "G4Run.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"

//root
#include "TFile.h"
#include "TTree.h"

RunAction::RunAction() : timer(0), _outputFileName("UA9.root")
  //  tree(0),
  //  hfile(0)
{
  timer = new G4Timer;  
}

RunAction::~RunAction()
{
  delete timer;
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  timer->Start();

  // Histogramming
  hfile = new TFile(_outputFileName, "RECREATE", "UA9 Simulation Data", 1);
  if (hfile->IsZombie()) exit(-1);
  tree = new TTree("T", "UA9 Data Tree");
  
  hfile->SetCompressionLevel(2);
  tree->SetAutoSave(1000000);

  // Create new event
  TTree::SetBranchStyle(0);
  //Event
  tree->Branch("EventID",  &EventInfo.EventID,  "Event/I");
  tree->Branch("BunchXID", &EventInfo.BunchXID, "BunchXID/I");
  tree->Branch("NTotPhot", &EventInfo.NTotPhot, "NTotPhot/I");
  tree->Branch("Nhits",    &EventInfo.Nhits,    "Nhits/I");
  tree->Branch("primType", &EventInfo.primType, "primType/I");
  tree->Branch("primMomX", &EventInfo.primMomX, "primMomX/D");
  tree->Branch("primMomY", &EventInfo.primMomY, "primMomY/D");
  tree->Branch("primMomZ", &EventInfo.primMomZ, "primMomZ/D");
  tree->Branch("primPosX", &EventInfo.primPosX, "primPosX/D");
  tree->Branch("primPosY", &EventInfo.primPosY, "primPosY/D");
  tree->Branch("primPosZ", &EventInfo.primPosZ, "primPosZ/D");
  tree->Branch("primTime", &EventInfo.primTime, "primTime/D");
  tree->Branch("trigTopL",   &EventInfo.trigTopL,    "trigTopL/D");
  tree->Branch("trigTopEdep",&EventInfo.trigTopEdep, "trigTopEdep/D");
  tree->Branch("trigBotL",   &EventInfo.trigBotL,    "trigBotL/D");
  tree->Branch("trigBotEdep",&EventInfo.trigBotEdep, "trigBotEdep/D");

  //Hits

  //tree->Branch("TrackID",    &HitInfo.TrackID,    "TrackID/I");
  //tree->Branch("ParentID",   &HitInfo.ParentID,   "ParentID/I");
  //tree->Branch("Energy",     &HitInfo.Energy,     "Energy/D");
  //tree->Branch("Wavelength", &HitInfo.Wavelength, "Wavelength/D");
  //tree->Branch("Time",       &HitInfo.Time,       "Time/D");
  //tree->Branch("photPathLen",&HitInfo.photPathLen,"photPathLen/D");
  //tree->Branch("SecID",      &HitInfo.SecID,      "SecID/I");
  //tree->Branch("chID",       &HitInfo.chID,       "chID/I");
  //tree->Branch("PosX",       &HitInfo.PosX,       "PosX/D");
  //tree->Branch("PosY",       &HitInfo.PosY,       "PosY/D");
  //tree->Branch("PosZ",       &HitInfo.PosZ,       "PosZ/D");
  //tree->Branch("trkMomX",    &HitInfo.trkMomX,    "trkMomX/D");
  //tree->Branch("trkMomY",    &HitInfo.trkMomY,    "trkMomY/D");
  //tree->Branch("trkMomZ",    &HitInfo.trkMomZ,    "trkMomZ/D");
  //tree->Branch("trkPosX",    &HitInfo.trkPosX,    "trkPosX/D");
  //tree->Branch("trkPosY",    &HitInfo.trkPosY,    "trkPosY/D");
  //tree->Branch("trkPosZ",    &HitInfo.trkPosZ,    "trkPosZ/D");
  //tree->Branch("trkT",       &HitInfo.trkT,       "trkT/D");
  //tree->Branch("trkLength",  &HitInfo.trkLength,  "trkLength/D");

  tree->Branch("nPhot", &_nPhot, "nPhot/I");
  tree->Branch("TrackID", _TrackID, "TrackID[nPhot]/I");
  tree->Branch("ParentID", _ParentID, "ParentID[nPhot]/I");
  tree->Branch("Energy", _Energy, "Energy[nPhot]/D");
  tree->Branch("Wavelength", _Wavelength, "Wavelength[nPhot]/D");
  tree->Branch("GlobalTime", _GlobalTime, "GlobalTime[nPhot]/D");
  tree->Branch("LocalTime", _LocalTime, "LocalTime[nPhot]/D");
  tree->Branch("photPathLen", _photPathLen,"photPathLen[nPhot]/D");
  tree->Branch("SecID",_SecID, "SecID[nPhot]/I");
  tree->Branch("chID", _chID, "chID[nPhot]/I");
  tree->Branch("PosX", _PosX, "PosX[nPhot]/D");
  tree->Branch("PosY", _PosY, "PosY[nPhot]/D");
  tree->Branch("PosZ", _PosZ, "PosZ[nPhot]/D");
  tree->Branch("MomDirX", _MomDirX, "MomDirX[nPhot]/D");
  tree->Branch("MomDirY", _MomDirY, "MomDirY[nPhot]/D");
  tree->Branch("MomDirZ", _MomDirZ, "MomDirZ[nPhot]/D");
  tree->Branch("MomX", _MomX, "MomX[nPhot]/D");
  tree->Branch("MomY", _MomY, "MomY[nPhot]/D");
  tree->Branch("MomZ", _MomZ, "MomZ[nPhot]/D");
  tree->Branch("trkMomX", _trkMomX, "trkMomX[nPhot]/D");
  tree->Branch("trkMomY", _trkMomY, "trkMomY[nPhot]/D");
  tree->Branch("trkMomZ", _trkMomZ, "trkMomZ[nPhot]/D");
  tree->Branch("trkPosX", _trkPosX, "trkPosX[nPhot]/D");
  tree->Branch("trkPosY", _trkPosY, "trkPosY[nPhot]/D");
  tree->Branch("trkPosZ", _trkPosZ, "trkPosZ[nPhot]/D");
  tree->Branch("trkT", _trkT, "trkT[nPhot]/D");
  tree->Branch("trkLength",  _trkLength, "trkLength[nPhot]/D");
  tree->Branch("trkCharge",  _trkCharge, "trkCharge[nPhot]/D");

  tree->Branch("src_confID",  &_src_confID, "src_confID/I");
  tree->Branch("src_posX",  &_src_posX, "src_posX/D");
  tree->Branch("src_posY",  &_src_posY, "src_posY/D");
  tree->Branch("src_posZ",  &_src_posZ, "src_posZ/D");
  tree->Branch("abs_width1",  &_abs_width1, "abs_width1/D");
  tree->Branch("abs_width2",  &_abs_width2, "abs_width2/D");
  tree->Branch("prob1",  &_prob1, "prob1/D");
  tree->Branch("prob2",  &_prob2, "prob2/D");

  tree->Branch("reflectionNum", _reflectionNum, "reflectionNum[nPhot]/I");
  tree->Branch("reflectionNum1", _reflectionNum1, "reflectionNum1[nPhot]/I");
  tree->Branch("reflectionNum2", _reflectionNum2, "reflectionNum2[nPhot]/I");
}

void RunAction::EndOfRunAction(const G4Run*)
{

  if (G4VVisManager::GetConcreteInstance()) {
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
  }

  hfile = tree->GetCurrentFile();
  hfile->Write();
  //tree->Print();
  timer->Stop();

  delete tree;
  delete hfile;
  G4cout << "Time: " << *timer << G4endl;
}
