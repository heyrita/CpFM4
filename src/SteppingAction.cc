#include "SteppingAction.hh"
#include "SensitiveDetector.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4SDManager.hh"
#include "G4ProcessManager.hh"
#include "G4RunManager.hh"
#include "G4UserEventAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "UA9Const.hh"

#include "TString.h"

#include <iomanip>


SteppingAction::SteppingAction(RunAction *runAct, PrimaryGeneratorAction* genAction) : runAction(runAct), _genAction(genAction)
{
    Reset();
    ResetPerEvent();
    eventaction = (EventAction*) G4RunManager::GetRunManager()->GetUserEventAction();

    _probOfReflection1 = 1.0;
    _probOfReflection2 = 1.0;

    fSteppingMessenger = new SteppingMessenger(this);
}

SteppingAction::~SteppingAction()
{
    delete fSteppingMessenger;
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    //G4RunManager *runManager = G4RunManager:: GetRunManager();

    const G4Track* aTrack = aStep->GetTrack();
    G4int trackID = aTrack->GetTrackID();

    G4StepPoint*          aPrePoint   = aStep->GetPreStepPoint();
    G4VPhysicalVolume*    aPrePV      = aPrePoint->GetPhysicalVolume();
    G4StepPoint*          aPostPoint  = aStep->GetPostStepPoint();
    G4VPhysicalVolume*    aPostPV     = aPostPoint->GetPhysicalVolume();

    // get volume of the current step
    G4VPhysicalVolume* volumeEdep = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

    // collect energy and track length step by step
    G4double edep = aStep->GetTotalEnergyDeposit();
    G4double stepl = 0.;

    if(aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)
        stepl = aStep->GetStepLength();

    if (volumeEdep->GetLogicalVolume()->GetName() == "trigTop")
    {
        EnergyTop += edep;
        TrackLTop += stepl;
    }
    if (volumeEdep->GetLogicalVolume()->GetName() == "trigBot")
    {
        EnergyBot += edep;
        TrackLBot += stepl;
    }

    if (!aPostPV) return;

    //
    //----- initial Track () -----
    //
    //if (aPrePV->GetName().contains("World") && aPostPV->GetName().contains("Sector") && trackID == 1)
    if (aPrePV->GetName().contains("World") && aPostPV->GetName().contains("Sector1") && aTrack->GetParticleDefinition()->GetPDGCharge() != 0.)
    {
        _SecID = 1;

        _trkMomX = aStep->GetPostStepPoint()->GetMomentum().getX();
        _trkMomY = aStep->GetPostStepPoint()->GetMomentum().getY();
        _trkMomZ = aStep->GetPostStepPoint()->GetMomentum().getZ();
        _trkPosX = aStep->GetPostStepPoint()->GetPosition().getX();
        _trkPosY = aStep->GetPostStepPoint()->GetPosition().getY();
        _trkPosZ = aStep->GetPostStepPoint()->GetPosition().getZ();
        _trkT = aStep->GetPostStepPoint()->GetGlobalTime();
        _trkLength = aTrack->GetTrackLength();
        _trkCharge = aTrack->GetParticleDefinition()->GetPDGCharge();
    }
    if (aPrePV->GetName().contains("World") && aPostPV->GetName().contains("Sector2") && aTrack->GetParticleDefinition()->GetPDGCharge() != 0.)
    {
        _SecID = 2;

        _trkMomX = aStep->GetPostStepPoint()->GetMomentum().getX();
        _trkMomY = aStep->GetPostStepPoint()->GetMomentum().getY();
        _trkMomZ = aStep->GetPostStepPoint()->GetMomentum().getZ();
        _trkPosX = aStep->GetPostStepPoint()->GetPosition().getX();
        _trkPosY = aStep->GetPostStepPoint()->GetPosition().getY();
        _trkPosZ = aStep->GetPostStepPoint()->GetPosition().getZ();
        _trkT = aStep->GetPostStepPoint()->GetGlobalTime();
        _trkLength = aTrack->GetTrackLength();
        _trkCharge = aTrack->GetParticleDefinition()->GetPDGCharge();
    }

    //
    //----- Optical photons  -----
    //
    G4ParticleDefinition* particleType = aTrack->GetDefinition();
    if (particleType != G4OpticalPhoton::OpticalPhotonDefinition())
      return;

    if (_particleID != trackID)
    {
        Reset();
        _particleID = trackID;
        if(aPrePV->GetName().contains("Sector1")) {InternalReflectionProbability(aTrack->GetTotalEnergy()/eV, probOfReflection,1);}
        if(aPrePV->GetName().contains("Sector2")) {InternalReflectionProbability(aTrack->GetTotalEnergy()/eV, probOfReflection,2);}
    }

    //LB
    if (aPrePV->GetName().contains("pmtWin2") && aPostPV->GetName().contains("Sensitive"))
    {
    }

    G4OpBoundaryProcessStatus boundaryStatus = Undefined;
    static G4OpBoundaryProcess* boundary = NULL;

    // Find boundary process
    if (!boundary)
    {
        G4ProcessManager* pm = aStep->GetTrack()->GetDefinition()->GetProcessManager();
        G4int nprocesses = pm->GetProcessListLength();
        G4ProcessVector* pv = pm->GetProcessList();
        for (G4int i = 0; i < nprocesses; i++)
        {
            if ((*pv)[i]->GetProcessName() == "OpBoundary")
            {
                boundary = (G4OpBoundaryProcess*)(*pv)[i];
                break;
            }
        }
    }

    if (!boundary) return;

    boundaryStatus = boundary->GetStatus();

    if (aPostPoint->GetStepStatus() == fGeomBoundary)
    {
        G4String sdName = "fTOF";
        G4SDManager* SDman = G4SDManager::GetSDMpointer();
        SensitiveDetector* sd = (SensitiveDetector*)SDman->FindSensitiveDetector(sdName);
        G4double flat = G4UniformRand();
        switch(boundaryStatus)
        {
        case Absorption:
            break;
        case Detection:
            if (sd)
            {
                HitData hitInfo;
                hitInfo.SecID = _SecID;
                hitInfo.trkMomX = _trkMomX;
                hitInfo.trkMomY = _trkMomY;
                hitInfo.trkMomZ = _trkMomZ;
                hitInfo.trkPosX = _trkPosX;
                hitInfo.trkPosY = _trkPosY;
                hitInfo.trkPosZ = _trkPosZ;
                hitInfo.trkT = _trkT;
                hitInfo.trkLength = _trkLength;
                hitInfo.trkCharge = _trkCharge;
                hitInfo.chID = _chID;
                hitInfo.reflectionNum = _reflectionNum[_particleID];
                hitInfo.reflectionNum1 = _reflectionNum1[_particleID];
                hitInfo.reflectionNum2 = _reflectionNum2[_particleID];
                sd->ProcessHits_fTOF(aStep, NULL, hitInfo);
            }
            break;
        case FresnelReflection:
            // Reflections of surfaces of different media
            break;
        case TotalInternalReflection:
            if (aPrePV->GetName().contains("Sector1") && flat > probOfReflection)
            {
                G4Track* aNonConstTrack = const_cast<G4Track*>(aTrack);
                aNonConstTrack->SetTrackStatus(fStopAndKill);
            }
            if (aPrePV->GetName().contains("Sector2") && flat > probOfReflection)
            {
                G4Track* aNonConstTrack = const_cast<G4Track*>(aTrack);
                aNonConstTrack->SetTrackStatus(fStopAndKill);
            }
            if (aPrePV->GetName().contains("Sector1"))
            {
                _reflectionNum[_particleID]++;
                _reflectionNum1[_particleID]++;
                if(_reflectionNum[_particleID] > 1000 && _probOfReflection1 == 1.0)
                {
                    G4Track* aNonConstTrack = const_cast<G4Track*>(aTrack);
                    aNonConstTrack->SetTrackStatus(fStopAndKill);
                }
            }
            if (aPrePV->GetName().contains("Sector2"))
            {                
                _reflectionNum[_particleID]++;
                _reflectionNum2[_particleID]++;
                if(_reflectionNum[_particleID] > 1000 && _probOfReflection2 == 1.0)
                {
                    G4Track* aNonConstTrack = const_cast<G4Track*>(aTrack);
                    aNonConstTrack->SetTrackStatus(fStopAndKill);
                }
            }
            break;
        case SpikeReflection:
            break;
        default:
            break;
        }
    }
}

void SteppingAction::ResetPerEvent()
{
    _SecID   = -1;
    _trkMomX = -999.0;
    _trkMomY = -999.0;
    _trkMomZ = -999.0;
    _trkPosX = -999.0;
    _trkPosY = -999.0;
    _trkPosZ = -999.0;
    _trkT    = -999.0;
    _trkLength = -999.0;
    _trkCharge = -999.0;

    for(int i = 0; i < _nPhotMax; i++)
    {
        _reflectionNum[i] = 0;
        _reflectionNum1[i] = 0;
        _reflectionNum2[i] = 0;
    }
}

void SteppingAction::Reset()
{
    probOfReflection = UA9Const::probability;
    _particleID = -1;
    _chID = -1;
}

void SteppingAction::InternalReflectionProbability(G4double energy, G4double& probability, G4int barID)
{
  probability = UA9Const::probability;

  /* this function simulate the internal reflection probability per one
     bounce - each time photon bounced this function is called
     and the reflection is tested if photon reflects or disappear -
     this function estimates loss of photons during imperfections
     of bar */

  G4double opticalPhotonEnergy[36]={
    1.90744901082931,1.93725290162352,1.96800294768103,1.99974493070815,
    2.03252763449025,2.06640309506508,2.10142687633737,2.13765837420526,
    2.17516115270009,2.21400331614116,2.25425792188918,2.29600343896121,
    2.33932425856425,2.38431126353664,2.43106246478245,2.4796837140781,
    2.53028950416133,2.58300386883136,2.63796139795543,2.6953083848675,
    2.75520412675345,2.81782240236148,2.88335315590477,2.95200442152155,
    3.02400452936354,3.09960464259763,3.17908168471551,3.26274172905013,
    3.35092393794338,3.44400515844181,3.54240530582586,3.64659369717368,
    3.75709653648197,3.87450580324703,3.99948986141629,4.13280619013017};


  G4double internalReflectivity[36] =
    {0.999895281,0.999891334,0.999885743,0.999878696,0.999870426,
     0.9998612,0.999851309,0.999841055,0.999830735,0.999820635,0.999811012,
     0.999802084,0.999794018,0.999786917,0.999780807,0.999775625,
     0.999771209,0.999767282,0.999763441,0.999759146,0.999753706,
     0.999746266,0.999735798,0.999721084,0.999700708,0.99967304,
     0.999636227,0.999588178,0.999526552,0.999448747,0.999351887,
     0.999232808,0.99908805,0.998913839,0.998706078,0.998460335};

  //-----------------------//
  //---- Normalization ----//
  //-----------------------//
  Double_t max = 0.0;
  for(Int_t i = 0; i < 36; i++) {if(max < internalReflectivity[i]) {max = internalReflectivity[i];}}
  for(Int_t i = 0; i < 36; i++) {internalReflectivity[i] = internalReflectivity[i]/max;}
  //-----------------------//

  G4int i;
  for(i = 0; i < 36;i++)
  {
      if(energy < opticalPhotonEnergy[i]) break;
  }

  probability = ((energy-opticalPhotonEnergy[i-1])/
                   (opticalPhotonEnergy[i]-opticalPhotonEnergy[i-1]))*
      (internalReflectivity[i]-internalReflectivity[i-1]) +
      internalReflectivity[i-1];

    if(barID == 1) {probability = probability*_probOfReflection1;}
    if(barID == 2) {probability = probability*_probOfReflection2;}
  }


void SteppingAction::SetProb1(G4double value)
{
    _probOfReflection1 = value;
}

void SteppingAction::SetProb2(G4double value)
{
    _probOfReflection2 = value;
}
