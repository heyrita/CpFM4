//my
#include "PrimaryGeneratorAction.hh"

//G4
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4RandomDirection.hh"
#include "G4RunManager.hh"

//root
#include "TMath.h"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    // Detector Messenger
    _src_shiftX = 0.0*mm;
    _src_shiftY = 0.0*mm;
    _src_shiftZ = 0.0*mm;
    _src_conf_id = 1;

    _particleGun = new G4ParticleGun(1);
    _particleGun->SetParticleEnergy(0*eV);
    _particleGun->SetParticlePosition(G4ThreeVector(_src_shiftX,_src_shiftY,_src_shiftZ));
    _particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

    _detector = (DetectorConstruction*) G4RunManager::GetRunManager()->GetUserDetectorConstruction();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete _particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    G4double zCopyShift =  UA9Const::_bar_size_X/TMath::Tan(UA9Const::angleDet) + UA9Const::_distBetweenBars/TMath::Tan(UA9Const::angleDet);

    _src_conf_id = _detector->GetSrcConfID();
    _src_shiftX = _detector->GetSrcShiftX();
    _src_shiftY = _detector->GetSrcShiftY();
    _src_shiftZ = _detector->GetSrcShiftZ();

    G4double PosX = 0.0*mm;
    G4double PosY = 0.0*mm;
    G4double PosZ = 0.0*mm;

    G4bool trkIsOk = false;
    G4int nMax = 1000000;

    for(G4int i = 0; i < nMax; i++)
    {
        PosX = G4UniformRand()*UA9Const::_src_length - UA9Const::_src_length/2;
        PosY = G4UniformRand()*UA9Const::_src_diam - UA9Const::_src_diam/2;
        PosZ = G4UniformRand()*UA9Const::_src_diam - UA9Const::_src_diam/2;

        if(PosX >= -UA9Const::_src_length/2 && PosX <= UA9Const::_src_length/2 && (PosY*PosY + PosZ*PosZ) <=  UA9Const::_src_diam*UA9Const::_src_diam/4.0)
        {
            trkIsOk = true;
            i = nMax;
        }

        if(trkIsOk)
        {
            if(_src_conf_id == 1)
            {
                PosX += -23.0*mm - 3.0*UA9Const::_bar_size_X/2 - UA9Const::_distBetweenBars - _src_shiftX + 7.0*mm;
                PosY += 0.0*mm + _src_shiftY;
                PosZ += -UA9Const::_bar_size_Z/2 + zCopyShift + _src_shiftZ;
            }
            if(_src_conf_id == 2)
            {
                PosX += 23.0*mm + UA9Const::_bar_size_X/2 + _src_shiftX - 7.0*mm;
                PosY += 0.0*mm + _src_shiftY;
                PosZ += -UA9Const::_bar_size_Z/2 + _src_shiftZ;
            }

            // 90Sr
            G4int Z = 38, A = 90;
            G4double ionCharge   = 0.*eplus;
            G4double excitEnergy = 0.*keV;

            G4ParticleDefinition* ion = G4ParticleTable::GetParticleTable()->GetIon(Z,A,excitEnergy);
            _particleGun->SetParticleDefinition(ion);
            _particleGun->SetParticleCharge(ionCharge);
            _particleGun->SetParticlePosition(G4ThreeVector(PosX,PosY,PosZ));
            _particleGun->SetParticleMomentumDirection(G4RandomDirection());
            _particleGun->GeneratePrimaryVertex(anEvent);
        }
    }

    if(!trkIsOk)
    {
        G4cout<<" ERROR --> trkIsOK == false, nMax ="<<nMax<<G4endl<<"           acseptance is too small"<<G4endl;
        assert(0);
    }
}
