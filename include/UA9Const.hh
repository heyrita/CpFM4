#ifndef UA9Const_h
#define UA9Const_h

#include "TMath.h"

using namespace CLHEP;

namespace UA9Const
{
    const G4double angleDet = 47.0*deg;
    const G4double probability = 1.0;
    const G4double _sensitive_size = 22.0*mm;

    const G4double _air_layer = 0.3*mm;

    const G4double _distBetweenBars = 1.0*mm;
    const G4double _bar_size_Z = 40.0*cm;
    const G4double _bar_size_X = 0.5*cm;
    const G4double _bar_size_Y = 1.0*cm;

    const G4double _src_length  = 11.0*mm;
    const G4double _src_diam    = 2.20*mm;

    const G4double _absorber_thick = 0.1*mm;
}

#endif
