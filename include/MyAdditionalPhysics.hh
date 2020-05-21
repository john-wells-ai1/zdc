#ifndef MyAdditionalPhysics_h
#define MyAdditionalPhysics_h

#include "G4VUserPhysicsList.hh"
#include "globals.hh"
#include "G4OpWLS.hh"
#include "G4VModularPhysicsList.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4OpAbsorption.hh"
#include "G4OpticalPhysics.hh"
#include "G4OpBoundaryProcess.hh"
class G4Cerenkov;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MyAdditionalPhysics : public G4VPhysicsConstructor
{
public:
  //  MyAdditionalPhysics(G4String aS) : G4VPhysicsConstructor(aS);
  MyAdditionalPhysics(G4String aS);

  //WLSOpticalPhysics(G4bool toggle=true);
//  virtual ~MyAdditionalPhysics();
  void ConstructParticle();
  void ConstructBosons();
  void ConstructProcess();
  G4OpWLS* GetWLSProcess() {return theWLSProcess;};
  G4Cerenkov* GetCerenkovProcess() {return theCerenkovProcess;}  
  G4OpAbsorption* GetAbsorptionProcess() {return theAbsorptionProcess;}
  G4OpBoundaryProcess* GetBoundaryProcess() { return theBoundaryProcess;}  
  void ConstructOp();
  
  void SetNbOfPhotonsCerenkov(G4int);
  

private:
  G4Cerenkov*          theCerenkovProcess;
  G4OpWLS*             theWLSProcess;
  G4OpAbsorption*      theAbsorptionProcess;
  G4OpBoundaryProcess* theBoundaryProcess;  
  G4bool AbsorptionOn;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


