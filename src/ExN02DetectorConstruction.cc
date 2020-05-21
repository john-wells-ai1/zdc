 
#include "ExN02DetectorConstruction.hh"
#include "ExN02DetectorMessenger.hh"
//#include "ExN02ChamberParameterisation.hh"
//#include "ExN02MagneticField.hh"
#include "CalorimeterSD.hh"
#include "Analysis.hh"
#include "HistoManager.hh"

#include "G4Material.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4tgrMaterialFactory.hh"
#include "G4EmCalculator.hh"
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SDManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4PVReplica.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4PVPlacement.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"




#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExN02DetectorConstruction::ExN02DetectorConstruction()
:solidWorld(0),  logicWorld(0),  physiWorld(0), 
 RPD_Quartz(0), RPD_Scil(0), stepLimit(0), aCalorimeterSD(0),
 fWorldLength(0.)
{
//  fpMagField = new ExN02MagneticField();
  detectorMessenger = new ExN02DetectorMessenger(this);
    Analysis::GetInstance()->SetDetectorConstruction(this);
    
    wlsfiberOrigin = 0.0;
    wlsfiberZ     = 1.*m;
    surfaceRoughness = 1;
    XYRatio = 1.0;
    mirrorToggle = true;
    mirrorPolish = 1.;
    mirrorReflectivity = 0.0;
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
ExN02DetectorConstruction::~ExN02DetectorConstruction()
{
//  delete fpMagField;
  delete stepLimit;
//  delete chamberParam;
  delete detectorMessenger;             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* ExN02DetectorConstruction::Construct()
{
//--------- Material definition ---------

  G4double a, z;
  G4double density, temperature, pressure;
  G4int nel;
  G4double fractionmass;
  G4double SizeMod=1.0;

  //Air
  G4Element* H = new G4Element("Hydrogen", "H", z=1., a=1.01*g/mole);
  G4Element* N = new G4Element("Nitrogen", "N", z=7., a= 14.01*g/mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8., a= 16.00*g/mole);
  G4Element* Si = new G4Element("Silicon" , "Si", z=14., a= 28.08*g/mole);
  G4Element* C = new G4Element("Carbon" , "C", z=6., a=12.00*g/mole);
//  G4Element* Cu = new G4Element("Copper", "Cu", z=29.,a=63.55*g/mole);
   


//  G4Material* Copper = new G4Material("Cu", z=29.,a=63.55*g/mole,density= 8.960*g/cm3);
//  Cu->AddElement(Cu, 100*perCent);

  
  G4Material* Vacuum =
  new G4Material("Vacuum", z=1., a=1.01*g/mole,density= universe_mean_density,
                           kStateGas, 2.73*kelvin, 3.e-18*pascal);
  //Lead

    
  //Xenon gas
  G4Material* Xenon = 
  new G4Material("XenonGas", z=54., a=131.29*g/mole, density= 5.458*mg/cm3,
                 kStateGas, temperature= 293.15*kelvin, pressure= 1*atmosphere);

  //Quartz
  const G4int NUMENTRIES = 25;
  G4Material* pQuartz = new G4Material("Quartz", 2.200*g/cm3, 2);
  pQuartz->AddElement(Si,1);
  pQuartz->AddElement(O,2);

  const G4int nEntriesWLS = 50;

  G4double PhotonEnergy[nEntriesWLS] =
  {2.00*eV,2.03*eV,2.06*eV,2.09*eV,2.12*eV,
   2.15*eV,2.18*eV,2.21*eV,2.24*eV,2.27*eV,
   2.30*eV,2.33*eV,2.36*eV,2.39*eV,2.42*eV,
   2.45*eV,2.48*eV,2.51*eV,2.54*eV,2.57*eV,
   2.60*eV,2.63*eV,2.66*eV,2.69*eV,2.72*eV,
   2.75*eV,2.78*eV,2.81*eV,2.84*eV,2.87*eV,
   2.90*eV,2.93*eV,2.96*eV,2.99*eV,3.02*eV,
   3.05*eV,3.08*eV,3.11*eV,3.14*eV,3.17*eV,
   3.20*eV,3.23*eV,3.26*eV,3.29*eV,3.32*eV,
   3.35*eV,3.38*eV,3.41*eV,3.44*eV,3.47*eV};

  G4double quartz_RIND[nEntriesWLS] = {1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585,
 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585,
 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585,
 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585,
 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585,
 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585, 1.4585}; // ref index


  G4double quartz_ABSL[nEntriesWLS] = {300.00*cm,
320.00*cm,
340.00*cm,
360.00*cm,
380.00*cm,
400.00*cm,
420.00*cm,
440.00*cm,
460.00*cm,
480.00*cm,
500.00*cm,
520.00*cm,
540.00*cm,
560.00*cm,
580.00*cm,
600.00*cm,
620.00*cm,
640.00*cm,
660.00*cm,
680.00*cm,
700.00*cm,
720.00*cm,
740.00*cm,
760.00*cm,
780.00*cm,
800.00*cm,
820.00*cm,
840.00*cm,
860.00*cm,
880.00*cm,
900.00*cm,
920.00*cm,
940.00*cm,
960.00*cm,
980.00*cm,
1000.00*cm,
1020.00*cm,
1040.00*cm,
1060.00*cm,
1080.00*cm,
1100.00*cm,
1120.00*cm,
1140.00*cm,
1160.00*cm,
1180.00*cm,
1200.00*cm,
1220.00*cm,
1240.00*cm,
1260.00*cm,
1280.00*cm}; // atten length  (EXCEL used)

  G4MaterialPropertiesTable *proQuartz = new G4MaterialPropertiesTable();
  proQuartz->AddProperty("RINDEX",PhotonEnergy,quartz_RIND,nEntriesWLS);
  proQuartz->AddProperty("ABSLENGTH",PhotonEnergy,quartz_ABSL,nEntriesWLS);
  pQuartz->SetMaterialPropertiesTable(proQuartz);
  
  const G4int EMNUMENTRIES = 3;
  G4Material* EM_Quartz = new G4Material("EMQuartz", 2.200*g/cm3, 2);
  EM_Quartz->AddElement(Si,1);
  EM_Quartz->AddElement(O,2);
  
G4double EMZSize=33*mm/SizeMod;
 //Tungsten
 
 G4NistManager* nistMan = G4NistManager::Instance();
  nistMan->SetVerbose(1);
  
  Air = nistMan->FindOrBuildMaterial("G4_AIR");
  Al = nistMan->FindOrBuildMaterial("G4_Al");   	//G4_Al
  Cu = nistMan->FindOrBuildMaterial("G4_Cu");
  Pb = nistMan->FindOrBuildMaterial("G4_Pb");  
  W = nistMan->FindOrBuildMaterial("G4_W");   
 
//////////////////////////////////////////////////
//////////////////////////////////////////////////
  G4int polyeth = 1;
  G4int nC_eth = 2*polyeth;
  G4int nH_eth = 4*polyeth;
  
  G4Material* Pethylene = new G4Material("Pethylene", density=1200*kg/m3,2);
  Pethylene->AddElement(H,nH_eth);
  Pethylene->AddElement(C,nC_eth);
  
// Aluminum Properties

  const G4int numAl =50;  

  G4double AllPhotonEnergies[nEntriesWLS] = {2.00*eV,
2.04*eV,
2.07*eV,
2.11*eV,
2.15*eV,
2.18*eV,
2.22*eV,
2.26*eV,
2.29*eV,
2.33*eV,
2.37*eV,
2.40*eV,
2.44*eV,
2.48*eV,
2.51*eV,
2.55*eV,
2.59*eV,
2.62*eV,
2.66*eV,
2.70*eV,
2.74*eV,
2.77*eV,
2.81*eV,
2.85*eV,
2.88*eV,
2.92*eV,
2.96*eV,
2.99*eV,
3.03*eV,
3.07*eV,
3.10*eV,
3.14*eV,
3.18*eV,
3.21*eV,
3.25*eV,
3.29*eV,
3.32*eV,
3.36*eV,
3.40*eV,
3.43*eV,
3.47*eV,
5.0*eV,
25.0*eV,
100.0*eV,
1000.00*eV,
10000.0*eV,
25000.0*eV,
50000.0*eV,
250000.*eV,
1000000.*eV};
 
//  G4double Al_PP[numAl] = {5.0*eV, 6.69*eV, 7.50*eV};
  G4double Al_refl[numAl] = {0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89,
					    0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89,
					    0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89,
					    0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89, 0.89,
					    0.89, 0.89, 0.89, 0.89, 0.79, 0.69, 0.49, 0.29, 0.19, 0.09};	// Hard Cut

//					    0.89, 0.89, 0.89, 0.89, 0.84, 0.79, 0.74, 0.69, 0.60, 0.50};	//Mid Cut



//  G4double Al_refr[numAl] = {0.49, 0.81, 1.26};
  G4double Al_ABSL[numAl] = {0.01*mm, 0.01*mm, 0.01*mm,0.01*mm, 0.01*mm, 0.01*mm,0.01*mm, 0.01*mm, 0.01*mm,0.01*mm,
					       0.01*mm, 0.01*mm, 0.01*mm,0.01*mm, 0.01*mm, 0.01*mm,0.01*mm, 0.01*mm, 0.01*mm,0.01*mm,
					       0.01*mm, 0.01*mm, 0.01*mm,0.01*mm, 0.01*mm, 0.01*mm,0.01*mm, 0.01*mm, 0.01*mm,0.01*mm,
					       0.01*mm, 0.01*mm, 0.01*mm,0.01*mm, 0.01*mm, 0.01*mm,0.01*mm, 0.01*mm, 0.01*mm,0.01*mm,
				               0.01*mm, 0.10*mm, 1.00*mm,0.01*m,    0.01*m,    0.10*m,  0.10*m,   1.00*m,    10.0*m,   10.0*m};
//	Soft Cut:
//					       0.01*mm, 0.01*mm, 0.01*mm,0.10*mm, 1.00*mm, 1.00*cm,  10.0*cm, 0.10*m,  1.0*m  ,1.0*m};


  G4MaterialPropertiesTable* mptAl = new G4MaterialPropertiesTable();
//  mptAl->AddProperty("RINDEX",Al_PP,Al_refr,numAl);
  mptAl->AddProperty("REFLECTIVITY",AllPhotonEnergies,Al_refl,numAl);
  mptAl->AddProperty("ABSLENGTH",AllPhotonEnergies,Al_ABSL,numAl);
  Al->SetMaterialPropertiesTable(mptAl);


  const G4int nEntries = 25;

     G4double RefractiveIndex[nEntriesWLS] =
  { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
    1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00
    , 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00
     , 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00};
  G4MaterialPropertiesTable* MPT = new G4MaterialPropertiesTable();
  MPT->AddProperty("RINDEX", PhotonEnergy, RefractiveIndex, nEntriesWLS);
  Air->SetMaterialPropertiesTable(MPT);


  G4double RefractiveIndexClad1[nEntriesWLS] =
  { 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49,
    1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49, 1.49};
 
 G4double AbsClad[nEntriesWLS] =
  {20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,
   20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m,20.0*m};


  // Add entries into properties table
  G4MaterialPropertiesTable* MPTClad1 = new G4MaterialPropertiesTable();
  MPTClad1->AddProperty("RINDEX",PhotonEnergy,RefractiveIndexClad1,nEntriesWLS);
  MPTClad1->AddProperty("ABSLENGTH",PhotonEnergy,AbsClad,nEntriesWLS);

  Pethylene->SetMaterialPropertiesTable(MPTClad1);
  
  G4int pmma = 1;
  G4int nC_pmma = 5;
  G4int nH_pmma = 8;
  G4int nO_pmma = 2;
  G4double d= 1.18*g/cm3;
  G4int natoms, ncomponents;
  G4Material* PMMA = new G4Material("PMMA",d,ncomponents=3);
  PMMA->AddElement(C,nC_pmma);
  PMMA->AddElement(H,nH_pmma);
  PMMA->AddElement(O,nO_pmma);  
  
    G4double RefractiveIndexWLSfiber[nEntriesWLS] =
  { 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60,
    1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60, 1.60};

  G4double AbsWLSfiber[nEntriesWLS] =
  {5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,
   5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,
   5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,5.40*m,1.10*m,
   1.10*m,1.10*m,1.10*m,1.10*m,1.10*m,1.10*m, 1.*mm, 1.*mm, 1.*mm, 1.*mm,
    1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm, 1.*mm};


  G4double EmissionFib[nEntriesWLS] =
  {0.05, 0.10, 0.30, 0.50, 0.75, 1.00, 1.50, 1.85, 2.30, 2.75,
   3.25, 3.80, 4.50, 5.20, 6.00, 7.00, 8.50, 9.50, 11.1, 12.4,
   12.9, 13.0, 12.8, 12.3, 11.1, 11.0, 12.0, 11.0, 17.0, 16.9,
   15.0, 9.00, 2.50, 1.00, 0.05, 0.00, 0.00, 0.00, 0.00, 0.00,
   0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00, 0.00};

  // Add entries into properties table
  G4MaterialPropertiesTable* MPTWLSfiber = new G4MaterialPropertiesTable();
  MPTWLSfiber->
           AddProperty("RINDEX",PhotonEnergy,RefractiveIndexWLSfiber,nEntriesWLS);
  // MPTWLSfiber->AddProperty("ABSLENGTH",PhotonEnergy,AbsWLSfiber,nEntries);
  MPTWLSfiber->AddProperty("WLSABSLENGTH",PhotonEnergy,AbsWLSfiber,nEntriesWLS);
  MPTWLSfiber->AddProperty("WLSCOMPONENT",PhotonEnergy,EmissionFib,nEntriesWLS);
  MPTWLSfiber->AddConstProperty("WLSTIMECONSTANT", 0.5*ns);

  PMMA->SetMaterialPropertiesTable(MPTWLSfiber);
  
  //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////  



G4Material* Grease = new G4Material("Grease", density=1.0*g/cm3,3);
  Grease->AddElement(C,1);
  Grease->AddElement(H,1);
  Grease->AddElement(O,1);
/*  const G4int OpG_NUMENTRIES_2 = 2;
  G4double OpG_RIND_Energy[OpG_NUMENTRIES_2]    = { 4.08*eV , 1.84*eV };
  G4double OpG_RIND_INDEX[OpG_NUMENTRIES_2]     = { 1.5, 1.5};
  G4double OpG_ABS_Energy[OpG_NUMENTRIES_2]     = { 4.08*eV , 1.84*eV };
  G4double OpG_ABS_LENGTH[OpG_NUMENTRIES_2]     = { 50.*m, 50.*m};
  G4MaterialPropertiesTable* OpG_mt = new G4MaterialPropertiesTable();
  OpG_mt->AddProperty("RINDEX",        OpG_RIND_Energy, OpG_RIND_INDEX,     OpG_NUMENTRIES_2);
  OpG_mt->AddProperty("ABSLENGTH",     OpG_ABS_Energy,  OpG_ABS_LENGTH,     OpG_NUMENTRIES_2);
  Grease->SetMaterialPropertiesTable(OpG_mt);

 */

   G4double RefractiveIndexSilicone[nEntriesWLS] =
   { 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46,
     1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46, 1.46};

  // Add entries into properties table
  G4MaterialPropertiesTable* MPTSilicone = new G4MaterialPropertiesTable();
  MPTSilicone->
           AddProperty("RINDEX",PhotonEnergy,RefractiveIndexSilicone,nEntriesWLS);
  MPTSilicone->AddProperty("ABSLENGTH",PhotonEnergy,AbsClad,nEntriesWLS);

  Grease->SetMaterialPropertiesTable(MPTSilicone);
 
  
  // Print all the materials defined.
  //
  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

//--------- Sizes of the principal geometrical components (solids)  ---------
  
  
//  fTargetLength  = 0.1 * cm;                        // Full length of Target
  
  //TargetMater  = Pb;
  ////////////////////////////
  ///RPD AND EM Specific Stuff
  ///////////////////////////
  
//MATERIALS//////////////////
  RPD_Quartz = pQuartz;
  EM_Absorber = W;
  EM_Fiber = EM_Quartz;
  RPD_Material=pQuartz;    //pQuartz/Air
  Cu_Material=Cu;
///////////////////////

   
  ////EM LENGTHS////////
  G4double EMXsize=4.6*cm/SizeMod;
  G4double EMYsize=5.0*cm/SizeMod;
  G4double EMZsize=33.0*mm/SizeMod;
  G4double zdcEMXDelta=0.8727*mm;
 G4double EMZInitial=-48.95*mm/SizeMod;
 G4double EMSeperation=1.37*mm/SizeMod;
 G4double EMZDelta=2.74*mm/SizeMod;
  ////////////////////////////////

  
  ///RPD LENGTHS/////////////////////
  G4double RPDXsize=0.9999*cm/SizeMod;
  G4double RPDYsize=0.9999*cm/SizeMod;
  G4double RPDZsize=0.5*cm/SizeMod;
  G4double RPDXInitial=-3.0*cm/SizeMod;
  G4double RPDXDelta=2.1*cm/SizeMod;
  G4double RPDYInitial=3.0*cm/SizeMod;
  G4double RPDYDelta=2.1*cm/SizeMod;
  ////////////////////////////////////////
  ////WLS variables//////////////
  G4double innerRadius = 0.*mm;
  G4double outerRadius = .5*mm;
  G4double CladinnerRadius = 0.*mm;
  G4double CladouterRadius = .499*mm;
  G4double WLSinnerRadius = 0.*mm;
  G4double WLSouterRadius = CladouterRadius*0.97;	//0.9
//  G4double WLSouterRadius = CladinnerRadius*.99;  
  G4double hz = 5.*cm;
  G4double startAngle = 0.*deg;
  G4double spanningAngle = 360.*deg;
  G4RotationMatrix* myRotation = new G4RotationMatrix();
   myRotation->rotateX(90.*deg);
   myRotation->rotateY(0.*deg);
   myRotation->rotateZ(0.*rad);

  fWorldLength = 35 * cm/SizeMod;
  
      
//--------- Definitions of Solids, Logical Volumes, Physical Volumes ---------
  
  //------------------------------ 
  // World
  //------------------------------ 

  G4double HalfWorldLength = 1*fWorldLength;
 
  G4GeometryManager::GetInstance()->SetWorldMaximumExtent(fWorldLength);
  G4cout << "Computed tolerance = "
         << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
         << " mm" << G4endl;

  solidWorld= new G4Box("world",HalfWorldLength,HalfWorldLength,HalfWorldLength);
  logicWorld= new G4LogicalVolume( solidWorld, Air, "World", 0, 0, 0);
  
  //  Must place the World Physical volume unrotated at (0,0,0).
  // 
  physiWorld = new G4PVPlacement(0,               // no rotation
                                 G4ThreeVector(), // at (0,0,0)
                                 logicWorld,      // its logical volume
				 "World",         // its name
                                 0,               // its mother  volume
                                 false,           // no boolean operations
                                 0);              // copy number
				 

  //--------------------------
  // EM Envelope
  //--------------------------
  G4ThreeVector positionEMEnvelope = G4ThreeVector(0,0,0); //will need to move this out of the way of other stuff
  solidEMEnvelope = new G4Box("emenvelope",4.9*cm/SizeMod,6.26*cm/SizeMod,50*mm/SizeMod);
  logicEMEnvelope = new G4LogicalVolume(solidEMEnvelope, Air, "EMEnvelope");
  physiEMEnvelope = new G4PVPlacement(0,
				      positionEMEnvelope,
				      logicEMEnvelope,
				      "EMEnvelope",
				      logicWorld,
				      false,
				      0);
  
  //--------------------------
  // EM Absorber
  //--------------------------
  G4ThreeVector positionEM = G4ThreeVector(0,0,EMZInitial);
  solidEM = new G4Box("em",EMXsize,EMYsize,1*mm/SizeMod);
  logicEM = new G4LogicalVolume(solidEM, EM_Absorber, "EM");
  physiEM = new G4PVPlacement(0,
			      positionEM,
			      logicEM,
			      "EM",
			      logicEMEnvelope,
			      false,
			      0);
  
  
  //--------------------------
  // EM Fiber
  //--------------------------
  G4ThreeVector positionEMFiber = G4ThreeVector(0,0,EMZInitial+EMSeperation);
  solidEMFiber = new G4Box("emfiber",EMXsize,EMYsize,0.35*mm/SizeMod);
  logicEMFiber = new G4LogicalVolume(solidEMFiber, EM_Fiber, "EMFiber");
  physiEMFiber = new G4PVPlacement(0,
				   positionEMFiber,
				   logicEMFiber,
				   "EMFiber",
				   logicEMEnvelope,
				   false,
				   0);
  
  //--------------------------
  // EM Absorber2
  //--------------------------
  G4ThreeVector positionEM2 = G4ThreeVector(0,0,(EMZInitial+(1*EMZDelta)));
  solidEM2 = new G4Box("em2",EMXsize,EMYsize,1*mm/SizeMod);
  logicEM2 = new G4LogicalVolume(solidEM2, EM_Absorber, "EM2");
  physiEM2 = new G4PVPlacement(0,
			       positionEM2,
			       logicEM2,
			       "EM2",
			       logicEMEnvelope,
			       false,
			       0);
  //empty line
  //--------------------------
  // EM Fiber2 
  //--------------------------
  G4ThreeVector positionEMFiber2 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(1*EMZDelta))); 
  solidEMFiber2 = new G4Box("emfiber2",EMXsize,EMYsize,0.35*mm/SizeMod);
  logicEMFiber2 = new G4LogicalVolume(solidEMFiber2, EM_Fiber, "EMFiber2");
  physiEMFiber2 = new G4PVPlacement(0,
				    positionEMFiber2,
				    logicEMFiber2,
				    "EMFiber2",
				    logicEMEnvelope,
				    false,
				    0); 
    //--------------------------
  // EM Absorber3
  //--------------------------
  G4ThreeVector positionEM3 = G4ThreeVector(0,0,(EMZInitial+(2*EMZDelta)));
  solidEM3 = new G4Box("em3",EMXsize,EMYsize,1*mm/SizeMod);
  logicEM3 = new G4LogicalVolume(solidEM3, EM_Absorber, "EM3");
  physiEM3 = new G4PVPlacement(0,
			       positionEM3,
			       logicEM3,
			       "EM3",
			       logicEMEnvelope,
			       false,
			       0);
  //empty line
  //--------------------------
  // EM Fiber3 
  //--------------------------
  G4ThreeVector positionEMFiber3 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(2*EMZDelta))); 
  solidEMFiber3 = new G4Box("emfiber3",EMXsize,EMYsize,0.35*mm/SizeMod);
  logicEMFiber3 = new G4LogicalVolume(solidEMFiber3, EM_Fiber, "EMFiber3");
  physiEMFiber3 = new G4PVPlacement(0,
				    positionEMFiber3,
				    logicEMFiber3,
				    "EMFiber3",
				    logicEMEnvelope,
				    false,
				    0); 
  //--------------------------
  // EM Absorber4
  //--------------------------
  G4ThreeVector positionEM4 = G4ThreeVector(0,0,(EMZInitial+(3*EMZDelta)));
  solidEM4 = new G4Box("em4",EMXsize,EMYsize,1*mm/SizeMod);
  logicEM4 = new G4LogicalVolume(solidEM4, EM_Absorber, "EM4");
  physiEM4 = new G4PVPlacement(0,
			       positionEM4,
			       logicEM4,
			       "EM4",
			       logicEMEnvelope,
			       false,
			       0);
  //empty line
  //--------------------------
  // EM Fiber4 
  //--------------------------
  G4ThreeVector positionEMFiber4 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(3*EMZDelta))); 
  solidEMFiber4 = new G4Box("emfiber4",EMXsize,EMYsize,0.35*mm/SizeMod);
  logicEMFiber4 = new G4LogicalVolume(solidEMFiber4, EM_Fiber, "EMFiber4");
  physiEMFiber4 = new G4PVPlacement(0,
				    positionEMFiber4,
				    logicEMFiber4,
				    "EMFiber4",
				    logicEMEnvelope,
				    false,
				    0); 
  //--------------------------
  // EM Absorber5
  //--------------------------
  G4ThreeVector positionEM5 = G4ThreeVector(0,0,(EMZInitial+(4*EMZDelta)));
  solidEM5 = new G4Box("em5",EMXsize,EMYsize,1*mm/SizeMod);
  logicEM5 = new G4LogicalVolume(solidEM5, EM_Absorber, "EM5");
  physiEM5 = new G4PVPlacement(0,
			       positionEM5,
			       logicEM5,
			       "EM5",
			       logicEMEnvelope,
			       false,
			       0);
  //empty line
  //--------------------------
  // EM Fiber5 
  //--------------------------
  G4ThreeVector positionEMFiber5 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(4*EMZDelta))); 
  solidEMFiber5 = new G4Box("emfiber5",EMXsize,EMYsize,0.35*mm/SizeMod);
  logicEMFiber5 = new G4LogicalVolume(solidEMFiber5, EM_Fiber, "EMFiber5");
  physiEMFiber5 = new G4PVPlacement(0,
				    positionEMFiber5,
				    logicEMFiber5,
				    "EMFiber5",
				    logicEMEnvelope,
				    false,
				    0); 
  //--------------------------
  // EM Absorber6
  //--------------------------
  G4ThreeVector positionEM6 = G4ThreeVector(0,0,(EMZInitial+(5*EMZDelta)));
  solidEM6 = new G4Box("em6",EMXsize,EMYsize,1*mm/SizeMod);
  logicEM6 = new G4LogicalVolume(solidEM6, EM_Absorber, "EM6");
  physiEM6 = new G4PVPlacement(0,
			       positionEM6,
			       logicEM6,
			       "EM6",
			       logicEMEnvelope,
			       false,
			       0);
  //empty line
  //--------------------------
  // EM Fiber6 
  //--------------------------
  G4ThreeVector positionEMFiber6 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(5*EMZDelta))); 
  solidEMFiber6 = new G4Box("emfiber6",EMXsize,EMYsize,0.35*mm/SizeMod);
  logicEMFiber6 = new G4LogicalVolume(solidEMFiber6, EM_Fiber, "EMFiber6");
  physiEMFiber6 = new G4PVPlacement(0,
				    positionEMFiber6,
				    logicEMFiber6,
				    "EMFiber6",
				    logicEMEnvelope,
				    false,
				    0); 
  //--------------------------
  // EM Absorber7
  //--------------------------
  G4ThreeVector positionEM7 = G4ThreeVector(0,0,(EMZInitial+(6*EMZDelta)));
  solidEM7 = new G4Box("em7",EMXsize,EMYsize,1*mm/SizeMod);
  logicEM7 = new G4LogicalVolume(solidEM7, EM_Absorber, "EM7");
  physiEM7 = new G4PVPlacement(0,
			       positionEM7,
			       logicEM7,
			       "EM7",
			       logicEMEnvelope,
			       false,
			       0);
  //empty line
  //--------------------------
  // EM Fiber7 
  //--------------------------
  G4ThreeVector positionEMFiber7 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(6*EMZDelta))); 
  solidEMFiber7 = new G4Box("emfiber7",EMXsize,EMYsize,0.35*mm/SizeMod);
  logicEMFiber7 = new G4LogicalVolume(solidEMFiber7, EM_Fiber, "EMFiber7");
  physiEMFiber7 = new G4PVPlacement(0,
				    positionEMFiber7,
				    logicEMFiber7,
				    "EMFiber7",
				    logicEMEnvelope,
				    false,
				    0); 
  //--------------------------
  // EM Absorber8
  //--------------------------
  G4ThreeVector positionEM8 = G4ThreeVector(0,0,(EMZInitial+(7*EMZDelta)));
  solidEM8 = new G4Box("em8",EMXsize,EMYsize,1*mm/SizeMod);
  logicEM8 = new G4LogicalVolume(solidEM8, EM_Absorber, "EM8");
  physiEM8 = new G4PVPlacement(0,
			       positionEM8,
			       logicEM8,
			       "EM8",
			       logicEMEnvelope,
			       false,
			       0);
  //empty line
  //--------------------------
  // EM Fiber8 
  //--------------------------
  G4ThreeVector positionEMFiber8 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(7*EMZDelta))); 
  solidEMFiber8 = new G4Box("emfiber8",EMXsize,EMYsize,0.35*mm/SizeMod);
  logicEMFiber8 = new G4LogicalVolume(solidEMFiber8, EM_Fiber, "EMFiber8");
  physiEMFiber8 = new G4PVPlacement(0,
				    positionEMFiber8,
				    logicEMFiber8,
				    "EMFiber8",
				    logicEMEnvelope,
				    false,
				    0); 
//--------------------------
// EM Absorber9
//--------------------------
  G4ThreeVector positionEM9 = G4ThreeVector(0,0,(EMZInitial+(8*EMZDelta)));
  solidEM9 = new G4Box("em9",EMXsize,EMYsize,1*mm/SizeMod);
  logicEM9 = new G4LogicalVolume(solidEM9, EM_Absorber, "EM9");
  physiEM9 = new G4PVPlacement(0,
			       positionEM9,
			       logicEM9,
			       "EM9",
			       logicEMEnvelope,
			       false,
			       0);
  //empty line
  //--------------------------
  // EM Fiber9 
  //--------------------------
  G4ThreeVector positionEMFiber9 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(8*EMZDelta))); 
  solidEMFiber9 = new G4Box("emfiber9",EMXsize,EMYsize,0.35*mm/SizeMod);
  logicEMFiber9 = new G4LogicalVolume(solidEMFiber9, EM_Fiber, "EMFiber9");
  physiEMFiber9 = new G4PVPlacement(0,
				    positionEMFiber9,
				    logicEMFiber9,
				    "EMFiber9",
				    logicEMEnvelope,
				    false,
				    0); 
  //--------------------------
  // EM Absorber10
  //--------------------------
  G4ThreeVector positionEM10 = G4ThreeVector(0,0,(EMZInitial+(9*EMZDelta)));
 solidEM10 = new G4Box("em10",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM10 = new G4LogicalVolume(solidEM10, EM_Absorber, "EM10");
 physiEM10 = new G4PVPlacement(0,
			       positionEM10,
			       logicEM10,
			       "EM10",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber10 
 //--------------------------
 G4ThreeVector positionEMFiber10 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(9*EMZDelta))); 
 solidEMFiber10 = new G4Box("emfiber10",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber10 = new G4LogicalVolume(solidEMFiber10, EM_Fiber, "EMFiber10");
 physiEMFiber10 = new G4PVPlacement(0,
				    positionEMFiber10,
				    logicEMFiber10,
				    "EMFiber10",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber11
 //--------------------------
 G4ThreeVector positionEM11 = G4ThreeVector(0,0,(EMZInitial+(10*EMZDelta)));
 solidEM11 = new G4Box("em11",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM11 = new G4LogicalVolume(solidEM11, EM_Absorber, "EM11");
 physiEM11 = new G4PVPlacement(0,
			       positionEM11,
			       logicEM11,
			       "EM11",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber11 
 //--------------------------
 G4ThreeVector positionEMFiber11 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(10*EMZDelta))); 
 solidEMFiber11 = new G4Box("emfiber11",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber11 = new G4LogicalVolume(solidEMFiber11, EM_Fiber, "Emfiber11");
 physiEMFiber11 = new G4PVPlacement(0,
				    positionEMFiber11,
				    logicEMFiber11,
				    "EMFiber11",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber12
 //--------------------------
 G4ThreeVector positionEM12 = G4ThreeVector(0,0,(EMZInitial+(11*EMZDelta)));
 solidEM12 = new G4Box("em12",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM12 = new G4LogicalVolume(solidEM12, EM_Absorber, "EM12");
 physiEM12 = new G4PVPlacement(0,
			       positionEM12,
			       logicEM12,
			       "EM12",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber12 
 //--------------------------
 G4ThreeVector positionEMFiber12 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(11*EMZDelta))); 
 solidEMFiber12 = new G4Box("emfiber12",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber12 = new G4LogicalVolume(solidEMFiber12, EM_Fiber, "EMFiber12");
 physiEMFiber12 = new G4PVPlacement(0,
				    positionEMFiber12,
				    logicEMFiber12,
				    "EMFiber12",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber13
 //--------------------------
 G4ThreeVector positionEM13 = G4ThreeVector(0,0,(EMZInitial+(12*EMZDelta)));
 solidEM13 = new G4Box("em13",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM13 = new G4LogicalVolume(solidEM13, EM_Absorber, "EM13");
 physiEM13 = new G4PVPlacement(0,
			       positionEM13,
			       logicEM13,
			       "EM13",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber13 
 //--------------------------
 G4ThreeVector positionEMFiber13 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(12*EMZDelta))); 
 solidEMFiber13 = new G4Box("emfiber13",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber13 = new G4LogicalVolume(solidEMFiber13, EM_Fiber, "EMFiber13");
 physiEMFiber13 = new G4PVPlacement(0,
				    positionEMFiber13,
				    logicEMFiber13,
				    "EMFiber13",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber14
 //--------------------------
 G4ThreeVector positionEM14 = G4ThreeVector(0,0,(EMZInitial+(13*EMZDelta)));
 solidEM14 = new G4Box("em14",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM14 = new G4LogicalVolume(solidEM14, EM_Absorber, "EM14");
 physiEM14 = new G4PVPlacement(0,
			       positionEM14,
			       logicEM14,
			       "EM14",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber14 
 //--------------------------
 G4ThreeVector positionEMFiber14 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(13*EMZDelta))); 
 solidEMFiber14 = new G4Box("emfiber14",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber14 = new G4LogicalVolume(solidEMFiber14, EM_Fiber, "EMFiber14");
 physiEMFiber14 = new G4PVPlacement(0,
				    positionEMFiber14,
				    logicEMFiber14,
				    "EMFiber14",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber15
 //--------------------------
 G4ThreeVector positionEM15 = G4ThreeVector(0,0,(EMZInitial+(14*EMZDelta)));
 solidEM15 = new G4Box("em15",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM15 = new G4LogicalVolume(solidEM15, EM_Absorber, "EM15");
 physiEM15 = new G4PVPlacement(0,
			       positionEM15,
			       logicEM15,
			       "EM15",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber15 
 //--------------------------
 G4ThreeVector positionEMFiber15 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(14*EMZDelta))); 
 solidEMFiber15 = new G4Box("emfiber15",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber15 = new G4LogicalVolume(solidEMFiber15, EM_Fiber, "EMFiber15");
 physiEMFiber15 = new G4PVPlacement(0,
				    positionEMFiber15,
				    logicEMFiber15,
				    "EMFiber15",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber16
 //--------------------------
 G4ThreeVector positionEM16 = G4ThreeVector(0,0,(EMZInitial+(15*EMZDelta)));
 solidEM16 = new G4Box("em16",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM16 = new G4LogicalVolume(solidEM16, EM_Absorber, "EM16");
 physiEM16 = new G4PVPlacement(0,
			       positionEM16,
			       logicEM16,
			       "EM16",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber16 
 //--------------------------
 G4ThreeVector positionEMFiber16 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(15*EMZDelta))); 
 solidEMFiber16 = new G4Box("emfiber16",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber16 = new G4LogicalVolume(solidEMFiber16, EM_Fiber, "EMFiber16");
 physiEMFiber16 = new G4PVPlacement(0,
				    positionEMFiber16,
				    logicEMFiber16,
				    "EMFiber16",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber17
 //--------------------------
 G4ThreeVector positionEM17 = G4ThreeVector(0,0,(EMZInitial+(16*EMZDelta)));
 solidEM17 = new G4Box("em17",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM17 = new G4LogicalVolume(solidEM17, EM_Absorber, "EM17");
 physiEM17 = new G4PVPlacement(0,
			       positionEM17,
			       logicEM17,
			       "EM17",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber17 
 //--------------------------
 G4ThreeVector positionEMFiber17 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(16*EMZDelta))); 
 solidEMFiber17 = new G4Box("emfiber17",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber17 = new G4LogicalVolume(solidEMFiber17, EM_Fiber, "EMFiber17");
 physiEMFiber17 = new G4PVPlacement(0,
				    positionEMFiber17,
				    logicEMFiber17,
				    "EMFiber17",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber18
 //--------------------------
 G4ThreeVector positionEM18 = G4ThreeVector(0,0,(EMZInitial+(17*EMZDelta)));
 solidEM18 = new G4Box("em18",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM18 = new G4LogicalVolume(solidEM18, EM_Absorber, "EM18");
 physiEM18 = new G4PVPlacement(0,
			       positionEM18,
			       logicEM18,
			       "EM18",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber18 
 //--------------------------
 G4ThreeVector positionEMFiber18 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(17*EMZDelta))); 
 solidEMFiber18 = new G4Box("emfiber18",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber18 = new G4LogicalVolume(solidEMFiber18, EM_Fiber, "EMFiber18");
 physiEMFiber18 = new G4PVPlacement(0,
				    positionEMFiber18,
				    logicEMFiber18,
				    "EMFiber18",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber19
 //--------------------------
 G4ThreeVector positionEM19 = G4ThreeVector(0,0,(EMZInitial+(18*EMZDelta)));
 solidEM19 = new G4Box("em19",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM19 = new G4LogicalVolume(solidEM19, EM_Absorber, "EM19");
 physiEM19 = new G4PVPlacement(0,
			       positionEM19,
			       logicEM19,
			       "EM19",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber19 
 //--------------------------
 G4ThreeVector positionEMFiber19 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(18*EMZDelta))); 
 solidEMFiber19 = new G4Box("emfiber19",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber19 = new G4LogicalVolume(solidEMFiber19, EM_Fiber, "EMFiber19");
 physiEMFiber19 = new G4PVPlacement(0,
				    positionEMFiber19,
				    logicEMFiber19,
				    "EMFiber19",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber20
 //--------------------------
 G4ThreeVector positionEM20 = G4ThreeVector(0,0,(EMZInitial+(19*EMZDelta)));
 solidEM20 = new G4Box("em20",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM20 = new G4LogicalVolume(solidEM20, EM_Absorber, "EM20");
 physiEM20 = new G4PVPlacement(0,
			       positionEM20,
			       logicEM20,
			       "EM20",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber20 
 //--------------------------
 G4ThreeVector positionEMFiber20 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(19*EMZDelta))); 
 solidEMFiber20 = new G4Box("emfiber20",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber20 = new G4LogicalVolume(solidEMFiber20, EM_Fiber, "EMFiber20");
 physiEMFiber20 = new G4PVPlacement(0,
				    positionEMFiber20,
				    logicEMFiber20,
				    "EMFiber20",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber21
 //--------------------------
 G4ThreeVector positionEM21 = G4ThreeVector(0,0,(EMZInitial+(20*EMZDelta)));
 solidEM21 = new G4Box("em21",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM21 = new G4LogicalVolume(solidEM21, EM_Absorber, "EM21");
 physiEM21 = new G4PVPlacement(0,
			       positionEM21,
			       logicEM21,
			       "EM21",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber21 
 //--------------------------
 G4ThreeVector positionEMFiber21 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(20*EMZDelta))); 
 solidEMFiber21 = new G4Box("emfiber21",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber21 = new G4LogicalVolume(solidEMFiber21, EM_Fiber, "EMFiber21");
 physiEMFiber21 = new G4PVPlacement(0,
				    positionEMFiber21,
				    logicEMFiber21,
				    "EMFiber21",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber22
 //--------------------------
 G4ThreeVector positionEM22 = G4ThreeVector(0,0,(EMZInitial+(21*EMZDelta)));
 solidEM22 = new G4Box("em22",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM22 = new G4LogicalVolume(solidEM22, EM_Absorber, "EM22");
 physiEM22 = new G4PVPlacement(0,
			       positionEM22,
			       logicEM22,
			       "EM22",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber22 
 //--------------------------
 G4ThreeVector positionEMFiber22 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(21*EMZDelta))); 
 solidEMFiber22 = new G4Box("emfiber22",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber22 = new G4LogicalVolume(solidEMFiber22, EM_Fiber, "EMFiber22");
 physiEMFiber22 = new G4PVPlacement(0,
				    positionEMFiber22,
				    logicEMFiber22,
				    "EMFiber22",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber23
 //--------------------------
 G4ThreeVector positionEM23 = G4ThreeVector(0,0,(EMZInitial+(22*EMZDelta)));
 solidEM23 = new G4Box("em23",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM23 = new G4LogicalVolume(solidEM23, EM_Absorber, "EM23");
 physiEM23 = new G4PVPlacement(0,
			       positionEM23,
			       logicEM23,
			       "EM23",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber23 
 //--------------------------
 G4ThreeVector positionEMFiber23 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(22*EMZDelta))); 
 solidEMFiber23 = new G4Box("emfiber23",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber23 = new G4LogicalVolume(solidEMFiber23, EM_Fiber, "EMFiber23");
 physiEMFiber23 = new G4PVPlacement(0,
				    positionEMFiber23,
				    logicEMFiber23,
				    "EMFiber23",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber24
 //--------------------------
 G4ThreeVector positionEM24 = G4ThreeVector(0,0,(EMZInitial+(23*EMZDelta)));
 solidEM24 = new G4Box("em24",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM24 = new G4LogicalVolume(solidEM24, EM_Absorber, "EM24");
 physiEM24 = new G4PVPlacement(0,
			       positionEM24,
			       logicEM24,
			       "EM24",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber24 
 //--------------------------
 G4ThreeVector positionEMFiber24 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(23*EMZDelta))); 
 solidEMFiber24 = new G4Box("emfiber24",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber24 = new G4LogicalVolume(solidEMFiber24, EM_Fiber, "EMFiber24");
 physiEMFiber24 = new G4PVPlacement(0,
				    positionEMFiber24,
				    logicEMFiber24,
				    "EMFiber24",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber25
 //--------------------------
 G4ThreeVector positionEM25 = G4ThreeVector(0,0,(EMZInitial+(24*EMZDelta)));
 solidEM25 = new G4Box("em25",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM25 = new G4LogicalVolume(solidEM25, EM_Absorber, "EM25");
 physiEM25 = new G4PVPlacement(0,
			       positionEM25,
			       logicEM25,
			       "EM25",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber25 
 //--------------------------
 G4ThreeVector positionEMFiber25 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(24*EMZDelta))); 
 solidEMFiber25 = new G4Box("emfiber25",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber25 = new G4LogicalVolume(solidEMFiber25, EM_Fiber, "EMFiber25");
 physiEMFiber25 = new G4PVPlacement(0,
				    positionEMFiber25,
				    logicEMFiber25,
				    "EMFiber25",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber26
 //--------------------------
 G4ThreeVector positionEM26 = G4ThreeVector(0,0,(EMZInitial+(25*EMZDelta)));
 solidEM26 = new G4Box("em26",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM26 = new G4LogicalVolume(solidEM26, EM_Absorber, "EM26");
 physiEM26 = new G4PVPlacement(0,
			       positionEM26,
			       logicEM26,
			       "EM26",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber26 
 //--------------------------
 G4ThreeVector positionEMFiber26 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(25*EMZDelta))); 
 solidEMFiber26 = new G4Box("emfiber26",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber26 = new G4LogicalVolume(solidEMFiber26, EM_Fiber, "EMFiber26");
 physiEMFiber26 = new G4PVPlacement(0,
				    positionEMFiber26,
				    logicEMFiber26,
				    "EMFiber26",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber27
 //--------------------------
 G4ThreeVector positionEM27 = G4ThreeVector(0,0,(EMZInitial+(26*EMZDelta)));
 solidEM27 = new G4Box("em27",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM27 = new G4LogicalVolume(solidEM27, EM_Absorber, "EM27");
 physiEM27 = new G4PVPlacement(0,
			       positionEM27,
			       logicEM27,
			       "EM27",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber27 
 //--------------------------
 G4ThreeVector positionEMFiber27 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(26*EMZDelta))); 
 solidEMFiber27 = new G4Box("emfiber27",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber27 = new G4LogicalVolume(solidEMFiber27, EM_Fiber, "EMFiber27");
 physiEMFiber27 = new G4PVPlacement(0,
				    positionEMFiber27,
				    logicEMFiber27,
				    "EMFiber27",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber28
 //--------------------------
 G4ThreeVector positionEM28 = G4ThreeVector(0,0,(EMZInitial+(27*EMZDelta)));
 solidEM28 = new G4Box("em28",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM28 = new G4LogicalVolume(solidEM28, EM_Absorber, "EM28");
 physiEM28 = new G4PVPlacement(0,
			       positionEM28,
			       logicEM28,
			       "EM28",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber28 
 //--------------------------
 G4ThreeVector positionEMFiber28 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(27*EMZDelta))); 
 solidEMFiber28 = new G4Box("emfiber28",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber28 = new G4LogicalVolume(solidEMFiber28, EM_Fiber, "EMFiber28");
 physiEMFiber28 = new G4PVPlacement(0,
				    positionEMFiber28,
				    logicEMFiber28,
				    "EMFiber28",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber29
 //--------------------------
 G4ThreeVector positionEM29 = G4ThreeVector(0,0,(EMZInitial+(28*EMZDelta)));
 solidEM29 = new G4Box("em29",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM29 = new G4LogicalVolume(solidEM29, EM_Absorber, "EM29");
 physiEM29 = new G4PVPlacement(0,
			       positionEM29,
			       logicEM29,
			       "EM29",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber29 
 //--------------------------
 G4ThreeVector positionEMFiber29 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(28*EMZDelta))); 
 solidEMFiber29 = new G4Box("emfiber29",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber29 = new G4LogicalVolume(solidEMFiber29, EM_Fiber, "EMFiber29");
 physiEMFiber29 = new G4PVPlacement(0,
				    positionEMFiber29,
				    logicEMFiber29,
				    "EMFiber29",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber30
 //--------------------------
 G4ThreeVector positionEM30 = G4ThreeVector(0,0,(EMZInitial+(29*EMZDelta)));
 solidEM30 = new G4Box("em30",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM30 = new G4LogicalVolume(solidEM30, EM_Absorber, "EM30");
 physiEM30 = new G4PVPlacement(0,
			       positionEM30,
			       logicEM30,
			       "EM30",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber30 
 //--------------------------
 G4ThreeVector positionEMFiber30 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(29*EMZDelta))); 
 solidEMFiber30 = new G4Box("emfiber30",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber30 = new G4LogicalVolume(solidEMFiber30, EM_Fiber, "EMFiber30");
 physiEMFiber30 = new G4PVPlacement(0,
				    positionEMFiber30,
				    logicEMFiber30,
				    "EMFiber30",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber31
 //--------------------------
 G4ThreeVector positionEM31 = G4ThreeVector(0,0,(EMZInitial+(30*EMZDelta)));
 solidEM31 = new G4Box("em31",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM31 = new G4LogicalVolume(solidEM31, EM_Absorber, "EM31");
 physiEM31 = new G4PVPlacement(0,
			       positionEM31,
			       logicEM31,
			       "EM31",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber31 
 //--------------------------
 G4ThreeVector positionEMFiber31 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(30*EMZDelta))); 
 solidEMFiber31 = new G4Box("emfiber31",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber31 = new G4LogicalVolume(solidEMFiber31, EM_Fiber, "EMFiber31");
 physiEMFiber31 = new G4PVPlacement(0,
				    positionEMFiber31,
				    logicEMFiber31,
				    "EMFiber31",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber32
 //--------------------------
 G4ThreeVector positionEM32 = G4ThreeVector(0,0,(EMZInitial+(31*EMZDelta)));
 solidEM32 = new G4Box("em32",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM32 = new G4LogicalVolume(solidEM32, EM_Absorber, "EM32");
 physiEM32 = new G4PVPlacement(0,
			       positionEM32,
			       logicEM32,
			       "EM32",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber32 
 //--------------------------
 G4ThreeVector positionEMFiber32 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(31*EMZDelta))); 
 solidEMFiber32 = new G4Box("emfiber32",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber32 = new G4LogicalVolume(solidEMFiber32, EM_Fiber, "EMFiber32");
 physiEMFiber32 = new G4PVPlacement(0,
				    positionEMFiber32,
				    logicEMFiber32,
				    "EMFiber32",
				    logicEMEnvelope,
				    false,
				    0); 
 //--------------------------
 // EM Absorber33
 //--------------------------
 G4ThreeVector positionEM33 = G4ThreeVector(0,0,(EMZInitial+(32*EMZDelta)));
 solidEM33 = new G4Box("em33",EMXsize,EMYsize,1*mm/SizeMod);
 logicEM33 = new G4LogicalVolume(solidEM33, EM_Absorber, "EM33");
 physiEM33 = new G4PVPlacement(0,
			       positionEM33,
			       logicEM33,
			       "EM33",
			       logicEMEnvelope,
			       false,
			       0);
 //empty line
 //--------------------------
 // EM Fiber33 
 //--------------------------
 G4ThreeVector positionEMFiber33 = G4ThreeVector(0,0,(EMZInitial+EMSeperation+(32*EMZDelta))); 
 solidEMFiber33 = new G4Box("emfiber33",EMXsize,EMYsize,0.35*mm/SizeMod);
 logicEMFiber33 = new G4LogicalVolume(solidEMFiber33, EM_Fiber, "EMFiber33");
 physiEMFiber33 = new G4PVPlacement(0,
				    positionEMFiber33,
				    logicEMFiber33,
				    "EMFiber33",
				    logicEMEnvelope,
				    false,
				    0); 
 
//--------------------------------------------------
///Chris 14 Oct 2014 - adding 2 mm copper before RPDs
// Cu WORLD
//--------------------------------------------------
  G4ThreeVector positionCuWorld = G4ThreeVector(0,0, 55*mm/SizeMod);
  solidCuWorld = new G4Box("CuWorld",5.01*cm/SizeMod,6.26*cm/SizeMod,0.1*cm/SizeMod);
  logicCuWorld = new G4LogicalVolume(solidCuWorld, Air, "CUWORLD");
  physiCuWorld = new G4PVPlacement(0,
  					positionCuWorld,
  					logicCuWorld,
  					"CUWORLD",
  					logicWorld,
  					false,
  					0);
//--------------------------
// Copper Panel 
//--------------------------
  G4ThreeVector positionCuPanel = G4ThreeVector(0,0,0*cm);
  CuPanel = new G4Box("cupan",4.01*cm/SizeMod,4.01*cm/SizeMod,1*mm/SizeMod);
  logicCu = new G4LogicalVolume(CuPanel, Cu_Material, "CUPANEL");
  physiCu = new G4PVPlacement(0,
				positionCuPanel,
				logicCu,
				"CUPANEL",
				logicCuWorld,
				false,
				0);
				



//--------------------------------------------------------------------------------------


  //----------------------------
  // RPD WORLD
  //----------------------------
  G4ThreeVector positionRPDWorld = G4ThreeVector(0,0,65*mm/SizeMod);
  solidRPDWorld = new G4Box("rpdworld",5.01*cm/SizeMod,6.26*cm/SizeMod,0.85*cm/SizeMod);
  logicRPDWorld = new G4LogicalVolume(solidRPDWorld, Air, "RPDWorld");
  physiRPDWorld = new G4PVPlacement(0,
				    positionRPDWorld,
				    logicRPDWorld,
				    "RPDWorld",
				    logicWorld,
				    false,
				    0);
				    
/////////////////// PMT /////////////////////
  
  mppcZ     = 0.25*mm;
  wlsfiberRY  = 0.5*mm;
  mppcHalfL = wlsfiberRY;

	///pmt 1
  solidPhotonDet4 = new G4Box("PhotonDet4",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet4 =
                     new G4LogicalVolume(solidPhotonDet4,
                     Pb,
                     "PhotonDet4");
  new G4PVPlacement(0,
                    G4ThreeVector(3.0*cm,RPDYInitial+2.25*cm,0.245*cm),
                    logicPhotonDet4,
                    "PhotonDet4",
                    logicRPDWorld,
                    false,
                    0);                    
//  G4OpticalSurface* PhotonDetSurface4 = new G4OpticalSurface("PhotonDetSurface4",
   //               glisur,
     //             ground,
       //           dielectric_metal,
         //         mppcPolish);

	///pmt 2
  solidPhotonDet3 = new G4Box("PhotonDet3",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet3 =
                     new G4LogicalVolume(solidPhotonDet3,
                     Pb,
                     "PhotonDet3");
  new G4PVPlacement(0,
                    G4ThreeVector(1.0*cm,RPDYInitial+2.25*cm,0.245*cm),
                    logicPhotonDet3,
                    "PhotonDet3",
                    logicRPDWorld,
                    false,
                    0);                    
//  G4OpticalSurface* PhotonDetSurface3 = new G4OpticalSurface("PhotonDetSurface3",
  //                glisur,
    //              ground,
      //            dielectric_metal,
        //          mppcPolish);
	///pmt 3
  solidPhotonDet2 = new G4Box("PhotonDet2",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet2 =
                     new G4LogicalVolume(solidPhotonDet2,
                     Pb,
                     "PhotonDet2");
  new G4PVPlacement(0,
                    G4ThreeVector(-1.0*cm,RPDYInitial+2.25*cm,0.245*cm),
                    logicPhotonDet2,
                    "PhotonDet2",
                    logicRPDWorld,
                    false,
                    0);                    
//  G4OpticalSurface* PhotonDetSurface2 = new G4OpticalSurface("PhotonDetSurface2",
  //                glisur,
    //              ground,
      //            dielectric_metal,
        //          mppcPolish);

	///pmt 4
  solidPhotonDet1 = new G4Box("PhotonDet1",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet1 =
                     new G4LogicalVolume(solidPhotonDet1,
                     Pb,
                     "PhotonDet1");
  pmtdet1 = new G4PVPlacement(0,
                    G4ThreeVector(-3.0*cm,RPDYInitial+2.25*cm,0.245*cm),
                    logicPhotonDet1,
                    "PhotonDet1",
                    logicRPDWorld,
                    false,
                    0);   

                 
//  G4OpticalSurface* PhotonDetSurface1 = new G4OpticalSurface("PhotonDetSurface1",
  //                glisur,
    //              ground,
      //            dielectric_metal,
        //          mppcPolish);
	///pmt 5
  solidPhotonDet8 = new G4Box("PhotonDet8",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet8 =
                     new G4LogicalVolume(solidPhotonDet8,
                     Pb,
                     "PhotonDet8");
  new G4PVPlacement(0,
                    G4ThreeVector(3.0*cm,RPDYInitial+2.25*cm,0.36*cm),
                    logicPhotonDet8,
                    "PhotonDet8",
                    logicRPDWorld,
                    false,
                    0);                    
//  G4OpticalSurface* PhotonDetSurface8 = new G4OpticalSurface("PhotonDetSurface8",
  //                glisur,
    ///              ground,
       //           dielectric_metal,
         //         mppcPolish);   

	///pmt 6
  solidPhotonDet7 = new G4Box("PhotonDet7",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet7 =
                     new G4LogicalVolume(solidPhotonDet7,
                     Pb,
                     "PhotonDet7");
  new G4PVPlacement(0,
                    G4ThreeVector(1.0*cm,RPDYInitial+2.25*cm,0.36*cm),
                    logicPhotonDet7,
                    "PhotonDet7",
                    logicRPDWorld,
                    false,
                    0);                    
/*  G4OpticalSurface* PhotonDetSurface7 = new G4OpticalSurface("PhotonDetSurface7",
                  glisur,
                  ground,
                  dielectric_metal,
                  mppcPolish); */
	///pmt 7
  solidPhotonDet6 = new G4Box("PhotonDet6",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet6 =
                     new G4LogicalVolume(solidPhotonDet6,
                     Pb,
                     "PhotonDet6");
  new G4PVPlacement(0,
                    G4ThreeVector(-1.0*cm,RPDYInitial+2.25*cm,0.36*cm),
                    logicPhotonDet6,
                    "PhotonDet6",
                    logicRPDWorld,
                    false,
                    0);                    
/*  G4OpticalSurface* PhotonDetSurface6 = new G4OpticalSurface("PhotonDetSurface6",
                  glisur,
                  ground,
                  dielectric_metal,
                  mppcPolish); */
	///pmt 8
  solidPhotonDet5 = new G4Box("PhotonDet5",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet5 =
                     new G4LogicalVolume(solidPhotonDet5,
                     Pb,
                     "PhotonDet5");
  new G4PVPlacement(0,
                    G4ThreeVector(-3.0*cm,RPDYInitial+2.25*cm,0.36*cm),
                    logicPhotonDet5,
                    "PhotonDet5",
                    logicRPDWorld,
                    false,
                    0);                    
/*  G4OpticalSurface* PhotonDetSurface5 = new G4OpticalSurface("PhotonDetSurface5",
                  glisur,
                  ground,
                  dielectric_metal,
                  mppcPolish); */
	///pmt 9
  solidPhotonDet12 = new G4Box("PhotonDet12",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet12 =
                     new G4LogicalVolume(solidPhotonDet12,
                     Pb,
                     "PhotonDet12");
  new G4PVPlacement(0,
                    G4ThreeVector(3.0*cm,RPDYInitial+2.25*cm,0.475*cm),
                    logicPhotonDet12,
                    "PhotonDet12",
                    logicRPDWorld,
                    false,
                    0);                    
/*  G4OpticalSurface* PhotonDetSurface12 = new G4OpticalSurface("PhotonDetSurface12",
                  glisur,
                  ground,
                  dielectric_metal,
                  mppcPolish); */
	///pmt 10
  solidPhotonDet11 = new G4Box("PhotonDet11",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet11 =
                     new G4LogicalVolume(solidPhotonDet11,
                     Pb,
                     "PhotonDet11");
  new G4PVPlacement(0,
                    G4ThreeVector(1.0*cm,RPDYInitial+2.25*cm,0.475*cm),
                    logicPhotonDet11,
                    "PhotonDet11",
                    logicRPDWorld,
                    false,
                    0);                    
/*  G4OpticalSurface* PhotonDetSurface11 = new G4OpticalSurface("PhotonDetSurface11",
                  glisur,
                  ground,
                  dielectric_metal,
                  mppcPolish); */
	///pmt 11
  solidPhotonDet10 = new G4Box("PhotonDet10",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet10 =
                     new G4LogicalVolume(solidPhotonDet10,
                     Pb,
                     "PhotonDet10");
  new G4PVPlacement(0,
                    G4ThreeVector(-1.0*cm,RPDYInitial+2.25*cm,0.475*cm),
                    logicPhotonDet10,
                    "PhotonDet10",
                    logicRPDWorld,
                    false,
                    0);                    
/*  G4OpticalSurface* PhotonDetSurface10 = new G4OpticalSurface("PhotonDetSurface10",
                  glisur,
                  ground,
                  dielectric_metal,
                  mppcPolish); */
	///pmt 12
  solidPhotonDet9 = new G4Box("PhotonDet9",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet9 =
                     new G4LogicalVolume(solidPhotonDet9,
                     Pb,
                     "PhotonDet9");
  new G4PVPlacement(0,
                    G4ThreeVector(-3.0*cm,RPDYInitial+2.25*cm,0.475*cm),
                    logicPhotonDet9,
                    "PhotonDet9",
                    logicRPDWorld,
                    false,
                    0);                    
/*  G4OpticalSurface* PhotonDetSurface9 = new G4OpticalSurface("PhotonDetSurface9",
                  glisur,
                  ground,
                  dielectric_metal,
                  mppcPolish); */
	///pmt 13
  solidPhotonDet16 = new G4Box("PhotonDet16",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet16 =
                     new G4LogicalVolume(solidPhotonDet16,
                     Pb,
                     "PhotonDet16");
  new G4PVPlacement(0,
                    G4ThreeVector(3.0*cm,RPDYInitial+2.25*cm,0.59*cm),
                    logicPhotonDet16,
                    "PhotonDet16",
                    logicRPDWorld,
                    false,
                    0);                    
/*  G4OpticalSurface* PhotonDetSurface16 = new G4OpticalSurface("PhotonDetSurface16",
                  glisur,
                  ground,
                  dielectric_metal,
                  mppcPolish); */
	///pmt 14
  solidPhotonDet15 = new G4Box("PhotonDet15",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet15 =
                     new G4LogicalVolume(solidPhotonDet15,
                     Pb,
                     "PhotonDet15");
  new G4PVPlacement(0,
                    G4ThreeVector(1.0*cm,RPDYInitial+2.25*cm,0.59*cm),
                    logicPhotonDet15,
                    "PhotonDet15",
                    logicRPDWorld,
                    false,
                    0);                    
/*  G4OpticalSurface* PhotonDetSurface15 = new G4OpticalSurface("PhotonDetSurface15",
                  glisur,
                  ground,
                  dielectric_metal,
                  mppcPolish); */
	///pmt 15
  solidPhotonDet14 = new G4Box("PhotonDet14",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet14 =
                     new G4LogicalVolume(solidPhotonDet14,
                     Pb,
                     "PhotonDet14");
  new G4PVPlacement(0,
                    G4ThreeVector(-1.0*cm,RPDYInitial+2.25*cm,0.59*cm),
                    logicPhotonDet14,
                    "PhotonDet14",
                    logicRPDWorld,
                    false,
                    0);                    
/*  G4OpticalSurface* PhotonDetSurface14 = new G4OpticalSurface("PhotonDetSurface14",
                  glisur,
                  ground,
                  dielectric_metal,
                  mppcPolish); */
	///pmt 16
  solidPhotonDet13 = new G4Box("PhotonDet13",.999*cm,.25*cm, .05*cm);
  G4LogicalVolume*   logicPhotonDet13 =
                     new G4LogicalVolume(solidPhotonDet13,
                     Pb,
                     "PhotonDet13");
  new G4PVPlacement(0,
                    G4ThreeVector(-3.0*cm,RPDYInitial+2.25*cm,0.59*cm),
                    logicPhotonDet13,
                    "PhotonDet13",
                    logicRPDWorld,
                    false,
                    0);                    
/*  G4OpticalSurface* PhotonDetSurface13 = new G4OpticalSurface("PhotonDetSurface13",
                  glisur,
                  ground,
                  dielectric_metal,
                  mppcPolish);                */  
//  G4MaterialPropertiesTable* PhotonDetSurfaceProperty =
       //            new G4MaterialPropertiesTable();
       
       
                   
////set pmt properties                   

  G4double p_mppc[2] = {2.00*eV, 3.47*eV};
  G4double refl_mppc[2] = {mppcReflectivity,mppcReflectivity};
  G4double effi_mppc[2] = {1, 1};
 
//  PhotonDetSurfaceProperty -> AddProperty("REFLECTIVITY",p_mppc,refl_mppc,2);
//  PhotonDetSurfaceProperty -> AddProperty("EFFICIENCY",p_mppc,effi_mppc,2);
  
/*  PhotonDetSurface1 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface2 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface3 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface4 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface5 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface6 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface7 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface8 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface9 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface10 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface11 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface12 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface13 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface14 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface15 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  PhotonDetSurface16 -> SetMaterialPropertiesTable(PhotonDetSurfaceProperty);
  
 
  new G4LogicalSkinSurface("PhotonDetSurface1",logicPhotonDet1,PhotonDetSurface1); 
  new G4LogicalSkinSurface("PhotonDetSurface2",logicPhotonDet2,PhotonDetSurface2); 
  new G4LogicalSkinSurface("PhotonDetSurface3",logicPhotonDet3,PhotonDetSurface3); 
  new G4LogicalSkinSurface("PhotonDetSurface4",logicPhotonDet4,PhotonDetSurface4); 
  new G4LogicalSkinSurface("PhotonDetSurface5",logicPhotonDet5,PhotonDetSurface5);
  new G4LogicalSkinSurface("PhotonDetSurface6",logicPhotonDet6,PhotonDetSurface6);
  new G4LogicalSkinSurface("PhotonDetSurface7",logicPhotonDet7,PhotonDetSurface7);
  new G4LogicalSkinSurface("PhotonDetSurface8",logicPhotonDet8,PhotonDetSurface8);
  new G4LogicalSkinSurface("PhotonDetSurface9",logicPhotonDet9,PhotonDetSurface9);
  new G4LogicalSkinSurface("PhotonDetSurface10",logicPhotonDet10,PhotonDetSurface10);
  new G4LogicalSkinSurface("PhotonDetSurface11",logicPhotonDet11,PhotonDetSurface11);
  new G4LogicalSkinSurface("PhotonDetSurface12",logicPhotonDet12,PhotonDetSurface12);
  new G4LogicalSkinSurface("PhotonDetSurface13",logicPhotonDet13,PhotonDetSurface13);
  new G4LogicalSkinSurface("PhotonDetSurface14",logicPhotonDet14,PhotonDetSurface14);
  new G4LogicalSkinSurface("PhotonDetSurface15",logicPhotonDet15,PhotonDetSurface15);
  new G4LogicalSkinSurface("PhotonDetSurface16",logicPhotonDet16,PhotonDetSurface16);
*/

////////////////////////////////////////////

//--------------------------
// Aluminum front cover 1
//--------------------------
  G4ThreeVector positionAlCov1 = G4ThreeVector(0,RPDYInitial,-.725*cm);
  AlCov1 = new G4Box("alcov1",4.61*cm/SizeMod,.99999*cm/SizeMod,.099*mm/SizeMod);
  logicAlCov1 = new G4LogicalVolume(AlCov1, Al, "ALCOV1");
  physiAlCov1 = new G4PVPlacement(0,
				positionAlCov1,
				logicAlCov1,
				"ALCOV1",
				logicRPDWorld,
				false,
				0);

//--------------------------
// RPD Channel 1
//--------------------------
/*

  G4ThreeVector positionRPD1 = G4ThreeVector(RPDXInitial+(0*RPDXDelta),RPDYInitial,-.2*cm);
  solidRPD1 = new G4Box("rpd1",RPDXsize,RPDYsize,0.5*cm/SizeMod);
  solidRPD1sub1 = new G4Box("rpd1",0.55*mm,RPDYsize+.1*mm,0.55*mm/SizeMod);
  G4SubtractionSolid* newRPD1a = new G4SubtractionSolid("newrpd1a", solidRPD1, solidRPD1sub1,0,G4ThreeVector(2*mm,0,0.45*cm));
  G4SubtractionSolid* newRPD1b = new G4SubtractionSolid("newrpd1b", newRPD1a, solidRPD1sub1,0,G4ThreeVector(6*mm,0,0.45*cm));
  G4SubtractionSolid* newRPD1c = new G4SubtractionSolid("newrpd1c", newRPD1b, solidRPD1sub1,0,G4ThreeVector(-2*mm,0,0.45*cm));
  G4SubtractionSolid* newRPD1d = new G4SubtractionSolid("newrpd1d", newRPD1c, solidRPD1sub1,0,G4ThreeVector(-6*mm,0,0.45*cm));
  logicRPD1 = new G4LogicalVolume(newRPD1d, pQuartz, "RPD1",0,0,0);
  physiRPD1 = new G4PVPlacement(0,               // no rotation
                                   positionRPD1,  // at (x,y,z)
                                   logicRPD1,     // its logical volume                             
                                   "RPD1",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	

*/
		

  G4ThreeVector positionAl1 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)),3.5*cm,.305*cm);
  solidAl1 = new G4Box("Al1",RPDXsize+.0425*cm,1.52*cm,0.025*mm/SizeMod);
  logicAl1 = new G4LogicalVolume(solidAl1, Al, "Al1");
  physiAl1 = new G4PVPlacement(0,
				positionAl1,
				logicAl1,
				"Al1",
				logicRPDWorld,
				false,
				0); 
				
/*  G4ThreeVector PMTAl1 = G4ThreeVector(0,RPDYInitial+2.*cm,0.70*cm);
  solidPMTAl1 = new G4Box("PMTAl1",4.1*RPDXsize,RPDYsize,0.01*mm/SizeMod);
  logicPMTAl1 = new G4LogicalVolume(solidPMTAl1, Al, "PMTAl1");
  physiPMTAl1 = new G4PVPlacement(0,
				PMTAl1,
				logicPMTAl1,
				"PMTAl1",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector PMTAl2 = G4ThreeVector(0,RPDYInitial+2.*cm,0.51*cm);
  solidPMTAl2 = new G4Box("PMTAl2",4.1*RPDXsize,RPDYsize,0.025*mm/SizeMod);
  logicPMTAl2 = new G4LogicalVolume(solidPMTAl2, Al, "PMTAl2");
  physiPMTAl2 = new G4PVPlacement(0,
				PMTAl2,
				logicPMTAl2,
				"PMTAl2",
				logicRPDWorld,
				false,
				0); 
  G4ThreeVector PMTAl3 = G4ThreeVector(0,RPDYInitial+2.*cm,0.44*cm);
  solidPMTAl3 = new G4Box("PMTAl3",4.1*RPDXsize,RPDYsize,0.01*mm/SizeMod);
  logicPMTAl3 = new G4LogicalVolume(solidPMTAl3, Al, "PMTAl3");
  physiPMTAl3 = new G4PVPlacement(0,
				PMTAl3,
				logicPMTAl3,
				"PMTAl3",
				logicRPDWorld,
				false,
				0); 
  G4ThreeVector PMTAl4 = G4ThreeVector(0,RPDYInitial+2.*cm,0.31*cm);
  solidPMTAl4 = new G4Box("PMTAl4",4.1*RPDXsize,.75*cm,0.01*mm/SizeMod);
  logicPMTAl4 = new G4LogicalVolume(solidPMTAl4, Al, "PMTAl4");
  physiPMTAl4 = new G4PVPlacement(0,
				PMTAl4,
				logicPMTAl4,
				"PMTAl4",
				logicRPDWorld,
				false,
				0);  */
				
  G4ThreeVector positionAltop1 = G4ThreeVector(0,RPDYInitial+RPDYsize+.5*mm,-.2*cm);
  solidAlsub = new G4Box("rpd1",0.55*mm,0.5*mm/SizeMod,0.55*mm/SizeMod);
  solidAltop1 = new G4Box("Altop1",4*(RPDXsize+.1*cm),0.04999*mm/SizeMod,RPDZsize);  
  G4SubtractionSolid* newAlTop1a = new G4SubtractionSolid("newaltop1a", solidAltop1, solidAlsub,0,G4ThreeVector(6*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1b = new G4SubtractionSolid("newaltop1b", newAlTop1a, solidAlsub,0,G4ThreeVector(10*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1c = new G4SubtractionSolid("newaltop1c", newAlTop1b, solidAlsub,0,G4ThreeVector(14*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1d = new G4SubtractionSolid("newaltop1d", newAlTop1c, solidAlsub,0,G4ThreeVector(18*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1e = new G4SubtractionSolid("newaltop1e", newAlTop1d, solidAlsub,0,G4ThreeVector(27*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1f = new G4SubtractionSolid("newaltop1f", newAlTop1e, solidAlsub,0,G4ThreeVector(31*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1g = new G4SubtractionSolid("newaltop1g", newAlTop1f, solidAlsub,0,G4ThreeVector(35*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1h = new G4SubtractionSolid("newaltop1h", newAlTop1g, solidAlsub,0,G4ThreeVector(39*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1i = new G4SubtractionSolid("newaltop1i", newAlTop1h, solidAlsub,0,G4ThreeVector(-3*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1j = new G4SubtractionSolid("newaltop1j", newAlTop1i, solidAlsub,0,G4ThreeVector(-7*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1k = new G4SubtractionSolid("newaltop1k", newAlTop1j, solidAlsub,0,G4ThreeVector(-11*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1l = new G4SubtractionSolid("newaltop1l", newAlTop1k, solidAlsub,0,G4ThreeVector(-15*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1m = new G4SubtractionSolid("newaltop1m", newAlTop1l, solidAlsub,0,G4ThreeVector(-24*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1n = new G4SubtractionSolid("newaltop1n", newAlTop1m, solidAlsub,0,G4ThreeVector(-28*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1o = new G4SubtractionSolid("newaltop1o", newAlTop1n, solidAlsub,0,G4ThreeVector(-32*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop1p = new G4SubtractionSolid("newaltop1p", newAlTop1o, solidAlsub,0,G4ThreeVector(-36*mm,0,0.45*cm));
  logicAltop1 = new G4LogicalVolume(newAlTop1p, Al, "Altop1");
  physiAltop1 = new G4PVPlacement(0,
				positionAltop1,
				logicAltop1,
				"Altop1",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector positionAlbottom1 = G4ThreeVector(0,RPDYInitial-RPDYsize-.25*mm,-.2*cm);
  solidAlbottom1 = new G4Box("Albottom1",4*(RPDXsize+.1*cm),0.04999*mm/SizeMod,RPDZsize);  
  logicAlbottom1 = new G4LogicalVolume(solidAlbottom1, Al, "Albottom1");
  physiAlbottom1 = new G4PVPlacement(0,
				positionAlbottom1,
				logicAlbottom1,
				"Albottom1",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector positionAlbottom2 = G4ThreeVector(0,RPDYInitial-3*(RPDYsize+.4*mm),-.1*cm);
  solidAlbottom2 = new G4Box("Albottom2",4*(RPDXsize+.1*cm),0.04999*mm/SizeMod,RPDZsize);  
  logicAlbottom2 = new G4LogicalVolume(solidAlbottom2, Al, "Albottom2");
  physiAlbottom2 = new G4PVPlacement(0,
				positionAlbottom2,
				logicAlbottom2,
				"Albottom2",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector positionAlbottom3 = G4ThreeVector(0,RPDYInitial-5*(RPDYsize+.45*mm),0*cm);
  solidAlbottom3 = new G4Box("Albottom3",4*(RPDXsize+.05*cm),0.04999*mm/SizeMod,RPDZsize);  
  logicAlbottom3 = new G4LogicalVolume(solidAlbottom3, Al, "Albottom3");
  physiAlbottom3 = new G4PVPlacement(0,
				positionAlbottom3,
				logicAlbottom3,
				"Albottom3",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector positionAlbottom4 = G4ThreeVector(0,RPDYInitial-7*(RPDYsize+.45*mm),0*cm);
  solidAlbottom4 = new G4Box("Albottom4",4*(RPDXsize+.05*cm),0.04999*mm/SizeMod,RPDZsize);  
  logicAlbottom4 = new G4LogicalVolume(solidAlbottom4, Al, "Albottom4");
  physiAlbottom4 = new G4PVPlacement(0,
				positionAlbottom4,
				logicAlbottom4,
				"Albottom4",
				logicRPDWorld,
				false,
				0); 				
				
  G4ThreeVector positionAltop2 = G4ThreeVector(0,RPDYInitial-RPDYsize-.55*mm,-.0999*cm);
  solidAltop2 = new G4Box("Altop2",4*(RPDXsize+.1*cm),0.04999*mm/SizeMod,RPDZsize);    
  G4SubtractionSolid* newAlTop2a = new G4SubtractionSolid("newaltop2a", solidAltop2, solidAlsub,0,G4ThreeVector(6*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2b = new G4SubtractionSolid("newaltop2b", newAlTop2a, solidAlsub,0,G4ThreeVector(10*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2c = new G4SubtractionSolid("newaltop2c", newAlTop2b, solidAlsub,0,G4ThreeVector(14*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2d = new G4SubtractionSolid("newaltop2d", newAlTop2c, solidAlsub,0,G4ThreeVector(18*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2e = new G4SubtractionSolid("newaltop2e", newAlTop2d, solidAlsub,0,G4ThreeVector(27*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2f = new G4SubtractionSolid("newaltop2f", newAlTop2e, solidAlsub,0,G4ThreeVector(31*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2g = new G4SubtractionSolid("newaltop2g", newAlTop2f, solidAlsub,0,G4ThreeVector(35*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2h = new G4SubtractionSolid("newaltop2h", newAlTop2g, solidAlsub,0,G4ThreeVector(39*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2i = new G4SubtractionSolid("newaltop2i", newAlTop2h, solidAlsub,0,G4ThreeVector(-3*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2j = new G4SubtractionSolid("newaltop2j", newAlTop2i, solidAlsub,0,G4ThreeVector(-7*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2k = new G4SubtractionSolid("newaltop2k", newAlTop2j, solidAlsub,0,G4ThreeVector(-11*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2l = new G4SubtractionSolid("newaltop2l", newAlTop2k, solidAlsub,0,G4ThreeVector(-15*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2m = new G4SubtractionSolid("newaltop2m", newAlTop2l, solidAlsub,0,G4ThreeVector(-24*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2n = new G4SubtractionSolid("newaltop2n", newAlTop2m, solidAlsub,0,G4ThreeVector(-28*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2o = new G4SubtractionSolid("newaltop2o", newAlTop2n, solidAlsub,0,G4ThreeVector(-32*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop2p = new G4SubtractionSolid("newaltop2p", newAlTop2o, solidAlsub,0,G4ThreeVector(-36*mm,0,0.45*cm));

  logicAltop2 = new G4LogicalVolume(newAlTop2p, Al, "Altop2");
  physiAltop2 = new G4PVPlacement(0,
				positionAltop2,
				logicAltop2,
				"Altop2",
				logicRPDWorld,
				false,
				0); 
				

  
  G4ThreeVector positionAltop3 = G4ThreeVector(0,RPDYInitial-3*(RPDYsize+.55*mm),0*cm);
  solidAltop3 = new G4Box("Altop3",4*(RPDXsize+.1*cm),0.04999*mm/SizeMod,RPDZsize);  
  G4SubtractionSolid* newAlTop3a = new G4SubtractionSolid("newaltop3a", solidAltop3, solidAlsub,0,G4ThreeVector(6*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3b = new G4SubtractionSolid("newaltop3b", newAlTop3a, solidAlsub,0,G4ThreeVector(10*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3c = new G4SubtractionSolid("newaltop3c", newAlTop3b, solidAlsub,0,G4ThreeVector(14*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3d = new G4SubtractionSolid("newaltop3d", newAlTop3c, solidAlsub,0,G4ThreeVector(18*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3e = new G4SubtractionSolid("newaltop3e", newAlTop3d, solidAlsub,0,G4ThreeVector(27*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3f = new G4SubtractionSolid("newaltop3f", newAlTop3e, solidAlsub,0,G4ThreeVector(31*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3g = new G4SubtractionSolid("newaltop3g", newAlTop3f, solidAlsub,0,G4ThreeVector(35*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3h = new G4SubtractionSolid("newaltop3h", newAlTop3g, solidAlsub,0,G4ThreeVector(39*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3i = new G4SubtractionSolid("newaltop3i", newAlTop3h, solidAlsub,0,G4ThreeVector(-3*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3j = new G4SubtractionSolid("newaltop3j", newAlTop3i, solidAlsub,0,G4ThreeVector(-7*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3k = new G4SubtractionSolid("newaltop3k", newAlTop3j, solidAlsub,0,G4ThreeVector(-11*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3l = new G4SubtractionSolid("newaltop3l", newAlTop3k, solidAlsub,0,G4ThreeVector(-15*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3m = new G4SubtractionSolid("newaltop3m", newAlTop3l, solidAlsub,0,G4ThreeVector(-24*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3n = new G4SubtractionSolid("newaltop3n", newAlTop3m, solidAlsub,0,G4ThreeVector(-28*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3o = new G4SubtractionSolid("newaltop3o", newAlTop3n, solidAlsub,0,G4ThreeVector(-32*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop3p = new G4SubtractionSolid("newaltop3p", newAlTop3o, solidAlsub,0,G4ThreeVector(-36*mm,0,0.45*cm));
  logicAltop3 = new G4LogicalVolume(newAlTop3p, Al, "Altop3");
  physiAltop3 = new G4PVPlacement(0,
				positionAltop3,
				logicAltop3,
				"Altop3",
				logicRPDWorld,
				false,
				0); 
  G4ThreeVector positionAltop4 = G4ThreeVector(0,RPDYInitial-5*(RPDYsize+.55*mm),.0999*cm);
  solidAltop4 = new G4Box("Altop4",4*(RPDXsize+.1*cm),0.04999*mm/SizeMod,RPDZsize);  
  G4SubtractionSolid* newAlTop4a = new G4SubtractionSolid("newaltop4a", solidAltop4, solidAlsub,0,G4ThreeVector(6*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4b = new G4SubtractionSolid("newaltop4b", newAlTop4a, solidAlsub,0,G4ThreeVector(10*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4c = new G4SubtractionSolid("newaltop4c", newAlTop4b, solidAlsub,0,G4ThreeVector(14*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4d = new G4SubtractionSolid("newaltop4d", newAlTop4c, solidAlsub,0,G4ThreeVector(18*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4e = new G4SubtractionSolid("newaltop4e", newAlTop4d, solidAlsub,0,G4ThreeVector(27*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4f = new G4SubtractionSolid("newaltop4f", newAlTop4e, solidAlsub,0,G4ThreeVector(31*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4g = new G4SubtractionSolid("newaltop4g", newAlTop4f, solidAlsub,0,G4ThreeVector(35*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4h = new G4SubtractionSolid("newaltop4h", newAlTop4g, solidAlsub,0,G4ThreeVector(39*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4i = new G4SubtractionSolid("newaltop4i", newAlTop4h, solidAlsub,0,G4ThreeVector(-3*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4j = new G4SubtractionSolid("newaltop4j", newAlTop4i, solidAlsub,0,G4ThreeVector(-7*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4k = new G4SubtractionSolid("newaltop4k", newAlTop4j, solidAlsub,0,G4ThreeVector(-11*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4l = new G4SubtractionSolid("newaltop4l", newAlTop4k, solidAlsub,0,G4ThreeVector(-15*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4m = new G4SubtractionSolid("newaltop4m", newAlTop4l, solidAlsub,0,G4ThreeVector(-24*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4n = new G4SubtractionSolid("newaltop4n", newAlTop4m, solidAlsub,0,G4ThreeVector(-28*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4o = new G4SubtractionSolid("newaltop4o", newAlTop4n, solidAlsub,0,G4ThreeVector(-32*mm,0,0.45*cm));
  G4SubtractionSolid* newAlTop4p = new G4SubtractionSolid("newaltop4p", newAlTop4o, solidAlsub,0,G4ThreeVector(-36*mm,0,0.45*cm));
  logicAltop4 = new G4LogicalVolume(newAlTop4p, Al, "Altop4");
  physiAltop4 = new G4PVPlacement(0,
				positionAltop4,
				logicAltop4,
				"Altop4",
				logicRPDWorld,
				false,
				0); 

  G4ThreeVector positionAlside1 = G4ThreeVector(RPDXInitial+RPDXsize+.5*mm,.9*cm,0*cm);
  solidAlside1 = new G4Box("Alside1",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize);  
  logicAlside1 = new G4LogicalVolume(solidAlside1, Al, "Alside1");
  physiAlside1 = new G4PVPlacement(0,
				positionAlside1,
				logicAlside1,
				"Alside1",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector positionAlside3 = G4ThreeVector(RPDXInitial+(RPDXsize+.5*mm),3.0*cm,-.1*cm);
  solidAlside3 = new G4Box("Alside3",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize);  
  logicAlside3 = new G4LogicalVolume(solidAlside3, Al, "Alside3");
  physiAlside3 = new G4PVPlacement(0,
				positionAlside3,
				logicAlside3,
				"Alside3",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector positionAlside5 = G4ThreeVector(RPDXInitial+(RPDXsize+.5*mm),-1.2*cm,0.1*cm);
  solidAlside5 = new G4Box("Alside5",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside5 = new G4LogicalVolume(solidAlside5, Al, "Alside5");
  physiAlside5 = new G4PVPlacement(0,
				positionAlside5,
				logicAlside5,
				"Alside5",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector positionAlside2 = G4ThreeVector(RPDXInitial+3*(RPDXsize+.5*mm),.9*cm,0*cm);
  solidAlside2 = new G4Box("Alside2",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside2 = new G4LogicalVolume(solidAlside2, Al, "Alside2");
  physiAlside2 = new G4PVPlacement(0,
				positionAlside2,
				logicAlside2,
				"Alside2",
				logicRPDWorld,
				false,
				0); 
				


				
  G4ThreeVector positionAlside4 = G4ThreeVector(RPDXInitial+3*(RPDXsize+.5*mm),3.0*cm,-.1*cm);
  solidAlside4 = new G4Box("Alside4",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside4 = new G4LogicalVolume(solidAlside4, Al, "Alside4");
  physiAlside4 = new G4PVPlacement(0,
				positionAlside4,
				logicAlside4,
				"Alside4",
				logicRPDWorld,
				false,
				0); 
				

				
  G4ThreeVector positionAlside6 = G4ThreeVector(RPDXInitial+3*(RPDXsize+.5*mm),-1.2*cm,0.1*cm);
  solidAlside6 = new G4Box("Alside6",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside6 = new G4LogicalVolume(solidAlside6, Al, "Alside6");
  physiAlside6 = new G4PVPlacement(0,
				positionAlside6,
				logicAlside6,
				"Alside6",
				logicRPDWorld,
				false,
				0); 
				

  G4ThreeVector positionAlside7 = G4ThreeVector(RPDXInitial+(RPDXsize+.5*mm),-3.3*cm,0.2*cm);
  solidAlside7 = new G4Box("Alside7",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside7 = new G4LogicalVolume(solidAlside7, Al, "Alside7");
  physiAlside7 = new G4PVPlacement(0,
				positionAlside7,
				logicAlside7,
				"Alside7",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector positionAlside8 = G4ThreeVector(RPDXInitial+3*(RPDXsize+.5*mm),-3.3*cm,0.2*cm);
  solidAlside8 = new G4Box("Alside8",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside8 = new G4LogicalVolume(solidAlside8, Al, "Alside8");
  physiAlside8 = new G4PVPlacement(0,
				positionAlside8,
				logicAlside8,
				"Alside8",
				logicRPDWorld,
				false,
				0); 
				
				
  G4ThreeVector positionAlside9 = G4ThreeVector(RPDXInitial+5*(RPDXsize+.5*mm),3.0*cm,-.1*cm);
  solidAlside9 = new G4Box("Alside9",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside9 = new G4LogicalVolume(solidAlside9, Al, "Alside9");
  physiAlside9 = new G4PVPlacement(0,
				positionAlside9,
				logicAlside9,
				"Alside9",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector positionAlside10 = G4ThreeVector(RPDXInitial+5*(RPDXsize+.5*mm),-1.2*cm,0.1*cm);
  solidAlside10 = new G4Box("Alside10",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside10 = new G4LogicalVolume(solidAlside10, Al, "Alside10");
  physiAlside10 = new G4PVPlacement(0,
				positionAlside10,
				logicAlside10,
				"Alside10",
				logicRPDWorld,
				false,
				0); 
				

  G4ThreeVector positionAlside11 = G4ThreeVector(RPDXInitial+5*(RPDXsize+.5*mm),.9*cm,0*cm);
  solidAlside11 = new G4Box("Alside11",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside11 = new G4LogicalVolume(solidAlside11, Al, "Alside11");
  physiAlside11 = new G4PVPlacement(0,
				positionAlside11,
				logicAlside11,
				"Alside11",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector positionAlside12 = G4ThreeVector(RPDXInitial+5*(RPDXsize+.5*mm),-3.3*cm,0.2*cm);
  solidAlside12 = new G4Box("Alside12",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside12 = new G4LogicalVolume(solidAlside12, Al, "Alside12");
  physiAlside12 = new G4PVPlacement(0,
				positionAlside12,
				logicAlside12,
				"Alside12",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector positionAlside13 = G4ThreeVector(RPDXInitial+7*(RPDXsize+.5*mm),3.0*cm,-.1*cm);
  solidAlside13 = new G4Box("Alside13",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside13 = new G4LogicalVolume(solidAlside13, Al, "Alside13");
  physiAlside13 = new G4PVPlacement(0,
				positionAlside13,
				logicAlside13,
				"Alside13",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector positionAlside14 = G4ThreeVector(RPDXInitial+7*(RPDXsize+.5*mm),-1.2*cm,0.1*cm);
  solidAlside14 = new G4Box("Alside14",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside14 = new G4LogicalVolume(solidAlside14, Al, "Alside14");
  physiAlside14 = new G4PVPlacement(0,
				positionAlside14,
				logicAlside14,
				"Alside14",
				logicRPDWorld,
				false,
				0); 
				

  G4ThreeVector positionAlside15 = G4ThreeVector(RPDXInitial+7*(RPDXsize+.5*mm),.9*cm,0*cm);
  solidAlside15 = new G4Box("Alside15",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside15 = new G4LogicalVolume(solidAlside15, Al, "Alside15");
  physiAlside15 = new G4PVPlacement(0,
				positionAlside15,
				logicAlside15,
				"Alside15",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector positionAlside16 = G4ThreeVector(RPDXInitial+7*(RPDXsize+.5*mm),-3.3*cm,0.2*cm);
  solidAlside16 = new G4Box("Alside16",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside16 = new G4LogicalVolume(solidAlside16, Al, "Alside16");
  physiAlside16 = new G4PVPlacement(0,
				positionAlside16,
				logicAlside16,
				"Alside16",
				logicRPDWorld,
				false,
				0); 
				
  G4ThreeVector positionAlside17 = G4ThreeVector(RPDXInitial-(RPDXsize+.5*mm),3.0*cm,-.1*cm);
  solidAlside17 = new G4Box("Alside17",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside17 = new G4LogicalVolume(solidAlside17, Al, "Alside17");
  physiAlside17 = new G4PVPlacement(0,
				positionAlside17,
				logicAlside17,
				"Alside17",
				logicRPDWorld,
				false,
				0); 

//red
  G4ThreeVector positionAlside19 = G4ThreeVector(RPDXInitial-(RPDXsize+.5*mm),.9*cm,0*cm);
  solidAlside19 = new G4Box("Alside19",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside19 = new G4LogicalVolume(solidAlside19, Al, "Alside19");
  physiAlside19 = new G4PVPlacement(0,
				positionAlside19,
				logicAlside19,
				"Alside19",
				logicRPDWorld,
				false,
				0); 
				
//purple
  G4ThreeVector positionAlside18 = G4ThreeVector(RPDXInitial-(RPDXsize+.5*mm),-1.2*cm,0.1*cm);
  solidAlside18 = new G4Box("Alside18",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside18 = new G4LogicalVolume(solidAlside18, Al, "Alside18");
  physiAlside18 = new G4PVPlacement(0,
				positionAlside18,
				logicAlside18,
				"Alside18",
				logicRPDWorld,
				false,
				0); 
				
				
  G4ThreeVector positionAlside20 = G4ThreeVector(RPDXInitial-(RPDXsize+.5*mm),-3.3*cm,0.2*cm);
  solidAlside20 = new G4Box("Alside20",0.04999*mm/SizeMod,RPDXsize+.1,RPDZsize+.1*cm);  
  logicAlside20 = new G4LogicalVolume(solidAlside20, Al, "Alside20");
  physiAlside20 = new G4PVPlacement(0,
				positionAlside20,
				logicAlside20,
				"Alside20",
				logicRPDWorld,
				false,

				0); 

/*				
  G4ThreeVector positionWLSWorld1 = G4ThreeVector(-3.6*cm,3.499*cm,.2475*cm);
  solidWLSWorld1 = new G4Box("wlsworld1",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld1 = new G4LogicalVolume(solidWLSWorld1, Grease, "WLSWorld1");
  physiWLSWorld1 = new G4PVPlacement(0,
				    positionWLSWorld1,
				    logicWLSWorld1,
				    "WLSWorld1",
				    logicRPDWorld,
				    false,
				    0);
  G4ThreeVector positionWLSWorld2 = G4ThreeVector(-3.2*cm,3.499*cm,.2475*cm);
  solidWLSWorld2 = new G4Box("wlsworld2",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld2 = new G4LogicalVolume(solidWLSWorld2, Grease, "WLSWorld2");
  physiWLSWorld2 = new G4PVPlacement(0,
				    positionWLSWorld2,
				    logicWLSWorld2,
				    "WLSWorld1",
				    logicRPDWorld,
				    false,
				    0);
  G4ThreeVector positionWLSWorld3 = G4ThreeVector(-2.8*cm,3.499*cm,.2475*cm);
  solidWLSWorld3 = new G4Box("wlsworld3",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld3 = new G4LogicalVolume(solidWLSWorld3, Grease, "WLSWorld3");
  physiWLSWorld3 = new G4PVPlacement(0,
				    positionWLSWorld3,
				    logicWLSWorld3,
				    "WLSWorld3",
				    logicRPDWorld,
				    false,
				    0);
  G4ThreeVector positionWLSWorld4 = G4ThreeVector(-2.4*cm,3.499*cm,.2475*cm);
  solidWLSWorld4 = new G4Box("wlsworld4",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld4 = new G4LogicalVolume(solidWLSWorld4, Grease, "WLSWorld4");
  physiWLSWorld4 = new G4PVPlacement(0,
				    positionWLSWorld4,
				    logicWLSWorld4,
				    "WLSWorld4",
				    logicRPDWorld,
				    false,
				    0);


*/
  


// Center Vector
  G4ThreeVector Pos_WLSRPD = G4ThreeVector(0.,0.,0.);
  G4ThreeVector wlsShift = G4ThreeVector(0., 0.0125*cm, 0.);
////


//These are now used in RPD solid
//**Union happens BEFORE any physical translation

  G4ThreeVector positionWLSWorld1 = G4ThreeVector(-3.6*cm,3.499*cm,.2475*cm);
  solidWLSWorld1 = new G4Box("wlsworld1",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld1 = new G4LogicalVolume(solidWLSWorld1, Grease, "WLSWorld1");

  G4ThreeVector positionWLSWorld2 = G4ThreeVector(-3.2*cm,3.499*cm,.2475*cm);
  solidWLSWorld2 = new G4Box("wlsworld2",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld2 = new G4LogicalVolume(solidWLSWorld2, Grease, "WLSWorld2");

  G4ThreeVector positionWLSWorld3 = G4ThreeVector(-2.8*cm,3.499*cm,.2475*cm);
  solidWLSWorld3 = new G4Box("wlsworld3",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld3 = new G4LogicalVolume(solidWLSWorld3, Grease, "WLSWorld3");

  G4ThreeVector positionWLSWorld4 = G4ThreeVector(-2.4*cm,3.499*cm,.2475*cm);
  solidWLSWorld4 = new G4Box("wlsworld4",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld4 = new G4LogicalVolume(solidWLSWorld4, Grease, "WLSWorld4");



  G4ThreeVector positionRPD1 = G4ThreeVector(RPDXInitial+(0*RPDXDelta),RPDYInitial,-.2*cm);
  solidRPD1 = new G4Box("rpd1",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD1a = new G4UnionSolid("newrpd1a", solidRPD1, solidWLSWorld1,0,(positionWLSWorld1-positionRPD1));
  G4UnionSolid* newRPD1b = new G4UnionSolid("newrpd1b", newRPD1a, solidWLSWorld2,0,(positionWLSWorld2-positionRPD1));
  G4UnionSolid* newRPD1c = new G4UnionSolid("newrpd1c", newRPD1b, solidWLSWorld3,0,(positionWLSWorld3-positionRPD1));
  G4UnionSolid* newRPD1d = new G4UnionSolid("newrpd1d", newRPD1c, solidWLSWorld4,0,(positionWLSWorld4-positionRPD1));
  logicRPD1 = new G4LogicalVolume(newRPD1d, RPD_Material, "RPD1",0,0,0);
  physiRPD1 = new G4PVPlacement(0,               // no rotation
                                   positionRPD1,  // at (x,y,z)
                                   logicRPD1,     // its logical volume                             
                                   "RPD1",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld1 = new G4PVPlacement(0,
				    (positionWLSWorld1-positionRPD1),
				    logicWLSWorld1,
				    "WLSWorld1",
				    logicRPD1,
				    false,
				    0);

  physiWLSWorld2 = new G4PVPlacement(0,
				    (positionWLSWorld2-positionRPD1),
				    logicWLSWorld2,
				    "WLSWorld1",
				    logicRPD1,
				    false,
				    0);

 physiWLSWorld3 = new G4PVPlacement(0,
				    (positionWLSWorld3-positionRPD1),
				    logicWLSWorld3,
				    "WLSWorld3",
				    logicRPD1,
				    false,
				    0);

 physiWLSWorld4 = new G4PVPlacement(0,
				    (positionWLSWorld4-positionRPD1),
				    logicWLSWorld4,
				    "WLSWorld4",
				    logicRPD1,
				    false,
				    0);



//WLS_1
  



//////cladding  
  G4VSolid* solidClad1;
  solidClad1 = new G4Tubs("Clad1",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad1 = new G4LogicalVolume(solidClad1,
                                      Pethylene,
                                      "Clad1");
  physiClad1 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad1,
                                    "Clad1",
                                    logicWLSWorld1,
                                    false,
                                    0); 
  /*                                
G4VSolid* solidCladAl1;
  solidCladAl1 = new G4Tubs("CladAl1",CladouterRadius*1.01,CladouterRadius*1.05,.5*cm,startAngle, spanningAngle);
  logicCladAl1 = new G4LogicalVolume(solidCladAl1,
                                      Al,
                                      "CladAl1");
  physiCladAl1 = new G4PVPlacement(myRotation,
                                   G4ThreeVector((RPDXInitial+(0*RPDXDelta)-.6*cm),4.5*cm,.245*cm),
                                    logicCladAl1,
                                    "CladAl1",
                                    logicRPDWorld,
                                    false,
                                    0); */
                                    
/////////wls
  WLSFiberLogic1 = new G4Tubs("wls1",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog1 = new G4LogicalVolume(WLSFiberLogic1,PMMA,"WLS1");
  WLSLog1->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber1 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog1,
                                       "WLSRPD1",
                                       logicClad1,
                                       false,
                                       0);

 ////


//WLS_2



//////cladding  
  G4VSolid* solidClad2;
  solidClad2 = new G4Tubs("Clad2",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad2 = new G4LogicalVolume(solidClad2,
                                      Pethylene,
                                      "Clad2");
  physiClad2 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad2,
                                    "Clad2",
                                    logicWLSWorld2,
                                    false,
                                    0);
/*G4VSolid* solidCladAl2;
  solidCladAl2 = new G4Tubs("CladAl2",CladouterRadius*1.01,CladouterRadius*1.05,.5*cm,startAngle, spanningAngle);
  logicCladAl2 = new G4LogicalVolume(solidCladAl2,
                                      Al,
                                      "CladAl2");
  physiCladAl2 = new G4PVPlacement(myRotation,
                                   G4ThreeVector((RPDXInitial+(0*RPDXDelta)-.2*cm),4.5*cm,.245*cm),
                                    logicCladAl2,
                                    "CladAl2",
                                    logicRPDWorld,
                                    false,
                                    0); */
/////////wls
  WLSFiberLogic2 = new G4Tubs("wls2",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog2 = new G4LogicalVolume(WLSFiberLogic2,PMMA,"WLS2");
  WLSLog2->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber2 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog2,
                                       "WLSRPD2",
                                       logicClad2,
                                       false,
                                       0);
				
  ////


//WLS_3



//////cladding  
  G4VSolid* solidClad3;
  solidClad3 = new G4Tubs("Clad3",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad3 = new G4LogicalVolume(solidClad3,
                                      Pethylene,
                                      "Clad3");
  physiClad3 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad3,
                                    "Clad3",
                                    logicWLSWorld3,
                                    false,
                                    0);
                                    
/*G4VSolid* solidCladAl3;
  solidCladAl3 = new G4Tubs("CladAl3",CladouterRadius*1.01,CladouterRadius*1.05,.5*cm,startAngle, spanningAngle);
  logicCladAl3 = new G4LogicalVolume(solidCladAl3,
                                      Al,
                                      "CladAl3");
  physiCladAl3 = new G4PVPlacement(myRotation,
                                   G4ThreeVector((RPDXInitial+(0*RPDXDelta)+.6*cm),4.5*cm,.245*cm),
                                    logicCladAl3,
                                    "CladAl3",
                                    logicRPDWorld,
                                    false,
                                    0); */
/////////wls
  WLSFiberLogic3 = new G4Tubs("wls3",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog3 = new G4LogicalVolume(WLSFiberLogic3,PMMA,"WLS3");
  WLSLog3->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber3 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog3,
                                       "WLSRPD3",
                                       logicClad3,
                                       false,
                                       0);
				
  ////


//WLS_4



  
  G4VSolid* solidClad4;
  solidClad4 = new G4Tubs("Clad4",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad4 = new G4LogicalVolume(solidClad4,
                                      Pethylene,
                                      "Clad4");
  physiClad4 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad4,
                                    "Clad4",
                                    logicWLSWorld4,
                                    false,
                                    0);
                                    


  WLSFiberLogic4 = new G4Tubs("wls4",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog4 = new G4LogicalVolume(WLSFiberLogic4,PMMA,"WLS4");
  WLSLog4->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber4 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog4,
                                       "WLSRPD4",
                                       logicClad4,
                                       false,
                                       0);
				


//////


  G4ThreeVector positionWLSWorld5 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)-.6*cm),3.499*cm,.2475*cm);
  solidWLSWorld5 = new G4Box("wlsworld5",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld5 = new G4LogicalVolume(solidWLSWorld5, Grease, "WLSWorld5");

  G4ThreeVector positionWLSWorld6 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)-.2*cm),3.499*cm,.2475*cm);
  solidWLSWorld6 = new G4Box("wlsworld5",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld6 = new G4LogicalVolume(solidWLSWorld6, Grease, "WLSWorld6");

 G4ThreeVector positionWLSWorld7 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)+.6*cm),3.499*cm,.2475*cm);
  solidWLSWorld7 = new G4Box("wlsworld7",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld7 = new G4LogicalVolume(solidWLSWorld7, Grease, "WLSWorld7");

  G4ThreeVector positionWLSWorld8 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)+.2*cm),3.499*cm,.2475*cm);
  solidWLSWorld8 = new G4Box("wlsworld8",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld8 = new G4LogicalVolume(solidWLSWorld8, Grease, "WLSWorld8");



  //empty line
  //--------------------------
  // RPD Channel 2
  //--------------------------
  G4ThreeVector positionRPD2 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)),RPDYInitial,-.2*cm);
  solidRPD2 = new G4Box("rpd2",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD2a = new G4UnionSolid("newrpd2a", solidRPD2, solidWLSWorld5,0,(positionWLSWorld5-positionRPD2));
  G4UnionSolid* newRPD2b = new G4UnionSolid("newrpd2b", newRPD2a, solidWLSWorld6,0,(positionWLSWorld6-positionRPD2));
  G4UnionSolid* newRPD2c = new G4UnionSolid("newrpd2c", newRPD2b, solidWLSWorld7,0,(positionWLSWorld7-positionRPD2));
  G4UnionSolid* newRPD2d = new G4UnionSolid("newrpd2d", newRPD2c, solidWLSWorld8,0,(positionWLSWorld8-positionRPD2));
  logicRPD2 = new G4LogicalVolume(newRPD2d, RPD_Material, "RPD2",0,0,0);
  physiRPD2 = new G4PVPlacement(0,               // no rotation
                                   positionRPD2,  // at (x,y,z)
                                   logicRPD2,     // its logical volume                             
                                   "RPD2",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld5 = new G4PVPlacement(0,
				    (positionWLSWorld5-positionRPD2),
				    logicWLSWorld5,
				    "WLSWorld5",
				    logicRPD2,
				    false,
				    0);

  physiWLSWorld6 = new G4PVPlacement(0,
				    (positionWLSWorld6-positionRPD2),
				    logicWLSWorld6,
				    "WLSWorld6",
				    logicRPD2,
				    false,
				    0);

 physiWLSWorld7 = new G4PVPlacement(0,
				    (positionWLSWorld7-positionRPD2),
				    logicWLSWorld7,
				    "WLSWorld7",
				    logicRPD2,
				    false,
				    0);

 physiWLSWorld8 = new G4PVPlacement(0,
				    (positionWLSWorld8-positionRPD2),
				    logicWLSWorld8,
				    "WLSWorld8",
				    logicRPD2,
				    false,
				    0);


				


				
  G4ThreeVector positionAl2 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)),3.51*cm,.305*cm);
  solidAl2 = new G4Box("Al2",RPDXsize+.0425*cm,1.5*cm,0.005*mm/SizeMod);
  logicAl2 = new G4LogicalVolume(solidAl2, Al, "Al2");
  physiAl2 = new G4PVPlacement(0,
				positionAl2,
				logicAl2,
				"Al2",
				logicRPDWorld,
				false,
				0); 				
				

////


//WLS_5



/////cladding  
  G4VSolid* solidClad5;
  solidClad5 = new G4Tubs("Clad5",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad5 = new G4LogicalVolume(solidClad5,
                                      Pethylene,
                                      "Clad5");
  physiClad5 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad5,
                                    "Clad5",
                                    logicWLSWorld5,
                                    false,
                                    0);
     

                             

/////////wls


  WLSFiberLogic5 = new G4Tubs("wls5",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog5 = new G4LogicalVolume(WLSFiberLogic5,PMMA,"WLS5");
  WLSLog5->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber5 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog5,
                                       "WLSRPD5",
                                       logicClad5,
                                       false,
                                       0);

////


//WLS_6




 
  G4VSolid* solidClad6;
  solidClad6 = new G4Tubs("Clad6",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad6 = new G4LogicalVolume(solidClad6,
                                      Pethylene,
                                      "Clad6");
  physiClad6 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad6,
                                    "Clad6",
                                    logicWLSWorld6,
                                    false,
                                    0);
/////////wls

  WLSFiberLogic6 = new G4Tubs("wls6",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog6 = new G4LogicalVolume(WLSFiberLogic6,PMMA,"WLS6");
  WLSLog6->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber6 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog6,
                                       "WLSRPD6",
                                       logicClad6,
                                       false,
                                       0);


////


//WLS_7




  G4VSolid* solidClad7;
  solidClad7 = new G4Tubs("Clad7",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad7 = new G4LogicalVolume(solidClad7,
                                      Pethylene,
                                      "Clad7");
  physiClad7 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad7,
                                    "Clad7",
                                    logicWLSWorld7,
                                    false,
                                    0);

/////////wls

  WLSFiberLogic7 = new G4Tubs("wls7",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog7 = new G4LogicalVolume(WLSFiberLogic7,PMMA,"WLS7");
  WLSLog7->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber7 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog7,
                                       "WLSRPD7",
                                       logicClad7,
                                       false,
                                       0);


////


//WLS_8




  G4VSolid* solidClad8;
  solidClad8 = new G4Tubs("Clad8",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad8 = new G4LogicalVolume(solidClad8,
                                      Pethylene,
                                      "Clad8");
  physiClad8 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad8,
                                    "Clad8",
                                    logicWLSWorld8,
                                    false,
                                    0);


  WLSFiberLogic8 = new G4Tubs("wls8",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog8 = new G4LogicalVolume(WLSFiberLogic8,PMMA,"WLS8");
  WLSLog8->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber8 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog8,
                                       "WLSRPD8",
                                       logicClad8,
                                       false,
                                       0);



  //empty line
  //--------------------------
  // RPD Channel 3
  //--------------------------
  G4ThreeVector positionWLSWorld9 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)-.6*cm),3.499*cm,.2475*cm);		//SUB
  solidWLSWorld9 = new G4Box("wlsworld9",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld9 = new G4LogicalVolume(solidWLSWorld9, Grease, "WLSWorld9");

  G4ThreeVector positionWLSWorld10 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)-.2*cm),3.499*cm,.2475*cm);	//SUB
  solidWLSWorld10 = new G4Box("wlsworld10",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld10 = new G4LogicalVolume(solidWLSWorld10, Grease, "WLSWorld10");

  G4ThreeVector positionWLSWorld11 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)+.6*cm),3.499*cm,.2475*cm);	//SUB	
  solidWLSWorld11 = new G4Box("wlsworld11",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld11 = new G4LogicalVolume(solidWLSWorld11, Grease, "WLSWorld11");

  G4ThreeVector positionWLSWorld12 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)+.2*cm),3.499*cm,.2475*cm);	//SUB
  solidWLSWorld12 = new G4Box("wlsworld12",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld12 = new G4LogicalVolume(solidWLSWorld12, Grease, "WLSWorld12");



  G4ThreeVector positionRPD3 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)),RPDYInitial,-.2*cm);			//SUB
  solidRPD3 = new G4Box("rpd3",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD3a = new G4UnionSolid("newrpd3a", solidRPD3, solidWLSWorld9,0,(positionWLSWorld9-positionRPD3));
  G4UnionSolid* newRPD3b = new G4UnionSolid("newrpd3b", newRPD3a, solidWLSWorld10,0,(positionWLSWorld10-positionRPD3));
  G4UnionSolid* newRPD3c = new G4UnionSolid("newrpd3c", newRPD3b, solidWLSWorld11,0,(positionWLSWorld11-positionRPD3));
  G4UnionSolid* newRPD3d = new G4UnionSolid("newrpd3d", newRPD3c, solidWLSWorld12,0,(positionWLSWorld12-positionRPD3));
  logicRPD3 = new G4LogicalVolume(newRPD3d, RPD_Material, "RPD3",0,0,0);
  physiRPD3 = new G4PVPlacement(0,               // no rotation
                                   positionRPD3,  // at (x,y,z)
                                   logicRPD3,     // its logical volume                             
                                   "RPD3",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld9 = new G4PVPlacement(0,
				    (positionWLSWorld9-positionRPD3),
				    logicWLSWorld9,
				    "WLSWorld9",
				    logicRPD3,
				    false,
				    0);

  physiWLSWorld10 = new G4PVPlacement(0,
				    (positionWLSWorld10-positionRPD3),
				    logicWLSWorld10,
				    "WLSWorld9",
				    logicRPD3,
				    false,
				    0);

 physiWLSWorld11 = new G4PVPlacement(0,
				    (positionWLSWorld11-positionRPD3),
				    logicWLSWorld11,
				    "WLSWorld11",
				    logicRPD3,
				    false,
				    0);

 physiWLSWorld12 = new G4PVPlacement(0,
				    (positionWLSWorld12-positionRPD3),
				    logicWLSWorld12,
				    "WLSWorld12",
				    logicRPD3,
				    false,
				    0);
////////				
  G4ThreeVector positionAl3 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)),3.51*cm,.305*cm);
  solidAl3 = new G4Box("Al3",RPDXsize+.0425*cm,1.5*cm,0.005*mm/SizeMod);
  logicAl3 = new G4LogicalVolume(solidAl3, Al, "Al3");
  physiAl3 = new G4PVPlacement(0,
				positionAl3,
				logicAl3,
				"Al3",
				logicRPDWorld,
				false,
				0); 				
////


//WLS_9



  G4VSolid* solidClad9;
  solidClad9 = new G4Tubs("Clad9",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad9 = new G4LogicalVolume(solidClad9,
                                      Pethylene,
                                      "Clad9");
  physiClad9 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad9,
                                    "Clad9",
                                    logicWLSWorld9,
                                    false,
                                    0);


  WLSFiberLogic9 = new G4Tubs("wls9",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog9 = new G4LogicalVolume(WLSFiberLogic9,PMMA,"WLS9");
  WLSLog9->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber9 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog9,
                                       "WLSRPD9",
                                       logicClad9,
                                       false,
                                       0);



                                    

////


//WLS_10




  G4VSolid* solidClad10;
  solidClad10 = new G4Tubs("Clad10",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad10 = new G4LogicalVolume(solidClad10,
                                      Pethylene,
                                      "Clad10");
  physiClad10 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad10,
                                    "Clad10",
                                    logicWLSWorld10,
                                    false,
                                    0);


  WLSFiberLogic10 = new G4Tubs("wls10",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog10 = new G4LogicalVolume(WLSFiberLogic10,PMMA,"WLS10");
  WLSLog10->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber10 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog10,
                                       "WLSRPD10",
                                       logicClad10,
                                       false,
                                       0);

////


//WLS_11




  G4VSolid* solidClad11;
  solidClad11 = new G4Tubs("Clad11",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad11 = new G4LogicalVolume(solidClad11,
                                      Pethylene,
                                      "Clad11");
  physiClad11 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad11,
                                    "Clad11",
                                    logicWLSWorld11,
                                    false,
                                    0);


  WLSFiberLogic11 = new G4Tubs("wls11",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog11 = new G4LogicalVolume(WLSFiberLogic11,PMMA,"WLS11");
  WLSLog11->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber11 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog11,
                                       "WLSRPD11",
                                       logicClad11,
                                       false,
                                       0);

////


//WLS_12





  G4VSolid* solidClad12;
  solidClad12 = new G4Tubs("Clad12",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad12 = new G4LogicalVolume(solidClad12,
                                      Pethylene,
                                      "Clad12");
  physiClad12 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad12,
                                    "Clad12",
                                    logicWLSWorld12,
                                    false,
                                    0);


  WLSFiberLogic12 = new G4Tubs("wls12",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog12 = new G4LogicalVolume(WLSFiberLogic12,PMMA,"WLS12");
  WLSLog12->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber12 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog12,
                                       "WLSRPD12",
                                       logicClad12,
                                       false,
                                       0);

  //empty line
  //--------------------------
  // RPD Channel 4
  //--------------------------
  G4ThreeVector positionWLSWorld13 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)-.6*cm),3.499*cm,.2475*cm);		//SUB
  solidWLSWorld13 = new G4Box("wlsworld13",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld13 = new G4LogicalVolume(solidWLSWorld13, Grease, "WLSWorld13");

  G4ThreeVector positionWLSWorld14 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)-.2*cm),3.499*cm,.2475*cm);		//SUB
  solidWLSWorld14 = new G4Box("wlsworld14",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld14 = new G4LogicalVolume(solidWLSWorld14, Grease, "WLSWorld14");

  G4ThreeVector positionWLSWorld15 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)+.6*cm),3.499*cm,.2475*cm);		//SUB
  solidWLSWorld15 = new G4Box("wlsworld15",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld15 = new G4LogicalVolume(solidWLSWorld15, Grease, "WLSWorld15");

  G4ThreeVector positionWLSWorld16 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)+.2*cm),3.499*cm,.2475*cm);	//SUB
  solidWLSWorld16 = new G4Box("wlsworld16",(0.54)*mm,1.501*cm,0.5*mm/SizeMod);
  logicWLSWorld16 = new G4LogicalVolume(solidWLSWorld16, Grease, "WLSWorld16");



  G4ThreeVector positionRPD4 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)),RPDYInitial,-.2*cm);	//SUB
  solidRPD4 = new G4Box("rpd4",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD4a = new G4UnionSolid("newrpd4a", solidRPD4, solidWLSWorld13,0,(positionWLSWorld13-positionRPD4));
  G4UnionSolid* newRPD4b = new G4UnionSolid("newrpd4b", newRPD4a, solidWLSWorld14,0,(positionWLSWorld14-positionRPD4));
  G4UnionSolid* newRPD4c = new G4UnionSolid("newrpd4c", newRPD4b, solidWLSWorld15,0,(positionWLSWorld15-positionRPD4));
  G4UnionSolid* newRPD4d = new G4UnionSolid("newrpd4d", newRPD4c, solidWLSWorld16,0,(positionWLSWorld16-positionRPD4));
  logicRPD4 = new G4LogicalVolume(newRPD4d, RPD_Material, "RPD4",0,0,0);
  physiRPD4 = new G4PVPlacement(0,               // no rotation
                                   positionRPD4,  // at (x,y,z)
                                   logicRPD4,     // its logical volume                             
                                   "RPD4",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy 16ber 	


  physiWLSWorld13 = new G4PVPlacement(0,
				    (positionWLSWorld13-positionRPD4),
				    logicWLSWorld13,
				    "WLSWorld13",
				    logicRPD4,
				    false,
				    0);

  physiWLSWorld14 = new G4PVPlacement(0,
				    (positionWLSWorld14-positionRPD4),
				    logicWLSWorld14,
				    "WLSWorld13",
				    logicRPD4,
				    false,
				    0);

 physiWLSWorld15 = new G4PVPlacement(0,
				    (positionWLSWorld15-positionRPD4),
				    logicWLSWorld15,
				    "WLSWorld15",
				    logicRPD4,
				    false,
				    0);

 physiWLSWorld16 = new G4PVPlacement(0,
				    (positionWLSWorld16-positionRPD4),
				    logicWLSWorld16,
				    "WLSWorld16",
				    logicRPD4,
				    false,
				    0);	


				
  G4ThreeVector positionAl4 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)),3.51*cm,.305*cm);
  solidAl4 = new G4Box("Al4",RPDXsize+.0425*cm,1.5*cm,0.005*mm/SizeMod);
  logicAl4 = new G4LogicalVolume(solidAl4, Al, "Al4");
  physiAl4 = new G4PVPlacement(0,
				positionAl4,
				logicAl4,
				"Al4",
				logicRPDWorld,
				false,
				0); 				

////


//WLS_13



  G4VSolid* solidClad13;
  solidClad13 = new G4Tubs("Clad13",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad13 = new G4LogicalVolume(solidClad13,
                                      Pethylene,
                                      "Clad13");
  physiClad13 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad13,
                                    "Clad13",
                                    logicWLSWorld13,
                                    false,
                                    0);


  WLSFiberLogic13 = new G4Tubs("wls13",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog13 = new G4LogicalVolume(WLSFiberLogic13,PMMA,"WLS13");
  WLSLog13->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber13 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog13,
                                       "WLSRPD13",
                                       logicClad13,
                                       false,
                                       0);



////


//WLS_14				




  G4VSolid* solidClad14;
  solidClad14 = new G4Tubs("Clad14",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad14 = new G4LogicalVolume(solidClad14,
                                      Pethylene,
                                      "Clad14");
  physiClad14 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad14,
                                    "Clad14",
                                    logicWLSWorld14,
                                    false,
                                    0);


  WLSFiberLogic14 = new G4Tubs("wls14",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog14 = new G4LogicalVolume(WLSFiberLogic14,PMMA,"WLS14");
  WLSLog14->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber14 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog14,
                                       "WLSRPD14",
                                       logicClad14,
                                       false,
                                       0);
				
////


//WLS_15



  G4VSolid* solidClad15;
  solidClad15 = new G4Tubs("Clad15",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad15 = new G4LogicalVolume(solidClad15,
                                      Pethylene,
                                      "Clad15");
  physiClad15 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad15,
                                    "Clad15",
                                    logicWLSWorld15,
                                    false,
                                    0);


  WLSFiberLogic15 = new G4Tubs("wls15",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog15 = new G4LogicalVolume(WLSFiberLogic15,PMMA,"WLS15");
  WLSLog15->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber15 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog15,
                                       "WLSRPD15",
                                       logicClad15,
                                       false,
                                       0);

////


//WLS_16



  G4VSolid* solidClad16;
  solidClad16 = new G4Tubs("Clad16",CladinnerRadius,CladouterRadius,1.5*cm,startAngle, spanningAngle);
  logicClad16 = new G4LogicalVolume(solidClad16,
                                      Pethylene,
                                      "Clad16");
  physiClad16 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad16,
                                    "Clad16",
                                    logicWLSWorld16,
                                    false,
                                    0);


  WLSFiberLogic16 = new G4Tubs("wls16",WLSinnerRadius,WLSouterRadius,1.5*cm,startAngle, spanningAngle);
  WLSLog16 = new G4LogicalVolume(WLSFiberLogic16,PMMA,"WLS16");
  WLSLog16->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber16 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog16,
                                       "WLSRPD16",
                                       logicClad16,
                                       false,
                                       0);
  //empty line
  //--------------------------
  // RPD Channel 5
  //--------------------------
  G4ThreeVector positionWLSWorld17 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)-.6*cm),2.499*cm,.36*cm);	//SUB
  solidWLSWorld17 = new G4Box("wlsworld17",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);
  logicWLSWorld17 = new G4LogicalVolume(solidWLSWorld17, Grease, "WLSWorld17");

  G4ThreeVector positionWLSWorld18 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)-.2*cm),2.499*cm,.36*cm);		//SUB
  solidWLSWorld18 = new G4Box("wlsworld18",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);
  logicWLSWorld18 = new G4LogicalVolume(solidWLSWorld18, Grease, "WLSWorld18");

  G4ThreeVector positionWLSWorld19 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)+.6*cm),2.499*cm,.36*cm);	//SUB
  solidWLSWorld19 = new G4Box("wlsworld19",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);
  logicWLSWorld19 = new G4LogicalVolume(solidWLSWorld19, Grease, "WLSWorld19");

  G4ThreeVector positionWLSWorld20 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)+.2*cm),2.499*cm,.36*cm);		//SUB
  solidWLSWorld20 = new G4Box("wlsworld20",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);
  logicWLSWorld20 = new G4LogicalVolume(solidWLSWorld20, Grease, "WLSWorld20");



  G4ThreeVector positionRPD5 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)),(RPDYInitial-(1*RPDYDelta)),-0.0875*cm);	//SUB (-0.1***
  solidRPD5 = new G4Box("rpd5",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD5a = new G4UnionSolid("newrpd5a", solidRPD5, solidWLSWorld17,0,(positionWLSWorld17-positionRPD5));
  G4UnionSolid* newRPD5b = new G4UnionSolid("newrpd5b", newRPD5a, solidWLSWorld18,0,(positionWLSWorld18-positionRPD5));
  G4UnionSolid* newRPD5c = new G4UnionSolid("newrpd5c", newRPD5b, solidWLSWorld19,0,(positionWLSWorld19-positionRPD5));
  G4UnionSolid* newRPD5d = new G4UnionSolid("newrpd5d", newRPD5c, solidWLSWorld20,0,(positionWLSWorld20-positionRPD5));
  logicRPD5 = new G4LogicalVolume(newRPD5d, RPD_Material, "RPD5",0,0,0);
  physiRPD5 = new G4PVPlacement(0,               // no rotation
                                   positionRPD5,  // at (x,y,z)
                                   logicRPD5,     // its logical volume                             
                                   "RPD5",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld17 = new G4PVPlacement(0,
				    (positionWLSWorld17-positionRPD5),
				    logicWLSWorld17,
				    "WLSWorld17",
				    logicRPD5,
				    false,
				    0);

  physiWLSWorld18 = new G4PVPlacement(0,
				    (positionWLSWorld18-positionRPD5),
				    logicWLSWorld18,
				    "WLSWorld18",
				    logicRPD5,
				    false,
				    0);

 physiWLSWorld19 = new G4PVPlacement(0,
				    (positionWLSWorld19-positionRPD5),
				    logicWLSWorld19,
				    "WLSWorld19",
				    logicRPD5,
				    false,
				    0);

 physiWLSWorld20 = new G4PVPlacement(0,
				    (positionWLSWorld20-positionRPD5),
				    logicWLSWorld20,
				    "WLSWorld20",
				    logicRPD5,
				    false,
				    0);


  G4ThreeVector positionAl5 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)),2.5*cm+.05*cm,.42*cm);
  solidAl5 = new G4Box("Al5",RPDXsize+.0425*cm,2.7*cm,0.025*mm/SizeMod);
  logicAl5 = new G4LogicalVolume(solidAl5, Al, "Al5");
  physiAl5 = new G4PVPlacement(0,
				positionAl5,
				logicAl5,
				"Al5",
				logicRPDWorld,
				false,
				0); 	
				
  G4ThreeVector positionAlCov2 = G4ThreeVector(0,(RPDYInitial-(1*RPDYDelta)),-.625*cm);
  AlCov2 = new G4Box("alcov2",4.61*cm/SizeMod,.99999*cm/SizeMod,.099*mm/SizeMod);
  logicAlCov2 = new G4LogicalVolume(AlCov2, Al, "ALCOV2");
  physiAlCov2 = new G4PVPlacement(0,
				positionAlCov2,
				logicAlCov2,
				"ALCOV2",
				logicRPDWorld,
				false,
				0);
				


 
//WLS_17





  G4VSolid* solidClad17;
  solidClad17 = new G4Tubs("Clad17",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad17 = new G4LogicalVolume(solidClad17,
                                      Pethylene,
                                      "Clad17");
  physiClad17 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad17,
                                    "Clad17",
                                    logicWLSWorld17,
                                    false,
                                    0);


  WLSFiberLogic17 = new G4Tubs("wls17",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog17 = new G4LogicalVolume(WLSFiberLogic17,PMMA,"WLS17");
  WLSLog17->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber17 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog17,
                                       "WLSRPD17",
                                       logicClad17,
                                       false,
                                       0);


////


//WLS_18





  G4VSolid* solidClad18;
  solidClad18 = new G4Tubs("Clad18",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad18 = new G4LogicalVolume(solidClad18,
                                      Pethylene,
                                      "Clad18");
  physiClad18 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad18,
                                    "Clad18",
                                    logicWLSWorld18,
                                    false,
                                    0);


  WLSFiberLogic18 = new G4Tubs("wls18",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog18 = new G4LogicalVolume(WLSFiberLogic18,PMMA,"WLS18");
  WLSLog18->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber18 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog18,
                                       "WLSRPD18",
                                       logicClad18,
                                       false,
                                       0);


////



 
//WLS_19



  G4VSolid* solidClad19;
  solidClad19 = new G4Tubs("Clad19",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad19 = new G4LogicalVolume(solidClad19,
                                      Pethylene,
                                      "Clad19");
  physiClad19 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad19,
                                    "Clad19",
                                    logicWLSWorld19,
                                    false,
                                    0);


  WLSFiberLogic19 = new G4Tubs("wls19",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog19 = new G4LogicalVolume(WLSFiberLogic19,PMMA,"WLS19");
  WLSLog19->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber19 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog19,
                                       "WLSRPD19",
                                       logicClad19,
                                       false,
                                       0);


////

 
//WLS_20





  G4VSolid* solidClad20;
  solidClad20 = new G4Tubs("Clad20",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad20 = new G4LogicalVolume(solidClad20,
                                      Pethylene,
                                      "Clad20");
  physiClad20 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad20,
                                    "Clad20",
                                    logicWLSWorld20,
                                    false,
                                    0);


  WLSFiberLogic20 = new G4Tubs("wls20",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog20 = new G4LogicalVolume(WLSFiberLogic20,PMMA,"WLS20");
  WLSLog20->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber20 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog20,
                                       "WLSRPD20",
                                       logicClad20,
                                       false,
                                       0);


////


//empty line
//--------------------------
// RPD Channel 6
//--------------------------


 G4ThreeVector positionWLSWorld21 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)-.6*cm),2.499*cm,.36*cm);			//SUB
  solidWLSWorld21 = new G4Box("wlsworld21",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);		//
  logicWLSWorld21 = new G4LogicalVolume(solidWLSWorld21, Grease, "WLSWorld21");

 G4ThreeVector positionWLSWorld22 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)-.2*cm),2.499*cm,.36*cm);
  solidWLSWorld22 = new G4Box("wlsworld22",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);		//
  logicWLSWorld22 = new G4LogicalVolume(solidWLSWorld22, Grease, "WLSWorld22");

 G4ThreeVector positionWLSWorld23 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)+.6*cm),2.499*cm,.36*cm);
  solidWLSWorld23 = new G4Box("wlsworld23",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);	//
  logicWLSWorld23 = new G4LogicalVolume(solidWLSWorld23, Grease, "WLSWorld23");

 G4ThreeVector positionWLSWorld24 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)+.2*cm),2.499*cm,.36*cm);
  solidWLSWorld24 = new G4Box("wlsworld24",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);		//
  logicWLSWorld24 = new G4LogicalVolume(solidWLSWorld24, Grease, "WLSWorld24");



  G4ThreeVector positionRPD6 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)),(RPDYInitial-(1*RPDYDelta)),-0.0875*cm);	//SUB
  solidRPD6 = new G4Box("rpd6",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD6a = new G4UnionSolid("newrpd6a", solidRPD6, solidWLSWorld21,0,(positionWLSWorld21-positionRPD6));
  G4UnionSolid* newRPD6b = new G4UnionSolid("newrpd6b", newRPD6a, solidWLSWorld22,0,(positionWLSWorld22-positionRPD6));
  G4UnionSolid* newRPD6c = new G4UnionSolid("newrpd6c", newRPD6b, solidWLSWorld23,0,(positionWLSWorld23-positionRPD6));
  G4UnionSolid* newRPD6d = new G4UnionSolid("newrpd6d", newRPD6c, solidWLSWorld24,0,(positionWLSWorld24-positionRPD6));
  logicRPD6 = new G4LogicalVolume(newRPD6d, RPD_Material, "RPD6",0,0,0);
  physiRPD6 = new G4PVPlacement(0,               // no rotation
                                   positionRPD6,  // at (x,y,z)
                                   logicRPD6,     // its logical volume                             
                                   "RPD6",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld21 = new G4PVPlacement(0,
				    (positionWLSWorld21-positionRPD6),
				    logicWLSWorld21,
				    "WLSWorld21",
				    logicRPD6,
				    false,
				    0);

  physiWLSWorld22 = new G4PVPlacement(0,
				    (positionWLSWorld22-positionRPD6),
				    logicWLSWorld22,
				    "WLSWorld21",
				    logicRPD6,
				    false,
				    0);

 physiWLSWorld23 = new G4PVPlacement(0,
				    (positionWLSWorld23-positionRPD6),
				    logicWLSWorld23,
				    "WLSWorld23",
				    logicRPD6,
				    false,
				    0);

 physiWLSWorld24 = new G4PVPlacement(0,
				    (positionWLSWorld24-positionRPD6),
				    logicWLSWorld24,
				    "WLSWorld24",
				    logicRPD6,
				    false,
				    0);
	
			       
  G4ThreeVector positionAl6 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)),2.5*cm+.05*cm,.42*cm);
  solidAl6 = new G4Box("Al6",RPDXsize+.0425*cm,2.7*cm,0.025*mm/SizeMod);
  logicAl6 = new G4LogicalVolume(solidAl6, Al, "Al6");
  physiAl6 = new G4PVPlacement(0,
				positionAl6,
				logicAl6,
				"Al6",
				logicRPDWorld,
				false,
				0); 			       
 
//WLS_21


  G4VSolid* solidClad21;
  solidClad21 = new G4Tubs("Clad21",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad21 = new G4LogicalVolume(solidClad21,
                                      Pethylene,
                                      "Clad21");
  physiClad21 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad21,
                                    "Clad21",
                                    logicWLSWorld21,
                                    false,
                                    0);


  WLSFiberLogic21 = new G4Tubs("wls21",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog21 = new G4LogicalVolume(WLSFiberLogic21,PMMA,"WLS21");
  WLSLog21->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber21 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog21,
                                       "WLSRPD21",
                                       logicClad21,
                                       false,
                                       0);


////

 
//WLS_22


  G4VSolid* solidClad22;
  solidClad22 = new G4Tubs("Clad22",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad22 = new G4LogicalVolume(solidClad22,
                                      Pethylene,
                                      "Clad22");
  physiClad22 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad22,
                                    "Clad22",
                                    logicWLSWorld22,
                                    false,
                                    0);


  WLSFiberLogic22 = new G4Tubs("wls22",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog22 = new G4LogicalVolume(WLSFiberLogic22,PMMA,"WLS22");
  WLSLog22->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber22 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog22,
                                       "WLSRPD22",
                                       logicClad22,
                                       false,
                                       0);


////

//WLS_23




  G4VSolid* solidClad23;
  solidClad23 = new G4Tubs("Clad23",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad23 = new G4LogicalVolume(solidClad23,
                                      Pethylene,
                                      "Clad23");
  physiClad23 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad23,
                                    "Clad23",
                                    logicWLSWorld23,
                                    false,
                                    0);


  WLSFiberLogic23 = new G4Tubs("wls23",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog23 = new G4LogicalVolume(WLSFiberLogic23,PMMA,"WLS23");
  WLSLog23->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber23 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog23,
                                       "WLSRPD23",
                                       logicClad23,
                                       false,
                                       0);


////

 
 
//WLS_24


  G4VSolid* solidClad24;
  solidClad24 = new G4Tubs("Clad24",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad24 = new G4LogicalVolume(solidClad24,
                                      Pethylene,
                                      "Clad24");
  physiClad24 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad24,
                                    "Clad24",
                                    logicWLSWorld24,
                                    false,
                                    0);


  WLSFiberLogic24 = new G4Tubs("wls24",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog24 = new G4LogicalVolume(WLSFiberLogic24,PMMA,"WLS24");
  WLSLog24->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber24 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog24,
                                       "WLSRPD24",
                                       logicClad24,
                                       false,
                                       0);


////

 
//empty line
//--------------------------
// RPD Channel 7
//--------------------------
 G4ThreeVector positionWLSWorld25 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)-.6*cm),2.499*cm,.36*cm);
  solidWLSWorld25 = new G4Box("wlsworld25",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);
  logicWLSWorld25 = new G4LogicalVolume(solidWLSWorld25, Grease, "WLSWorld25");

 G4ThreeVector positionWLSWorld26 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)-.2*cm),2.499*cm,.36*cm);
  solidWLSWorld26 = new G4Box("wlsworld26",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);
  logicWLSWorld26 = new G4LogicalVolume(solidWLSWorld26, Grease, "WLSWorld26");

 G4ThreeVector positionWLSWorld27 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)+.6*cm),2.499*cm,.36*cm);
  solidWLSWorld27 = new G4Box("wlsworld27",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);
  logicWLSWorld27 = new G4LogicalVolume(solidWLSWorld27, Grease, "WLSWorld27");

 G4ThreeVector positionWLSWorld28 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)+.2*cm),2.499*cm,.36*cm);
  solidWLSWorld28 = new G4Box("wlsworld28",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);
  logicWLSWorld28 = new G4LogicalVolume(solidWLSWorld28, Grease, "WLSWorld28");



 G4ThreeVector positionRPD7 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)),(RPDYInitial-(1*RPDYDelta)),-0.0875*cm);
  solidRPD7 = new G4Box("rpd7",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD7a = new G4UnionSolid("newrpd7a", solidRPD7, solidWLSWorld25,0,(positionWLSWorld25-positionRPD7));
  G4UnionSolid* newRPD7b = new G4UnionSolid("newrpd7b", newRPD7a, solidWLSWorld26,0,(positionWLSWorld26-positionRPD7));
  G4UnionSolid* newRPD7c = new G4UnionSolid("newrpd7c", newRPD7b, solidWLSWorld27,0,(positionWLSWorld27-positionRPD7));
  G4UnionSolid* newRPD7d = new G4UnionSolid("newrpd7d", newRPD7c, solidWLSWorld28,0,(positionWLSWorld28-positionRPD7));
  logicRPD7 = new G4LogicalVolume(newRPD7d, RPD_Material, "RPD7",0,0,0);
  physiRPD7 = new G4PVPlacement(0,               // no rotation
                                   positionRPD7,  // at (x,y,z)
                                   logicRPD7,     // its logical volume                             
                                   "RPD7",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld25 = new G4PVPlacement(0,
				    (positionWLSWorld25-positionRPD7),
				    logicWLSWorld25,
				    "WLSWorld25",
				    logicRPD7,
				    false,
				    0);

  physiWLSWorld26 = new G4PVPlacement(0,
				    (positionWLSWorld26-positionRPD7),
				    logicWLSWorld26,
				    "WLSWorld25",
				    logicRPD7,
				    false,
				    0);

 physiWLSWorld27 = new G4PVPlacement(0,
				    (positionWLSWorld27-positionRPD7),
				    logicWLSWorld27,
				    "WLSWorld27",
				    logicRPD7,
				    false,
				    0);

 physiWLSWorld28 = new G4PVPlacement(0,
				    (positionWLSWorld28-positionRPD7),
				    logicWLSWorld28,
				    "WLSWorld28",
				    logicRPD7,
				    false,
				    0);
  G4ThreeVector positionAl7 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)),2.5*cm+.05*cm,.42*cm);
  solidAl7 = new G4Box("Al7",RPDXsize+.0425*cm,2.7*cm,0.025*mm/SizeMod);
  logicAl7 = new G4LogicalVolume(solidAl7, Al, "Al7");
  physiAl7 = new G4PVPlacement(0,
				positionAl7,
				logicAl7,
				"Al7",
				logicRPDWorld,
				false,
				0); 			       
				
 
//WLS_25




  G4VSolid* solidClad25;
  solidClad25 = new G4Tubs("Clad25",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad25 = new G4LogicalVolume(solidClad25,
                                      Pethylene,
                                      "Clad25");
  physiClad25 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad25,
                                    "Clad25",
                                    logicWLSWorld25,
                                    false,
                                    0);


  WLSFiberLogic25 = new G4Tubs("wls25",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog25 = new G4LogicalVolume(WLSFiberLogic25,PMMA,"WLS25");
  WLSLog25->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber25 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog25,
                                       "WLSRPD25",
                                       logicClad25,
                                       false,
                                       0);


////

//WLS_26



  G4VSolid* solidClad26;
  solidClad26 = new G4Tubs("Clad26",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad26 = new G4LogicalVolume(solidClad26,
                                      Pethylene,
                                      "Clad26");
  physiClad26 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad26,
                                    "Clad26",
                                    logicWLSWorld26,
                                    false,
                                    0);


  WLSFiberLogic26 = new G4Tubs("wls26",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog26 = new G4LogicalVolume(WLSFiberLogic26,PMMA,"WLS26");
  WLSLog26->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber26 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog26,
                                       "WLSRPD26",
                                       logicClad26,
                                       false,
                                       0);


////



 
//WLS_27



  G4VSolid* solidClad27;
  solidClad27 = new G4Tubs("Clad27",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad27 = new G4LogicalVolume(solidClad27,
                                      Pethylene,
                                      "Clad27");
  physiClad27 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad27,
                                    "Clad27",
                                    logicWLSWorld27,
                                    false,
                                    0);


  WLSFiberLogic27 = new G4Tubs("wls27",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog27 = new G4LogicalVolume(WLSFiberLogic27,PMMA,"WLS27");
  WLSLog27->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber27 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog27,
                                       "WLSRPD27",
                                       logicClad27,
                                       false,
                                       0);


////
 
//WLS_28



  G4VSolid* solidClad28;
  solidClad28 = new G4Tubs("Clad28",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad28 = new G4LogicalVolume(solidClad28,
                                      Pethylene,
                                      "Clad28");
  physiClad28 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad28,
                                    "Clad28",
                                    logicWLSWorld28,
                                    false,
                                    0);


  WLSFiberLogic28 = new G4Tubs("wls28",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog28 = new G4LogicalVolume(WLSFiberLogic28,PMMA,"WLS28");
  WLSLog28->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber28 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog28,
                                       "WLSRPD28",
                                       logicClad28,
                                       false,
                                       0);


////


 //empty line
 //--------------------------
 // RPD Channel 8
 //--------------------------

 G4ThreeVector positionWLSWorld29 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)-.6*cm),2.499*cm,.36*cm);
  solidWLSWorld29 = new G4Box("wlsworld29",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);
  logicWLSWorld29 = new G4LogicalVolume(solidWLSWorld29, Grease, "WLSWorld29");

 G4ThreeVector positionWLSWorld30 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)-.2*cm),2.499*cm,.36*cm);
  solidWLSWorld30 = new G4Box("wlsworld30",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);
  logicWLSWorld30 = new G4LogicalVolume(solidWLSWorld30, Grease, "WLSWorld30");

 G4ThreeVector positionWLSWorld31 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)+.6*cm),2.499*cm,.36*cm);
  solidWLSWorld31 = new G4Box("wlsworld31",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);
  logicWLSWorld31 = new G4LogicalVolume(solidWLSWorld31, Grease, "WLSWorld31");

 G4ThreeVector positionWLSWorld32 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)+.2*cm),2.499*cm,.36*cm);
  solidWLSWorld32 = new G4Box("wlsworld32",(0.54)*mm,2.501*cm,0.5*mm/SizeMod);
  logicWLSWorld32 = new G4LogicalVolume(solidWLSWorld32, Grease, "WLSWorld32");



 G4ThreeVector positionRPD8 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)),(RPDYInitial-(1*RPDYDelta)),-0.0875*cm);
  solidRPD8 = new G4Box("rpd8",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD8a = new G4UnionSolid("newrpd8a", solidRPD8, solidWLSWorld29,0,(positionWLSWorld29-positionRPD8));
  G4UnionSolid* newRPD8b = new G4UnionSolid("newrpd8b", newRPD8a, solidWLSWorld30,0,(positionWLSWorld30-positionRPD8));
  G4UnionSolid* newRPD8c = new G4UnionSolid("newrpd8c", newRPD8b, solidWLSWorld31,0,(positionWLSWorld31-positionRPD8));
  G4UnionSolid* newRPD8d = new G4UnionSolid("newrpd8d", newRPD8c, solidWLSWorld32,0,(positionWLSWorld32-positionRPD8));
  logicRPD8 = new G4LogicalVolume(newRPD8d, RPD_Material, "RPD8",0,0,0);
  physiRPD8 = new G4PVPlacement(0,               // no rotation
                                   positionRPD8,  // at (x,y,z)
                                   logicRPD8,     // its logical volume                             
                                   "RPD8",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld29 = new G4PVPlacement(0,
				    (positionWLSWorld29-positionRPD8),
				    logicWLSWorld29,
				    "WLSWorld29",
				    logicRPD8,
				    false,
				    0);

  physiWLSWorld30 = new G4PVPlacement(0,
				    (positionWLSWorld30-positionRPD8),
				    logicWLSWorld30,
				    "WLSWorld29",
				    logicRPD8,
				    false,
				    0);

 physiWLSWorld31 = new G4PVPlacement(0,
				    (positionWLSWorld31-positionRPD8),
				    logicWLSWorld31,
				    "WLSWorld31",
				    logicRPD8,
				    false,
				    0);

 physiWLSWorld32 = new G4PVPlacement(0,
				    (positionWLSWorld32-positionRPD8),
				    logicWLSWorld32,
				    "WLSWorld32",
				    logicRPD8,
				    false,
				    0);	
			       
  G4ThreeVector positionAl8 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)),2.5*cm+.05*cm,.42*cm);
  solidAl8 = new G4Box("Al8",RPDXsize+.0425*cm,2.7*cm,0.025*mm/SizeMod);
  logicAl8 = new G4LogicalVolume(solidAl8, Al, "Al8");
  physiAl8 = new G4PVPlacement(0,
				positionAl8,
				logicAl8,
				"Al8",
				logicRPDWorld,
				false,
				0); 			       
//WLS_29




  G4VSolid* solidClad29;
  solidClad29 = new G4Tubs("Clad29",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad29 = new G4LogicalVolume(solidClad29,
                                      Pethylene,
                                      "Clad29");
  physiClad29 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad29,
                                    "Clad29",
                                    logicWLSWorld29,
                                    false,
                                    0);


  WLSFiberLogic29 = new G4Tubs("wls29",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog29 = new G4LogicalVolume(WLSFiberLogic29,PMMA,"WLS29");
  WLSLog29->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber29 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog29,
                                       "WLSRPD29",
                                       logicClad29,
                                       false,
                                       0);


////


 
//WLS_30




  G4VSolid* solidClad30;
  solidClad30 = new G4Tubs("Clad30",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad30 = new G4LogicalVolume(solidClad30,
                                      Pethylene,
                                      "Clad30");
  physiClad30 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad30,
                                    "Clad30",
                                    logicWLSWorld30,
                                    false,
                                    0);


  WLSFiberLogic30 = new G4Tubs("wls30",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog30 = new G4LogicalVolume(WLSFiberLogic30,PMMA,"WLS30");
  WLSLog30->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber30 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog30,
                                       "WLSRPD30",
                                       logicClad30,
                                       false,
                                       0);


////

 
//WLS_31




  G4VSolid* solidClad31;
  solidClad31 = new G4Tubs("Clad31",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad31 = new G4LogicalVolume(solidClad31,
                                      Pethylene,
                                      "Clad31");
  physiClad31 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad31,
                                    "Clad31",
                                    logicWLSWorld31,
                                    false,
                                    0);


  WLSFiberLogic31 = new G4Tubs("wls31",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog31 = new G4LogicalVolume(WLSFiberLogic31,PMMA,"WLS31");
  WLSLog31->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber31 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog31,
                                       "WLSRPD31",
                                       logicClad31,
                                       false,
                                       0);


////

 
//WLS_32



  G4VSolid* solidClad32;
  solidClad32 = new G4Tubs("Clad32",CladinnerRadius,CladouterRadius,2.5*cm,startAngle, spanningAngle);
  logicClad32 = new G4LogicalVolume(solidClad32,
                                      Pethylene,
                                      "Clad32");
  physiClad32 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad32,
                                    "Clad32",
                                    logicWLSWorld32,
                                    false,
                                    0);


  WLSFiberLogic32 = new G4Tubs("wls32",WLSinnerRadius,WLSouterRadius,2.5*cm,startAngle, spanningAngle);
  WLSLog32 = new G4LogicalVolume(WLSFiberLogic32,PMMA,"WLS32");
  WLSLog32->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber32 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog32,
                                       "WLSRPD32",
                                       logicClad32,
                                       false,
                                       0);


////


//empty line
 //--------------------------
 // RPD Channel 9
 //--------------------------
 G4ThreeVector positionWLSWorld33 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)-.6*cm),1.499*cm,.475*cm);
  solidWLSWorld33 = new G4Box("wlsworld33",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld33 = new G4LogicalVolume(solidWLSWorld33, Grease, "WLSWorld33");

 G4ThreeVector positionWLSWorld34 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)-.2*cm),1.499*cm,.475*cm);
  solidWLSWorld34 = new G4Box("wlsworld34",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld34 = new G4LogicalVolume(solidWLSWorld34, Grease, "WLSWorld34");

 G4ThreeVector positionWLSWorld35 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)+.6*cm),1.499*cm,.475*cm);
  solidWLSWorld35 = new G4Box("wlsworld35",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld35 = new G4LogicalVolume(solidWLSWorld35, Grease, "WLSWorld35");

 G4ThreeVector positionWLSWorld36 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)+.2*cm),1.499*cm,.475*cm);
  solidWLSWorld36 = new G4Box("wlsworld36",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld36 = new G4LogicalVolume(solidWLSWorld36, Grease, "WLSWorld36");



 G4ThreeVector positionRPD9 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)),(RPDYInitial-(2*RPDYDelta)),0.025*cm);
  solidRPD9 = new G4Box("rpd9",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD9a = new G4UnionSolid("newrpd9a", solidRPD9, solidWLSWorld33,0,(positionWLSWorld33-positionRPD9));
  G4UnionSolid* newRPD9b = new G4UnionSolid("newrpd9b", newRPD9a, solidWLSWorld34,0,(positionWLSWorld34-positionRPD9));
  G4UnionSolid* newRPD9c = new G4UnionSolid("newrpd9c", newRPD9b, solidWLSWorld35,0,(positionWLSWorld35-positionRPD9));
  G4UnionSolid* newRPD9d = new G4UnionSolid("newrpd9d", newRPD9c, solidWLSWorld36,0,(positionWLSWorld36-positionRPD9));
  logicRPD9 = new G4LogicalVolume(newRPD9d, RPD_Material, "RPD9",0,0,0);
  physiRPD9 = new G4PVPlacement(0,               // no rotation
                                   positionRPD9,  // at (x,y,z)
                                   logicRPD9,     // its logical volume                             
                                   "RPD9",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld33 = new G4PVPlacement(0,
				    (positionWLSWorld33-positionRPD9),
				    logicWLSWorld33,
				    "WLSWorld33",
				    logicRPD9,
				    false,
				    0);

  physiWLSWorld34 = new G4PVPlacement(0,
				    (positionWLSWorld34-positionRPD9),
				    logicWLSWorld34,
				    "WLSWorld33",
				    logicRPD9,
				    false,
				    0);

 physiWLSWorld35 = new G4PVPlacement(0,
				    (positionWLSWorld35-positionRPD9),
				    logicWLSWorld35,
				    "WLSWorld35",
				    logicRPD9,
				    false,
				    0);

 physiWLSWorld36 = new G4PVPlacement(0,
				    (positionWLSWorld36-positionRPD9),
				    logicWLSWorld36,
				    "WLSWorld36",
				    logicRPD9,
				    false,
				    0);
			       
  G4ThreeVector positionAl9 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)),1.5*cm,.53*cm);
  solidAl9 = new G4Box("Al9",RPDXsize+.0425*cm,3.75*cm,0.025*mm/SizeMod);
  logicAl9 = new G4LogicalVolume(solidAl9, Al, "Al9");
  physiAl9 = new G4PVPlacement(0,
				positionAl9,
				logicAl9,
				"Al9",
				logicRPDWorld,
				false,
				0); 			       
				
  G4ThreeVector positionAlCov3 = G4ThreeVector(0,(RPDYInitial-(2*RPDYDelta)),-.525*cm);
  AlCov3 = new G4Box("alcov3",4.61*cm/SizeMod,.99999*cm/SizeMod,.099*mm/SizeMod);
  logicAlCov3 = new G4LogicalVolume(AlCov3, Al, "ALCOV3");
  physiAlCov3 = new G4PVPlacement(0,
				positionAlCov3,
				logicAlCov3,
				"ALCOV3",
				logicRPDWorld,
				false,
				0);
 
//WLS_33




  G4VSolid* solidClad33;
  solidClad33 = new G4Tubs("Clad33",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad33 = new G4LogicalVolume(solidClad33,
                                      Pethylene,
                                      "Clad33");
  physiClad33 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad33,
                                    "Clad33",
                                    logicWLSWorld33,
                                    false,
                                    0);


  WLSFiberLogic33 = new G4Tubs("wls33",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog33 = new G4LogicalVolume(WLSFiberLogic33,PMMA,"WLS33");
  WLSLog33->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber33 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog33,
                                       "WLSRPD33",
                                       logicClad33,
                                       false,
                                       0);


////
//WLS_34




  G4VSolid* solidClad34;
  solidClad34 = new G4Tubs("Clad34",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad34 = new G4LogicalVolume(solidClad34,
                                      Pethylene,
                                      "Clad34");
  physiClad34 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad34,
                                    "Clad34",
                                    logicWLSWorld34,
                                    false,
                                    0);


  WLSFiberLogic34 = new G4Tubs("wls34",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog34 = new G4LogicalVolume(WLSFiberLogic34,PMMA,"WLS34");
  WLSLog34->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber34 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog34,
                                       "WLSRPD34",
                                       logicClad34,
                                       false,
                                       0);


////

 
//WLS_35



  G4VSolid* solidClad35;
  solidClad35 = new G4Tubs("Clad35",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad35 = new G4LogicalVolume(solidClad35,
                                      Pethylene,
                                      "Clad35");
  physiClad35 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad35,
                                    "Clad35",
                                    logicWLSWorld35,
                                    false,
                                    0);


  WLSFiberLogic35 = new G4Tubs("wls35",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog35 = new G4LogicalVolume(WLSFiberLogic35,PMMA,"WLS35");
  WLSLog35->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber35 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog35,
                                       "WLSRPD35",
                                       logicClad35,
                                       false,
                                       0);


////

 
 
//WLS_36



  G4VSolid* solidClad36;
  solidClad36 = new G4Tubs("Clad36",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad36 = new G4LogicalVolume(solidClad36,
                                      Pethylene,
                                      "Clad36");
  physiClad36 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad36,
                                    "Clad36",
                                    logicWLSWorld36,
                                    false,
                                    0);


  WLSFiberLogic36 = new G4Tubs("wls36",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog36 = new G4LogicalVolume(WLSFiberLogic36,PMMA,"WLS36");
  WLSLog36->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber36 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog36,
                                       "WLSRPD36",
                                       logicClad36,
                                       false,
                                       0);


////
 
 //empty line
 //--------------------------
 // RPD Channel 10
 //--------------------------

			
 
//WLS_37

 G4ThreeVector positionWLSWorld37 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)-.6*cm),1.499*cm,.475*cm);
  solidWLSWorld37 = new G4Box("wlsworld37",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld37 = new G4LogicalVolume(solidWLSWorld37, Grease, "WLSWorld37");

 G4ThreeVector positionWLSWorld38 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)-.2*cm),1.499*cm,.475*cm);
  solidWLSWorld38 = new G4Box("wlsworld38",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld38 = new G4LogicalVolume(solidWLSWorld38, Grease, "WLSWorld38");

 G4ThreeVector positionWLSWorld39 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)+.6*cm),1.499*cm,.475*cm);
  solidWLSWorld39 = new G4Box("wlsworld39",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld39 = new G4LogicalVolume(solidWLSWorld39, Grease, "WLSWorld39");

 G4ThreeVector positionWLSWorld40 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)+.2*cm),1.499*cm,.475*cm);
  solidWLSWorld40 = new G4Box("wlsworld40",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld40 = new G4LogicalVolume(solidWLSWorld40, Grease, "WLSWorld40");



 G4ThreeVector positionRPD10 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)),(RPDYInitial-(2*RPDYDelta)),0.025*cm);
  solidRPD10 = new G4Box("rpd10",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD10a = new G4UnionSolid("newrpd10a", solidRPD10, solidWLSWorld37,0,(positionWLSWorld37-positionRPD10));
  G4UnionSolid* newRPD10b = new G4UnionSolid("newrpd10b", newRPD10a, solidWLSWorld38,0,(positionWLSWorld38-positionRPD10));
  G4UnionSolid* newRPD10c = new G4UnionSolid("newrpd10c", newRPD10b, solidWLSWorld39,0,(positionWLSWorld39-positionRPD10));
  G4UnionSolid* newRPD10d = new G4UnionSolid("newrpd10d", newRPD10c, solidWLSWorld40,0,(positionWLSWorld40-positionRPD10));
  logicRPD10 = new G4LogicalVolume(newRPD10d, RPD_Material, "RPD10",0,0,0);
  physiRPD10 = new G4PVPlacement(0,               // no rotation
                                   positionRPD10,  // at (x,y,z)
                                   logicRPD10,     // its logical volume                             
                                   "RPD10",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld37 = new G4PVPlacement(0,
				    (positionWLSWorld37-positionRPD10),
				    logicWLSWorld37,
				    "WLSWorld37",
				    logicRPD10,
				    false,
				    0);

  physiWLSWorld38 = new G4PVPlacement(0,
				    (positionWLSWorld38-positionRPD10),
				    logicWLSWorld38,
				    "WLSWorld37",
				    logicRPD10,
				    false,
				    0);

 physiWLSWorld39 = new G4PVPlacement(0,
				    (positionWLSWorld39-positionRPD10),
				    logicWLSWorld39,
				    "WLSWorld39",
				    logicRPD10,
				    false,
				    0);

 physiWLSWorld40 = new G4PVPlacement(0,
				    (positionWLSWorld40-positionRPD10),
				    logicWLSWorld40,
				    "WLSWorld40",
				    logicRPD10,
				    false,
				    0);
	
  G4ThreeVector positionAl10 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)),1.5*cm,.53*cm);
  solidAl10 = new G4Box("Al10",RPDXsize+.0425*cm,3.75*cm,0.025*mm/SizeMod);
  logicAl10 = new G4LogicalVolume(solidAl10, Al, "Al10");
  physiAl10 = new G4PVPlacement(0,
				positionAl10,
				logicAl10,
				"Al10",
				logicRPDWorld,
				false,
				0); 				
				
	


  G4VSolid* solidClad37;
  solidClad37 = new G4Tubs("Clad37",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad37 = new G4LogicalVolume(solidClad37,
                                      Pethylene,
                                      "Clad37");
  physiClad37 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad37,
                                    "Clad37",
                                    logicWLSWorld37,
                                    false,
                                    0);


  WLSFiberLogic37 = new G4Tubs("wls37",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog37 = new G4LogicalVolume(WLSFiberLogic37,PMMA,"WLS37");
  WLSLog37->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber37 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog37,
                                       "WLSRPD37",
                                       logicClad37,
                                       false,
                                       0);


////

 
//WLS_38




  G4VSolid* solidClad38;
  solidClad38 = new G4Tubs("Clad38",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad38 = new G4LogicalVolume(solidClad38,
                                      Pethylene,
                                      "Clad38");
  physiClad38 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad38,
                                    "Clad38",
                                    logicWLSWorld38,
                                    false,
                                    0);


  WLSFiberLogic38 = new G4Tubs("wls38",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog38 = new G4LogicalVolume(WLSFiberLogic38,PMMA,"WLS38");
  WLSLog38->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber38 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog38,
                                       "WLSRPD38",
                                       logicClad38,
                                       false,
                                       0);


////


 
//WLS_39


  G4VSolid* solidClad39;
  solidClad39 = new G4Tubs("Clad39",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad39 = new G4LogicalVolume(solidClad39,
                                      Pethylene,
                                      "Clad39");
  physiClad39 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad39,
                                    "Clad39",
                                    logicWLSWorld39,
                                    false,
                                    0);


  WLSFiberLogic39 = new G4Tubs("wls39",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog39 = new G4LogicalVolume(WLSFiberLogic39,PMMA,"WLS39");
  WLSLog39->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber39 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog39,
                                       "WLSRPD39",
                                       logicClad39,
                                       false,
                                       0);


////

 
//WLS_40



  G4VSolid* solidClad40;
  solidClad40 = new G4Tubs("Clad40",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad40 = new G4LogicalVolume(solidClad40,
                                      Pethylene,
                                      "Clad40");
  physiClad40 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad40,
                                    "Clad40",
                                    logicWLSWorld40,
                                    false,
                                    0);


  WLSFiberLogic40 = new G4Tubs("wls40",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog40 = new G4LogicalVolume(WLSFiberLogic40,PMMA,"WLS40");
  WLSLog40->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber40 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog40,
                                       "WLSRPD40",
                                       logicClad40,
                                       false,
                                       0);


////


 //empty line
 //--------------------------
 // RPD Channel 11
 //--------------------------

 G4ThreeVector positionWLSWorld41 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)-.6*cm),1.499*cm,.475*cm);
  solidWLSWorld41 = new G4Box("wlsworld41",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld41 = new G4LogicalVolume(solidWLSWorld41, Grease, "WLSWorld41");

 G4ThreeVector positionWLSWorld42 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)-.2*cm),1.499*cm,.475*cm);
  solidWLSWorld42 = new G4Box("wlsworld42",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld42 = new G4LogicalVolume(solidWLSWorld42, Grease, "WLSWorld42");

 G4ThreeVector positionWLSWorld43 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)+.6*cm),1.499*cm,.475*cm);
  solidWLSWorld43 = new G4Box("wlsworld43",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld43 = new G4LogicalVolume(solidWLSWorld43, Grease, "WLSWorld43");

 G4ThreeVector positionWLSWorld44 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)+.2*cm),1.499*cm,.475*cm);
  solidWLSWorld44 = new G4Box("wlsworld44",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld44 = new G4LogicalVolume(solidWLSWorld44, Grease, "WLSWorld44");



 G4ThreeVector positionRPD11 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)),(RPDYInitial-(2*RPDYDelta)),0.025*cm);
  solidRPD11 = new G4Box("rpd11",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD11a = new G4UnionSolid("newrpd11a", solidRPD11, solidWLSWorld41,0,(positionWLSWorld41-positionRPD11));
  G4UnionSolid* newRPD11b = new G4UnionSolid("newrpd11b", newRPD11a, solidWLSWorld42,0,(positionWLSWorld42-positionRPD11));
  G4UnionSolid* newRPD11c = new G4UnionSolid("newrpd11c", newRPD11b, solidWLSWorld43,0,(positionWLSWorld43-positionRPD11));
  G4UnionSolid* newRPD11d = new G4UnionSolid("newrpd11d", newRPD11c, solidWLSWorld44,0,(positionWLSWorld44-positionRPD11));
  logicRPD11 = new G4LogicalVolume(newRPD11d, RPD_Material, "RPD11",0,0,0);
  physiRPD11 = new G4PVPlacement(0,               // no rotation
                                   positionRPD11,  // at (x,y,z)
                                   logicRPD11,     // its logical volume                             
                                   "RPD11",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld41 = new G4PVPlacement(0,
				    (positionWLSWorld41-positionRPD11),
				    logicWLSWorld41,
				    "WLSWorld41",
				    logicRPD11,
				    false,
				    0);

  physiWLSWorld42 = new G4PVPlacement(0,
				    (positionWLSWorld42-positionRPD11),
				    logicWLSWorld42,
				    "WLSWorld41",
				    logicRPD11,
				    false,
				    0);

 physiWLSWorld43 = new G4PVPlacement(0,
				    (positionWLSWorld43-positionRPD11),
				    logicWLSWorld43,
				    "WLSWorld43",
				    logicRPD11,
				    false,
				    0);

 physiWLSWorld44 = new G4PVPlacement(0,
				    (positionWLSWorld44-positionRPD11),
				    logicWLSWorld44,
				    "WLSWorld44",
				    logicRPD11,
				    false,
				    0);	
				
  G4ThreeVector positionAl11 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)),1.5*cm,.53*cm);
  solidAl11 = new G4Box("Al11",RPDXsize+.0425*cm,3.75*cm,0.025*mm/SizeMod);
  logicAl11 = new G4LogicalVolume(solidAl11, Al, "Al11");
  physiAl11 = new G4PVPlacement(0,
				positionAl11,
				logicAl11,
				"Al11",
				logicRPDWorld,
				false,
				0); 				
				
//WLS_41




  G4VSolid* solidClad41;
  solidClad41 = new G4Tubs("Clad41",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad41 = new G4LogicalVolume(solidClad41,
                                      Pethylene,
                                      "Clad41");
  physiClad41 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad41,
                                    "Clad41",
                                    logicWLSWorld41,
                                    false,
                                    0);


  WLSFiberLogic41 = new G4Tubs("wls41",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog41 = new G4LogicalVolume(WLSFiberLogic41,PMMA,"WLS41");
  WLSLog41->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber41 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog41,
                                       "WLSRPD41",
                                       logicClad41,
                                       false,
                                       0);


////


 
//WLS_42



  G4VSolid* solidClad42;
  solidClad42 = new G4Tubs("Clad42",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad42 = new G4LogicalVolume(solidClad42,
                                      Pethylene,
                                      "Clad42");
  physiClad42 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad42,
                                    "Clad42",
                                    logicWLSWorld42,
                                    false,
                                    0);


  WLSFiberLogic42 = new G4Tubs("wls42",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog42 = new G4LogicalVolume(WLSFiberLogic42,PMMA,"WLS42");
  WLSLog42->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber42 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog42,
                                       "WLSRPD42",
                                       logicClad42,
                                       false,
                                       0);


////
		

 
//WLS_43



  G4VSolid* solidClad43;
  solidClad43 = new G4Tubs("Clad43",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad43 = new G4LogicalVolume(solidClad43,
                                      Pethylene,
                                      "Clad43");
  physiClad43 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad43,
                                    "Clad43",
                                    logicWLSWorld43,
                                    false,
                                    0);


  WLSFiberLogic43 = new G4Tubs("wls43",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog43 = new G4LogicalVolume(WLSFiberLogic43,PMMA,"WLS43");
  WLSLog43->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber43 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog43,
                                       "WLSRPD43",
                                       logicClad43,
                                       false,
                                       0);


////
			
 
//WLS_44



  G4VSolid* solidClad44;
  solidClad44 = new G4Tubs("Clad44",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad44 = new G4LogicalVolume(solidClad44,
                                      Pethylene,
                                      "Clad44");
  physiClad44 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad44,
                                    "Clad44",
                                    logicWLSWorld44,
                                    false,
                                    0);


  WLSFiberLogic44 = new G4Tubs("wls44",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog44 = new G4LogicalVolume(WLSFiberLogic44,PMMA,"WLS44");
  WLSLog44->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber44 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog44,
                                       "WLSRPD44",
                                       logicClad44,
                                       false,
                                       0);


////

 
 //empty line
 //--------------------------
 // RPD Channel 12
 //--------------------------

 G4ThreeVector positionWLSWorld45 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)-.6*cm),1.499*cm,.475*cm);
  solidWLSWorld45 = new G4Box("wlsworld45",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld45 = new G4LogicalVolume(solidWLSWorld45, Grease, "WLSWorld45");

 G4ThreeVector positionWLSWorld46 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)-.2*cm),1.499*cm,.475*cm);
  solidWLSWorld46 = new G4Box("wlsworld46",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld46 = new G4LogicalVolume(solidWLSWorld46, Grease, "WLSWorld46");

 G4ThreeVector positionWLSWorld47 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)+.6*cm),1.499*cm,.475*cm);
  solidWLSWorld47 = new G4Box("wlsworld47",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld47 = new G4LogicalVolume(solidWLSWorld47, Grease, "WLSWorld47");

 G4ThreeVector positionWLSWorld48 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)+.2*cm),1.499*cm,.475*cm);
  solidWLSWorld48 = new G4Box("wlsworld48",(0.54)*mm,3.501*cm,0.5*mm/SizeMod);
  logicWLSWorld48 = new G4LogicalVolume(solidWLSWorld48, Grease, "WLSWorld48");



 G4ThreeVector positionRPD12 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)),(RPDYInitial-(2*RPDYDelta)),0.025*cm);
  solidRPD12 = new G4Box("rpd12",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD12a = new G4UnionSolid("newrpd12a", solidRPD12, solidWLSWorld45,0,(positionWLSWorld45-positionRPD12));
  G4UnionSolid* newRPD12b = new G4UnionSolid("newrpd12b", newRPD12a, solidWLSWorld46,0,(positionWLSWorld46-positionRPD12));
  G4UnionSolid* newRPD12c = new G4UnionSolid("newrpd12c", newRPD12b, solidWLSWorld47,0,(positionWLSWorld47-positionRPD12));
  G4UnionSolid* newRPD12d = new G4UnionSolid("newrpd12d", newRPD12c, solidWLSWorld48,0,(positionWLSWorld48-positionRPD12));
  logicRPD12 = new G4LogicalVolume(newRPD12d, RPD_Material, "RPD12",0,0,0);
  physiRPD12 = new G4PVPlacement(0,               // no rotation
                                   positionRPD12,  // at (x,y,z)
                                   logicRPD12,     // its logical volume                             
                                   "RPD12",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld45 = new G4PVPlacement(0,
				    (positionWLSWorld45-positionRPD12),
				    logicWLSWorld45,
				    "WLSWorld45",
				    logicRPD12,
				    false,
				    0);

  physiWLSWorld46 = new G4PVPlacement(0,
				    (positionWLSWorld46-positionRPD12),
				    logicWLSWorld46,
				    "WLSWorld45",
				    logicRPD12,
				    false,
				    0);

 physiWLSWorld47 = new G4PVPlacement(0,
				    (positionWLSWorld47-positionRPD12),
				    logicWLSWorld47,
				    "WLSWorld47",
				    logicRPD12,
				    false,
				    0);

 physiWLSWorld48 = new G4PVPlacement(0,
				    (positionWLSWorld48-positionRPD12),
				    logicWLSWorld48,
				    "WLSWorld48",
				    logicRPD12,
				    false,
				    0);	
				
  G4ThreeVector positionAl12 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)),1.5*cm,.53*cm);
  solidAl12 = new G4Box("Al12",RPDXsize+.0425*cm,3.75*cm,0.025*mm/SizeMod);
  logicAl12 = new G4LogicalVolume(solidAl12, Al, "Al12");
  physiAl12 = new G4PVPlacement(0,
				positionAl12,
				logicAl12,
				"Al12",
				logicRPDWorld,
				false,
				0); 				
				
//WLS_45



  G4VSolid* solidClad45;
  solidClad45 = new G4Tubs("Clad45",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad45 = new G4LogicalVolume(solidClad45,
                                      Pethylene,
                                      "Clad45");
  physiClad45 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad45,
                                    "Clad45",
                                    logicWLSWorld45,
                                    false,
                                    0);


  WLSFiberLogic45 = new G4Tubs("wls45",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog45 = new G4LogicalVolume(WLSFiberLogic45,PMMA,"WLS45");
  WLSLog45->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber45 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog45,
                                       "WLSRPD45",
                                       logicClad45,
                                       false,
                                       0);


////


 
//WLS_46



  G4VSolid* solidClad46;
  solidClad46 = new G4Tubs("Clad46",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad46 = new G4LogicalVolume(solidClad46,
                                      Pethylene,
                                      "Clad46");
  physiClad46 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad46,
                                    "Clad46",
                                    logicWLSWorld46,
                                    false,
                                    0);


  WLSFiberLogic46 = new G4Tubs("wls46",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog46 = new G4LogicalVolume(WLSFiberLogic46,PMMA,"WLS46");
  WLSLog46->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber46 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog46,
                                       "WLSRPD46",
                                       logicClad46,
                                       false,
                                       0);


////

 
//WLS_47



  G4VSolid* solidClad47;
  solidClad47 = new G4Tubs("Clad47",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad47 = new G4LogicalVolume(solidClad47,
                                      Pethylene,
                                      "Clad47");
  physiClad47 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad47,
                                    "Clad47",
                                    logicWLSWorld47,
                                    false,
                                    0);


  WLSFiberLogic47 = new G4Tubs("wls47",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog47 = new G4LogicalVolume(WLSFiberLogic47,PMMA,"WLS47");
  WLSLog47->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber47 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog47,
                                       "WLSRPD47",
                                       logicClad47,
                                       false,
                                       0);


////
 
//WLS_48




  G4VSolid* solidClad48;
  solidClad48 = new G4Tubs("Clad48",CladinnerRadius,CladouterRadius,3.5*cm,startAngle, spanningAngle);
  logicClad48 = new G4LogicalVolume(solidClad48,
                                      Pethylene,
                                      "Clad48");
  physiClad48 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad48,
                                    "Clad48",
                                    logicWLSWorld48,
                                    false,
                                    0);


  WLSFiberLogic48 = new G4Tubs("wls48",WLSinnerRadius,WLSouterRadius,3.5*cm,startAngle, spanningAngle);
  WLSLog48 = new G4LogicalVolume(WLSFiberLogic48,PMMA,"WLS48");
  WLSLog48->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber48 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog48,
                                       "WLSRPD48",
                                       logicClad48,
                                       false,
                                       0);


////


 //empty line
 //--------------------------
 // RPD Channel 13
 //--------------------------
 G4ThreeVector positionWLSWorld49 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)-.6*cm),.499*cm,.59*cm);
  solidWLSWorld49 = new G4Box("wlsworld49",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld49 = new G4LogicalVolume(solidWLSWorld49, Grease, "WLSWorld49");

 G4ThreeVector positionWLSWorld50 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)-.2*cm),.499*cm,.59*cm);
  solidWLSWorld50 = new G4Box("wlsworld50",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld50 = new G4LogicalVolume(solidWLSWorld50, Grease, "WLSWorld50");

 G4ThreeVector positionWLSWorld51 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)+.6*cm),.499*cm,.59*cm);
  solidWLSWorld51 = new G4Box("wlsworld51",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld51 = new G4LogicalVolume(solidWLSWorld51, Grease, "WLSWorld51");

 G4ThreeVector positionWLSWorld52 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)+.2*cm),.499*cm,.59*cm);
  solidWLSWorld52 = new G4Box("wlsworld52",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld52 = new G4LogicalVolume(solidWLSWorld52, Grease, "WLSWorld52");



 G4ThreeVector positionRPD13 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)),(RPDYInitial-(3*RPDYDelta)),0.1375*cm);
  solidRPD13 = new G4Box("rpd13",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD13a = new G4UnionSolid("newrpd13a", solidRPD13, solidWLSWorld49,0,(positionWLSWorld49-positionRPD13));
  G4UnionSolid* newRPD13b = new G4UnionSolid("newrpd13b", newRPD13a, solidWLSWorld50,0,(positionWLSWorld50-positionRPD13));
  G4UnionSolid* newRPD13c = new G4UnionSolid("newrpd13c", newRPD13b, solidWLSWorld51,0,(positionWLSWorld51-positionRPD13));
  G4UnionSolid* newRPD13d = new G4UnionSolid("newrpd13d", newRPD13c, solidWLSWorld52,0,(positionWLSWorld52-positionRPD13));
  logicRPD13 = new G4LogicalVolume(newRPD13d, RPD_Material, "RPD13",0,0,0);
  physiRPD13 = new G4PVPlacement(0,               // no rotation
                                   positionRPD13,  // at (x,y,z)
                                   logicRPD13,     // its logical volume                             
                                   "RPD13",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld49 = new G4PVPlacement(0,
				    (positionWLSWorld49-positionRPD13),
				    logicWLSWorld49,
				    "WLSWorld49",
				    logicRPD13,
				    false,
				    0);

  physiWLSWorld50 = new G4PVPlacement(0,
				    (positionWLSWorld50-positionRPD13),
				    logicWLSWorld50,
				    "WLSWorld49",
				    logicRPD13,
				    false,
				    0);

 physiWLSWorld51 = new G4PVPlacement(0,
				    (positionWLSWorld51-positionRPD13),
				    logicWLSWorld51,
				    "WLSWorld51",
				    logicRPD13,
				    false,
				    0);

 physiWLSWorld52 = new G4PVPlacement(0,
				    (positionWLSWorld52-positionRPD13),
				    logicWLSWorld52,
				    "WLSWorld52",
				    logicRPD13,
				    false,
				    0);
	

  G4ThreeVector positionAl13 = G4ThreeVector((RPDXInitial+(0*RPDXDelta)),.5*cm,.65*cm);
  solidAl13 = new G4Box("Al13",RPDXsize+.0425*cm,4.51*cm,0.025*mm/SizeMod);
  logicAl13 = new G4LogicalVolume(solidAl13, Al, "Al13");
  physiAl13 = new G4PVPlacement(0,
				positionAl13,
				logicAl13,
				"Al13",
				logicRPDWorld,
				false,
				0); 			
				
  G4ThreeVector positionAlCov4 = G4ThreeVector(0,(RPDYInitial-(3*RPDYDelta)),-.425*cm);
  AlCov4 = new G4Box("alcov4",4.61*cm/SizeMod,.99999*cm/SizeMod,.099*mm/SizeMod);
  logicAlCov4 = new G4LogicalVolume(AlCov4, Al, "ALCOV4");
  physiAlCov4 = new G4PVPlacement(0,
				positionAlCov4,
				logicAlCov4,
				"ALCOV4",
				logicRPDWorld,
				false,
				0);
				
//WLS_49



  G4VSolid* solidClad49;
  solidClad49 = new G4Tubs("Clad49",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad49 = new G4LogicalVolume(solidClad49,
                                      Pethylene,
                                      "Clad49");
  physiClad49 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad49,
                                    "Clad49",
                                    logicWLSWorld49,
                                    false,
                                    0);


  WLSFiberLogic49 = new G4Tubs("wls49",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog49 = new G4LogicalVolume(WLSFiberLogic49,PMMA,"WLS49");
  WLSLog49->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber49 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog49,
                                       "WLSRPD49",
                                       logicClad49,
                                       false,
                                       0);


////


 
//WLS_50


  G4VSolid* solidClad50;
  solidClad50 = new G4Tubs("Clad50",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad50 = new G4LogicalVolume(solidClad50,
                                      Pethylene,
                                      "Clad50");
  physiClad50 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad50,
                                    "Clad50",
                                    logicWLSWorld50,
                                    false,
                                    0);


  WLSFiberLogic50 = new G4Tubs("wls50",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog50 = new G4LogicalVolume(WLSFiberLogic50,PMMA,"WLS50");
  WLSLog50->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber50 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog50,
                                       "WLSRPD50",
                                       logicClad50,
                                       false,
                                       0);


////

 
//WLS_51



  G4VSolid* solidClad51;
  solidClad51 = new G4Tubs("Clad51",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad51 = new G4LogicalVolume(solidClad51,
                                      Pethylene,
                                      "Clad51");
  physiClad51 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad51,
                                    "Clad51",
                                    logicWLSWorld51,
                                    false,
                                    0);


  WLSFiberLogic51 = new G4Tubs("wls51",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog51 = new G4LogicalVolume(WLSFiberLogic51,PMMA,"WLS51");
  WLSLog51->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber51 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog51,
                                       "WLSRPD51",
                                       logicClad51,
                                       false,
                                       0);


////

//WLS_52


  G4VSolid* solidClad52;
  solidClad52 = new G4Tubs("Clad52",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad52 = new G4LogicalVolume(solidClad52,
                                      Pethylene,
                                      "Clad52");
  physiClad52 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad52,
                                    "Clad52",
                                    logicWLSWorld52,
                                    false,
                                    0);


  WLSFiberLogic52 = new G4Tubs("wls52",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog52 = new G4LogicalVolume(WLSFiberLogic52,PMMA,"WLS52");
  WLSLog52->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber52 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog52,
                                       "WLSRPD52",
                                       logicClad52,
                                       false,
                                       0);


////

 
//empty line
 //--------------------------
 // RPD Channel 14
 //--------------------------

 G4ThreeVector positionWLSWorld53 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)-.6*cm),.499*cm,.59*cm);
  solidWLSWorld53 = new G4Box("wlsworld53",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld53 = new G4LogicalVolume(solidWLSWorld53, Grease, "WLSWorld53");

 G4ThreeVector positionWLSWorld54 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)-.2*cm),.499*cm,.59*cm);
  solidWLSWorld54 = new G4Box("wlsworld54",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld54 = new G4LogicalVolume(solidWLSWorld54, Grease, "WLSWorld54");

 G4ThreeVector positionWLSWorld55 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)+.6*cm),.499*cm,.59*cm);
  solidWLSWorld55 = new G4Box("wlsworld55",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld55 = new G4LogicalVolume(solidWLSWorld55, Grease, "WLSWorld55");

 G4ThreeVector positionWLSWorld56 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)+.2*cm),.499*cm,.59*cm);
  solidWLSWorld56 = new G4Box("wlsworld56",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld56 = new G4LogicalVolume(solidWLSWorld56, Grease, "WLSWorld56");



 G4ThreeVector positionRPD14 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)),(RPDYInitial-(3*RPDYDelta)),0.1375*cm);
  solidRPD14 = new G4Box("rpd14",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD14a = new G4UnionSolid("newrpd14a", solidRPD14, solidWLSWorld53,0,(positionWLSWorld53-positionRPD14));
  G4UnionSolid* newRPD14b = new G4UnionSolid("newrpd14b", newRPD14a, solidWLSWorld54,0,(positionWLSWorld54-positionRPD14));
  G4UnionSolid* newRPD14c = new G4UnionSolid("newrpd14c", newRPD14b, solidWLSWorld55,0,(positionWLSWorld55-positionRPD14));
  G4UnionSolid* newRPD14d = new G4UnionSolid("newrpd14d", newRPD14c, solidWLSWorld56,0,(positionWLSWorld56-positionRPD14));
  logicRPD14 = new G4LogicalVolume(newRPD14d, RPD_Material, "RPD14",0,0,0);
  physiRPD14 = new G4PVPlacement(0,               // no rotation
                                   positionRPD14,  // at (x,y,z)
                                   logicRPD14,     // its logical volume                             
                                   "RPD14",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld53 = new G4PVPlacement(0,
				    (positionWLSWorld53-positionRPD14),
				    logicWLSWorld53,
				    "WLSWorld53",
				    logicRPD14,
				    false,
				    0);

  physiWLSWorld54 = new G4PVPlacement(0,
				    (positionWLSWorld54-positionRPD14),
				    logicWLSWorld54,
				    "WLSWorld53",
				    logicRPD14,
				    false,
				    0);

 physiWLSWorld55 = new G4PVPlacement(0,
				    (positionWLSWorld55-positionRPD14),
				    logicWLSWorld55,
				    "WLSWorld55",
				    logicRPD14,
				    false,
				    0);

 physiWLSWorld56 = new G4PVPlacement(0,
				    (positionWLSWorld56-positionRPD14),
				    logicWLSWorld56,
				    "WLSWorld56",
				    logicRPD14,
				    false,
				    0);	
				
  G4ThreeVector positionAl14 = G4ThreeVector((RPDXInitial+(1*RPDXDelta)),.5*cm,.65*cm);
  solidAl14 = new G4Box("Al14",RPDXsize+.0425*cm,4.51*cm,0.025*mm/SizeMod);
  logicAl14 = new G4LogicalVolume(solidAl14, Al, "Al14");
  physiAl14 = new G4PVPlacement(0,
				positionAl14,
				logicAl14,
				"Al14",
				logicRPDWorld,
				false,
				0); 				
 
//WLS_53



  G4VSolid* solidClad53;
  solidClad53 = new G4Tubs("Clad53",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad53 = new G4LogicalVolume(solidClad53,
                                      Pethylene,
                                      "Clad53");
  physiClad53 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad53,
                                    "Clad53",
                                    logicWLSWorld53,
                                    false,
                                    0);


  WLSFiberLogic53 = new G4Tubs("wls53",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog53 = new G4LogicalVolume(WLSFiberLogic53,PMMA,"WLS53");
  WLSLog53->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber53 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog53,
                                       "WLSRPD53",
                                       logicClad53,
                                       false,
                                       0);


////

 
//WLS_54



  G4VSolid* solidClad54;
  solidClad54 = new G4Tubs("Clad54",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad54 = new G4LogicalVolume(solidClad54,
                                      Pethylene,
                                      "Clad54");
  physiClad54 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad54,
                                    "Clad54",
                                    logicWLSWorld54,
                                    false,
                                    0);


  WLSFiberLogic54 = new G4Tubs("wls54",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog54 = new G4LogicalVolume(WLSFiberLogic54,PMMA,"WLS54");
  WLSLog54->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber54 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog54,
                                       "WLSRPD54",
                                       logicClad54,
                                       false,
                                       0);


////

 
//WLS_55


  G4VSolid* solidClad55;
  solidClad55 = new G4Tubs("Clad55",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad55 = new G4LogicalVolume(solidClad55,
                                      Pethylene,
                                      "Clad55");
  physiClad55 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad55,
                                    "Clad55",
                                    logicWLSWorld55,
                                    false,
                                    0);


  WLSFiberLogic55 = new G4Tubs("wls55",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog55 = new G4LogicalVolume(WLSFiberLogic55,PMMA,"WLS55");
  WLSLog55->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber55 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog55,
                                       "WLSRPD55",
                                       logicClad55,
                                       false,
                                       0);


////
//WLS_56


  G4VSolid* solidClad56;
  solidClad56 = new G4Tubs("Clad56",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad56 = new G4LogicalVolume(solidClad56,
                                      Pethylene,
                                      "Clad56");
  physiClad56 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad56,
                                    "Clad56",
                                    logicWLSWorld56,
                                    false,
                                    0);


  WLSFiberLogic56 = new G4Tubs("wls56",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog56 = new G4LogicalVolume(WLSFiberLogic56,PMMA,"WLS56");
  WLSLog56->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber56 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog56,
                                       "WLSRPD56",
                                       logicClad56,
                                       false,
                                       0);


////

 
//empty line
 //--------------------------
 // RPD Channel 15
 //--------------------------
 G4ThreeVector positionWLSWorld57 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)-.6*cm),.499*cm,.59*cm);
  solidWLSWorld57 = new G4Box("wlsworld57",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld57 = new G4LogicalVolume(solidWLSWorld57, Grease, "WLSWorld57");

 G4ThreeVector positionWLSWorld58 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)-.2*cm),.499*cm,.59*cm);
  solidWLSWorld58 = new G4Box("wlsworld58",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld58 = new G4LogicalVolume(solidWLSWorld58, Grease, "WLSWorld58");

 G4ThreeVector positionWLSWorld59 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)+.6*cm),.499*cm,.59*cm);
  solidWLSWorld59 = new G4Box("wlsworld59",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld59 = new G4LogicalVolume(solidWLSWorld59, Grease, "WLSWorld59");

 G4ThreeVector positionWLSWorld60 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)+.2*cm),.499*cm,.59*cm);
  solidWLSWorld60 = new G4Box("wlsworld60",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld60 = new G4LogicalVolume(solidWLSWorld60, Grease, "WLSWorld60");



 G4ThreeVector positionRPD15 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)),(RPDYInitial-(3*RPDYDelta)),0.1375*cm);
  solidRPD15 = new G4Box("rpd15",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD15a = new G4UnionSolid("newrpd15a", solidRPD15, solidWLSWorld57,0,(positionWLSWorld57-positionRPD15));
  G4UnionSolid* newRPD15b = new G4UnionSolid("newrpd15b", newRPD15a, solidWLSWorld58,0,(positionWLSWorld58-positionRPD15));
  G4UnionSolid* newRPD15c = new G4UnionSolid("newrpd15c", newRPD15b, solidWLSWorld59,0,(positionWLSWorld59-positionRPD15));
  G4UnionSolid* newRPD15d = new G4UnionSolid("newrpd15d", newRPD15c, solidWLSWorld60,0,(positionWLSWorld60-positionRPD15));
  logicRPD15 = new G4LogicalVolume(newRPD15d, RPD_Material, "RPD15",0,0,0);
  physiRPD15 = new G4PVPlacement(0,               // no rotation
                                   positionRPD15,  // at (x,y,z)
                                   logicRPD15,     // its logical volume                             
                                   "RPD15",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld57 = new G4PVPlacement(0,
				    (positionWLSWorld57-positionRPD15),	//Overlaps
				    logicWLSWorld57,
				    "WLSWorld57",
				    logicRPD15,
				    false,
				    0);

  physiWLSWorld58 = new G4PVPlacement(0,
				    (positionWLSWorld58-positionRPD15),
				    logicWLSWorld58,
				    "WLSWorld58",
				    logicRPD15,
				    false,
				    0);

 physiWLSWorld59 = new G4PVPlacement(0,
				    (positionWLSWorld59-positionRPD15),
				    logicWLSWorld59,
				    "WLSWorld59",
				    logicRPD15,
				    false,
				    0);

 physiWLSWorld60 = new G4PVPlacement(0,
				    (positionWLSWorld60-positionRPD15),
				    logicWLSWorld60,
				    "WLSWorld60",
				    logicRPD15,
				    false,
				    0);
	
				
  G4ThreeVector positionAl15 = G4ThreeVector((RPDXInitial+(2*RPDXDelta)),.5*cm,.65*cm);
  solidAl15 = new G4Box("Al15",RPDXsize+.0425*cm,4.51*cm,0.025*mm/SizeMod);
  logicAl15 = new G4LogicalVolume(solidAl15, Al, "Al15");
  physiAl15 = new G4PVPlacement(0,
				positionAl15,
				logicAl15,
				"Al15",
				logicRPDWorld,
				false,
				0); 				
 
//WLS_57



  G4VSolid* solidClad57;
  solidClad57 = new G4Tubs("Clad57",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad57 = new G4LogicalVolume(solidClad57,
                                      Pethylene,
                                      "Clad57");
  physiClad57 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad57,
                                    "Clad57",
                                    logicWLSWorld57,
                                    false,
                                    0);


  WLSFiberLogic57 = new G4Tubs("wls57",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog57 = new G4LogicalVolume(WLSFiberLogic57,PMMA,"WLS57");
  WLSLog57->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber57 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog57,
                                       "WLSRPD57",
                                       logicClad57,
                                       false,
                                       0);


////

 
//WLS_58

  G4VSolid* solidClad58;
  solidClad58 = new G4Tubs("Clad58",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad58 = new G4LogicalVolume(solidClad58,
                                      Pethylene,
                                      "Clad58");
  physiClad58 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad58,
                                    "Clad58",
                                    logicWLSWorld58,
                                    false,
                                    0);


  WLSFiberLogic58 = new G4Tubs("wls58",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog58 = new G4LogicalVolume(WLSFiberLogic58,PMMA,"WLS58");
  WLSLog58->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber58 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog58,
                                       "WLSRPD58",
                                       logicClad58,
                                       false,
                                       0);


////



 
//WLS_59



  G4VSolid* solidClad59;
  solidClad59 = new G4Tubs("Clad59",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad59 = new G4LogicalVolume(solidClad59,
                                      Pethylene,
                                      "Clad59");
  physiClad59 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad59,
                                    "Clad59",
                                    logicWLSWorld59,
                                    false,
                                    0);


  WLSFiberLogic59 = new G4Tubs("wls59",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog59 = new G4LogicalVolume(WLSFiberLogic59,PMMA,"WLS59");
  WLSLog59->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber59 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog59,
                                       "WLSRPD59",
                                       logicClad59,
                                       false,
                                       0);

//WLS_60


  G4VSolid* solidClad60;
  solidClad60 = new G4Tubs("Clad60",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad60 = new G4LogicalVolume(solidClad60,
                                      Pethylene,
                                      "Clad60");
  physiClad60 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad60,
                                    "Clad60",
                                    logicWLSWorld60,
                                    false,
                                    0);


  WLSFiberLogic60 = new G4Tubs("wls60",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog60 = new G4LogicalVolume(WLSFiberLogic60,PMMA,"WLS60");
  WLSLog60->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber60 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog60,
                                       "WLSRPD60",
                                       logicClad60,
                                       false,
                                       0);


////

 
//empty line
 //--------------------------
 // RPD Channel 16
 //--------------------------
 G4ThreeVector positionWLSWorld61 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)-.6*cm),.499*cm,.59*cm);
  solidWLSWorld61 = new G4Box("wlsworld61",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld61 = new G4LogicalVolume(solidWLSWorld61, Grease, "WLSWorld61");

 G4ThreeVector positionWLSWorld62 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)-.2*cm),.499*cm,.59*cm);
  solidWLSWorld62 = new G4Box("wlsworld62",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld62 = new G4LogicalVolume(solidWLSWorld62, Grease, "WLSWorld62");

 G4ThreeVector positionWLSWorld63 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)+.2*cm),.499*cm,.59*cm);
  solidWLSWorld63 = new G4Box("wlsworld63",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld63 = new G4LogicalVolume(solidWLSWorld63, Grease, "WLSWorld63");

 G4ThreeVector positionWLSWorld64 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)+.6*cm),.499*cm,.59*cm);
  solidWLSWorld64 = new G4Box("wlsworld64",(0.54)*mm,4.501*cm,0.5*mm/SizeMod);
  logicWLSWorld64 = new G4LogicalVolume(solidWLSWorld64, Grease, "WLSWorld64");



 G4ThreeVector positionRPD16 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)),(RPDYInitial-(3*RPDYDelta)),0.1375*cm);
  solidRPD16 = new G4Box("rpd16",RPDXsize,RPDYsize,0.5*cm/SizeMod);

  G4UnionSolid* newRPD16a = new G4UnionSolid("newrpd16a", solidRPD16, solidWLSWorld61,0,(positionWLSWorld61-positionRPD16));
  G4UnionSolid* newRPD16b = new G4UnionSolid("newrpd16b", newRPD16a, solidWLSWorld62,0,(positionWLSWorld62-positionRPD16));
  G4UnionSolid* newRPD16c = new G4UnionSolid("newrpd16c", newRPD16b, solidWLSWorld63,0,(positionWLSWorld63-positionRPD16));
  G4UnionSolid* newRPD16d = new G4UnionSolid("newrpd16d", newRPD16c, solidWLSWorld64,0,(positionWLSWorld64-positionRPD16));
  logicRPD16 = new G4LogicalVolume(newRPD16d, RPD_Material, "RPD16",0,0,0);
  physiRPD16 = new G4PVPlacement(0,               // no rotation
                                   positionRPD16,  // at (x,y,z)
                                   logicRPD16,     // its logical volume                             
                                   "RPD16",        // its name
                                   logicRPDWorld,      // its mother  volume
                                   true,           // no boolean operations
                                   0);              // copy number 	


  physiWLSWorld61 = new G4PVPlacement(0,
				    (positionWLSWorld61-positionRPD16),
				    logicWLSWorld61,
				    "WLSWorld61",
				    logicRPD16,
				    false,
				    0);

  physiWLSWorld62 = new G4PVPlacement(0,
				    (positionWLSWorld62-positionRPD16),
				    logicWLSWorld62,
				    "WLSWorld61",
				    logicRPD16,
				    false,
				    0);

 physiWLSWorld63 = new G4PVPlacement(0,
				    (positionWLSWorld63-positionRPD16),
				    logicWLSWorld63,
				    "WLSWorld63",
				    logicRPD16,
				    false,
				    0);

 physiWLSWorld64 = new G4PVPlacement(0,
				    (positionWLSWorld64-positionRPD16),
				    logicWLSWorld64,
				    "WLSWorld64",
				    logicRPD16,
				    false,
				    0);	
				
  G4ThreeVector positionAl16 = G4ThreeVector((RPDXInitial+(3*RPDXDelta)),.5*cm,.65*cm);
  solidAl16 = new G4Box("Al16",RPDXsize+.0425*cm,4.51*cm,0.025*mm/SizeMod);
  logicAl16 = new G4LogicalVolume(solidAl16, Al, "Al16");
  physiAl16 = new G4PVPlacement(0,
				positionAl16,
				logicAl16,
				"Al16",
				logicRPDWorld,
				false,
				0); 				
				
 
//WLS_61




  G4VSolid* solidClad61;
  solidClad61 = new G4Tubs("Clad61",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad61 = new G4LogicalVolume(solidClad61,
                                      Pethylene,
                                      "Clad61");
  physiClad61 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad61,
                                    "Clad61",
                                    logicWLSWorld61,
                                    false,
                                    0);


  WLSFiberLogic61 = new G4Tubs("wls61",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog61 = new G4LogicalVolume(WLSFiberLogic61,PMMA,"WLS61");
  WLSLog61->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber61 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog61,
                                       "WLSRPD61",
                                       logicClad61,
                                       false,
                                       0);


////

 
//WLS_62



  G4VSolid* solidClad62;
  solidClad62 = new G4Tubs("Clad62",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad62 = new G4LogicalVolume(solidClad62,
                                      Pethylene,
                                      "Clad62");
  physiClad62 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad62,
                                    "Clad62",
                                    logicWLSWorld62,
                                    false,
                                    0);


  WLSFiberLogic62 = new G4Tubs("wls62",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog62 = new G4LogicalVolume(WLSFiberLogic62,PMMA,"WLS62");
  WLSLog62->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber62 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog62,
                                       "WLSRPD62",
                                       logicClad62,
                                       false,
                                       0);


////


 
//WLS_63



  G4VSolid* solidClad63;
  solidClad63 = new G4Tubs("Clad63",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad63 = new G4LogicalVolume(solidClad63,
                                      Pethylene,
                                      "Clad63");
  physiClad63 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad63,
                                    "Clad63",
                                    logicWLSWorld63,
                                    false,
                                    0);


  WLSFiberLogic63 = new G4Tubs("wls63",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog63 = new G4LogicalVolume(WLSFiberLogic63,PMMA,"WLS63");
  WLSLog63->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber63 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog63,
                                       "WLSRPD63",
                                       logicClad63,
                                       false,
                                       0);


////

 
//WLS_64


  G4VSolid* solidClad64;
  solidClad64 = new G4Tubs("Clad64",CladinnerRadius,CladouterRadius,4.5*cm,startAngle, spanningAngle);
  logicClad64 = new G4LogicalVolume(solidClad64,
                                      Pethylene,
                                      "Clad64");
  physiClad64 = new G4PVPlacement(myRotation,
                                    Pos_WLSRPD,
                                    logicClad64,
                                    "Clad64",
                                    logicWLSWorld64,
                                    false,
                                    0);


  WLSFiberLogic64 = new G4Tubs("wls64",WLSinnerRadius,WLSouterRadius,4.5*cm,startAngle, spanningAngle);
  WLSLog64 = new G4LogicalVolume(WLSFiberLogic64,PMMA,"WLS64");
  WLSLog64->SetUserLimits(new G4UserLimits(DBL_MAX,DBL_MAX,10*ms));
  WLSFiber64 = new G4PVPlacement(0,
                                       Pos_WLSRPD,
                                       WLSLog64,
                                       "WLSRPD64",
                                       logicClad64,
                                       false,
                                       0);


////




//empty line
 
 
 G4OpticalSurface* OpWLSSurface1 = new G4OpticalSurface("wlsSurf1");
 OpWLSSurface1->SetModel(glisur);
 OpWLSSurface1->SetType(dielectric_dielectric);
 OpWLSSurface1->SetFinish(polished);
 G4LogicalBorderSurface* wlsSurf1 = new G4LogicalBorderSurface("wlsSurf1",physiRPD1,physiClad1, OpWLSSurface1);
 
  G4OpticalSurface* OpWLSSurface2 = new G4OpticalSurface("wlsSurf2");
 OpWLSSurface2->SetModel(glisur);
 OpWLSSurface2->SetType(dielectric_dielectric);
 OpWLSSurface2->SetFinish(polished);
 G4LogicalBorderSurface* wlsSurf2 = new G4LogicalBorderSurface("wlsSurf2",physiRPD1,physiWLSWorld1, OpWLSSurface2);
 
   G4OpticalSurface* OpWLSSurface3 = new G4OpticalSurface("wlsSurf3");
 OpWLSSurface3->SetModel(glisur);
 OpWLSSurface3->SetType(dielectric_dielectric);
 OpWLSSurface3->SetFinish(polished);
 G4LogicalBorderSurface* wlsSurf3 = new G4LogicalBorderSurface("wlsSurf3",physiWLSWorld1,pmtdet1, OpWLSSurface3);
 
   G4OpticalSurface* OpWLSSurface4 = new G4OpticalSurface("wlsSurf4");
 OpWLSSurface4->SetModel(glisur);
 OpWLSSurface4->SetType(dielectric_dielectric);
 OpWLSSurface4->SetFinish(polished);
 G4LogicalBorderSurface* wlsSurf4 = new G4LogicalBorderSurface("wlsSurf4",physiClad1,pmtdet1, OpWLSSurface4);
 
    G4OpticalSurface* OpWLSSurface5 = new G4OpticalSurface("wlsSurf5");
 OpWLSSurface5->SetModel(glisur);
 OpWLSSurface5->SetType(dielectric_dielectric);
 OpWLSSurface5->SetFinish(polished);
 G4LogicalBorderSurface* wlsSurf5 = new G4LogicalBorderSurface("wlsSurf5",physiClad1,pmtdet1, OpWLSSurface5);
 
 
 
 
  //------------------------------------------------ 
  // Sensitive detectors
  //------------------------------------------------ 


/*  G4VSensitiveDetector* detRPD;
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector(detRPD);
  logicRPD->SetSensitiveDetector(detRPD);
  SDman->AddNewDetector(detRPD);
*/
//   G4SDManager* SDman = G4SDManager::GetSDMpointer();
// 
//   G4String trackerChamberSDname = "ExN02/TrackerChamberSD";
//   ExN02TrackerSD* aTrackerSD = new ExN02TrackerSD( trackerChamberSDname );
//   SDman->AddNewDetector( aTrackerSD );
//   logicChamber->SetSensitiveDetector( aTrackerSD );

CalorimeterSD* PMTsensitive1 = new  CalorimeterSD("/PhotonDet1");  
logicPhotonDet1->SetSensitiveDetector(PMTsensitive1);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive1);
CalorimeterSD* PMTsensitive2 = new  CalorimeterSD("/PhotonDet2");  
logicPhotonDet2->SetSensitiveDetector(PMTsensitive2);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive2);
CalorimeterSD* PMTsensitive3 = new  CalorimeterSD("/PhotonDet3");  
logicPhotonDet3->SetSensitiveDetector(PMTsensitive3);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive3);
CalorimeterSD* PMTsensitive4 = new  CalorimeterSD("/PhotonDet4");  
logicPhotonDet4->SetSensitiveDetector(PMTsensitive4);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive4);
CalorimeterSD* PMTsensitive5 = new  CalorimeterSD("/PhotonDet5");  
logicPhotonDet5->SetSensitiveDetector(PMTsensitive5);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive5);
CalorimeterSD* PMTsensitive6 = new  CalorimeterSD("/PhotonDet6");  
logicPhotonDet6->SetSensitiveDetector(PMTsensitive6);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive6);
CalorimeterSD* PMTsensitive7 = new  CalorimeterSD("/PhotonDet7");  
logicPhotonDet7->SetSensitiveDetector(PMTsensitive7);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive7);
CalorimeterSD* PMTsensitive8 = new  CalorimeterSD("/PhotonDet8");  
logicPhotonDet8->SetSensitiveDetector(PMTsensitive8);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive8);
CalorimeterSD* PMTsensitive9 = new  CalorimeterSD("/PhotonDet9");  
logicPhotonDet9->SetSensitiveDetector(PMTsensitive9);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive9);
CalorimeterSD* PMTsensitive10 = new  CalorimeterSD("/PhotonDet10");  
logicPhotonDet10->SetSensitiveDetector(PMTsensitive10);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive10);
CalorimeterSD* PMTsensitive11 = new  CalorimeterSD("/PhotonDet11");  
logicPhotonDet11->SetSensitiveDetector(PMTsensitive11);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive11);
CalorimeterSD* PMTsensitive12 = new  CalorimeterSD("/PhotonDet12");  
logicPhotonDet12->SetSensitiveDetector(PMTsensitive12);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive12);
CalorimeterSD* PMTsensitive13 = new  CalorimeterSD("/PhotonDet13");  
logicPhotonDet13->SetSensitiveDetector(PMTsensitive13);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive13);
CalorimeterSD* PMTsensitive14 = new  CalorimeterSD("/PhotonDet14");  
logicPhotonDet14->SetSensitiveDetector(PMTsensitive14);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive14);
CalorimeterSD* PMTsensitive15 = new  CalorimeterSD("/PhotonDet15");  
logicPhotonDet15->SetSensitiveDetector(PMTsensitive15);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive15);
CalorimeterSD* PMTsensitive16 = new  CalorimeterSD("/PhotonDet16");  
logicPhotonDet16->SetSensitiveDetector(PMTsensitive16);
G4SDManager::GetSDMpointer()->AddNewDetector(PMTsensitive16);

  CalorimeterSD* sensitive1 = new CalorimeterSD("/RPD1");  
  logicRPD1->SetSensitiveDetector(sensitive1);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive1);

  CalorimeterSD* sensitive2 = new CalorimeterSD("/RPD2");  
  logicRPD2->SetSensitiveDetector(sensitive2);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive2);

  CalorimeterSD* sensitive3 = new CalorimeterSD("/RPD3");  
  logicRPD3->SetSensitiveDetector(sensitive3);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive3);

  CalorimeterSD* sensitive4 = new CalorimeterSD("/RPD4");  
  logicRPD4->SetSensitiveDetector(sensitive4);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive4);

  CalorimeterSD* sensitive5 = new CalorimeterSD("/RPD5");  
  logicRPD5->SetSensitiveDetector(sensitive5);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive5);

  CalorimeterSD* sensitive6 = new CalorimeterSD("/RPD6");  
  logicRPD6->SetSensitiveDetector(sensitive6);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive6);

  CalorimeterSD* sensitive7 = new CalorimeterSD("/RPD7");  
  logicRPD7->SetSensitiveDetector(sensitive7);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive7);

  CalorimeterSD* sensitive8 = new CalorimeterSD("/RPD8");  
  logicRPD8->SetSensitiveDetector(sensitive8);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive8);

  CalorimeterSD* sensitive9 = new CalorimeterSD("/RPD9");  
  logicRPD9->SetSensitiveDetector(sensitive9);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive9);

  CalorimeterSD* sensitive10 = new CalorimeterSD("/RPD10");  
  logicRPD10->SetSensitiveDetector(sensitive10);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive10);

  CalorimeterSD* sensitive11 = new CalorimeterSD("/RPD11");  
  logicRPD11->SetSensitiveDetector(sensitive11);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive11);

  CalorimeterSD* sensitive12 = new CalorimeterSD("/RPD12");  
  logicRPD12->SetSensitiveDetector(sensitive12);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive12);

  CalorimeterSD* sensitive13 = new CalorimeterSD("/RPD13");  
  logicRPD13->SetSensitiveDetector(sensitive13);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive13);

  CalorimeterSD* sensitive14 = new CalorimeterSD("/RPD14");  
  logicRPD14->SetSensitiveDetector(sensitive14);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive14);

  CalorimeterSD* sensitive15 = new CalorimeterSD("/RPD15");  
  logicRPD15->SetSensitiveDetector(sensitive15);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive15);

  CalorimeterSD* sensitive16 = new CalorimeterSD("/RPD16");  
  logicRPD16->SetSensitiveDetector(sensitive16);
G4SDManager::GetSDMpointer()->AddNewDetector(sensitive16);


//--------- Visualization attributes -------------------------------
//G4cout<<"got some sensitive detectors added"<<G4endl;
  G4VisAttributes* BoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  
    G4VisAttributes* BoxVisAttBlack= new G4VisAttributes(G4Colour(0.0,0.0,0.0));
    G4VisAttributes* BoxVisAttGrey= new G4VisAttributes(G4Colour(0.5,0.5,0.5));

    G4VisAttributes* BoxVisAttYellow= new G4VisAttributes(G4Colour(1.0,1.0,0.0));

    G4VisAttributes* BoxVisAttGreen= new G4VisAttributes(G4Colour(0.0,1.0,0.0));

    G4VisAttributes* BoxVisAttBlue= new G4VisAttributes(G4Colour(0.0,0.0,1.0));
//logicRPDEnvelope4->SetVisAttributes(BoxVisAttBlue);

    G4VisAttributes* BoxVisAttRed= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  
    G4VisAttributes* BoxVisAttPurple= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
    
    G4VisAttributes* Hidden= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
    Hidden->SetVisibility(false);
    
// logicWorld  ->SetVisAttributes(BoxVisAttBlack);  
// logicRPDWorld->SetVisAttributes(BoxVisAttBlack);



 logicWorld  ->SetVisAttributes(Hidden);  
 logicRPDWorld->SetVisAttributes(Hidden);
 logicCuWorld->SetVisAttributes(Hidden);
 logicWLSWorld17->SetVisAttributes(BoxVisAttPurple); 
 logicWLSWorld18->SetVisAttributes(BoxVisAttBlue); 
 logicWLSWorld19->SetVisAttributes(BoxVisAttPurple); 
 logicWLSWorld20->SetVisAttributes(BoxVisAttBlue); 
 logicWLSWorld5->SetVisAttributes(BoxVisAttBlue);  

 
  logicClad1->SetVisAttributes(BoxVisAttPurple);
 logicClad2->SetVisAttributes(BoxVisAttPurple);
 logicClad3->SetVisAttributes(BoxVisAttPurple);
 logicClad4->SetVisAttributes(BoxVisAttPurple);
  logicClad5->SetVisAttributes(BoxVisAttPurple);
 logicClad6->SetVisAttributes(BoxVisAttPurple);
 logicClad7->SetVisAttributes(BoxVisAttPurple);
 logicClad8->SetVisAttributes(BoxVisAttPurple);
  logicClad9->SetVisAttributes(BoxVisAttPurple);
 logicClad10->SetVisAttributes(BoxVisAttPurple);
 logicClad11->SetVisAttributes(BoxVisAttPurple);
 logicClad12->SetVisAttributes(BoxVisAttPurple);
  logicClad13->SetVisAttributes(BoxVisAttPurple);
 logicClad14->SetVisAttributes(BoxVisAttPurple);
 logicClad15->SetVisAttributes(BoxVisAttPurple);
 logicClad16->SetVisAttributes(BoxVisAttPurple);
 
  logicClad17->SetVisAttributes(BoxVisAttBlue);
 logicClad18->SetVisAttributes(BoxVisAttPurple);
 logicClad19->SetVisAttributes(BoxVisAttBlue);
 logicClad20->SetVisAttributes(BoxVisAttPurple);
  logicClad21->SetVisAttributes(BoxVisAttPurple);
 logicClad22->SetVisAttributes(BoxVisAttPurple);
 logicClad23->SetVisAttributes(BoxVisAttPurple);
 logicClad24->SetVisAttributes(BoxVisAttPurple);
  logicClad25->SetVisAttributes(BoxVisAttPurple);
 logicClad26->SetVisAttributes(BoxVisAttPurple);
 logicClad27->SetVisAttributes(BoxVisAttPurple);
 logicClad28->SetVisAttributes(BoxVisAttPurple);
  logicClad29->SetVisAttributes(BoxVisAttPurple);
 logicClad30->SetVisAttributes(BoxVisAttPurple);
 logicClad31->SetVisAttributes(BoxVisAttPurple);
 logicClad32->SetVisAttributes(BoxVisAttPurple);
 
  logicClad33->SetVisAttributes(BoxVisAttPurple);
 logicClad34->SetVisAttributes(BoxVisAttPurple);
 logicClad35->SetVisAttributes(BoxVisAttPurple);
 logicClad36->SetVisAttributes(BoxVisAttPurple);
  logicClad37->SetVisAttributes(BoxVisAttPurple);
 logicClad38->SetVisAttributes(BoxVisAttPurple);
 logicClad39->SetVisAttributes(BoxVisAttPurple);
 logicClad40->SetVisAttributes(BoxVisAttPurple);
  logicClad41->SetVisAttributes(BoxVisAttPurple);
 logicClad42->SetVisAttributes(BoxVisAttPurple);
 logicClad43->SetVisAttributes(BoxVisAttPurple);
 logicClad44->SetVisAttributes(BoxVisAttPurple);
  logicClad45->SetVisAttributes(BoxVisAttPurple);
 logicClad46->SetVisAttributes(BoxVisAttPurple);
 logicClad47->SetVisAttributes(BoxVisAttPurple);
 logicClad48->SetVisAttributes(BoxVisAttPurple);
 
  logicClad49->SetVisAttributes(BoxVisAttPurple);
 logicClad50->SetVisAttributes(BoxVisAttPurple);
 logicClad51->SetVisAttributes(BoxVisAttPurple);
 logicClad52->SetVisAttributes(BoxVisAttPurple);
  logicClad53->SetVisAttributes(BoxVisAttPurple);
 logicClad54->SetVisAttributes(BoxVisAttPurple);
 logicClad55->SetVisAttributes(BoxVisAttPurple);
 logicClad56->SetVisAttributes(BoxVisAttPurple);
  logicClad57->SetVisAttributes(BoxVisAttPurple);
 logicClad58->SetVisAttributes(BoxVisAttPurple);
 logicClad59->SetVisAttributes(BoxVisAttPurple);
 logicClad60->SetVisAttributes(BoxVisAttPurple);
  logicClad61->SetVisAttributes(BoxVisAttPurple);
 logicClad62->SetVisAttributes(BoxVisAttPurple);
 logicClad63->SetVisAttributes(BoxVisAttPurple);
 logicClad64->SetVisAttributes(BoxVisAttPurple); 
  


 WLSLog1->SetVisAttributes(BoxVisAttRed);
 WLSLog2->SetVisAttributes(BoxVisAttRed);
 WLSLog3->SetVisAttributes(BoxVisAttRed);
 WLSLog4->SetVisAttributes(BoxVisAttRed);
  WLSLog5->SetVisAttributes(BoxVisAttRed);
 WLSLog6->SetVisAttributes(Hidden);
 WLSLog7->SetVisAttributes(Hidden);
 WLSLog8->SetVisAttributes(Hidden);
  WLSLog9->SetVisAttributes(Hidden);
 WLSLog10->SetVisAttributes(Hidden);
 WLSLog11->SetVisAttributes(Hidden);
 WLSLog12->SetVisAttributes(Hidden);
  WLSLog13->SetVisAttributes(Hidden);
 WLSLog14->SetVisAttributes(Hidden);
 WLSLog15->SetVisAttributes(Hidden);
 WLSLog16->SetVisAttributes(Hidden);
 
  WLSLog17->SetVisAttributes(Hidden);
 WLSLog18->SetVisAttributes(Hidden);
 WLSLog19->SetVisAttributes(Hidden);
 WLSLog20->SetVisAttributes(Hidden);
  WLSLog21->SetVisAttributes(Hidden);
 WLSLog22->SetVisAttributes(Hidden);
 WLSLog23->SetVisAttributes(Hidden);
 WLSLog24->SetVisAttributes(Hidden);
  WLSLog25->SetVisAttributes(Hidden);
 WLSLog26->SetVisAttributes(Hidden);
 WLSLog27->SetVisAttributes(Hidden);
 WLSLog28->SetVisAttributes(Hidden);
  WLSLog29->SetVisAttributes(Hidden);
 WLSLog30->SetVisAttributes(Hidden);
 WLSLog31->SetVisAttributes(Hidden);
 WLSLog32->SetVisAttributes(Hidden);
 
  WLSLog33->SetVisAttributes(Hidden);
 WLSLog34->SetVisAttributes(Hidden);
 WLSLog35->SetVisAttributes(Hidden);
 WLSLog36->SetVisAttributes(Hidden);
  WLSLog37->SetVisAttributes(Hidden);
 WLSLog38->SetVisAttributes(Hidden);
 WLSLog39->SetVisAttributes(Hidden);
 WLSLog40->SetVisAttributes(Hidden);
  WLSLog41->SetVisAttributes(Hidden);
 WLSLog42->SetVisAttributes(Hidden);
 WLSLog43->SetVisAttributes(Hidden);
 WLSLog44->SetVisAttributes(Hidden);
  WLSLog45->SetVisAttributes(Hidden);
 WLSLog46->SetVisAttributes(Hidden);
 WLSLog47->SetVisAttributes(Hidden);
 WLSLog48->SetVisAttributes(Hidden);
 
  WLSLog49->SetVisAttributes(Hidden);
 WLSLog50->SetVisAttributes(Hidden);
 WLSLog51->SetVisAttributes(Hidden);
 WLSLog52->SetVisAttributes(Hidden);
  WLSLog53->SetVisAttributes(Hidden);
 WLSLog54->SetVisAttributes(Hidden);
 WLSLog55->SetVisAttributes(Hidden);
 WLSLog56->SetVisAttributes(Hidden);
  WLSLog57->SetVisAttributes(Hidden);
 WLSLog58->SetVisAttributes(Hidden);
 WLSLog59->SetVisAttributes(Hidden);
 WLSLog60->SetVisAttributes(Hidden);
  WLSLog61->SetVisAttributes(Hidden);
 WLSLog62->SetVisAttributes(Hidden);
 WLSLog63->SetVisAttributes(Hidden);
 WLSLog64->SetVisAttributes(Hidden); 

 logicCu->SetVisAttributes(Hidden);

logicAl1->SetVisAttributes(Hidden);
logicAl2->SetVisAttributes(Hidden);
logicAl3->SetVisAttributes(Hidden);
logicAl4->SetVisAttributes(Hidden);
logicAl5->SetVisAttributes(Hidden);
logicAl6->SetVisAttributes(Hidden);
logicAl7->SetVisAttributes(Hidden);
logicAl8->SetVisAttributes(Hidden);
logicAl9->SetVisAttributes(Hidden);
logicAl10->SetVisAttributes(Hidden);
logicAl11->SetVisAttributes(Hidden);
logicAl12->SetVisAttributes(Hidden);
logicAl13->SetVisAttributes(Hidden);
logicAl14->SetVisAttributes(Hidden);
logicAl15->SetVisAttributes(Hidden); 
logicAl16->SetVisAttributes(Hidden);

logicAltop1->SetVisAttributes(Hidden);
logicAltop2->SetVisAttributes(Hidden);
logicAltop3->SetVisAttributes(Hidden);
logicAltop4->SetVisAttributes(Hidden);

logicAlside1->SetVisAttributes(Hidden);
logicAlside2->SetVisAttributes(Hidden);
logicAlside3->SetVisAttributes(Hidden);
logicAlside4->SetVisAttributes(Hidden);
logicAlside5->SetVisAttributes(Hidden);
logicAlside6->SetVisAttributes(Hidden);
logicAlside7->SetVisAttributes(Hidden);
logicAlside8->SetVisAttributes(Hidden);
logicAlside9->SetVisAttributes(Hidden);
logicAlside10->SetVisAttributes(Hidden);
logicAlside11->SetVisAttributes(Hidden);
logicAlside12->SetVisAttributes(Hidden);
logicAlside13->SetVisAttributes(Hidden);
logicAlside14->SetVisAttributes(Hidden);
logicAlside15->SetVisAttributes(Hidden);
logicAlside16->SetVisAttributes(Hidden);
logicAlside17->SetVisAttributes(Hidden);
logicAlside18->SetVisAttributes(Hidden);
logicAlside19->SetVisAttributes(Hidden);
logicAlside20->SetVisAttributes(Hidden);

logicAlCov1->SetVisAttributes(Hidden);
logicAlCov2->SetVisAttributes(Hidden);
logicAlCov3->SetVisAttributes(Hidden);
logicAlCov4->SetVisAttributes(Hidden);

logicAlbottom1->SetVisAttributes(Hidden);
logicAlbottom2->SetVisAttributes(Hidden);
logicAlbottom3->SetVisAttributes(Hidden);
logicAlbottom4->SetVisAttributes(Hidden);


 
 //EM Visual Stuff
/*
  logicEM->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber->SetVisAttributes(BoxVisAttYellow);
 logicEM2->SetVisAttributes(BoxVisAttPurple);
  logicEMFiber2->SetVisAttributes(BoxVisAttYellow);
  logicEM3->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber3->SetVisAttributes(BoxVisAttYellow);
 logicEM4->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber4->SetVisAttributes(BoxVisAttYellow);
 logicEM5->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber5->SetVisAttributes(BoxVisAttYellow);
 logicEM6->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber6->SetVisAttributes(BoxVisAttYellow);
 logicEM7->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber7->SetVisAttributes(BoxVisAttYellow);
 logicEM8->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber8->SetVisAttributes(BoxVisAttYellow);
 logicEM9->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber9->SetVisAttributes(BoxVisAttYellow);
 logicEM10->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber10->SetVisAttributes(BoxVisAttYellow);
 logicEM11->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber11->SetVisAttributes(BoxVisAttYellow);
 logicEM12->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber12->SetVisAttributes(BoxVisAttYellow);
 logicEM13->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber13->SetVisAttributes(BoxVisAttYellow);
 logicEM14->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber14->SetVisAttributes(BoxVisAttYellow);
 logicEM15->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber15->SetVisAttributes(BoxVisAttYellow);
 logicEM16->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber16->SetVisAttributes(BoxVisAttYellow);
 logicEM17->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber17->SetVisAttributes(BoxVisAttYellow);
 logicEM18->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber18->SetVisAttributes(BoxVisAttYellow);
 logicEM19->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber19->SetVisAttributes(BoxVisAttYellow);
 logicEM20->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber20->SetVisAttributes(BoxVisAttYellow);
 logicEM21->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber21->SetVisAttributes(BoxVisAttYellow);
 logicEM22->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber22->SetVisAttributes(BoxVisAttYellow);
 logicEM23->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber23->SetVisAttributes(BoxVisAttYellow);
 logicEM24->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber24->SetVisAttributes(BoxVisAttYellow);
 logicEM25->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber25->SetVisAttributes(BoxVisAttYellow);
 logicEM26->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber26->SetVisAttributes(BoxVisAttYellow);
 logicEM27->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber27->SetVisAttributes(BoxVisAttYellow);
 logicEM28->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber28->SetVisAttributes(BoxVisAttYellow);
 logicEM29->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber29->SetVisAttributes(BoxVisAttYellow);
 logicEM30->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber30->SetVisAttributes(BoxVisAttYellow);
 logicEM31->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber31->SetVisAttributes(BoxVisAttYellow);
 logicEM32->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber32->SetVisAttributes(BoxVisAttYellow);
 logicEM33->SetVisAttributes(BoxVisAttPurple);
 logicEMFiber33->SetVisAttributes(BoxVisAttYellow); 
 */
   logicEM->SetVisAttributes(Hidden);
 logicEMFiber->SetVisAttributes(Hidden);
 logicEM2->SetVisAttributes(Hidden);
  logicEMFiber2->SetVisAttributes(Hidden);
  logicEM3->SetVisAttributes(Hidden);
 logicEMFiber3->SetVisAttributes(Hidden);
 logicEM4->SetVisAttributes(Hidden);
 logicEMFiber4->SetVisAttributes(Hidden);
 logicEM5->SetVisAttributes(Hidden);
 logicEMFiber5->SetVisAttributes(Hidden);
 logicEM6->SetVisAttributes(Hidden);
 logicEMFiber6->SetVisAttributes(Hidden);
 logicEM7->SetVisAttributes(Hidden);
 logicEMFiber7->SetVisAttributes(Hidden);
 logicEM8->SetVisAttributes(Hidden);
 logicEMFiber8->SetVisAttributes(Hidden);
 logicEM9->SetVisAttributes(Hidden);
 logicEMFiber9->SetVisAttributes(Hidden);
 logicEM10->SetVisAttributes(Hidden);
 logicEMFiber10->SetVisAttributes(Hidden);
 logicEM11->SetVisAttributes(Hidden);
 logicEMFiber11->SetVisAttributes(Hidden);
 logicEM12->SetVisAttributes(Hidden);
 logicEMFiber12->SetVisAttributes(Hidden);
 logicEM13->SetVisAttributes(Hidden);
 logicEMFiber13->SetVisAttributes(Hidden);
 logicEM14->SetVisAttributes(Hidden);
 logicEMFiber14->SetVisAttributes(Hidden);
 logicEM15->SetVisAttributes(Hidden);
 logicEMFiber15->SetVisAttributes(Hidden);
 logicEM16->SetVisAttributes(Hidden);
 logicEMFiber16->SetVisAttributes(Hidden);
 logicEM17->SetVisAttributes(Hidden);
 logicEMFiber17->SetVisAttributes(Hidden);
 logicEM18->SetVisAttributes(Hidden);
 logicEMFiber18->SetVisAttributes(Hidden);
 logicEM19->SetVisAttributes(Hidden);
 logicEMFiber19->SetVisAttributes(Hidden);
 logicEM20->SetVisAttributes(Hidden);
 logicEMFiber20->SetVisAttributes(Hidden);
 logicEM21->SetVisAttributes(Hidden);
 logicEMFiber21->SetVisAttributes(Hidden);
 logicEM22->SetVisAttributes(Hidden);
 logicEMFiber22->SetVisAttributes(Hidden);
 logicEM23->SetVisAttributes(Hidden);
 logicEMFiber23->SetVisAttributes(Hidden);
 logicEM24->SetVisAttributes(Hidden);
 logicEMFiber24->SetVisAttributes(Hidden);
 logicEM25->SetVisAttributes(Hidden);
 logicEMFiber25->SetVisAttributes(Hidden);
 logicEM26->SetVisAttributes(Hidden);
 logicEMFiber26->SetVisAttributes(Hidden);
 logicEM27->SetVisAttributes(Hidden);
 logicEMFiber27->SetVisAttributes(Hidden);
 logicEM28->SetVisAttributes(Hidden);
 logicEMFiber28->SetVisAttributes(Hidden);
 logicEM29->SetVisAttributes(Hidden);
 logicEMFiber29->SetVisAttributes(Hidden);
 logicEM30->SetVisAttributes(Hidden);
 logicEMFiber30->SetVisAttributes(Hidden);
 logicEM31->SetVisAttributes(Hidden);
 logicEMFiber31->SetVisAttributes(Hidden);
 logicEM32->SetVisAttributes(Hidden);
 logicEMFiber32->SetVisAttributes(Hidden);
 logicEM33->SetVisAttributes(Hidden);
 logicEMFiber33->SetVisAttributes(Hidden);
 
 logicEMEnvelope->SetVisAttributes(Hidden);
 
 
// logicPhotonDet1->SetVisAttributes(BoxVisAttYellow);
 logicPhotonDet2->SetVisAttributes(Hidden);
 logicPhotonDet3->SetVisAttributes(Hidden);
 logicPhotonDet4->SetVisAttributes(Hidden);
 logicPhotonDet5->SetVisAttributes(Hidden);
 logicPhotonDet6->SetVisAttributes(Hidden);
 logicPhotonDet7->SetVisAttributes(Hidden);
 logicPhotonDet8->SetVisAttributes(Hidden);
 logicPhotonDet9->SetVisAttributes(Hidden);
 logicPhotonDet10->SetVisAttributes(Hidden);
 logicPhotonDet11->SetVisAttributes(Hidden);
 logicPhotonDet12->SetVisAttributes(Hidden);
 logicPhotonDet13->SetVisAttributes(Hidden);
 logicPhotonDet14->SetVisAttributes(Hidden);
 logicPhotonDet15->SetVisAttributes(Hidden);
 logicPhotonDet16->SetVisAttributes(Hidden);


  logicRPD1->SetVisAttributes(BoxVisAttYellow);
 logicRPD2->SetVisAttributes(BoxVisAttYellow);
 logicRPD3->SetVisAttributes(BoxVisAttYellow);
 logicRPD4->SetVisAttributes(BoxVisAttYellow);
 logicRPD5->SetVisAttributes(BoxVisAttYellow);
 logicRPD6->SetVisAttributes(BoxVisAttYellow);
 logicRPD7->SetVisAttributes(BoxVisAttYellow);
 logicRPD8->SetVisAttributes(BoxVisAttYellow);
 logicRPD9->SetVisAttributes(BoxVisAttYellow);
 logicRPD10->SetVisAttributes(BoxVisAttYellow);
 logicRPD11->SetVisAttributes(BoxVisAttYellow);
 logicRPD12->SetVisAttributes(BoxVisAttYellow);
 logicRPD13->SetVisAttributes(BoxVisAttYellow);
 logicRPD14->SetVisAttributes(BoxVisAttYellow);
 logicRPD15->SetVisAttributes(BoxVisAttYellow);
 logicRPD16->SetVisAttributes(BoxVisAttYellow); 


/*
 logicRPD1->SetVisAttributes(Hidden);
 logicRPD2->SetVisAttributes(Hidden);
 logicRPD3->SetVisAttributes(Hidden);
 logicRPD4->SetVisAttributes(Hidden);
 logicRPD5->SetVisAttributes(Hidden);
 logicRPD6->SetVisAttributes(Hidden);
 logicRPD7->SetVisAttributes(Hidden);
 logicRPD8->SetVisAttributes(Hidden);
 logicRPD9->SetVisAttributes(Hidden);
 logicRPD10->SetVisAttributes(Hidden);
 logicRPD11->SetVisAttributes(Hidden);
 logicRPD12->SetVisAttributes(Hidden);
 logicRPD13->SetVisAttributes(Hidden);
 logicRPD14->SetVisAttributes(Hidden);
 logicRPD15->SetVisAttributes(Hidden);
 logicRPD16->SetVisAttributes(Hidden); 
 
 
 */
  
//--------- example of User Limits -------------------------------

  // below is an example of how to set tracking constraints in a given
  // logical volume(see also in N02PhysicsList how to setup the processes
  // G4StepLimiter or G4UserSpecialCuts).
    
  // Sets a max Step length in the tracker region, with G4StepLimiter
  //
//  G4cout<<"going to set some maxStep limits"<<G4endl;
 G4double maxStep = 0.5*mm;
 //G4double maxStep=0.5*mm;
 stepLimit = new G4UserLimits(maxStep);
//   G4double maxStep = 0.5*2.0*mm;
//   stepLimit = new G4UserLimits(maxStep);

logicWorld->SetUserLimits(stepLimit);
 
 logicEMEnvelope->SetUserLimits(stepLimit);
 logicEM->SetUserLimits(stepLimit);
 logicEMFiber->SetUserLimits(stepLimit);
 logicEM2->SetUserLimits(stepLimit);
 logicEMFiber2->SetUserLimits(stepLimit);
 logicEM3->SetUserLimits(stepLimit);
 logicEMFiber3->SetUserLimits(stepLimit);
 logicEM4->SetUserLimits(stepLimit);
 logicEMFiber4->SetUserLimits(stepLimit);
 logicEM5->SetUserLimits(stepLimit);
 logicEMFiber5->SetUserLimits(stepLimit);
 logicEM6->SetUserLimits(stepLimit);
 logicEMFiber6->SetUserLimits(stepLimit);
 logicEM7->SetUserLimits(stepLimit);
 logicEMFiber7->SetUserLimits(stepLimit);
 logicEM8->SetUserLimits(stepLimit);
 logicEMFiber8->SetUserLimits(stepLimit);
 logicEM9->SetUserLimits(stepLimit);
 logicEMFiber9->SetUserLimits(stepLimit);
 logicEM10->SetUserLimits(stepLimit);
 logicEMFiber10->SetUserLimits(stepLimit);
 logicEM11->SetUserLimits(stepLimit);
 logicEMFiber11->SetUserLimits(stepLimit);
 logicEM12->SetUserLimits(stepLimit);
 logicEMFiber12->SetUserLimits(stepLimit);
 logicEM13->SetUserLimits(stepLimit);
 logicEMFiber13->SetUserLimits(stepLimit);
 logicEM14->SetUserLimits(stepLimit);
 logicEMFiber14->SetUserLimits(stepLimit);
 logicEM15->SetUserLimits(stepLimit);
 logicEMFiber15->SetUserLimits(stepLimit);
 logicEM16->SetUserLimits(stepLimit);
 logicEMFiber16->SetUserLimits(stepLimit);
 logicEM17->SetUserLimits(stepLimit);
 logicEMFiber17->SetUserLimits(stepLimit);
 logicEM18->SetUserLimits(stepLimit);
 logicEMFiber18->SetUserLimits(stepLimit);
 logicEM19->SetUserLimits(stepLimit);
 logicEMFiber19->SetUserLimits(stepLimit);
 logicEM20->SetUserLimits(stepLimit);
 logicEMFiber20->SetUserLimits(stepLimit);
 logicEM21->SetUserLimits(stepLimit);
 logicEMFiber21->SetUserLimits(stepLimit);
 logicEM22->SetUserLimits(stepLimit);
 logicEMFiber22->SetUserLimits(stepLimit);
 logicEM23->SetUserLimits(stepLimit);
 logicEMFiber23->SetUserLimits(stepLimit);
 logicEM24->SetUserLimits(stepLimit);
 logicEMFiber24->SetUserLimits(stepLimit);
 logicEM25->SetUserLimits(stepLimit);
 logicEMFiber25->SetUserLimits(stepLimit);
 logicEM26->SetUserLimits(stepLimit);
 logicEMFiber26->SetUserLimits(stepLimit);
 logicEM27->SetUserLimits(stepLimit);
 logicEMFiber27->SetUserLimits(stepLimit);
 logicEM28->SetUserLimits(stepLimit);
 logicEMFiber28->SetUserLimits(stepLimit);
 logicEM29->SetUserLimits(stepLimit);
 logicEMFiber29->SetUserLimits(stepLimit);
 logicEM30->SetUserLimits(stepLimit);
 logicEMFiber30->SetUserLimits(stepLimit);
 logicEM31->SetUserLimits(stepLimit);
 logicEMFiber31->SetUserLimits(stepLimit);
 logicEM32->SetUserLimits(stepLimit);
 logicEMFiber32->SetUserLimits(stepLimit);
 logicEM33->SetUserLimits(stepLimit);
 logicEMFiber33->SetUserLimits(stepLimit);
   
// //  logicEM->SetUserLimits(stepLimit);
   logicRPD1->SetUserLimits(stepLimit);
   logicRPD2->SetUserLimits(stepLimit);  
   logicRPD3->SetUserLimits(stepLimit);
   logicRPD4->SetUserLimits(stepLimit);
   logicRPD5->SetUserLimits(stepLimit);
   logicRPD6->SetUserLimits(stepLimit);
   logicRPD7->SetUserLimits(stepLimit);
   logicRPD8->SetUserLimits(stepLimit);
   logicRPD9->SetUserLimits(stepLimit);
   logicRPD10->SetUserLimits(stepLimit);
   logicRPD11->SetUserLimits(stepLimit);
   logicRPD12->SetUserLimits(stepLimit);
   logicRPD13->SetUserLimits(stepLimit);
   logicRPD14->SetUserLimits(stepLimit);
   logicRPD15->SetUserLimits(stepLimit);
   logicRPD16->SetUserLimits(stepLimit);
//   logicCu->SetUserLimits(stepLimit);
// //
// //   logicEMEnvelope->SetUserLimits(stepLimit);
//   logicEMFiber1->SetUserLimits(stepLimit);
//   logicEMLayer1->SetUserLimits(stepLimit);
//   logicRPDEnvelope->SetUserLimits(stepLimit);
//   logicRPDEnvelope2->SetUserLimits(stepLimit);  
//   logicRPDEnvelope3->SetUserLimits(stepLimit);
//   logicRPDEnvelope4->SetUserLimits(stepLimit);
// //  logicTracker->SetUserLimits(stepLimit);
//   
//   // Set additional contraints on the track, with G4UserSpecialCuts
//   //
//   G4double maxLength = 2*EMZsize, maxTime = 0.1*ns, minEkin = 100*MeV;
// //    logicEM->SetUserLimits(new G4UserLimits(maxStep,maxLength,maxTime,minEkin));
//   logicEMAbsorber1->SetUserLimits(new G4UserLimits(maxStep,maxLength,maxTime,minEkin));
//   


//Place fibers
   // ********************************************************************************
   //   - Add wave length shifting fiber by uncommenting below
   // ********************************************************************************
/*   
  
     
   G4RotationMatrix* rm = new G4RotationMatrix();
   rm->rotateY(90*deg);
 
   G4LogicalVolume *fiber_log = ConstructWLSFiber();
 
   for(G4int i=0;i<4;i++){
     G4double Y=-(4*cm/4)*(4-1)*0.5 + i*4*cm/4;
     new G4PVPlacement(rm,G4ThreeVector(0.,Y,0.),fiber_log,"WLSfiber",positionRPD1,false,i); 
   }  
   */



  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
// void ExN02DetectorConstruction::setTargetMaterial(G4String materialName)
// {
//   // search the material by its name 
//   G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
//   if (pttoMaterial)
//      {TargetMater = pttoMaterial;
//       logicTarget->SetMaterial(pttoMaterial); 
//       G4cout << "\n----> The target is " << fTargetLength/cm << " cm of "
//              << materialName << G4endl;
//      }             
// }
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// void ExN02DetectorConstruction::setChamberMaterial(G4String materialName)
// {
//   // search the material by its name 
//   G4Material* pttoMaterial = G4Material::GetMaterial(materialName);  
//   if (pttoMaterial)
//      {ChamberMater = pttoMaterial;
//       logicChamber->SetMaterial(pttoMaterial); 
//       G4cout << "\n----> The chambers are " << ChamberWidth/cm << " cm of "
//              << materialName << G4endl;
//      }             
// }
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
// void ExN02DetectorConstruction::SetMagField(G4double fieldValue)
// {
//   fpMagField->SetMagFieldValue(fieldValue);  
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02DetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((stepLimit)&&(maxStep>0.)) stepLimit->SetMaxAllowedStep(maxStep);
}


////////the following are used in SteppingAction, added for WLS fibers
G4double ExN02DetectorConstruction::GetWLSFiberEnd()
{
  return wlsfiberOrigin + wlsfiberZ;
}

G4bool ExN02DetectorConstruction::IsPerfectFiber()
{ 
  return     surfaceRoughness == 1. && XYRatio == 1.
             && (!mirrorToggle    || 
             (mirrorPolish    == 1. && mirrorReflectivity == 1.));
}

 G4Material* ExN02DetectorConstruction::FindMaterial(G4String name) {
     G4Material* material = G4Material::GetMaterial(name,true);
     return material;
 }
 
 void ExN02DetectorConstruction::SetMirrorReflectivity(G4double reflectivity)
{
  mirrorReflectivity = reflectivity;
}

// Set the Polish of the PhotonDet, polish of 1 is a perfect mirror surface
// Pre: 0 < polish <= 1
void ExN02DetectorConstruction::SetPhotonDetPolish(G4double polish)
{
  mppcPolish = polish;
}

// Set the Reflectivity of the PhotonDet, reflectivity of 1 is a perfect mirror
// Pre: 0 < reflectivity <= 1
void ExN02DetectorConstruction::SetPhotonDetReflectivity(G4double reflectivity)
{
  mppcReflectivity = reflectivity;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
