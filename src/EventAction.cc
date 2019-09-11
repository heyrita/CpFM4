//My
#include "EventAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "HitMy.hh"
#include "StackingAction.hh"
#include "UA9Const.hh"
#include "G4RunManager.hh"

//G4
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "globals.hh"
#include "G4UnitsTable.hh"

//ROOT
#include "TROOT.h"
#include "TMath.h"

EventAction::EventAction(RunAction* runact, SteppingAction* steppingAction) :  runAction(runact), _steppingAction(steppingAction), printModulo(100000)
{
    thePhotonCollectionID = -1;
    _steppingAction->EnergyTop = 0.0;
    _steppingAction->EnergyBot = 0.0;
    _steppingAction->TrackLTop = 0.0;
    _steppingAction->TrackLBot = 0.0;

    _detector = (DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* event)
{
    // Print number of events
    G4int eventNum = event->GetEventID();

    if (eventNum%printModulo == 0)
    {
        G4cout << "\n---> Begin of Event: " << eventNum << G4endl;
    }

    if (thePhotonCollectionID < 0)
    {
        G4String colName;
        thePhotonCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(colName = "OpticalPhotonCollection");
    }

    _steppingAction->Reset();
    _steppingAction->ResetPerEvent();

    // initialisation per event
    _steppingAction->EnergyTop = 0.0;
    _steppingAction->EnergyBot = 0.0;
    _steppingAction->TrackLTop = 0.0;
    _steppingAction->TrackLBot = 0.0;
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    // Print info about end of the event
    G4int eventNum = event->GetEventID();

    if (thePhotonCollectionID < 0) return;

    // Get the Hit Collection
    G4HCofThisEvent* HCE = event->GetHCofThisEvent();
    HitsCollection * THC = 0;

    if (HCE) THC = (HitsCollection*)(HCE->GetHC(thePhotonCollectionID));

    if (0 == THC) return;

    G4int nTotPhot = _stackingAction->GetTotPhotNum();
    G4int nHit = -1;
    nHit = THC->entries();

    runAction->EventInfo.EventID = eventNum;
    runAction->EventInfo.NTotPhot = nTotPhot;
    runAction->EventInfo.Nhits = nHit;
    runAction->EventInfo.primType = event->GetPrimaryVertex()->GetPrimary()->GetPDGcode();
    //G4cout<<"primType = "<<event->GetPrimaryVertex()->GetPrimary()->GetPDGcode()<<G4endl;
    runAction->EventInfo.primMomX = event->GetPrimaryVertex()->GetPrimary()->GetPx();
    runAction->EventInfo.primMomY = event->GetPrimaryVertex()->GetPrimary()->GetPy();
    runAction->EventInfo.primMomZ = event->GetPrimaryVertex()->GetPrimary()->GetPz();
    runAction->EventInfo.primPosX = event->GetPrimaryVertex()->GetX0();
    runAction->EventInfo.primPosY = event->GetPrimaryVertex()->GetY0();
    runAction->EventInfo.primPosZ = event->GetPrimaryVertex()->GetZ0();
    runAction->EventInfo.primTime = event->GetPrimaryVertex()->GetT0();
    runAction->EventInfo.trigTopL = _steppingAction->TrackLTop;
    runAction->EventInfo.trigTopEdep = _steppingAction->EnergyTop;
    runAction->EventInfo.trigBotL =_steppingAction->TrackLBot;
    runAction->EventInfo.trigBotEdep = _steppingAction->EnergyBot;

    runAction->_nPhot = nHit;
    for (G4int i = 0; i < nHit; i++)
    {
        runAction->_TrackID[i] = (*THC)[i]->myData.TrackID;
        runAction->_ParentID[i] = (*THC)[i]->myData.ParentID;
        runAction->_Energy[i] = (*THC)[i]->myData.Energy;
        runAction->_Wavelength[i] = (*THC)[i]->myData.Wavelength;
        runAction->_GlobalTime[i] = (*THC)[i]->myData.GlobalTime;
        runAction->_LocalTime[i] = (*THC)[i]->myData.LocalTime;
        runAction->_photPathLen[i] = (*THC)[i]->myData.photPathLen;
        runAction->_SecID[i] = (*THC)[i]->myData.SecID;
        runAction->_chID[i] = (*THC)[i]->myData.chID;
        runAction->_PosX[i] = (*THC)[i]->myData.PosX;
        runAction->_PosY[i] = (*THC)[i]->myData.PosY;
        runAction->_PosZ[i] = (*THC)[i]->myData.PosZ;
        runAction->_MomDirX[i] = TMath::ACos((*THC)[i]->myData.MomDirX)*180.0/TMath::Pi();
        runAction->_MomDirY[i] = TMath::ACos((*THC)[i]->myData.MomDirY)*180.0/TMath::Pi();
        runAction->_MomDirZ[i] = 180.0 - TMath::ACos((*THC)[i]->myData.MomDirZ)*180.0/TMath::Pi();
        runAction->_MomX[i] = (*THC)[i]->myData.MomX;
        runAction->_MomY[i] = (*THC)[i]->myData.MomY;
        runAction->_MomZ[i] = (*THC)[i]->myData.MomZ;
        runAction->_trkMomX[i] = (*THC)[i]->myData.trkMomX;
        runAction->_trkMomY[i] = (*THC)[i]->myData.trkMomY;
        runAction->_trkMomZ[i] = (*THC)[i]->myData.trkMomZ;
        runAction->_trkPosX[i] = (*THC)[i]->myData.trkPosX;
        runAction->_trkPosY[i] = (*THC)[i]->myData.trkPosY;
        runAction->_trkPosZ[i] = (*THC)[i]->myData.trkPosZ;
        runAction->_trkT[i] = (*THC)[i]->myData.trkT;
        runAction->_trkLength[i] = (*THC)[i]->myData.trkLength;
        runAction->_trkCharge[i] = (*THC)[i]->myData.trkCharge;
        runAction->_reflectionNum[i] = (*THC)[i]->myData.reflectionNum;
        runAction->_reflectionNum1[i] = (*THC)[i]->myData.reflectionNum1;
        runAction->_reflectionNum2[i] = (*THC)[i]->myData.reflectionNum2;
    }

    runAction->_src_confID    = _detector->GetSrcConfID();
    runAction->_src_posX      = _detector->GetSrcShiftX();
    runAction->_src_posY      = _detector->GetSrcShiftY();
    runAction->_src_posZ      = _detector->GetSrcShiftZ();
    runAction->_abs_width1     = _detector->GetAbsWidth1();
    runAction->_abs_width2     = _detector->GetAbsWidth2();
    runAction->_prob1     = _steppingAction->GetProb1();
    runAction->_prob2     = _steppingAction->GetProb2();

    runAction->tree->Fill();
}
