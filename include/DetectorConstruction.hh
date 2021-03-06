#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1
//My
#include "VolumeStructures.hh"
#include "DetectorMessenger.hh"
#include "G4SubtractionSolid.hh"

//G4
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "RunAction.hh"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TString.h"
#include "TTreeReaderArray.h"
#include "TChainElement.h"
#include <utility>
#include <vector>

using namespace std; 

pair <vector<vector<Double_t>>, 
      vector<vector<Double_t>>> 
      read_sizes (const char *fName);

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

  TString fName_g ;
  TString fName_b ;

  vector <vector<Double_t>> vWidths_good ;
  vector <vector <Double_t>> vLenghs_good ;
  vector <vector <Double_t>> vWidths_bad ;
  vector <vector <Double_t>> vLenghs_bad ;
  vector <G4double> abs_amount_g;
  vector <G4double> abs_amount_b;
  vector < vector<G4double> > start_points_1;
  vector < vector<G4double> > start_points_2;
  G4double nBrinks;
  G4double zMy; 

  ///Bar 1 ///
  vector < vector <G4VSolid*>> Sabsorbers_vec1;
  vector < vector <G4LogicalVolume*>> Labsorbers_vec1;
  vector <G4SubtractionSolid*> subtraction_vec1;
  vector <G4LogicalVolume*> Labsorbers_result_vec1;
 
  ///Bar 2
  vector <vector <G4VSolid*> >Sabsorbers_vec2;
  vector < vector <G4LogicalVolume*>> Labsorbers_vec2;
  vector <G4LogicalVolume*> Labsorbers_result_vec2;
  vector <G4SubtractionSolid*> subtraction_vec2;
  

 

   
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
  vector <SecStruct> absorbers; 

  Abs1Struct abs1;
  Abs1Struct abs21;
  Abs1Struct abs22;
  Abs1Struct abs23;
  Abs1Struct abs24;

  FiberStruct fiberCorr;
  FiberStruct fiberClad;
  FiberStruct fiberCoat;
  FiberStruct fiberBuff;

  // RunAction* temp = nullptr;

  //LB need to be done stability tests
  //G4UserLimits* stepLimit;  // pointer to user step limits
};

#endif
