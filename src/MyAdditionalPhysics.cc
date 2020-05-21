
#include "globals.hh"
#include "MyAdditionalPhysics.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsLHEP.hh"
#include "G4HadronQElasticPhysics.hh"
#include "G4ChargeExchangePhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4NeutronCrossSectionXS.hh"
#include "G4QStoppingPhysics.hh"
#include "G4LHEPStoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4IonPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmProcessOptions.hh"

#include "HadronPhysicsFTFP_BERT.hh"
#include "HadronPhysicsFTF_BIC.hh"
#include "HadronPhysicsLHEP.hh"
#include "HadronPhysicsLHEP_EMV.hh"
#include "G4HadronInelasticQBBC.hh"
#include "HadronPhysicsQGSC_BERT.hh"
#include "HadronPhysicsQGSP.hh"
#include "HadronPhysicsQGSP_BERT.hh"
#include "HadronPhysicsQGSP_BERT_HP.hh"
#include "HadronPhysicsQGSP_BIC.hh"
#include "HadronPhysicsQGSP_BIC_HP.hh"
#include "HadronPhysicsQGSP_FTFP_BERT.hh"
#include "HadronPhysicsQGS_BIC.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4Cerenkov.hh"
#include "G4OpWLS.hh"

#include "G4IonPhysics.hh"

#include "G4LossTableManager.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"


#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//MyAdditionalPhysics::MyAdditionalPhysics():  G4VUserPhysicsList()
MyAdditionalPhysics::MyAdditionalPhysics(G4String aS): G4VPhysicsConstructor(aS)
{
   theCerenkovProcess           = NULL;
   theAbsorptionProcess         = NULL;
   theWLSProcess                = NULL;
   theAbsorptionProcess         = NULL;
  theBoundaryProcess           = NULL;

//  AbsorptionOn                 = toggle;
  
   SetVerboseLevel(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//MyAdditionalPhysics::~MyAdditionalPhysics()
//{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MyAdditionalPhysics::ConstructParticle()
{
  G4OpticalPhoton::OpticalPhotonDefinition();
  ConstructBosons();
}




void MyAdditionalPhysics::ConstructBosons()
{
  //define optical photons
  G4OpticalPhoton::OpticalPhotonDefinition();
}



void MyAdditionalPhysics::ConstructProcess()
{
  theWLSProcess = new G4OpWLS("OpWLS");
  G4OpAbsorption* theAbsorptionProcess;
  G4OpBoundaryProcess* theBoundaryProcess;

  theAbsorptionProcess=new G4OpAbsorption();

  theBoundaryProcess=new G4OpBoundaryProcess();
  theBoundaryProcess->SetModel(unified);

  G4ProcessManager * pManager = 0;

  pManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

  pManager->AddDiscreteProcess(theAbsorptionProcess);

  theBoundaryProcess->SetModel(glisur);
  pManager->AddDiscreteProcess(theBoundaryProcess);

  theWLSProcess->UseTimeProfile("delta");
  pManager->AddDiscreteProcess(theWLSProcess);

  G4Cerenkov*   theCerenkovProcess = new G4Cerenkov("Cerenkov");  

  G4int MaxNumPhotons = 300;
  theCerenkovProcess->SetTrackSecondariesFirst(true);		//true
  theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumPhotons);  


  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if (theCerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
    }
  }


  
  
  
  
}

 // G4int MaxNumPhotons = 300;
 

//void MyAdditionalPhysics::SetNbOfPhotonsCerenkov(G4int MaxNumber)
//{
//  theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumber);
//}

