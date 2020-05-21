
#ifndef ExN02DetectorConstruction_h
#define ExN02DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
//#include "ExN02MagneticField.hh"
#include "CalorimeterSD.hh"
#include "HistoManager.hh"
#include "G4VSolid.hh"

class G4Box;
class G4LogicalVolume;
class G4VSolid;
class G4VPhysicalVolume;
#include "G4NistManager.hh"
class G4Material;
class G4VPVParameterisation;
class G4UserLimits;
class ExN02DetectorMessenger;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
     ExN02DetectorConstruction();
    ~ExN02DetectorConstruction();
    
    G4double GetWLSFiberEnd();
    G4bool   IsPerfectFiber();
    
    G4Material* FindMaterial(G4String);
    G4double mppcPolish;

  public:
  
     G4VPhysicalVolume* Materials();
     G4VPhysicalVolume* Construct();
     
     
     void SetMirrorReflectivity    (G4double);
    // Set the polish of the mirror
    void SetMirrorPolish          (G4double);
    // Set the reflectivity of the mirror
    void SetPhotonDetReflectivity (G4double);
    void SetPhotonDetPolish       (G4double);
    G4double mppcReflectivity;
    
    G4double GetWorldFullLength()   {return fWorldLength;}; 

     void SetMaxStep (G4double);     
     
  private:

  G4double           mppcZ;
  G4double           wlsfiberRY;
  G4double           mppcHalfL;
  
  G4NistManager*     nistMan;

    G4Material*        Air;

    G4Material*        PMMA;
    G4Material*        Pethylene;
    G4Material*        FPethylene;
    G4Material*        Polystyrene;
    G4Material*        Silicone;
    G4Material*        Coating;
    G4Material*		   Al;
    G4Material*		   Cu;
    G4Material*		   Pb;
    G4Material*		   W;
    G4Material*		   Grease;

    G4double coupleOrigin;
    G4double mirrorOrigin;
    G4double mppcOriginX;
    G4double mppcOriginZ;
 
    G4int numOfCladLayers;

    G4String           mppcShape;
    G4double           mppcTheta;
        G4double           clrfiberZ;
    G4double           mppcDist;

    G4double extrusionPolish;
    G4double extrusionReflectivity;



    G4double barLength;
    G4double barBase;
    G4double holeRadius;
    G4double holeLength;
    G4double coatingThickness;
    G4double coatingRadius;
    
  
  G4double wlsfiberOrigin;
  G4double           wlsfiberZ;
  G4double surfaceRoughness;
  G4double XYRatio;
  G4bool             mirrorToggle;
  G4double mirrorPolish;
  G4double mirrorReflectivity;
  G4Box*             solidWorld;    // pointer to the solid envelope 
  G4LogicalVolume*   logicWorld;    // pointer to the logical envelope
  G4VPhysicalVolume* physiWorld;    // pointer to the physical envelope

  
//------------------------------------------------------------------------
  G4Box* solidPMTAl1;
  G4LogicalVolume* logicPMTAl1;
  G4VPhysicalVolume* physiPMTAl1;
    G4Box* solidPMTAl2;
  G4LogicalVolume* logicPMTAl2;
  G4VPhysicalVolume* physiPMTAl2;
    G4Box* solidPMTAl3;
  G4LogicalVolume* logicPMTAl3;
  G4VPhysicalVolume* physiPMTAl3;
    G4Box* solidPMTAl4;
  G4LogicalVolume* logicPMTAl4;
  G4VPhysicalVolume* physiPMTAl4;



  G4Box* AlCov4;
  G4LogicalVolume* logicAlCov4;
  G4VPhysicalVolume* physiAlCov4;
    G4Box* AlCov3;
  G4LogicalVolume* logicAlCov3;
  G4VPhysicalVolume* physiAlCov3;
    G4Box* AlCov2;
  G4LogicalVolume* logicAlCov2;
  G4VPhysicalVolume* physiAlCov2;
    G4Box* AlCov1;
  G4LogicalVolume* logicAlCov1;
  G4VPhysicalVolume* physiAlCov1;
  
    G4Box* solidAltop4;
  G4LogicalVolume* logicAltop4;
  G4VPhysicalVolume* physiAltop4;
    G4Box* solidAltop3;
  G4LogicalVolume* logicAltop3;
  G4VPhysicalVolume* physiAltop3;
    G4Box* solidAltop2;
  G4LogicalVolume* logicAltop2;
  G4VPhysicalVolume* physiAltop2;
    G4Box* solidAltop1;
  G4LogicalVolume* logicAltop1;
  G4VPhysicalVolume* physiAltop1;
  
      G4Box* solidAlbottom4;
  G4LogicalVolume* logicAlbottom4;
  G4VPhysicalVolume* physiAlbottom4;
    G4Box* solidAlbottom3;
  G4LogicalVolume* logicAlbottom3;
  G4VPhysicalVolume* physiAlbottom3;
    G4Box* solidAlbottom2;
  G4LogicalVolume* logicAlbottom2;
  G4VPhysicalVolume* physiAlbottom2;
    G4Box* solidAlbottom1;
  G4LogicalVolume* logicAlbottom1;
  G4VPhysicalVolume* physiAlbottom1;
  
      G4Box* solidAlside1;
  G4LogicalVolume* logicAlside1;
  G4VPhysicalVolume* physiAlside1;
  
    G4VPhysicalVolume* pmtdet1;
  
        G4Box* solidAlside2;
  G4LogicalVolume* logicAlside2;
  G4VPhysicalVolume* physiAlside2;
        G4Box* solidAlside3;
  G4LogicalVolume* logicAlside3;
  G4VPhysicalVolume* physiAlside3;
        G4Box* solidAlside4;
  G4LogicalVolume* logicAlside4;
  G4VPhysicalVolume* physiAlside4;
        G4Box* solidAlside5;
  G4LogicalVolume* logicAlside5;
  G4VPhysicalVolume* physiAlside5;
        G4Box* solidAlside6;
  G4LogicalVolume* logicAlside6;
  G4VPhysicalVolume* physiAlside6;
        G4Box* solidAlside7;
  G4LogicalVolume* logicAlside7;
  G4VPhysicalVolume* physiAlside7;
        G4Box* solidAlside8;
  G4LogicalVolume* logicAlside8;
  G4VPhysicalVolume* physiAlside8;
        G4Box* solidAlside9;
  G4LogicalVolume* logicAlside9;
  G4VPhysicalVolume* physiAlside9;
        G4Box* solidAlside10;
  G4LogicalVolume* logicAlside10;
  G4VPhysicalVolume* physiAlside10;
        G4Box* solidAlside11;
  G4LogicalVolume* logicAlside11;
  G4VPhysicalVolume* physiAlside11;
        G4Box* solidAlside12;
  G4LogicalVolume* logicAlside12;
  G4VPhysicalVolume* physiAlside12;
  G4Box* solidAlside13;
  G4LogicalVolume* logicAlside13;
  G4VPhysicalVolume* physiAlside13;
  G4Box* solidAlside14;
  G4LogicalVolume* logicAlside14;
  G4VPhysicalVolume* physiAlside14;
  G4Box* solidAlside15;
  G4LogicalVolume* logicAlside15;
  G4VPhysicalVolume* physiAlside15;
  G4Box* solidAlside16;
  G4LogicalVolume* logicAlside16;
  G4VPhysicalVolume* physiAlside16;
  G4Box* solidAlside17;
  G4LogicalVolume* logicAlside17;
  G4VPhysicalVolume* physiAlside17;
  G4Box* solidAlside18;
  G4LogicalVolume* logicAlside18;
  G4VPhysicalVolume* physiAlside18;
  G4Box* solidAlside19;
  G4LogicalVolume* logicAlside19;
  G4VPhysicalVolume* physiAlside19;
    G4Box* solidAlside20;
  G4LogicalVolume* logicAlside20;
  G4VPhysicalVolume* physiAlside20;

//EM SECTION

///Absorber From EM/////////////
  G4Box* solidEMEnvelope;
  G4LogicalVolume* logicEMEnvelope;
  G4VPhysicalVolume* physiEMEnvelope;
  
  G4Box* solidEM;
  G4LogicalVolume* logicEM;
  G4VPhysicalVolume* physiEM;  

  G4Box* solidEMFiber;
  G4LogicalVolume* logicEMFiber;
  G4VPhysicalVolume* physiEMFiber;

  G4Box* solidAl1;
  G4LogicalVolume* logicAl1;
  G4VPhysicalVolume* physiAl1;
  
    G4Box* solidAl2;
  G4LogicalVolume* logicAl2;
  G4VPhysicalVolume* physiAl2;
  
    G4Box* solidAl3;
  G4LogicalVolume* logicAl3;
  G4VPhysicalVolume* physiAl3;
  
    G4Box* solidAl4;
  G4LogicalVolume* logicAl4;
  G4VPhysicalVolume* physiAl4;
  
    G4Box* solidAl5;
  G4LogicalVolume* logicAl5;
  G4VPhysicalVolume* physiAl5;

    G4Box* solidAl6;
  G4LogicalVolume* logicAl6;
  G4VPhysicalVolume* physiAl6;

    G4Box* solidAl7;
  G4LogicalVolume* logicAl7;
  G4VPhysicalVolume* physiAl7;

    G4Box* solidAl8;
  G4LogicalVolume* logicAl8;
  G4VPhysicalVolume* physiAl8;

    G4Box* solidAl9;
  G4LogicalVolume* logicAl9;
  G4VPhysicalVolume* physiAl9;

    G4Box* solidAl10;
  G4LogicalVolume* logicAl10;
  G4VPhysicalVolume* physiAl10;

    G4Box* solidAl11;
  G4LogicalVolume* logicAl11;
  G4VPhysicalVolume* physiAl11;

    G4Box* solidAl12;
  G4LogicalVolume* logicAl12;
  G4VPhysicalVolume* physiAl12;

    G4Box* solidAl13;
  G4LogicalVolume* logicAl13;
  G4VPhysicalVolume* physiAl13;

    G4Box* solidAl14;
  G4LogicalVolume* logicAl14;
  G4VPhysicalVolume* physiAl14;

    G4Box* solidAl15;
  G4LogicalVolume* logicAl15;
  G4VPhysicalVolume* physiAl15;

    G4Box* solidAl16;
  G4LogicalVolume* logicAl16;
  G4VPhysicalVolume* physiAl16;

////////////////////////////////////////
////////////////////2//////////////////
G4Box* solidEM2;
G4LogicalVolume* logicEM2;
G4VPhysicalVolume* physiEM2;
//empty space 
G4Box* solidEMFiber2;
G4LogicalVolume* logicEMFiber2;
G4VPhysicalVolume* physiEMFiber2;
////////////////////////////////////////
////////////////////3//////////////////
G4Box* solidEM3;
G4LogicalVolume* logicEM3;
G4VPhysicalVolume* physiEM3;
//empty space 
G4Box* solidEMFiber3;
G4LogicalVolume* logicEMFiber3;
G4VPhysicalVolume* physiEMFiber3;
////////////////////////////////////////
////////////////////4//////////////////
G4Box* solidEM4;
G4LogicalVolume* logicEM4;
G4VPhysicalVolume* physiEM4;
//empty space 
G4Box* solidEMFiber4;
G4LogicalVolume* logicEMFiber4;
G4VPhysicalVolume* physiEMFiber4;
////////////////////////////////////////
////////////////////5//////////////////
G4Box* solidEM5;
G4LogicalVolume* logicEM5;
G4VPhysicalVolume* physiEM5;
//empty space 
G4Box* solidEMFiber5;
G4LogicalVolume* logicEMFiber5;
G4VPhysicalVolume* physiEMFiber5;
////////////////////////////////////////
////////////////////6//////////////////
G4Box* solidEM6;
G4LogicalVolume* logicEM6;
G4VPhysicalVolume* physiEM6;
//empty space 
G4Box* solidEMFiber6;
G4LogicalVolume* logicEMFiber6;
G4VPhysicalVolume* physiEMFiber6;
////////////////////////////////////////
////////////////////7//////////////////
G4Box* solidEM7;
G4LogicalVolume* logicEM7;
G4VPhysicalVolume* physiEM7;
//empty space 
G4Box* solidEMFiber7;
G4LogicalVolume* logicEMFiber7;
G4VPhysicalVolume* physiEMFiber7;
////////////////////////////////////////
////////////////////8//////////////////
G4Box* solidEM8;
G4LogicalVolume* logicEM8;
G4VPhysicalVolume* physiEM8;
//empty space 
G4Box* solidEMFiber8;
G4LogicalVolume* logicEMFiber8;
G4VPhysicalVolume* physiEMFiber8;
////////////////////////////////////////
////////////////////9//////////////////
G4Box* solidEM9;
G4LogicalVolume* logicEM9;
G4VPhysicalVolume* physiEM9;
//empty space 
G4Box* solidEMFiber9;
G4LogicalVolume* logicEMFiber9;
G4VPhysicalVolume* physiEMFiber9;
////////////////////////////////////////
////////////////////10//////////////////
G4Box* solidEM10;
G4LogicalVolume* logicEM10;
G4VPhysicalVolume* physiEM10;
//empty space 
G4Box* solidEMFiber10;
G4LogicalVolume* logicEMFiber10;
G4VPhysicalVolume* physiEMFiber10;
////////////////////////////////////////
////////////////////11//////////////////
G4Box* solidEM11;
G4LogicalVolume* logicEM11;
G4VPhysicalVolume* physiEM11;
//empty space 
G4Box* solidEMFiber11;
G4LogicalVolume* logicEMFiber11;
G4VPhysicalVolume* physiEMFiber11;
////////////////////////////////////////
////////////////////12//////////////////
G4Box* solidEM12;
G4LogicalVolume* logicEM12;
G4VPhysicalVolume* physiEM12;
//empty space 
G4Box* solidEMFiber12;
G4LogicalVolume* logicEMFiber12;
G4VPhysicalVolume* physiEMFiber12;
////////////////////////////////////////
////////////////////13//////////////////
G4Box* solidEM13;
G4LogicalVolume* logicEM13;
G4VPhysicalVolume* physiEM13;
//empty space 
G4Box* solidEMFiber13;
G4LogicalVolume* logicEMFiber13;
G4VPhysicalVolume* physiEMFiber13;
////////////////////////////////////////
////////////////////14//////////////////
G4Box* solidEM14;
G4LogicalVolume* logicEM14;
G4VPhysicalVolume* physiEM14;
//empty space 
G4Box* solidEMFiber14;
G4LogicalVolume* logicEMFiber14;
G4VPhysicalVolume* physiEMFiber14;
////////////////////////////////////////
////////////////////15//////////////////
G4Box* solidEM15;
G4LogicalVolume* logicEM15;
G4VPhysicalVolume* physiEM15;
//empty space 
G4Box* solidEMFiber15;
G4LogicalVolume* logicEMFiber15;
G4VPhysicalVolume* physiEMFiber15;
////////////////////////////////////////
////////////////////16//////////////////
G4Box* solidEM16;
G4LogicalVolume* logicEM16;
G4VPhysicalVolume* physiEM16;
//empty space 
G4Box* solidEMFiber16;
G4LogicalVolume* logicEMFiber16;
G4VPhysicalVolume* physiEMFiber16;
////////////////////////////////////////
////////////////////17//////////////////
G4Box* solidEM17;
G4LogicalVolume* logicEM17;
G4VPhysicalVolume* physiEM17;
//empty space 
G4Box* solidEMFiber17;
G4LogicalVolume* logicEMFiber17;
G4VPhysicalVolume* physiEMFiber17;
////////////////////////////////////////
////////////////////18//////////////////
G4Box* solidEM18;
G4LogicalVolume* logicEM18;
G4VPhysicalVolume* physiEM18;
//empty space 
G4Box* solidEMFiber18;
G4LogicalVolume* logicEMFiber18;
G4VPhysicalVolume* physiEMFiber18;
////////////////////////////////////////
////////////////////19//////////////////
G4Box* solidEM19;
G4LogicalVolume* logicEM19;
G4VPhysicalVolume* physiEM19;
//empty space 
G4Box* solidEMFiber19;
G4LogicalVolume* logicEMFiber19;
G4VPhysicalVolume* physiEMFiber19;
////////////////////////////////////////
////////////////////20//////////////////
G4Box* solidEM20;
G4LogicalVolume* logicEM20;
G4VPhysicalVolume* physiEM20;
//empty space 
G4Box* solidEMFiber20;
G4LogicalVolume* logicEMFiber20;
G4VPhysicalVolume* physiEMFiber20;
////////////////////////////////////////
////////////////////21//////////////////
G4Box* solidEM21;
G4LogicalVolume* logicEM21;
G4VPhysicalVolume* physiEM21;
//empty space 
G4Box* solidEMFiber21;
G4LogicalVolume* logicEMFiber21;
G4VPhysicalVolume* physiEMFiber21;
////////////////////////////////////////
////////////////////22//////////////////
G4Box* solidEM22;
G4LogicalVolume* logicEM22;
G4VPhysicalVolume* physiEM22;
//empty space 
G4Box* solidEMFiber22;
G4LogicalVolume* logicEMFiber22;
G4VPhysicalVolume* physiEMFiber22;
////////////////////////////////////////
////////////////////23//////////////////
G4Box* solidEM23;
G4LogicalVolume* logicEM23;
G4VPhysicalVolume* physiEM23;
//empty space 
G4Box* solidEMFiber23;
G4LogicalVolume* logicEMFiber23;
G4VPhysicalVolume* physiEMFiber23;
////////////////////////////////////////
////////////////////24//////////////////
G4Box* solidEM24;
G4LogicalVolume* logicEM24;
G4VPhysicalVolume* physiEM24;
//empty space 
G4Box* solidEMFiber24;
G4LogicalVolume* logicEMFiber24;
G4VPhysicalVolume* physiEMFiber24;
////////////////////////////////////////
////////////////////25//////////////////
G4Box* solidEM25;
G4LogicalVolume* logicEM25;
G4VPhysicalVolume* physiEM25;
//empty space 
G4Box* solidEMFiber25;
G4LogicalVolume* logicEMFiber25;
G4VPhysicalVolume* physiEMFiber25;
////////////////////////////////////////
////////////////////26//////////////////
G4Box* solidEM26;
G4LogicalVolume* logicEM26;
G4VPhysicalVolume* physiEM26;
//empty space 
G4Box* solidEMFiber26;
G4LogicalVolume* logicEMFiber26;
G4VPhysicalVolume* physiEMFiber26;
////////////////////////////////////////
////////////////////27//////////////////
G4Box* solidEM27;
G4LogicalVolume* logicEM27;
G4VPhysicalVolume* physiEM27;
//empty space 
G4Box* solidEMFiber27;
G4LogicalVolume* logicEMFiber27;
G4VPhysicalVolume* physiEMFiber27;
////////////////////////////////////////
////////////////////28//////////////////
G4Box* solidEM28;
G4LogicalVolume* logicEM28;
G4VPhysicalVolume* physiEM28;
//empty space 
G4Box* solidEMFiber28;
G4LogicalVolume* logicEMFiber28;
G4VPhysicalVolume* physiEMFiber28;
////////////////////////////////////////
////////////////////29//////////////////
G4Box* solidEM29;
G4LogicalVolume* logicEM29;
G4VPhysicalVolume* physiEM29;
//empty space 
G4Box* solidEMFiber29;
G4LogicalVolume* logicEMFiber29;
G4VPhysicalVolume* physiEMFiber29;
////////////////////////////////////////
////////////////////30//////////////////
G4Box* solidEM30;
G4LogicalVolume* logicEM30;
G4VPhysicalVolume* physiEM30;
//empty space 
G4Box* solidEMFiber30;
G4LogicalVolume* logicEMFiber30;
G4VPhysicalVolume* physiEMFiber30;
////////////////////////////////////////
////////////////////31//////////////////
G4Box* solidEM31;
G4LogicalVolume* logicEM31;
G4VPhysicalVolume* physiEM31;
//empty space 
G4Box* solidEMFiber31;
G4LogicalVolume* logicEMFiber31;
G4VPhysicalVolume* physiEMFiber31;
////////////////////////////////////////
////////////////////32//////////////////
G4Box* solidEM32;
G4LogicalVolume* logicEM32;
G4VPhysicalVolume* physiEM32;
//empty space 
G4Box* solidEMFiber32;
G4LogicalVolume* logicEMFiber32;
G4VPhysicalVolume* physiEMFiber32;
////////////////////////////////////////
////////////////////33//////////////////
G4Box* solidEM33;
G4LogicalVolume* logicEM33;
G4VPhysicalVolume* physiEM33;
//empty space 
G4Box* solidEMFiber33;
G4LogicalVolume* logicEMFiber33;
G4VPhysicalVolume* physiEMFiber33;



//////Cu WORLD///////////////
  G4Box* solidCuWorld;
  G4LogicalVolume* logicCuWorld;
  G4VPhysicalVolume* physiCuWorld;
////////////////////////////////////////
////////////////////Copper Panel//////////////////
G4Box* CuPanel;
G4LogicalVolume* logicCu;
G4VPhysicalVolume* physiCu; 

///////////WLS//////////
G4VSolid* WLSFiberLogic1;
  G4LogicalVolume* WLSLog1;
  G4VPhysicalVolume* WLSFiber1;

G4VSolid* WLSFiberLogic2;
  G4LogicalVolume* WLSLog2;
  G4VPhysicalVolume* WLSFiber2;
  
G4VSolid* WLSFiberLogic3;
  G4LogicalVolume* WLSLog3;
  G4VPhysicalVolume* WLSFiber3;
  
G4VSolid* WLSFiberLogic4;
  G4LogicalVolume* WLSLog4;
  G4VPhysicalVolume* WLSFiber4;

G4VSolid* WLSFiberLogic5;
  G4LogicalVolume* WLSLog5;
  G4VPhysicalVolume* WLSFiber5;

G4VSolid* WLSFiberLogic6;
  G4LogicalVolume* WLSLog6;
  G4VPhysicalVolume* WLSFiber6;
  
G4VSolid* WLSFiberLogic7;
  G4LogicalVolume* WLSLog7;
  G4VPhysicalVolume* WLSFiber7;
  
G4VSolid* WLSFiberLogic8;
  G4LogicalVolume* WLSLog8;
  G4VPhysicalVolume* WLSFiber8;
  
G4VSolid* WLSFiberLogic9;
  G4LogicalVolume* WLSLog9;
  G4VPhysicalVolume* WLSFiber9;

G4VSolid* WLSFiberLogic10;
  G4LogicalVolume* WLSLog10;
  G4VPhysicalVolume* WLSFiber10;
  
G4VSolid* WLSFiberLogic11;
  G4LogicalVolume* WLSLog11;
  G4VPhysicalVolume* WLSFiber11;
  
G4VSolid* WLSFiberLogic12;
  G4LogicalVolume* WLSLog12;
  G4VPhysicalVolume* WLSFiber12;
  
G4VSolid* WLSFiberLogic13;
  G4LogicalVolume* WLSLog13;
  G4VPhysicalVolume* WLSFiber13;

G4VSolid* WLSFiberLogic14;
  G4LogicalVolume* WLSLog14;
  G4VPhysicalVolume* WLSFiber14;
  
G4VSolid* WLSFiberLogic15;
  G4LogicalVolume* WLSLog15;
  G4VPhysicalVolume* WLSFiber15;
  
G4VSolid* WLSFiberLogic16;
  G4LogicalVolume* WLSLog16;
  G4VPhysicalVolume* WLSFiber16;
  
G4VSolid* WLSFiberLogic17;
  G4LogicalVolume* WLSLog17;
  G4VPhysicalVolume* WLSFiber17;

G4VSolid* WLSFiberLogic18;
  G4LogicalVolume* WLSLog18;
  G4VPhysicalVolume* WLSFiber18;
  
G4VSolid* WLSFiberLogic19;
  G4LogicalVolume* WLSLog19;
  G4VPhysicalVolume* WLSFiber19;
  
G4VSolid* WLSFiberLogic20;
  G4LogicalVolume* WLSLog20;
  G4VPhysicalVolume* WLSFiber20;
  
G4VSolid* WLSFiberLogic21;
  G4LogicalVolume* WLSLog21;
  G4VPhysicalVolume* WLSFiber21;

G4VSolid* WLSFiberLogic22;
  G4LogicalVolume* WLSLog22;
  G4VPhysicalVolume* WLSFiber22;
  
G4VSolid* WLSFiberLogic23;
  G4LogicalVolume* WLSLog23;
  G4VPhysicalVolume* WLSFiber23;
  
G4VSolid* WLSFiberLogic24;
  G4LogicalVolume* WLSLog24;
  G4VPhysicalVolume* WLSFiber24;
  
  
G4VSolid* WLSFiberLogic25;
  G4LogicalVolume* WLSLog25;
  G4VPhysicalVolume* WLSFiber25;

G4VSolid* WLSFiberLogic26;
  G4LogicalVolume* WLSLog26;
  G4VPhysicalVolume* WLSFiber26;
  
G4VSolid* WLSFiberLogic27;
  G4LogicalVolume* WLSLog27;
  G4VPhysicalVolume* WLSFiber27;
  
G4VSolid* WLSFiberLogic28;
  G4LogicalVolume* WLSLog28;
  G4VPhysicalVolume* WLSFiber28;


G4VSolid* WLSFiberLogic29;
  G4LogicalVolume* WLSLog29;
  G4VPhysicalVolume* WLSFiber29;

G4VSolid* WLSFiberLogic30;
  G4LogicalVolume* WLSLog30;
  G4VPhysicalVolume* WLSFiber30;
  
G4VSolid* WLSFiberLogic31;
  G4LogicalVolume* WLSLog31;
  G4VPhysicalVolume* WLSFiber31;
  
G4VSolid* WLSFiberLogic32;
  G4LogicalVolume* WLSLog32;
  G4VPhysicalVolume* WLSFiber32;
  
G4VSolid* WLSFiberLogic33;
  G4LogicalVolume* WLSLog33;
  G4VPhysicalVolume* WLSFiber33;

G4VSolid* WLSFiberLogic34;
  G4LogicalVolume* WLSLog34;
  G4VPhysicalVolume* WLSFiber34;
  
G4VSolid* WLSFiberLogic35;
  G4LogicalVolume* WLSLog35;
  G4VPhysicalVolume* WLSFiber35;
  
G4VSolid* WLSFiberLogic36;
  G4LogicalVolume* WLSLog36;
  G4VPhysicalVolume* WLSFiber36;
  
G4VSolid* WLSFiberLogic37;
  G4LogicalVolume* WLSLog37;
  G4VPhysicalVolume* WLSFiber37;

G4VSolid* WLSFiberLogic38;
  G4LogicalVolume* WLSLog38;
  G4VPhysicalVolume* WLSFiber38;
  
G4VSolid* WLSFiberLogic39;
  G4LogicalVolume* WLSLog39;
  G4VPhysicalVolume* WLSFiber39;
  
G4VSolid* WLSFiberLogic40;
  G4LogicalVolume* WLSLog40;
  G4VPhysicalVolume* WLSFiber40;
  
G4VSolid* WLSFiberLogic41;
  G4LogicalVolume* WLSLog41;
  G4VPhysicalVolume* WLSFiber41;

G4VSolid* WLSFiberLogic42;
  G4LogicalVolume* WLSLog42;
  G4VPhysicalVolume* WLSFiber42;
  
G4VSolid* WLSFiberLogic43;
  G4LogicalVolume* WLSLog43;
  G4VPhysicalVolume* WLSFiber43;
  
G4VSolid* WLSFiberLogic44;
  G4LogicalVolume* WLSLog44;
  G4VPhysicalVolume* WLSFiber44;
  
G4VSolid* WLSFiberLogic45;
  G4LogicalVolume* WLSLog45;
  G4VPhysicalVolume* WLSFiber45;

G4VSolid* WLSFiberLogic46;
  G4LogicalVolume* WLSLog46;
  G4VPhysicalVolume* WLSFiber46;
  
G4VSolid* WLSFiberLogic47;
  G4LogicalVolume* WLSLog47;
  G4VPhysicalVolume* WLSFiber47;
  
G4VSolid* WLSFiberLogic48;
  G4LogicalVolume* WLSLog48;
  G4VPhysicalVolume* WLSFiber48;
  
G4VSolid* WLSFiberLogic49;
  G4LogicalVolume* WLSLog49;
  G4VPhysicalVolume* WLSFiber49;

G4VSolid* WLSFiberLogic50;
  G4LogicalVolume* WLSLog50;
  G4VPhysicalVolume* WLSFiber50;
  
G4VSolid* WLSFiberLogic51;
  G4LogicalVolume* WLSLog51;
  G4VPhysicalVolume* WLSFiber51;
  
G4VSolid* WLSFiberLogic52;
  G4LogicalVolume* WLSLog52;
  G4VPhysicalVolume* WLSFiber52;

G4VSolid* WLSFiberLogic53;
  G4LogicalVolume* WLSLog53;
  G4VPhysicalVolume* WLSFiber53;

G4VSolid* WLSFiberLogic54;
  G4LogicalVolume* WLSLog54;
  G4VPhysicalVolume* WLSFiber54;
  
G4VSolid* WLSFiberLogic55;
  G4LogicalVolume* WLSLog55;
  G4VPhysicalVolume* WLSFiber55;
  
G4VSolid* WLSFiberLogic56;
  G4LogicalVolume* WLSLog56;
  G4VPhysicalVolume* WLSFiber56;
  
G4VSolid* WLSFiberLogic57;
  G4LogicalVolume* WLSLog57;
  G4VPhysicalVolume* WLSFiber57;

G4VSolid* WLSFiberLogic58;
  G4LogicalVolume* WLSLog58;
  G4VPhysicalVolume* WLSFiber58;
  
G4VSolid* WLSFiberLogic59;
  G4LogicalVolume* WLSLog59;
  G4VPhysicalVolume* WLSFiber59;
  
G4VSolid* WLSFiberLogic60;
  G4LogicalVolume* WLSLog60;
  G4VPhysicalVolume* WLSFiber60;
  
G4VSolid* WLSFiberLogic61;
  G4LogicalVolume* WLSLog61;
  G4VPhysicalVolume* WLSFiber61;

G4VSolid* WLSFiberLogic62;
  G4LogicalVolume* WLSLog62;
  G4VPhysicalVolume* WLSFiber62;
  
G4VSolid* WLSFiberLogic63;
  G4LogicalVolume* WLSLog63;
  G4VPhysicalVolume* WLSFiber63;
  
G4VSolid* WLSFiberLogic64;
  G4LogicalVolume* WLSLog64;
  G4VPhysicalVolume* WLSFiber64;



  G4Box* solidWLSWorld1;
  G4LogicalVolume* logicWLSWorld1;
  G4VPhysicalVolume* physiWLSWorld1;
    G4Box* solidWLSWorld2;
  G4LogicalVolume* logicWLSWorld2;
  G4VPhysicalVolume* physiWLSWorld2;
    G4Box* solidWLSWorld3;
  G4LogicalVolume* logicWLSWorld3;
  G4VPhysicalVolume* physiWLSWorld3;
    G4Box* solidWLSWorld4;
  G4LogicalVolume* logicWLSWorld4;
  G4VPhysicalVolume* physiWLSWorld4;
    G4Box* solidWLSWorld5;
  G4LogicalVolume* logicWLSWorld5;
  G4VPhysicalVolume* physiWLSWorld5;
    G4Box* solidWLSWorld6;
  G4LogicalVolume* logicWLSWorld6;
  G4VPhysicalVolume* physiWLSWorld6;
    G4Box* solidWLSWorld7;
  G4LogicalVolume* logicWLSWorld7;
  G4VPhysicalVolume* physiWLSWorld7;
    G4Box* solidWLSWorld8;
  G4LogicalVolume* logicWLSWorld8;
  G4VPhysicalVolume* physiWLSWorld8;
    G4Box* solidWLSWorld9;
  G4LogicalVolume* logicWLSWorld9;
  G4VPhysicalVolume* physiWLSWorld9;  
    G4Box* solidWLSWorld10;
  G4LogicalVolume* logicWLSWorld10;
  G4VPhysicalVolume* physiWLSWorld10;
    G4Box* solidWLSWorld11;
  G4LogicalVolume* logicWLSWorld11;
  G4VPhysicalVolume* physiWLSWorld11;
    G4Box* solidWLSWorld12;
  G4LogicalVolume* logicWLSWorld12;
  G4VPhysicalVolume* physiWLSWorld12;
    G4Box* solidWLSWorld13;
  G4LogicalVolume* logicWLSWorld13;
  G4VPhysicalVolume* physiWLSWorld13;
    G4Box* solidWLSWorld14;
  G4LogicalVolume* logicWLSWorld14;
  G4VPhysicalVolume* physiWLSWorld14;
    G4Box* solidWLSWorld15;
  G4LogicalVolume* logicWLSWorld15;
  G4VPhysicalVolume* physiWLSWorld15;
    G4Box* solidWLSWorld16;
  G4LogicalVolume* logicWLSWorld16;
  G4VPhysicalVolume* physiWLSWorld16;
    G4Box* solidWLSWorld17;
  G4LogicalVolume* logicWLSWorld17;
  G4VPhysicalVolume* physiWLSWorld17;
    G4Box* solidWLSWorld18;
  G4LogicalVolume* logicWLSWorld18;
  G4VPhysicalVolume* physiWLSWorld18;
    G4Box* solidWLSWorld19;
  G4LogicalVolume* logicWLSWorld19;
  G4VPhysicalVolume* physiWLSWorld19;
    G4Box* solidWLSWorld20;
  G4LogicalVolume* logicWLSWorld20;
  G4VPhysicalVolume* physiWLSWorld20;
    G4Box* solidWLSWorld21;
  G4LogicalVolume* logicWLSWorld21;
  G4VPhysicalVolume* physiWLSWorld21;
    G4Box* solidWLSWorld22;
  G4LogicalVolume* logicWLSWorld22;
  G4VPhysicalVolume* physiWLSWorld22;
    G4Box* solidWLSWorld23;
  G4LogicalVolume* logicWLSWorld23;
  G4VPhysicalVolume* physiWLSWorld23;
    G4Box* solidWLSWorld24;
  G4LogicalVolume* logicWLSWorld24;
  G4VPhysicalVolume* physiWLSWorld24;
    G4Box* solidWLSWorld25;
  G4LogicalVolume* logicWLSWorld25;
  G4VPhysicalVolume* physiWLSWorld25;
    G4Box* solidWLSWorld26;
  G4LogicalVolume* logicWLSWorld26;
  G4VPhysicalVolume* physiWLSWorld26;
    G4Box* solidWLSWorld27;
  G4LogicalVolume* logicWLSWorld27;
  G4VPhysicalVolume* physiWLSWorld27;
    G4Box* solidWLSWorld28;
  G4LogicalVolume* logicWLSWorld28;
  G4VPhysicalVolume* physiWLSWorld28;
    G4Box* solidWLSWorld29;
  G4LogicalVolume* logicWLSWorld29;
  G4VPhysicalVolume* physiWLSWorld29;
    G4Box* solidWLSWorld30;
  G4LogicalVolume* logicWLSWorld30;
  G4VPhysicalVolume* physiWLSWorld30;
    G4Box* solidWLSWorld31;
  G4LogicalVolume* logicWLSWorld31;
  G4VPhysicalVolume* physiWLSWorld31;
    G4Box* solidWLSWorld32;
  G4LogicalVolume* logicWLSWorld32;
  G4VPhysicalVolume* physiWLSWorld32;
    G4Box* solidWLSWorld33;
  G4LogicalVolume* logicWLSWorld33;
  G4VPhysicalVolume* physiWLSWorld33;
    G4Box* solidWLSWorld34;
  G4LogicalVolume* logicWLSWorld34;
  G4VPhysicalVolume* physiWLSWorld34;
    G4Box* solidWLSWorld35;
  G4LogicalVolume* logicWLSWorld35;
  G4VPhysicalVolume* physiWLSWorld35;
    G4Box* solidWLSWorld36;
  G4LogicalVolume* logicWLSWorld36;
  G4VPhysicalVolume* physiWLSWorld36;
    G4Box* solidWLSWorld37;
  G4LogicalVolume* logicWLSWorld37;
  G4VPhysicalVolume* physiWLSWorld37;
    G4Box* solidWLSWorld38;
  G4LogicalVolume* logicWLSWorld38;
  G4VPhysicalVolume* physiWLSWorld38;
    G4Box* solidWLSWorld39;
  G4LogicalVolume* logicWLSWorld39;
  G4VPhysicalVolume* physiWLSWorld39;
    G4Box* solidWLSWorld40;
  G4LogicalVolume* logicWLSWorld40;
  G4VPhysicalVolume* physiWLSWorld40;
    G4Box* solidWLSWorld41;
  G4LogicalVolume* logicWLSWorld41;
  G4VPhysicalVolume* physiWLSWorld41;
    G4Box* solidWLSWorld42;
  G4LogicalVolume* logicWLSWorld42;
  G4VPhysicalVolume* physiWLSWorld42;
    G4Box* solidWLSWorld43;
  G4LogicalVolume* logicWLSWorld43;
  G4VPhysicalVolume* physiWLSWorld43;
    G4Box* solidWLSWorld44;
  G4LogicalVolume* logicWLSWorld44;
  G4VPhysicalVolume* physiWLSWorld44;
    G4Box* solidWLSWorld45;
  G4LogicalVolume* logicWLSWorld45;
  G4VPhysicalVolume* physiWLSWorld45;
    G4Box* solidWLSWorld46;
  G4LogicalVolume* logicWLSWorld46;
  G4VPhysicalVolume* physiWLSWorld46;
    G4Box* solidWLSWorld47;
  G4LogicalVolume* logicWLSWorld47;
  G4VPhysicalVolume* physiWLSWorld47;
    G4Box* solidWLSWorld48;
  G4LogicalVolume* logicWLSWorld48;
  G4VPhysicalVolume* physiWLSWorld48;
    G4Box* solidWLSWorld49;
  G4LogicalVolume* logicWLSWorld49;
  G4VPhysicalVolume* physiWLSWorld49;
    G4Box* solidWLSWorld50;
  G4LogicalVolume* logicWLSWorld50;
  G4VPhysicalVolume* physiWLSWorld50;
    G4Box* solidWLSWorld51;
  G4LogicalVolume* logicWLSWorld51;
  G4VPhysicalVolume* physiWLSWorld51;
    G4Box* solidWLSWorld52;
  G4LogicalVolume* logicWLSWorld52;
  G4VPhysicalVolume* physiWLSWorld52;
    G4Box* solidWLSWorld53;
  G4LogicalVolume* logicWLSWorld53;
  G4VPhysicalVolume* physiWLSWorld53;
    G4Box* solidWLSWorld54;
  G4LogicalVolume* logicWLSWorld54;
  G4VPhysicalVolume* physiWLSWorld54;
    G4Box* solidWLSWorld55;
  G4LogicalVolume* logicWLSWorld55;
  G4VPhysicalVolume* physiWLSWorld55;
    G4Box* solidWLSWorld56;
  G4LogicalVolume* logicWLSWorld56;
  G4VPhysicalVolume* physiWLSWorld56;
    G4Box* solidWLSWorld57;
  G4LogicalVolume* logicWLSWorld57;
  G4VPhysicalVolume* physiWLSWorld57;
    G4Box* solidWLSWorld58;
  G4LogicalVolume* logicWLSWorld58;
  G4VPhysicalVolume* physiWLSWorld58;
    G4Box* solidWLSWorld59;
  G4LogicalVolume* logicWLSWorld59;
  G4VPhysicalVolume* physiWLSWorld59;
    G4Box* solidWLSWorld60;
  G4LogicalVolume* logicWLSWorld60;
  G4VPhysicalVolume* physiWLSWorld60;
    G4Box* solidWLSWorld61;
  G4LogicalVolume* logicWLSWorld61;
  G4VPhysicalVolume* physiWLSWorld61;
    G4Box* solidWLSWorld62;
  G4LogicalVolume* logicWLSWorld62;
  G4VPhysicalVolume* physiWLSWorld62;
    G4Box* solidWLSWorld63;
  G4LogicalVolume* logicWLSWorld63;
  G4VPhysicalVolume* physiWLSWorld63;
    G4Box* solidWLSWorld64;
  G4LogicalVolume* logicWLSWorld64;
  G4VPhysicalVolume* physiWLSWorld64;






//////RPD WORLD///////////////
  G4Box* solidRPDWorld;
  G4LogicalVolume* logicRPDWorld;
  G4VPhysicalVolume* physiRPDWorld;

G4VSolid* solidAlsub;
G4VSolid* solidRPD1sub1;
G4VSolid* solidRPD2sub1;
G4VSolid* solidRPD3sub1;
G4VSolid* solidRPD4sub1;
G4VSolid* solidRPD5sub1;
G4VSolid* solidRPD6sub1;
G4VSolid* solidRPD7sub1;
G4VSolid* solidRPD8sub1;
G4VSolid* solidRPD9sub1;
G4VSolid* solidRPD10sub1;
G4VSolid* solidRPD11sub1;
G4VSolid* solidRPD12sub1;
G4VSolid* solidRPD13sub1;
G4VSolid* solidRPD14sub1;
G4VSolid* solidRPD15sub1;
G4VSolid* solidRPD16sub1;


G4Box* solidRPD;
G4LogicalVolume* logicRPD;
G4VPhysicalVolume* physiRPD;
////////////////////////////////////////
////////////////////RPD1//////////////////
G4Box* solidRPD1;
G4LogicalVolume* logicRPD1;
G4VPhysicalVolume* physiRPD1;
////////////////////////////////////////
////////////////////RPD2//////////////////
  G4Box* solidRPD2;
  G4LogicalVolume* logicRPD2;
  G4VPhysicalVolume* physiRPD2;
////////////////////////////////////////
////////////////////RPD3//////////////////
  G4Box* solidRPD3;
  G4LogicalVolume* logicRPD3;
  G4VPhysicalVolume* physiRPD3;
////////////////////////////////////////
////////////////////RPD4//////////////////
  G4Box* solidRPD4;
  G4LogicalVolume* logicRPD4;
  G4VPhysicalVolume* physiRPD4;
////////////////////////////////////////
////////////////////RPD5//////////////////
G4Box* solidRPD5;
G4LogicalVolume* logicRPD5;
G4VPhysicalVolume* physiRPD5;
////////////////////////////////////////
////////////////////RPD6//////////////////
G4Box* solidRPD6;
G4LogicalVolume* logicRPD6;
G4VPhysicalVolume* physiRPD6;
////////////////////////////////////////
////////////////////RPD7//////////////////
G4Box* solidRPD7;
G4LogicalVolume* logicRPD7;
G4VPhysicalVolume* physiRPD7;
////////////////////////////////////////
////////////////////RPD8//////////////////
G4Box* solidRPD8;
G4LogicalVolume* logicRPD8;
G4VPhysicalVolume* physiRPD8;
////////////////////////////////////////
////////////////////RPD9//////////////////
G4Box* solidRPD9;
G4LogicalVolume* logicRPD9;
G4VPhysicalVolume* physiRPD9;
////////////////////////////////////////
////////////////////RPD10//////////////////
G4Box* solidRPD10;
G4LogicalVolume* logicRPD10;
G4VPhysicalVolume* physiRPD10;
////////////////////////////////////////
////////////////////RPD11//////////////////
G4Box* solidRPD11;
G4LogicalVolume* logicRPD11;
G4VPhysicalVolume* physiRPD11;
////////////////////////////////////////
////////////////////RPD12//////////////////
G4Box* solidRPD12;
G4LogicalVolume* logicRPD12;
G4VPhysicalVolume* physiRPD12;
////////////////////////////////////////
////////////////////RPD13//////////////////
G4Box* solidRPD13;
G4LogicalVolume* logicRPD13;
G4VPhysicalVolume* physiRPD13;
////////////////////////////////////////
////////////////////RPD14//////////////////
G4Box* solidRPD14;
G4LogicalVolume* logicRPD14;
G4VPhysicalVolume* physiRPD14;
////////////////////////////////////////
////////////////////RPD15//////////////////
G4Box* solidRPD15;
G4LogicalVolume* logicRPD15;
G4VPhysicalVolume* physiRPD15;
////////////////////////////////////////
////////////////////RPD16//////////////////
G4Box* solidRPD16;
G4LogicalVolume* logicRPD16;
G4VPhysicalVolume* physiRPD16;

G4LogicalVolume   *logicClad1;
G4VPhysicalVolume *physiClad1;
G4LogicalVolume   *logicClad2;
G4VPhysicalVolume *physiClad2;
G4LogicalVolume   *logicClad3;
G4VPhysicalVolume *physiClad3;
G4LogicalVolume   *logicClad4;
G4VPhysicalVolume *physiClad4;
G4LogicalVolume   *logicClad5;
G4VPhysicalVolume *physiClad5;
G4LogicalVolume   *logicClad6;
G4VPhysicalVolume *physiClad6;
G4LogicalVolume   *logicClad7;
G4VPhysicalVolume *physiClad7;
G4LogicalVolume   *logicClad8;
G4VPhysicalVolume *physiClad8;
G4LogicalVolume   *logicClad9;
G4VPhysicalVolume *physiClad9;
G4LogicalVolume   *logicClad10;
G4VPhysicalVolume *physiClad10;
G4LogicalVolume   *logicClad11;
G4VPhysicalVolume *physiClad11;
G4LogicalVolume   *logicClad12;
G4VPhysicalVolume *physiClad12;
G4LogicalVolume   *logicClad13;
G4VPhysicalVolume *physiClad13;
G4LogicalVolume   *logicClad14;
G4VPhysicalVolume *physiClad14;
G4LogicalVolume   *logicClad15;
G4VPhysicalVolume *physiClad15;
G4LogicalVolume   *logicClad16;
G4VPhysicalVolume *physiClad16;
G4LogicalVolume   *logicClad17;
G4VPhysicalVolume *physiClad17;
G4LogicalVolume   *logicClad18;
G4VPhysicalVolume *physiClad18;
G4LogicalVolume   *logicClad19;
G4VPhysicalVolume *physiClad19;
G4LogicalVolume   *logicClad20;
G4VPhysicalVolume *physiClad20;
G4LogicalVolume   *logicClad21;
G4VPhysicalVolume *physiClad21;
G4LogicalVolume   *logicClad22;
G4VPhysicalVolume *physiClad22;
G4LogicalVolume   *logicClad23;
G4VPhysicalVolume *physiClad23;
G4LogicalVolume   *logicClad24;
G4VPhysicalVolume *physiClad24;
G4LogicalVolume   *logicClad25;
G4VPhysicalVolume *physiClad25;
G4LogicalVolume   *logicClad26;
G4VPhysicalVolume *physiClad26;
G4LogicalVolume   *logicClad27;
G4VPhysicalVolume *physiClad27;
G4LogicalVolume   *logicClad28;
G4VPhysicalVolume *physiClad28;
G4LogicalVolume   *logicClad29;
G4VPhysicalVolume *physiClad29;
G4LogicalVolume   *logicClad30;
G4VPhysicalVolume *physiClad30;
G4LogicalVolume   *logicClad31;
G4VPhysicalVolume *physiClad31;
G4LogicalVolume   *logicClad32;
G4VPhysicalVolume *physiClad32;
G4LogicalVolume   *logicClad33;
G4VPhysicalVolume *physiClad33;
G4LogicalVolume   *logicClad34;
G4VPhysicalVolume *physiClad34;
G4LogicalVolume   *logicClad35;
G4VPhysicalVolume *physiClad35;
G4LogicalVolume   *logicClad36;
G4VPhysicalVolume *physiClad36;
G4LogicalVolume   *logicClad37;
G4VPhysicalVolume *physiClad37;
G4LogicalVolume   *logicClad38;
G4VPhysicalVolume *physiClad38;
G4LogicalVolume   *logicClad39;
G4VPhysicalVolume *physiClad39;
G4LogicalVolume   *logicClad40;
G4VPhysicalVolume *physiClad40;
G4LogicalVolume   *logicClad41;
G4VPhysicalVolume *physiClad41;
G4LogicalVolume   *logicClad42;
G4VPhysicalVolume *physiClad42;
G4LogicalVolume   *logicClad43;
G4VPhysicalVolume *physiClad43;
G4LogicalVolume   *logicClad44;
G4VPhysicalVolume *physiClad44;
G4LogicalVolume   *logicClad45;
G4VPhysicalVolume *physiClad45;
G4LogicalVolume   *logicClad46;
G4VPhysicalVolume *physiClad46;
G4LogicalVolume   *logicClad47;
G4VPhysicalVolume *physiClad47;
G4LogicalVolume   *logicClad48;
G4VPhysicalVolume *physiClad48;
G4LogicalVolume   *logicClad49;
G4VPhysicalVolume *physiClad49;
G4LogicalVolume   *logicClad50;
G4VPhysicalVolume *physiClad50;
G4LogicalVolume   *logicClad51;
G4VPhysicalVolume *physiClad51;
G4LogicalVolume   *logicClad52;
G4VPhysicalVolume *physiClad52;
G4LogicalVolume   *logicClad53;
G4VPhysicalVolume *physiClad53;
G4LogicalVolume   *logicClad54;
G4VPhysicalVolume *physiClad54;
G4LogicalVolume   *logicClad55;
G4VPhysicalVolume *physiClad55;
G4LogicalVolume   *logicClad56;
G4VPhysicalVolume *physiClad56;
G4LogicalVolume   *logicClad57;
G4VPhysicalVolume *physiClad57;
G4LogicalVolume   *logicClad58;
G4VPhysicalVolume *physiClad58;
G4LogicalVolume   *logicClad59;
G4VPhysicalVolume *physiClad59;
G4LogicalVolume   *logicClad60;
G4VPhysicalVolume *physiClad60;
G4LogicalVolume   *logicClad61;
G4VPhysicalVolume *physiClad61;
G4LogicalVolume   *logicClad62;
G4VPhysicalVolume *physiClad62;
G4LogicalVolume   *logicClad63;
G4VPhysicalVolume *physiClad63;
G4LogicalVolume   *logicClad64;
G4VPhysicalVolume *physiClad64;

G4LogicalVolume   *logicCladAl1;
G4VPhysicalVolume *physiCladAl1;
G4LogicalVolume   *logicCladAl2;
G4VPhysicalVolume *physiCladAl2;
G4LogicalVolume   *logicCladAl3;
G4VPhysicalVolume *physiCladAl3;
G4LogicalVolume   *logicCladAl4;
G4VPhysicalVolume *physiCladAl4;
G4LogicalVolume   *logicCladAl5;
G4VPhysicalVolume *physiCladAl5;
G4LogicalVolume   *logicCladAl6;
G4VPhysicalVolume *physiCladAl6;
G4LogicalVolume   *logicCladAl7;
G4VPhysicalVolume *physiCladAl7;
G4LogicalVolume   *logicCladAl8;
G4VPhysicalVolume *physiCladAl8;
G4LogicalVolume   *logicCladAl9;
G4VPhysicalVolume *physiCladAl9;
G4LogicalVolume   *logicCladAl10;
G4VPhysicalVolume *physiCladAl10;
G4LogicalVolume   *logicCladAl11;
G4VPhysicalVolume *physiCladAl11;
G4LogicalVolume   *logicCladAl12;
G4VPhysicalVolume *physiCladAl12;
G4LogicalVolume   *logicCladAl13;
G4VPhysicalVolume *physiCladAl13;
G4LogicalVolume   *logicCladAl14;
G4VPhysicalVolume *physiCladAl14;
G4LogicalVolume   *logicCladAl15;
G4VPhysicalVolume *physiCladAl15;
G4LogicalVolume   *logicCladAl16;
G4VPhysicalVolume *physiCladAl16;
G4LogicalVolume   *logicCladAl17;
G4VPhysicalVolume *physiCladAl17;
G4LogicalVolume   *logicCladAl18;
G4VPhysicalVolume *physiCladAl18;
G4LogicalVolume   *logicCladAl19;
G4VPhysicalVolume *physiCladAl19;
G4LogicalVolume   *logicCladAl20;
G4VPhysicalVolume *physiCladAl20;
G4LogicalVolume   *logicCladAl21;
G4VPhysicalVolume *physiCladAl21;
G4LogicalVolume   *logicCladAl22;
G4VPhysicalVolume *physiCladAl22;
G4LogicalVolume   *logicCladAl23;
G4VPhysicalVolume *physiCladAl23;
G4LogicalVolume   *logicCladAl24;
G4VPhysicalVolume *physiCladAl24;
G4LogicalVolume   *logicCladAl25;
G4VPhysicalVolume *physiCladAl25;
G4LogicalVolume   *logicCladAl26;
G4VPhysicalVolume *physiCladAl26;
G4LogicalVolume   *logicCladAl27;
G4VPhysicalVolume *physiCladAl27;
G4LogicalVolume   *logicCladAl28;
G4VPhysicalVolume *physiCladAl28;
G4LogicalVolume   *logicCladAl29;
G4VPhysicalVolume *physiCladAl29;
G4LogicalVolume   *logicCladAl30;
G4VPhysicalVolume *physiCladAl30;
G4LogicalVolume   *logicCladAl31;
G4VPhysicalVolume *physiCladAl31;
G4LogicalVolume   *logicCladAl32;
G4VPhysicalVolume *physiCladAl32;
G4LogicalVolume   *logicCladAl33;
G4VPhysicalVolume *physiCladAl33;
G4LogicalVolume   *logicCladAl34;
G4VPhysicalVolume *physiCladAl34;
G4LogicalVolume   *logicCladAl35;
G4VPhysicalVolume *physiCladAl35;
G4LogicalVolume   *logicCladAl36;
G4VPhysicalVolume *physiCladAl36;
G4LogicalVolume   *logicCladAl37;
G4VPhysicalVolume *physiCladAl37;
G4LogicalVolume   *logicCladAl38;
G4VPhysicalVolume *physiCladAl38;
G4LogicalVolume   *logicCladAl39;
G4VPhysicalVolume *physiCladAl39;
G4LogicalVolume   *logicCladAl40;
G4VPhysicalVolume *physiCladAl40;
G4LogicalVolume   *logicCladAl41;
G4VPhysicalVolume *physiCladAl41;
G4LogicalVolume   *logicCladAl42;
G4VPhysicalVolume *physiCladAl42;
G4LogicalVolume   *logicCladAl43;
G4VPhysicalVolume *physiCladAl43;
G4LogicalVolume   *logicCladAl44;
G4VPhysicalVolume *physiCladAl44;
G4LogicalVolume   *logicCladAl45;
G4VPhysicalVolume *physiCladAl45;
G4LogicalVolume   *logicCladAl46;
G4VPhysicalVolume *physiCladAl46;
G4LogicalVolume   *logicCladAl47;
G4VPhysicalVolume *physiCladAl47;
G4LogicalVolume   *logicCladAl48;
G4VPhysicalVolume *physiCladAl48;
G4LogicalVolume   *logicCladAl49;
G4VPhysicalVolume *physiCladAl49;
G4LogicalVolume   *logicCladAl50;
G4VPhysicalVolume *physiCladAl50;
G4LogicalVolume   *logicCladAl51;
G4VPhysicalVolume *physiCladAl51;
G4LogicalVolume   *logicCladAl52;
G4VPhysicalVolume *physiCladAl52;
G4LogicalVolume   *logicCladAl53;
G4VPhysicalVolume *physiCladAl53;
G4LogicalVolume   *logicCladAl54;
G4VPhysicalVolume *physiCladAl54;
G4LogicalVolume   *logicCladAl55;
G4VPhysicalVolume *physiCladAl55;
G4LogicalVolume   *logicCladAl56;
G4VPhysicalVolume *physiCladAl56;
G4LogicalVolume   *logicCladAl57;
G4VPhysicalVolume *physiCladAl57;
G4LogicalVolume   *logicCladAl58;
G4VPhysicalVolume *physiCladAl58;
G4LogicalVolume   *logicCladAl59;
G4VPhysicalVolume *physiCladAl59;
G4LogicalVolume   *logicCladAl60;
G4VPhysicalVolume *physiCladAl60;
G4LogicalVolume   *logicCladAl61;
G4VPhysicalVolume *physiCladAl61;
G4LogicalVolume   *logicCladAl62;
G4VPhysicalVolume *physiCladAl62;
G4LogicalVolume   *logicCladAl63;
G4VPhysicalVolume *physiCladAl63;
G4LogicalVolume   *logicCladAl64;
G4VPhysicalVolume *physiCladAl64;

G4VSolid* solidPhotonDet1;
G4VSolid* solidPhotonDet2;
G4VSolid* solidPhotonDet3;
G4VSolid* solidPhotonDet4;
G4VSolid* solidPhotonDet5;
G4VSolid* solidPhotonDet6;
G4VSolid* solidPhotonDet7;
G4VSolid* solidPhotonDet8;
G4VSolid* solidPhotonDet9;
G4VSolid* solidPhotonDet10;
G4VSolid* solidPhotonDet11;
G4VSolid* solidPhotonDet12;
G4VSolid* solidPhotonDet13;
G4VSolid* solidPhotonDet14;
G4VSolid* solidPhotonDet15;
G4VSolid* solidPhotonDet16;

  G4Material*         RPD_Quartz;   //Hopefully this works
  G4Material*         RPD_Scil;  //Hopefully this works too
  G4Material*         WLS_MAT;  //Hopefully this works three
  G4Material*         RPD_Material;
  G4Material*         Cu_Material;  
  G4Material*         EM_Absorber;
  G4Material*         EM_Fiber;
  
  G4UserLimits* stepLimit;             // pointer to user step limits
  
  ExN02DetectorMessenger* detectorMessenger;  // pointer to the Messenger
  CalorimeterSD* aCalorimeterSD;  
  
  G4double fWorldLength;            // Full length of the world volume
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
