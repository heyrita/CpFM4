#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

//My
#include "VolumeStructures.hh"
#include "DetectorMessenger.hh"

//G4
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"

class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  DetectorConstruction();
  ~DetectorConstruction();

  void SetSrcConfID(G4int);
  void SetSrcShiftX(G4double);
  void SetSrcShiftY(G4double);
  void SetSrcShiftZ(G4double);
  void SetAbsWidth1(G4double);
  void SetAbsWidth2(G4double);
  void ModifyGeometry();

  G4int GetSrcConfID()      {return _src_conf_id;}
  G4double GetSrcShiftX()   {return _src_shiftX;}
  G4double GetSrcShiftY()   {return _src_shiftY;}
  G4double GetSrcShiftZ()   {return _src_shiftZ;}
  G4double GetAbsWidth1()   {return _abs_width1;}
  G4double GetAbsWidth2()   {return _abs_width2;}
   
private:
  G4int _src_conf_id;
  G4double _src_shiftX;
  G4double _src_shiftY;
  G4double _src_shiftZ;
  G4double _abs_width1;
  G4double _abs_width2;

  DetectorMessenger* fDetectorMessenger;

public:
  
  G4VPhysicalVolume* Construct();
  void ConstructDetector();

private:
  void DefineMaterials();
  G4UserLimits* stepLimit;        // pointer to user step limits

private:

  // Various visibility attributes
  G4VisAttributes* worldVisAtt;
  G4VisAttributes* quartzVisAtt;
  G4VisAttributes* sensitiveVisAtt;
  G4VisAttributes* pmtboxVisAtt;
  G4VisAttributes* absVisAtt;
  G4VisAttributes* viewportVisAtt;
  G4VisAttributes* sourceShieldVisAtt1;
  G4VisAttributes* sourceShieldVisAtt2;
  G4VisAttributes* absorberVisAtt;

  WorldStruct world;
  SecStruct sec1;
  SecStruct sec2;
  SecStruct sec3;
  SenDetStruct sensitive;  
  SecStruct viewport;
  SecStruct source_shield1;
  SecStruct source_shield2;
  SecStruct absorber_1;
  SecStruct absorber_2;

  Abs1Struct abs1;
  Abs1Struct abs21;
  Abs1Struct abs22;
  Abs1Struct abs23;
  Abs1Struct abs24;

  FiberStruct fiberCorr;
  FiberStruct fiberClad;
  FiberStruct fiberCoat;
  FiberStruct fiberBuff;

  //LB need to be done stability tests
  //G4UserLimits* stepLimit;  // pointer to user step limits
};

#endif
