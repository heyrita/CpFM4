//my
#include "SensitiveDetector.hh"
#include "TrackInformation.hh"
//#include "HitMy.hh"
#include "UA9Const.hh"

//G4
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"


SensitiveDetector::SensitiveDetector(G4String name) :
  G4VSensitiveDetector(name)
{
  //  G4RunManager* runManager = G4RunManager::GetRunManager();  
  G4String HCname;
  collectionName.insert(HCname="OpticalPhotonCollection");
}

SensitiveDetector::~SensitiveDetector() { }

void SensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
  OpticalPhotonCollection =
    new HitsCollection(SensitiveDetectorName, collectionName[0]);

  static G4int HCID = -1;
  if (HCID < 0) {
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  }
  HCE->AddHitsCollection(HCID, OpticalPhotonCollection);
}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep,
                        G4TouchableHistory* ROhist)
{
  HitData blat;
  return ProcessHits_fTOF(aStep, ROhist, blat);
}

G4bool
SensitiveDetector::ProcessHits_fTOF(const G4Step* aStep, G4TouchableHistory*, HitData hitInfo)
{
  G4Track* aTrack = aStep->GetTrack();
  // Get the pointer to the Touchable volumes
  //G4TouchableHistory* theTouchable = (G4TouchableHistory*)(aStep->GetPostStepPoint()->GetTouchable());
  //G4cout<<" theTouchable->GetCopyNumber() "<<theTouchable->GetCopyNumber()<<G4endl;
  // if (theTouchable->GetCopyNumber() == 0)
  //return true;
  
  G4ThreeVector globalPosition  = aStep->GetPostStepPoint()->GetPosition();
  G4ThreeVector globalMom       = aStep->GetPostStepPoint()->GetMomentum();
  G4ThreeVector globalMomDir    = aStep->GetPostStepPoint()->GetMomentumDirection();
  //G4cout<<"before"<<G4endl;
  //G4cout<<"globalMomDir.x()="<<TMath::ACos(globalMomDir.x())*180.0/TMath::Pi()<<" | globalMomDir.y()="<<TMath::ACos(globalMomDir.y())*180.0/TMath::Pi()<<" | globalMomDir.z()="<<TMath::ACos(globalMomDir.z())*180.0/TMath::Pi()<<G4endl;
  globalMomDir.rotateY(-(90.0*deg - UA9Const::angleDet));
  //globalMomDir.rotateY(1.0*deg);
  //G4cout<<"after"<<G4endl;
  //G4cout<<"globalMomDir.x()="<<TMath::ACos(globalMomDir.x())*180.0/TMath::Pi()<<" | globalMomDir.y()="<<TMath::ACos(globalMomDir.y())*180.0/TMath::Pi()<<" | globalMomDir.z()="<<TMath::ACos(globalMomDir.z())*180.0/TMath::Pi()<<G4endl;

  //const G4AffineTransform transformation = 
  //theTouchable->GetHistory()->GetTopTransform();
  //G4ThreeVector localPosition = transformation.TransformPoint(globalPosition);
  
  // Get Parent Track info
  //TrackInformation* info = (TrackInformation*)(aStep->GetTrack()->GetUserInformation());

  
  //G4cout<<"wwewe"<<G4endl;
  //info->Print();
  
  //G4double thetaC = 
  //info->GetMyOriginalMomentum().angle(info->GetOriginalMomentum());
  //G4ThreeVector xDir(1., 0., 0.);
  //G4double phiC = 
  //info->GetMyOriginalMomentum().azimAngle(info->GetOriginalMomentum().cross(xDir));
  
  //G4double genThetaX = std::atan2(info->GetMyOriginalMomentum().x(),
  //			  info->GetMyOriginalMomentum().z());
  //G4double genThetaY = std::atan2(info->GetMyOriginalMomentum().y(),
  //			  info->GetMyOriginalMomentum().z());
  
  //if(hitInfo.detection){
  // Create new hit and set the values
  HitMy* newHit = new HitMy();
  newHit->myData = hitInfo;
  
  //if(newHit->myData.SecID != -3)
  //G4cout<<"newHit->myData.SecID = "<<newHit->myData.SecID<<G4endl;
  
  // Fill the stuff that wasn't passed in the hitInfo struct
  newHit->myData.TrackID = aTrack->GetTrackID();
  newHit->myData.ParentID = aTrack->GetParentID();
  newHit->myData.Energy = aTrack->GetKineticEnergy();
  newHit->myData.Wavelength = twopi*hbarc/newHit->myData.Energy/nm;
  newHit->myData.PosX = globalPosition.x();
  newHit->myData.PosY = globalPosition.y();
  newHit->myData.PosZ = globalPosition.z();
  newHit->myData.MomDirX = globalMomDir.x();
  newHit->myData.MomDirY = globalMomDir.y();
  newHit->myData.MomDirZ = globalMomDir.z();
  newHit->myData.MomX = globalMom.x();
  newHit->myData.MomY = globalMom.y();
  newHit->myData.MomZ = globalMom.z();
  newHit->myData.GlobalTime = aTrack->GetGlobalTime();
  newHit->myData.LocalTime = aTrack->GetLocalTime();
  newHit->myData.photPathLen = aTrack->GetTrackLength();
  //newHit->myData.LocPos[0] = localPosition.x();
  //newHit->myData.LocPos[1] = localPosition.y();
  //newHit->myData.LocPos[2] = localPosition.z();
  //newHit->myData.ThetaC = thetaC;
  //newHit->myData.PhiC = phiC;
  //newHit->myData.GenThetaX = genThetaX;
  //newHit->myData.GenThetaY = genThetaY;
  
  // Insert this hit
  OpticalPhotonCollection->insert(newHit);
  newHit->Draw();
  //  }
  //else{
  //G4cout<<" WARNING hitInfo.detection == "<<hitInfo.detection<<G4endl;
  //}
  
  return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{

  //G4int NbHits = OpticalPhotonCollection->entries();
  //G4cout << "\n--------> Hits Collection: in this event there are " << NbHits
  // << " hits from Optical Photons." << G4endl;
  //for (G4int i = 0; i<NbHits; i++)
  //(*OpticalPhotonCollection)[i]->Print();
  
}
