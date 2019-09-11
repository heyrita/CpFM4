//G4
#include "G4Material.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Point3D.hh"
#include "G4TwoVector.hh"
#include "globals.hh"

//My
#include "UA9Const.hh"

struct VolumeStruct {
  G4Material*        material;
  G4VSolid*          solid;
  G4LogicalVolume*   logical;
  G4VPhysicalVolume* physical;
  VolumeStruct() :
    material(0),
    solid(0),
    logical(0),
    physical(0)
  {;}
  ~VolumeStruct() {;}
};

//World Volume
struct WorldStruct : VolumeStruct {
  // Defined as a G4Box
  const G4double sizeX;
  const G4double sizeY;
  const G4double sizeZ;
  WorldStruct() :
    sizeX(0.0),
    sizeY(0.0),
    sizeZ(0.0)
  {;}
};

//Quartz detector volume (1)
struct SecStruct : VolumeStruct {
  // Defined as a G4Box 
  const G4double sizeX;
  const G4double sizeY;
  const G4double sizeZ;
  SecStruct() :
    sizeX(0.0),
    sizeY(0.0),
    sizeZ(0.0)
  {;}
};

//Sensitive volume
struct SenDetStruct : VolumeStruct {
  // Defined as a G4Tubs 
  const G4double sizeL;
  const G4double sizeRmin;
  const G4double sizeRmax;
  SenDetStruct() :
    sizeL(0.0),
    sizeRmin(0.0),
    sizeRmax(0.0)
  {;}
};

struct SecStructWindow : VolumeStruct {
  // Defined as a G4Tube 
  const G4double sizeL;
  const G4double sizeRmin;
  const G4double sizeRmax;
  SecStructWindow() :
    sizeL(0.0),
    sizeRmin(0.0),
    sizeRmax(0.0)
  {;}
};

struct beamPipeStruct : VolumeStruct {
  // Defined as a G4Tube 
  const G4double sizeL;
  const G4double sizeRmin;
  const G4double sizeRmax;
  beamPipeStruct() :
    sizeL(0.0),
    sizeRmin(0.0),
    sizeRmax(0.0)
  {;}
};

//Absorber (1) from the small (1 x 1 cm side of the quartz radiator)
struct Abs1Struct : VolumeStruct {
  // Defined as a G4Box 
  const G4double sizeX;
  const G4double sizeY;
  const G4double sizeZ;
  Abs1Struct() :
    sizeX(0.0),
    sizeY(0.0),
    sizeZ(0.0)
  {;}
};

//Absorber (2) from the sides (the holder of the quartz finger)
struct Abs2Struct : VolumeStruct {
  // Defined as a G4Box 
  const G4double sizeX;
  const G4double sizeY;
  const G4double sizeZ;
  Abs2Struct() :
    sizeX(0.0),
    sizeY(0.0),
    sizeZ(0.0)
  {;}
};

//Fiber
struct FiberStruct : VolumeStruct {
  // Defined as a G4Tube 
  const G4double sizeL;
  FiberStruct() :
    sizeL(0.0)
  {;}
};
