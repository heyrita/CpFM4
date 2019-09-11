//My
#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

//G4
#include "G4RunManager.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Torus.hh"
#include "G4Trap.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4Para.hh"
#include "G4Paraboloid.hh"
#include "G4EllipticalTube.hh"
#include "G4ExtrudedSolid.hh"
#include "G4VSolid.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Color.hh"
#include "G4TwoVector.hh"
#include "G4SDManager.hh"
#include "globals.hh"
#include "G4RegionStore.hh"
//magnetic field
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4UserLimits.hh"
//GDML
//#include <G4GDMLParser.hh>

//root 
#include "TMath.h"

DetectorConstruction::DetectorConstruction()
{
    //magField = new MagneticField();
    worldVisAtt = new G4VisAttributes();
    quartzVisAtt = new G4VisAttributes();
    sensitiveVisAtt = new G4VisAttributes();
    pmtboxVisAtt = new G4VisAttributes();
    absVisAtt = new G4VisAttributes();
    viewportVisAtt = new G4VisAttributes();
    sourceShieldVisAtt1 = new G4VisAttributes();
    sourceShieldVisAtt2 = new G4VisAttributes();
    absorberVisAtt = new G4VisAttributes();
    // Define Materials to be used
    DefineMaterials();

    // Detector Messenger
    _src_shiftX = 0.0*mm;
    _src_shiftY = 0.0*mm;
    _src_shiftZ = 0.0*mm;
    _abs_width1 = 0.0*mm;
    _abs_width2 = 0.0*mm;
    _src_conf_id = 1;
    fDetectorMessenger = new DetectorMessenger(this);
}

DetectorConstruction::~DetectorConstruction()
{
    //delete magField;
    delete worldVisAtt;
    delete quartzVisAtt;
    delete sensitiveVisAtt;
    delete pmtboxVisAtt;
    delete absVisAtt;
    delete stepLimit;
    delete viewportVisAtt;
    delete sourceShieldVisAtt1;
    delete sourceShieldVisAtt2;
    delete fDetectorMessenger;
}

void DetectorConstruction::DefineMaterials()
{
    // Define elements
    G4Element* N = G4NistManager::Instance()->FindOrBuildElement("N");
    G4Element* O = G4NistManager::Instance()->FindOrBuildElement("O");
    G4Element* Si = G4NistManager::Instance()->FindOrBuildElement("Si");
    G4Element* Al = G4NistManager::Instance()->FindOrBuildElement("Al");
    G4Element* Zn = G4NistManager::Instance()->FindOrBuildElement("Zn");
    G4Element* Cu = G4NistManager::Instance()->FindOrBuildElement("Cu");

    // Quartz Material (SiO2)
    G4Material* SiO2 = new G4Material("quartz",2.200*g/cm3,2);
    SiO2->AddElement(Si,1);
    SiO2->AddElement(O,2);

    // Quartz Material (SiO2_cladd)
    G4Material* SiO2_cladd = new G4Material("quartzCladd",2.200*g/cm3,2);
    SiO2_cladd->AddElement(Si,1);
    SiO2_cladd->AddElement(O,2);

    // Quartz Material (SiO2_coat)
    G4Material* SiO2_coat = new G4Material("quartzCoat",2.200*g/cm3,2);
    SiO2_coat->AddElement(Si,1);
    SiO2_coat->AddElement(O,2);

    // Air
    G4Material* Air = new G4Material("Air",1.290*mg/cm3,2);
    Air->AddElement(N,70*perCent);
    Air->AddElement(O,30*perCent);

    // Aluminum
    G4Material* Aluminum = new G4Material("Aluminum",2.7*g/cm3,1);
    Aluminum->AddElement(Al,100*perCent);

    // Brass
    G4Material* Brass = new G4Material("Brass",8.40*g/cm3,2);
    Brass->AddElement(Zn, 30*perCent);
    Brass->AddElement(Cu, 70*perCent);

    // Plexiglass
    G4NistManager::Instance()->FindOrBuildMaterial("G4_PLEXIGLASS");
    G4Material* Plexiglass = G4Material::GetMaterial("G4_PLEXIGLASS");

    // Assign Materials
    world.material = Air;
    sec1.material = SiO2;
    sec3.material = SiO2;
    fiberCorr.material = SiO2;
    fiberClad.material = SiO2_cladd;
    //fiberCoat.material = SiO2_coat;
    fiberCoat.material = Aluminum;
    sensitive.material = Aluminum;
    abs1.material = Aluminum;
    abs21.material = Aluminum;
    abs22.material = Aluminum;
    abs23.material = Aluminum;
    abs24.material = Aluminum;
    viewport.material = SiO2;
    source_shield1.material = Plexiglass;
    source_shield2.material = Brass;
    absorber_1.material = Aluminum;
    absorber_2.material = Aluminum;

    //
    // Generate and Add Material Properties Table
    //
    const G4int num = 36;
    G4double WaveLength[num];
    //G4double Absorption[num];      // Default value for absorption
    G4double AirAbsorption[num];
    G4double AirRefractiveIndex[num];
    G4double PhotonEnergy[num];

    // Absorption of quartz per 1m
    G4double QuartzAbsorption[num] =
      {0.999572036,0.999544661,0.999515062,0.999483019,0.999448285,
       0.999410586,0.999369611,0.999325013,0.999276402,0.999223336,
       0.999165317,0.999101778,0.999032079,0.998955488,0.998871172,
       0.998778177,0.99867541 ,0.998561611,0.998435332,0.998294892,
       0.998138345,0.997963425,0.997767484,0.997547418,0.99729958 ,
       0.99701966 ,0.99670255 ,0.996342167,0.995931242,0.995461041,
       0.994921022,0.994298396,0.993577567,0.992739402,0.991760297,
       0.990610945};

    for (int i=0; i<num; i++) {
      WaveLength[i] = (300 + i*10)*nanometer;
      //Absorption[i] = 100*m;      // Fake number for no absorption
      //initially
      AirAbsorption[i] = 100.*cm;   // If photon hits air, kill it
      //AirAbsorption[i] = 1.*cm;   // If photon hits air, kill it
      //AirAbsorption[i] = 0.1*cm;   // If photon hits air, kill it
      //AirAbsorption[i] = 100.0*m;
      AirRefractiveIndex[i] = 1.;
      PhotonEnergy[num - (i+1)] = twopi*hbarc/WaveLength[i];
      /* Absorption is given per length and G4 needs mean free path
         length, calculate it here
         mean free path length - taken as probablility equal 1/e
         that the photon will be absorbed */
      QuartzAbsorption[i] = (-1)/log(QuartzAbsorption[i])*100*cm;
      //EpotekAbsorption[i] = (-1)/log(EpotekAbsorption[i])*
      //epotekBarJoint.thickness;
    }

    G4double QuartzRefractiveIndex[num] =
      {1.456535,1.456812,1.4571  ,1.457399,1.457712,1.458038,
       1.458378,1.458735,1.459108,1.4595  ,1.459911,1.460344,
       1.460799,1.46128 ,1.461789,1.462326,1.462897,1.463502,
       1.464146,1.464833,1.465566,1.46635 ,1.46719 ,1.468094,
       1.469066,1.470116,1.471252,1.472485,1.473826,1.475289,
       1.476891,1.478651,1.480592,1.482739,1.485127,1.487793};

    G4double CladdingRefractiveIndex[num];

    for(int i=0; i<num; i++){
      CladdingRefractiveIndex[i] = TMath::Sqrt(QuartzRefractiveIndex[i]*QuartzRefractiveIndex[i]-0.22*0.22);
      //CladdingRefractiveIndex[i] = 1.0;
    }

    // Assign absorption and refraction to materials

    // Quartz
    G4MaterialPropertiesTable* QuartzMPT = new G4MaterialPropertiesTable();
    QuartzMPT->AddProperty("RINDEX", PhotonEnergy, QuartzRefractiveIndex, num);
    QuartzMPT->AddProperty("ABSLENGTH", PhotonEnergy, QuartzAbsorption, num);

    // Cladding (of the fiber) only for the fiber aplication
    G4MaterialPropertiesTable* CladdingMPT = new G4MaterialPropertiesTable();
    CladdingMPT->AddProperty("RINDEX", PhotonEnergy, CladdingRefractiveIndex, num);
    CladdingMPT->AddProperty("ABSLENGTH", PhotonEnergy, QuartzAbsorption, num);

    // Assign this material to the bars
    sec1.material->SetMaterialPropertiesTable(QuartzMPT);
    fiberCorr.material->SetMaterialPropertiesTable(QuartzMPT);
    fiberClad.material->SetMaterialPropertiesTable(CladdingMPT);

    // Air
    G4MaterialPropertiesTable* AirMPT = new G4MaterialPropertiesTable();
    AirMPT->AddProperty("RINDEX", PhotonEnergy, AirRefractiveIndex, num);
    AirMPT->AddProperty("ABSLENGTH", PhotonEnergy, AirAbsorption, num);

    // Assign these properties to the world volume
    world.material->SetMaterialPropertiesTable(AirMPT);

    // Viewport
    viewport.material->SetMaterialPropertiesTable(QuartzMPT);
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4double world_sizeX = 40.0*cm;
    G4double world_sizeY = 20.0*cm;
    //Without fibers
    //G4double world_sizeZ = 40.0*cm;
    //With fibers
    G4double world_sizeZ = 80.0*cm;

    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//
    G4double sec1_sizeZ = UA9Const::_bar_size_Z;
    G4double sec1_sizeX = UA9Const::_bar_size_X;
    G4double sec1_sizeY = UA9Const::_bar_size_Y;
    G4double viewport_sizeX = 35.0*mm;
    G4double viewport_sizeY = 35.0*mm;
    G4double viewport_sizeZ = 3.0*mm;
    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//


    //Volume for subtraction
    G4double sec5_sizeX = sec1_sizeX/TMath::Sin(UA9Const::angleDet);
    G4double sec5_sizeY = sec1_sizeY*2.0;
    G4double sec5_sizeZ = sec1_sizeX;

    //bool sec4volIsOn = false;
    //G4double sec4_sizeX = 3.0*cm;
    //G4double sec4_sizeY = sec1_sizeY;
    //G4double sec4_sizeZ = 5.0*mm;

    //bool abs2volIsOn = false;
    //G4double abs2_sizeX = 1.0*mm;
    //G4double abs2_sizeY = sec1_sizeY;
    //G4double abs2_sizeZ = sec4_sizeZ;

    //G4double abs3_sizeX = 1.0*mm;
    //G4double abs3_sizeY = sec1_sizeY;
    //G4double abs3_sizeZ = 2.0*cm;

    //G4double abs4_sizeX = abs3_sizeX;
    //G4double abs4_sizeY = abs2_sizeZ;
    //G4double abs4_sizeZ = abs3_sizeZ;

    //G4double xCopyShift = (sec1_sizeX + 0.5)/TMath::Cos(UA9Const::angleDet);
    //Last development
    G4double distBetweenBars = UA9Const::_distBetweenBars;
    G4double xCopyShift = -sec1_sizeX - distBetweenBars;
    G4double zCopyShift =  sec1_sizeX/TMath::Tan(UA9Const::angleDet) + distBetweenBars/TMath::Tan(UA9Const::angleDet);

    //G4double  para_alpha = 0.0;
    G4double  para_theta = UA9Const::angleDet;
    //G4double  para_phi = 0.0;
    //G4double  para_dx = sec1_sizeX/TMath::Cos(para_theta)/2.0;
    //G4double  para_dy = sec1_sizeY/2.0;
    //G4double  para_dz = sec1_sizeZ*TMath::Cos(para_theta)/2.0;

    //in case of 90 deg angle
    //G4double senDet_sizeX = sec1_sizeX/2.0;
    //G4double senDet_sizeY = sec1_sizeY/2.0;
    //in case of inclination angle
    //G4double senDet_sizeX = para_dx;
    //G4double senDet_sizeY = para_dy;
    //G4double senDet_sizeZ = 1.0*mm;
    G4double senDet_sizeX = UA9Const::_sensitive_size;
    G4double senDet_sizeY = UA9Const::_sensitive_size;
    G4double senDet_sizeZ = 1.0*mm;

    //in case of 90 deg angle
    //G4double abs1_sizeX = sec1_sizeX/2.0;
    //G4double abs1_sizeY = sec1_sizeY/2.0;
    //in case of inclination angle
    //G4double abs1_sizeX = para_dx;
    //G4double abs1_sizeY = para_dy;
    //G4double abs1_sizeZ = 1.0*mm;

    G4double fiberCorr_Rmin = 0.0;
    G4double fiberCorr_Rmax = 0.6*mm/2.0;
    G4double fiberCorr_L    = 50*cm;
    //G4double fiber_sizeXshift = 0.0*mm;

    G4double fiberClad_Rmin = fiberCorr_Rmax;
    G4double fiberClad_Rmax = 0.66*mm/2.0;
    G4double fiberClad_L    = fiberCorr_L;

    G4double fiberCoat_Rmin = fiberClad_Rmax;
    G4double fiberCoat_Rmax = 0.70*mm/2.0;
    G4double fiberCoat_L    = fiberCorr_L;

    G4RotationMatrix Ra;
    G4ThreeVector Ta;
    G4Transform3D Tr;

    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//
    // WORLD
    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//
    world.solid = new G4Box("World",world_sizeX/2.0,world_sizeY/2.0,world_sizeZ/2.0);
    world.logical = new G4LogicalVolume(world.solid,world.material,"World");
    world.physical = new G4PVPlacement(0,G4ThreeVector(),world.logical,"World",0,false,0);
    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//
    // SOURCE SHIELD
    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//

    G4VSolid* source_shield_tube1 = new G4Tubs("Shield1",0.0,30.0*mm,23.0*mm,0.0*deg,360.0*deg);
    //G4VSolid* source_shield_hole1 = new G4Tubs("Hole1",0.0,4.0*mm,4.0*mm,0.0*deg,360.0*deg);
    G4VSolid* source_shield_hole2 = new G4Tubs("Hole2",0.0,1.5*mm,7.0*mm,0.0*deg,360.0*deg);
    G4VSolid* source_shield_hole3 = new G4Tubs("Hole3",0.0,5.6*mm,20.0*mm,0.0*deg,360.0*deg);

    G4ThreeVector Translation;
    Translation.setX(0.0);
    Translation.setY(0.0);
    Translation.setZ(7.0*mm);

    G4RotationMatrix Rotation;
    Rotation.rotateY(90.0*deg);
    G4Transform3D Transform;
    Transform = G4Transform3D(Rotation, Translation);
    Rotation.rotateY(-90.0*deg);

    //G4SubtractionSolid* subtr_shield1 = new G4SubtractionSolid("Hole1",source_shield_tube1, source_shield_hole1,0,G4ThreeVector(0.0,0.0,19.0*mm));
    G4SubtractionSolid* subtr_shield2 = new G4SubtractionSolid("Hole2",source_shield_tube1, source_shield_hole2,0,G4ThreeVector(0.0,0.0,17.5*mm));
    G4SubtractionSolid* subtr_shield3 = new G4SubtractionSolid("Hole3",subtr_shield2, source_shield_hole3,Transform);

    source_shield1.logical = new G4LogicalVolume(subtr_shield3, source_shield1.material,"Shield1");
    source_shield1.physical = new G4PVPlacement(0,G4ThreeVector(),source_shield1.logical,"Shield1", world.logical, false,0);

    G4VSolid* source_shield_tube2 = new G4Tubs("Shield2",0.0,5.5*mm,20.0*mm,0.0*deg,360.0*deg);
    G4VSolid* source_shield_hole4 = new G4Tubs("Hole4",0.0,1.1*mm,7*mm,0.0*deg,360.0*deg);

    Translation.setX(0.0);
    Translation.setY(0.0);
    Translation.setZ(0.0);
    Rotation.rotateY(90.0*deg);
    Transform = G4Transform3D(Rotation, Translation);

    G4SubtractionSolid* subtr_shield4 = new G4SubtractionSolid("Hole4",source_shield_tube2, source_shield_hole4,Transform);

    Translation.setX(0.0);
    Translation.setY(0.0);
    Translation.setZ(7.0*mm);
    Transform = G4Transform3D(Rotation, Translation);

    source_shield2.logical = new G4LogicalVolume(subtr_shield4, source_shield2.material,"Shield2");
    source_shield2.physical = new G4PVPlacement(Transform,source_shield2.logical,"Shield2", world.logical, false,0);

    // Move to position

    G4ThreeVector Translation1, Translation2;
    G4RotationMatrix* Rotation1 = new G4RotationMatrix();
    G4RotationMatrix* Rotation2 = new G4RotationMatrix();

    if(_src_conf_id == 1)
    {
        Translation1.setX(-23.0*mm - 3.0*UA9Const::_bar_size_X/2 - UA9Const::_distBetweenBars - _src_shiftX);
        Translation1.setY(_src_shiftY);
        Translation1.setZ(-UA9Const::_bar_size_Z/2 + zCopyShift + _src_shiftZ);
        Rotation1->rotateY(-90.0*deg);

        Translation2.setX(-23.0*mm - 3.0*UA9Const::_bar_size_X/2 - UA9Const::_distBetweenBars - _src_shiftX + 7.0*mm);
        Translation2.setY(_src_shiftY);
        Translation2.setZ(-UA9Const::_bar_size_Z/2 + zCopyShift + _src_shiftZ);
        Rotation2->rotateY(0.0*deg);
    }
    if(_src_conf_id == 2)
    {
        Translation1.setX(23.0*mm + UA9Const::_bar_size_X/2 + _src_shiftX);
        Translation1.setY(_src_shiftY);
        Translation1.setZ(-UA9Const::_bar_size_Z/2 + _src_shiftZ);
        Rotation1->rotateY(90.0*deg);

        Translation2.setX(23.0*mm + UA9Const::_bar_size_X/2 + _src_shiftX - 7.0*mm);
        Translation2.setY(_src_shiftY);
        Translation2.setZ(-UA9Const::_bar_size_Z/2 + _src_shiftZ);
        Rotation2->rotateY(0.0*deg);
    }
    source_shield1.physical->SetRotation(Rotation1);
    source_shield1.physical->SetTranslation(Translation1);

    source_shield2.physical->SetRotation(Rotation2);
    source_shield2.physical->SetTranslation(Translation2);

    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//
    // SENSETIVE
    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//
    // Calculation
    G4ThreeVector TaMySen;
    G4double alphaRotSen = twopi/4.0 - UA9Const::angleDet;
    TaMySen.set((2.0*sec1_sizeX+distBetweenBars)/(TMath::Sin(UA9Const::angleDet)*2.0),0.0,senDet_sizeZ/2.0 + UA9Const::_air_layer*2 + viewport_sizeZ);
    TaMySen.rotateY(alphaRotSen);
    G4double zNewsen = TaMySen.getZ();
    G4double xNewsen = TaMySen.getX();

    sensitive.solid = new G4Box("Sensitive", senDet_sizeX/2.0, senDet_sizeY/2.0, senDet_sizeZ/2.0);
    sensitive.logical = new G4LogicalVolume(sensitive.solid, sensitive.material,"Sensitive");

    Ta.setX(sec1_sizeX/2.0 - xNewsen);
    Ta.setY(0.0);
    Ta.setZ(-sec1_sizeZ/2.0 - zNewsen);

    Ra.rotateY(alphaRotSen);
    Tr = G4Transform3D(Ra, Ta);
    Ra.rotateY(-alphaRotSen);
    new G4PVPlacement(Tr, sensitive.logical,"Sensitive", world.logical, false,0);
    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//
    // VIEWPORT
    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//
    // Calculation
    G4ThreeVector TaMyView;
    G4double alphaRotView = twopi/4.0 - UA9Const::angleDet;
    TaMyView.set(viewport_sizeX/2.0,0.0,viewport_sizeZ/2.0 + UA9Const::_air_layer);
    TaMyView.rotateY(alphaRotView);
    G4double zNewView = TaMyView.getZ();
    G4double xNewView = TaMyView.getX();

    viewport.solid = new G4Box("Viewport", viewport_sizeX/2.0, viewport_sizeY/2.0, viewport_sizeZ/2.0);
    viewport.logical = new G4LogicalVolume(viewport.solid, viewport.material,"Viewport");

    Ta.setX(sec1_sizeX/2.0 - xNewView);
    Ta.setY(0.0);
    Ta.setZ(-sec1_sizeZ/2.0 - zNewView);
    Ra.rotateY(alphaRotSen);
    Tr = G4Transform3D(Ra, Ta);
    Ra.rotateY(-alphaRotSen);
    new G4PVPlacement(Tr, viewport.logical,"Viewport", world.logical, false,0);

    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//
    // QUARTZ BARS
    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//

    G4ThreeVector TaMy;
    G4double alphaRot = twopi/4.0 - UA9Const::angleDet;
    TaMy.set(sec5_sizeX/2.0,0.0,sec5_sizeZ/2.0);
    TaMy.rotateY(alphaRot);
    G4double zNew = TaMy.getZ();
    G4double xNew = TaMy.getX();

    // Subtraction Solid
    G4VSolid *sec5Solid = new G4Box("Sector", sec5_sizeX/2.0, sec5_sizeY/2.0, sec5_sizeZ/2.0);
    G4LogicalVolume *sec5logical = new G4LogicalVolume(sec5Solid,sec1.material,"Sector");
    // Bar Solid
    sec1.solid = new G4Box("Sector", sec1_sizeX/2.0, sec1_sizeY/2.0, sec1_sizeZ/2.0);
    sec1.logical = new G4LogicalVolume(sec1.solid, sec1.material, "Sector");

    Ta.setX(sec1_sizeX/2.0 - xNew);
    Ta.setY(0.0);
    Ta.setZ(-sec1_sizeZ/2.0 - zNew);
    Ra.rotateY(alphaRot);
    Tr = G4Transform3D(Ra, Ta);
    Ra.rotateY(-alphaRot);
    G4SubtractionSolid* subtraction = new G4SubtractionSolid("Sector", sec1.solid, sec5Solid,Tr);


    G4LogicalVolume *resultlogical = new G4LogicalVolume(subtraction,sec1.material,"Sector");

    // CpFM2
    Ta.setX(0);
    Ta.setY(0);
    Ta.setZ(0);
    Tr = G4Transform3D(Ra, Ta);
    new G4PVPlacement(Tr, resultlogical, "Sector2", world.logical, true, 0);

    // CpFM1
    Ta.setX(xCopyShift);
    Ta.setY(0);
    Ta.setZ(zCopyShift);
    Tr = G4Transform3D(Ra, Ta);
    new G4PVPlacement(Tr, resultlogical, "Sector1", world.logical, true, 0);

    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//
    // ABSORBERS
    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//

    G4VSolid *absorber_subpart1_1 = new G4Box("Absorber", sec1_sizeX/2.0 + UA9Const::_absorber_thick, sec1_sizeY/2.0 + UA9Const::_absorber_thick, sec1_sizeZ/2.0);
    G4VSolid *absorber_subpart2_1 = new G4Box("Absorber", sec1_sizeX/2.0, sec1_sizeY/2.0, sec1_sizeZ);
    G4VSolid *absorber_subpart3_1 = new G4Box("Absorber", sec1_sizeX/2.0 - _abs_width1, (sec1_sizeY/2.0 + UA9Const::_absorber_thick)*2.0, sec1_sizeZ);
    G4VSolid *absorber_subpart4_1 = new G4Box("Absorber", (sec1_sizeX/2.0 + UA9Const::_absorber_thick)*2.0, sec1_sizeY/2.0 - _abs_width1, sec1_sizeZ);
    G4SubtractionSolid* absorber_subpart5_1 = new G4SubtractionSolid("Absorber", absorber_subpart1_1, absorber_subpart2_1,0,G4ThreeVector());
    G4SubtractionSolid* absorber_subpart6_1 = new G4SubtractionSolid("Absorber", absorber_subpart5_1, absorber_subpart3_1,0,G4ThreeVector());
    G4SubtractionSolid* absorber_subpart7_1 = new G4SubtractionSolid("Absorber", absorber_subpart6_1, absorber_subpart4_1,0,G4ThreeVector());
    G4VSolid *absorber_subpart8_1 = new G4Box("Absorber", sec5_sizeX, sec5_sizeY/2.0, sec5_sizeZ/2.0);

    G4VSolid *absorber_subpart1_2 = new G4Box("Absorber", sec1_sizeX/2.0 + UA9Const::_absorber_thick, sec1_sizeY/2.0 + UA9Const::_absorber_thick, sec1_sizeZ/2.0);
    G4VSolid *absorber_subpart2_2 = new G4Box("Absorber", sec1_sizeX/2.0, sec1_sizeY/2.0, sec1_sizeZ);
    G4VSolid *absorber_subpart3_2 = new G4Box("Absorber", sec1_sizeX/2.0 - _abs_width2, (sec1_sizeY/2.0 + UA9Const::_absorber_thick)*2.0, sec1_sizeZ);
    G4VSolid *absorber_subpart4_2 = new G4Box("Absorber", (sec1_sizeX/2.0 + UA9Const::_absorber_thick)*2.0, sec1_sizeY/2.0 - _abs_width2, sec1_sizeZ);
    G4SubtractionSolid* absorber_subpart5_2 = new G4SubtractionSolid("Absorber", absorber_subpart1_2, absorber_subpart2_2,0,G4ThreeVector());
    G4SubtractionSolid* absorber_subpart6_2 = new G4SubtractionSolid("Absorber", absorber_subpart5_2, absorber_subpart3_2,0,G4ThreeVector());
    G4SubtractionSolid* absorber_subpart7_2 = new G4SubtractionSolid("Absorber", absorber_subpart6_2, absorber_subpart4_2,0,G4ThreeVector());
    G4VSolid *absorber_subpart8_2 = new G4Box("Absorber", sec5_sizeX, sec5_sizeY/2.0, sec5_sizeZ/2.0);

    Ta.setX(sec1_sizeX/2.0 - xNew);
    Ta.setY(0.0);
    Ta.setZ(-sec1_sizeZ/2.0 - zNew);
    Ra.rotateY(alphaRot);
    Tr = G4Transform3D(Ra, Ta);
    Ra.rotateY(-alphaRot);
    absorber_1.solid = new G4SubtractionSolid("Sector", absorber_subpart7_1, absorber_subpart8_1,Tr);
    absorber_1.logical = new G4LogicalVolume(absorber_1.solid,absorber_1.material,"Absorber");
    absorber_2.solid = new G4SubtractionSolid("Sector", absorber_subpart7_2, absorber_subpart8_2,Tr);
    absorber_2.logical = new G4LogicalVolume(absorber_2.solid,absorber_2.material,"Absorber");

    // CpFM2
    Ta.setX(0);
    Ta.setY(0);
    Ta.setZ(0);
    Tr = G4Transform3D(Ra, Ta);
    new G4PVPlacement(Tr, absorber_2.logical, "Absorber", world.logical, true, 0);

    // CpFM1
    Ta.setX(xCopyShift);
    Ta.setY(0);
    Ta.setZ(zCopyShift);
    Tr = G4Transform3D(Ra, Ta);
    new G4PVPlacement(Tr, absorber_1.logical, "Absorber", world.logical, true, 0);

    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//
    // FIBERS
    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//

    fiberCorr.solid = new G4Tubs("Sector", fiberCorr_Rmin, fiberCorr_Rmax, fiberCorr_L/2.0, 0.0,  360.0*deg);
    fiberCorr.logical = new G4LogicalVolume(fiberCorr.solid, fiberCorr.material, "Sector");
    fiberClad.solid = new G4Tubs("SectorClad", fiberClad_Rmin, fiberClad_Rmax, fiberClad_L/2.0, 0.0,  360.0*deg);
    fiberClad.logical = new G4LogicalVolume(fiberClad.solid, fiberClad.material, "SectorClad");
    fiberCoat.solid = new G4Tubs("SectorCoat", fiberCoat_Rmin, fiberCoat_Rmax, fiberCoat_L/2.0, 0.0,  360.0*deg);
    fiberCoat.logical = new G4LogicalVolume(fiberCoat.solid, fiberCoat.material, "SectorCoat");


    int ii = 0;//along z dirrection
    int jj = 0;//along y dirrection

    G4double fiber_X0 = sec1_sizeZ/2.0*TMath::Sin(para_theta) - senDet_sizeX + fiberCoat_Rmax;
    G4double fiber_Y0 = -sec1_sizeY/2.0 + fiberCoat_Rmax;
    G4double fiber_Z0 = sec1_sizeZ/2.0*TMath::Cos(para_theta) + fiberCorr_L/2.0;
    G4double fiber_dd = 0.02*mm;
    //0
    for(jj = 0;jj<16;jj++){
      //for(ii = 0;ii<19;ii++){
      for(ii = 0;ii<10;ii++){
        if(jj%2 == 0){
      Ta.setX(fiber_X0 + ii*2*(fiberCoat_Rmax + fiber_dd));
        }
        else{
      Ta.setX(fiber_X0 + ii*2*(fiberCoat_Rmax + fiber_dd) - 2*(fiberCoat_Rmax + fiber_dd)*TMath::Sin(30.0*deg));
        }
        Ta.setY(fiber_Y0 + jj*2*(fiberCoat_Rmax + fiber_dd)*TMath::Cos(30.0*deg));
        Ta.setZ(fiber_Z0);
        Tr = G4Transform3D(Ra, Ta);
        //fiberCorr.physical = new G4PVPlacement( Tr, fiberCorr.logical, "Sector", world.logical, false, 0);
        //fiberClad.physical = new G4PVPlacement( Tr, fiberClad.logical, "SectorClad", world.logical, false, 0);
        //fiberCoat.physical = new G4PVPlacement( Tr, fiberCoat.logical, "SectorCoat", world.logical, false, 0);
      }
    }
    //1
    for(jj = 0;jj<16;jj++){
      for(ii = 0;ii<6;ii++){
        if(jj%2 == 0){
      Ta.setX(fiber_X0 + xCopyShift + ii*2*(fiberCoat_Rmax + fiber_dd));
        }
        else{
      Ta.setX(fiber_X0 + xCopyShift + ii*2*(fiberCoat_Rmax + fiber_dd) - 2*(fiberCoat_Rmax + fiber_dd)*TMath::Sin(30.0*deg));
        }
        Ta.setY(fiber_Y0 + jj*2*(fiberCoat_Rmax + fiber_dd)*TMath::Cos(30.0*deg));
        Ta.setZ(fiber_Z0);
        Tr = G4Transform3D(Ra, Ta);
        //fiberCorr.physical = new G4PVPlacement( Tr, fiberCorr.logical, "Sector", world.logical, false, 0);
        //fiberClad.physical = new G4PVPlacement( Tr, fiberClad.logical, "SectorClad", world.logical, false, 0);
        //fiberCoat.physical = new G4PVPlacement( Tr, fiberCoat.logical, "SectorCoat", world.logical, false, 0);
      }
    }
    //2
    for(jj = 0;jj<16;jj++){
      for(ii = 0;ii<6;ii++){
        if(jj%2 == 0){
      Ta.setX(fiber_X0 + 2*xCopyShift + ii*2*(fiberCoat_Rmax + fiber_dd));
        }
        else{
      Ta.setX(fiber_X0 + 2*xCopyShift + ii*2*(fiberCoat_Rmax + fiber_dd) - 2*(fiberCoat_Rmax + fiber_dd)*TMath::Sin(30.0*deg));
        }
        Ta.setY(fiber_Y0 + jj*2*(fiberCoat_Rmax + fiber_dd)*TMath::Cos(30.0*deg));
        Ta.setZ(fiber_Z0);
        Tr = G4Transform3D(Ra, Ta);
        //fiberCorr.physical = new G4PVPlacement( Tr, fiberCorr.logical, "Sector", world.logical, false, 0);
        //fiberClad.physical = new G4PVPlacement( Tr, fiberClad.logical, "SectorClad", world.logical, false, 0);
        //fiberCoat.physical = new G4PVPlacement( Tr, fiberCoat.logical, "SectorCoat", world.logical, false, 0);
      }
    }

    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//
    // VISUALIZATION ATTRIBUTES
    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//

    G4Color green       = G4Color(0., 1., 0.);
    G4Color blue        = G4Color(0., 0., 1.);
    G4Color red         = G4Color(1., 0., 0.);
    G4Color white       = G4Color(1., 1., 1.);
    G4Color brown       = G4Color(1., 0., 0.);

    worldVisAtt->SetColor(white);
    worldVisAtt->SetVisibility(true);
    quartzVisAtt->SetColor(blue);
    quartzVisAtt->SetVisibility(true);
    sensitiveVisAtt->SetColor(red);
    sensitiveVisAtt->SetVisibility(true);
    viewportVisAtt->SetColor(white);
    viewportVisAtt->SetVisibility(true);
    sourceShieldVisAtt1->SetColor(white);
    sourceShieldVisAtt1->SetVisibility(true);
    sourceShieldVisAtt2->SetColor(brown);
    sourceShieldVisAtt2->SetVisibility(true);
    absorberVisAtt->SetColor(green);
    absorberVisAtt->SetVisibility(true);

    sec1.logical->SetVisAttributes(quartzVisAtt);
    resultlogical->SetVisAttributes(quartzVisAtt);
    sec5logical->SetVisAttributes(quartzVisAtt);

    sensitive.logical->SetVisAttributes(sensitiveVisAtt);

    fiberCorr.logical->SetVisAttributes(quartzVisAtt);
    fiberClad.logical->SetVisAttributes(quartzVisAtt);
    fiberCoat.logical->SetVisAttributes(absVisAtt);
    viewport.logical->SetVisAttributes(viewportVisAtt);

    source_shield1.logical->SetVisAttributes(sourceShieldVisAtt1);
    source_shield2.logical->SetVisAttributes(sourceShieldVisAtt2);    

    absorber_1.logical->SetVisAttributes(absorberVisAtt);
    absorber_2.logical->SetVisAttributes(absorberVisAtt);

    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//
    // OPTICAL BORDERS
    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//

    // Surface for killing photons at borders
    const G4int num1 = 2;
    G4double Ephoton[num1] = {1.5*eV, 5.8*eV};

    G4OpticalSurface* OpVolumeKillSurface = new G4OpticalSurface("VolumeKillSurface");
    OpVolumeKillSurface->SetType(dielectric_metal);
    OpVolumeKillSurface->SetFinish(polished);
    OpVolumeKillSurface->SetModel(glisur);

    G4double ReflectivityKill[num1] = {0., 0.};
    G4double EfficiencyKill[num1] = {1., 1.};
    G4MaterialPropertiesTable* VolumeKill = new G4MaterialPropertiesTable();
    VolumeKill->AddProperty("REFLECTIVITY", Ephoton, ReflectivityKill, num1);
    VolumeKill->AddProperty("EFFICIENCY",   Ephoton, EfficiencyKill,   num1);
    OpVolumeKillSurface->SetMaterialPropertiesTable(VolumeKill);
    new G4LogicalSkinSurface("SensitiveSurface", sensitive.logical, OpVolumeKillSurface);

    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//
    // SENSITIVE DETECTOR
    //-----------------------------------------------------------------------------------------------////-----------------------------------------------------------------------------------------------//

    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    SensitiveDetector* aSD = new SensitiveDetector("fTOF");
    SDman->AddNewDetector(aSD);

    G4cout << *(G4Material::GetMaterialTable()) << G4endl;

    G4double maxStep   = 0.1*mm;
    G4double maxLength = 5.0*m;
    G4double maxTime   = 20.0*ns;
    G4double minEkin   = 1.0/100*MeV;
    G4double mionRang  = 0.01*mm;
    stepLimit = new G4UserLimits(maxStep,maxLength,maxTime,minEkin,mionRang);
    sec1.logical->SetUserLimits(stepLimit);
    viewport.logical->SetUserLimits(stepLimit);

    return world.physical;
}

void DetectorConstruction::SetSrcConfID(G4int value)
{
    _src_conf_id = value;
}

void DetectorConstruction::SetSrcShiftX(G4double value)
{
    _src_shiftX = value;
}

void DetectorConstruction::SetSrcShiftY(G4double value)
{
    _src_shiftY = value;
}

void DetectorConstruction::SetSrcShiftZ(G4double value)
{
    _src_shiftZ = value;
}

void DetectorConstruction::SetAbsWidth1(G4double value)
{
    _abs_width1 = value;
}

void DetectorConstruction::SetAbsWidth2(G4double value)
{
    _abs_width2 = value;
}

void DetectorConstruction::ModifyGeometry()
{
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}
