
#include "ExN02PrimaryGeneratorAction.hh"
#include "ExN02DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
//#include "WLSDetectorConstruction.hh" //accounted for in ExN02Detec...
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
//#include "WLSPrimaryGeneratorMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PrimaryGeneratorAction::ExN02PrimaryGeneratorAction(
                                               ExN02DetectorConstruction* myDC)
:myDetector(myDC)
{
  G4int n_particle = 1;

  particleGun = new G4GeneralParticleSource();

// default particle

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("neutron");
  
  particleGun->SetParticleDefinition(particle);

}


///////added the following section for WLS
/*WLSPrimaryGeneratorAction::
                         WLSPrimaryGeneratorAction(ExN02DetectorConstruction* DC)
{
  detector = DC;
  theIntegralTable = NULL;

  particleGun = new G4GeneralParticleSource();
 
  gunMessenger = new WLSPrimaryGeneratorMessenger(this);

  // G4String particleName;
  // G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  timeConstant = 0.;

//  particleGun->SetParticleDefinition(particleTable->
//                               FindParticle(particleName="opticalphoton"));
} */

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PrimaryGeneratorAction::~ExN02PrimaryGeneratorAction()
{
  delete particleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 

  particleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

