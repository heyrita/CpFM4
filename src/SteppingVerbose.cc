#include "SteppingVerbose.hh"

#include "G4SteppingManager.hh"
#include "G4UnitsTable.hh"

SteppingVerbose::SteppingVerbose()
{}

SteppingVerbose::~SteppingVerbose()
{}

void SteppingVerbose::StepInfo()
{
  CopyState();

  G4int prec = G4cout.precision(3);

  if (verboseLevel >= 1) {
    if (verboseLevel >= 4) VerboseTrack();
    if (verboseLevel >= 3) {
      G4cout << G4endl;
      G4cout << std::setw( 5) << "#Step#"     << " "
	     << std::setw( 6) << "X"          << " "
	     << std::setw( 6) << "Y"          << " "
	     << std::setw( 6) << "Z"          << " "
	     << std::setw( 9) << "KinE"       << " "
	     << std::setw( 9) << "dEStep"     << " "
	     << std::setw(10) << "StepLen"    << " "
	     << std::setw(10) << "TrackLen"   << " "
	     << std::setw(10) << "Volume"     << " "
	     << std::setw(10) << "Process"    << " " << G4endl;
    }
    G4cout << std::setw(5) << fTrack->GetCurrentStepNumber() << " "
	   << std::setw(6) << G4BestUnit(fTrack->GetPosition().x(), "Length")
	   << std::setw(6) << G4BestUnit(fTrack->GetPosition().y(), "Length")
	   << std::setw(6) << G4BestUnit(fTrack->GetPosition().z(), "Length")
	   << std::setw(6) << G4BestUnit(fTrack->GetKineticEnergy(), "Energy")
	   << std::setw(6) << G4BestUnit(fStep->GetTotalEnergyDeposit(), "Energy")
	   << std::setw(6) << G4BestUnit(fStep->GetStepLength(), "Length")
	   << std::setw(6) << G4BestUnit(fTrack->GetTrackLength(), "Length")
	   << " ";

    if (fTrack->GetNextVolume() != 0) {
      G4cout << std::setw(10) << fTrack->GetVolume()->GetName();
    }
    else {
      G4cout << std::setw(10) << "OutOfWorld";
    }

    if (fStep->GetPostStepPoint()->GetProcessDefinedStep() != 0) {
      G4cout << " "
	     << std::setw(10)
	     << fStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    }
    else {
      G4cout << " UserLimit";
    }

    G4cout << G4endl;

    if (verboseLevel == 2) {
      G4int tN2ndariesTot = fN2ndariesAtRestDoIt +
	fN2ndariesAlongStepDoIt +
	fN2ndariesPostStepDoIt;

      if (tN2ndariesTot > 0) {
	G4cout << " :----- List of 2ndaries -----"
	       << "#SpawnInStep=" << std::setw(3) << tN2ndariesTot
	       << "(Rest="        << std::setw(2) << fN2ndariesAtRestDoIt
	       << ",Along="       << std::setw(2) << fN2ndariesAlongStepDoIt
	       << ",Post="        << std::setw(2) << fN2ndariesPostStepDoIt
	       << "),"
	       << "#SpawnTotal=" << std::setw(3) << (*fSecondary).size()
	       << "------------------"
	       << G4endl;

	G4cout << "  :-------------------------"
	       << "------------------------------"
	       << "-- EndOf2ndariesInfo--------"
	       << G4endl;
	  
      }
    }
  }
  G4cout.precision(prec);
}

//Tracking Started

void SteppingVerbose::TrackingStarted()
{
  CopyState();
  G4int prec = G4cout.precision(3);

  G4cout.precision(prec);
}
