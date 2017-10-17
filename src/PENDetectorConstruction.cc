#include "PENDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"

#include <iostream>
#include <fstream>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENDetectorConstruction::PENDetectorConstruction()
 : G4VUserDetectorConstruction()
{
  fExpHall_x = fExpHall_y = fExpHall_z = 0.5*m;
  fTank_x    = fTank_y    = fTank_z    =  0.5*m;
  fBubble_x  = fBubble_y  = fBubble_z  =  0.5*m;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENDetectorConstruction::~PENDetectorConstruction(){;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* PENDetectorConstruction::Construct()
{

// ------------- Materials -------------

  G4double a, z, density;
  G4int nelements;

// Air
//
  G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);

  G4Material* air = new G4Material("Air", density=1.29*mg/cm3, nelements=2);
  air->AddElement(N, 70.*perCent);
  air->AddElement(O, 30.*perCent);

  G4NistManager* man = G4NistManager::Instance();

  G4Material* glass = man->FindOrBuildMaterial("G4_Pyrex_Glass");

  G4MaterialPropertiesTable* glassMPT = new G4MaterialPropertiesTable();
  G4double photonEnergy2[] = {2.479684*eV, 2.610194*eV, 2.755204*eV, 2.883353*eV, 2.917275*eV, 3.099605*eV};
  G4double refractiveIndex4[] = {1.52, 1.52, 1.52, 1.52, 1.52, 1.52};
  G4double absorption[] = {10*m,10*m,10*m,10*m,10*m,10*m};
  assert(sizeof(refractiveIndex4) == sizeof(photonEnergy2));
  glassMPT->AddProperty("RINDEX",photonEnergy2,refractiveIndex4,6)->SetSpline(true);
  glassMPT->AddProperty("ABSLENGTH",photonEnergy2,absorption,6);
  glass->SetMaterialPropertiesTable(glassMPT);

  G4Material* aluminium = man->FindOrBuildMaterial("G4_Al");
// Water
//
  G4Element* H = new G4Element("Hydrogen", "H", z=1 , a=1.01*g/mole);

  G4Material* water = new G4Material("Water", density= 1.0*g/cm3, nelements=2);
  water->AddElement(H, 2);
  water->AddElement(O, 1);

  G4Element* C = new G4Element("Carbon", "C", z=12, a=12*g/mole);

  //
  // PEN
  //

  G4Material* PEN = new G4Material("PEN", density= 1.3*g/cm3, nelements=3);
  G4int number_of_atoms;
	PEN->AddElement(O, number_of_atoms=4);
	PEN->AddElement(H, number_of_atoms=10);
	PEN->AddElement(C, number_of_atoms=14);

  G4double wavelength;
  char filler;
  G4double varabsorlength;
  G4double ems;
  G4double rindex;

  G4double absEnergy[101]  = {0};
  G4double abs[101]={0};
  G4double emission[101]={0};
  G4double rIndex[101]={0};

  G4int absEntries = 0;
  ifstream ReadAbs;

//  G4String abs_file = "../input_files/OldPen.csv";
//  G4String abs_file = "../input_files/highAbs.csv";
    G4String abs_file = "../input_files/Exp4.csv";
  ReadAbs.open(abs_file);

  if(ReadAbs.is_open())
  {
    while(!ReadAbs.eof())
    {
      ReadAbs>>wavelength>>filler>>varabsorlength>>filler>>ems>>filler>>rindex;
      if(ReadAbs.eof()){
        break;
      }
      cout<<absEntries << " " <<wavelength << " " << varabsorlength << " " << ems <<endl;
      absEnergy[absEntries] = (1240/wavelength)*eV;
      abs[absEntries] = varabsorlength*mm;
      emission[absEntries] = ems;
      rIndex[absEntries] = rindex;
      absEntries++;
    }
  }
  else G4cout<<"Error opening file: " <<abs_file<<G4endl;
  ReadAbs.close();
  absEntries--;

  const G4int nEntries1 = sizeof(absEnergy)/sizeof(G4double);
  assert(sizeof(rIndex) == sizeof(absEnergy));
  assert(sizeof(abs) == sizeof(absEnergy));
  assert(sizeof(emission) == sizeof(absEnergy));

  G4MaterialPropertiesTable* penMPT = new G4MaterialPropertiesTable();

  penMPT->AddProperty("RINDEX",       absEnergy, rIndex, nEntries1)->SetSpline(true);
  penMPT->AddProperty("ABSLENGTH",    absEnergy, abs, nEntries1)->SetSpline(true);
  penMPT->AddProperty("FASTCOMPONENT",absEnergy, emission, nEntries1)->SetSpline(true);
  penMPT->AddProperty("SLOWCOMPONENT",absEnergy, emission, nEntries1)->SetSpline(true);

  penMPT->AddConstProperty("SCINTILLATIONYIELD",100./MeV);
  penMPT->AddConstProperty("RESOLUTIONSCALE",1.0);
  penMPT->AddConstProperty("FASTTIMECONSTANT", 5.198*ns);
  penMPT->AddConstProperty("SLOWTIMECONSTANT",24.336*ns);
  penMPT->AddConstProperty("YIELDRATIO",0.05);

  PEN->SetMaterialPropertiesTable(penMPT);

  //
  // Air
  //
    G4double refractiveIndex2[] =
            { 1.00,1.00,1.00,1.00,1.00,1.00 };
        //  { 1.50,1.50,1.50,1.50,1.50,1.50 };
    G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
    myMPT2->AddProperty("RINDEX", absEnergy, refractiveIndex2, 6);

    air->SetMaterialPropertiesTable(myMPT2);

//
// ------------- Volumes --------------

// The experimental Hall
//
  G4Box* expHall_box = new G4Box("World",fExpHall_x,fExpHall_y,fExpHall_z);

  G4LogicalVolume* expHall_log
    = new G4LogicalVolume(expHall_box,air,"World",0,0,0);

  G4VPhysicalVolume* expHall_phys
    = new G4PVPlacement(0,G4ThreeVector(),expHall_log,"World",0,false,0);


// PEN Tile
  G4Box* penTile_box = new G4Box("Tile", 17.5*mm,17.5*mm, 2.5*mm);

  G4LogicalVolume* penTile_log = new G4LogicalVolume(penTile_box,PEN, "Tile",0,0,0);
  G4RotationMatrix* rot = new G4RotationMatrix();
  rot->rotateX(-45*deg);
  G4VPhysicalVolume* penTile_phys = new G4PVPlacement(rot,G4ThreeVector(),penTile_log,"Tile",expHall_log,false,0);

// PMT
  G4double innerRadius_pmt = 0.*cm;
  G4double outerRadius_pmt = 5.3*cm;
  G4double outerRadius_cath = 4.6*cm;
  G4double height_pmt = 63.5*mm;
  G4double height_cath = 62.*mm;
  G4double startAngle_pmt = 0.*deg;
  G4double spanningAngle_pmt = 360.*deg;

  G4Tubs* pmt = new G4Tubs("pmt_tube",innerRadius_pmt,outerRadius_pmt, height_pmt,startAngle_pmt,spanningAngle_pmt);

  //the "photocathode" is a metal slab at the back of the glass that
  //is only a very rough approximation of the real thing since it only
  //absorbs or detects the photons based on the efficiency set below


  G4LogicalVolume* pmt_log = new G4LogicalVolume(pmt,glass, "pmt_log");
  G4RotationMatrix* rotm = new G4RotationMatrix();
  rotm->rotateX(90*deg);
  G4VPhysicalVolume* pmt_phys = new G4PVPlacement(rotm,G4ThreeVector(0,81*mm,0),pmt_log,"pmt",expHall_log,false,0);

  G4Tubs* Photocath = new G4Tubs("photocath_tube",innerRadius_pmt,outerRadius_cath,
                          height_cath,startAngle_pmt,spanningAngle_pmt);
  G4LogicalVolume* photocath_log = new G4LogicalVolume(Photocath,
                                       aluminium,
                                       "photocath_log");

  G4VPhysicalVolume* cath_phys = new G4PVPlacement(0,G4ThreeVector(0,0,0),
  photocath_log,"photocath",pmt_log,false,0);

  G4double photocath_energy[] = {2.479684*eV, 2.610194*eV, 2.755204*eV, 2.883353*eV , 2.917275*eV, 3.099605*eV};
  G4double photocath_EFF[]={0.25,0.23,0.22,0.20,0.19,0.16};
  assert(sizeof(photocatch_EFF) == sizeof(photocath_energy));
  G4double photocath_REFL[] = {0.,0.,0.,0.,0.,0.};
  assert(sizeof(photocath_REFL) == sizeof(photocath_energy));

  G4OpticalSurface* photocath_optsurf = new G4OpticalSurface("photocath_opsurf",glisur,polished, dielectric_metal);
  G4MaterialPropertiesTable* photocath_MT = new G4MaterialPropertiesTable();
  photocath_MT->AddProperty("EFFICIENCY", photocath_energy, photocath_EFF,6);
  photocath_MT->AddProperty("REFLECTIVITY", photocath_energy, photocath_REFL,6);
  photocath_optsurf->SetMaterialPropertiesTable(photocath_MT);
  new G4LogicalSkinSurface("photocath_surf",photocath_log,photocath_optsurf);


//-------------- Define Sensative Detector --------------

//  boundaryStatus=boundary->GetStatus();

// ------------- Surfaces --------------
//
// Water Tank
//
/*
  G4OpticalSurface* opWaterSurface = new G4OpticalSurface("WaterSurface");
  opWaterSurface->SetType(dielectric_dielectric);
  opWaterSurface->SetFinish(ground);
  opWaterSurface->SetModel(unified);

  new G4LogicalBorderSurface("WaterSurface",
                                 waterTank_phys,expHall_phys,opWaterSurface);
*/
// Air Bubble
//
/*
  G4OpticalSurface* opAirSurface = new G4OpticalSurface("AirSurface");
  opAirSurface->SetType(dielectric_dielectric);
  opAirSurface->SetFinish(polished);
  opAirSurface->SetModel(glisur);

  G4LogicalSkinSurface* airSurface =
          new G4LogicalSkinSurface("AirSurface", bubbleAir_log, opAirSurface);

  G4OpticalSurface* opticalSurface = dynamic_cast <G4OpticalSurface*>
        (airSurface->GetSurface(bubbleAir_log)->GetSurfaceProperty());

  if (opticalSurface) opticalSurface->DumpInfo();

//
// Generate & Add Material Properties Table attached to the optical surfaces
//
  const G4int num = 2;
  G4double ephoton[num] = {2.034*eV, 4.136*eV};

  //OpticalWaterSurface
  G4double refractiveIndex[num] = {1.35, 1.40};
  G4double specularLobe[num]    = {0.3, 0.3};
  G4double specularSpike[num]   = {0.2, 0.2};
  G4double backScatter[num]     = {0.2, 0.2};

  G4MaterialPropertiesTable* myST1 = new G4MaterialPropertiesTable();

  myST1->AddProperty("RINDEX",                ephoton, refractiveIndex, num);
  myST1->AddProperty("SPECULARLOBECONSTANT",  ephoton, specularLobe,    num);
  myST1->AddProperty("SPECULARSPIKECONSTANT", ephoton, specularSpike,   num);
  myST1->AddProperty("BACKSCATTERCONSTANT",   ephoton, backScatter,     num);

  G4cout << "Water Surface G4MaterialPropertiesTable" << G4endl;
  myST1->DumpTable();

//  opWaterSurface->SetMaterialPropertiesTable(myST1);

  //OpticalAirSurface
  G4double reflectivity[num] = {0.3, 0.5};
  G4double efficiency[num]   = {0.8, 1.0};

  G4MaterialPropertiesTable *myST2 = new G4MaterialPropertiesTable();

  myST2->AddProperty("REFLECTIVITY", ephoton, reflectivity, num);
  myST2->AddProperty("EFFICIENCY",   ephoton, efficiency,   num);

  G4cout << "Air Surface G4MaterialPropertiesTable" << G4endl;
  myST2->DumpTable();

  opAirSurface->SetMaterialPropertiesTable(myST2);
*/
//always return the physical World
  return expHall_phys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
