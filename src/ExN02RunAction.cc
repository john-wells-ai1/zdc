#include "ExN02RunAction.hh"
#include "ExN02EventAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include "ExN02DetectorConstruction.hh"
#include "ExN02PrimaryGeneratorAction.hh"
#include "HistoManager.hh"
#include "G4SDManager.hh"
//#include "Analysis.hh"

//#include "ExN02HistoManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02RunAction::ExN02RunAction(ExN02DetectorConstruction* det, 
			       ExN02PrimaryGeneratorAction* kin,
			       HistoManager* histo):Det(det),Kin(kin), myana(histo)
{
//  ana = Analysis::GetInstance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02RunAction::~ExN02RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RunAction::BeginOfRunAction(const G4Run* aRun)
{
  rpd1=0;
  rpd2=0;
  rpd3=0;
  rpd4=0;
  rpd5=0;
  rpd6=0;
  rpd7=0;
  rpd8=0;
  rpd9=0;
  rpd10=0;
  rpd11=0;
  rpd12=0;
  rpd13=0;
  rpd14=0;
  rpd15=0;
  rpd16=0;

  pmt1=0;
  pmt2=0;
  pmt3=0;
  pmt4=0;
  pmt5=0;
  pmt6=0;
  pmt7=0;
  pmt8=0;
  pmt9=0;
  pmt10=0;
  pmt11=0;
  pmt12=0;
  pmt13=0;
  pmt14=0;
  pmt15=0;
  pmt16=0;

  myana->Clear();  
//  G4int JobRunNumber = myana->GetJobRunNumber();
//  ((G4Run *)(aRun))->SetRunID(JobRunNumber);
//  G4cout << "### Job Run # " << JobRunNumber << " start." << G4endl;
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
    G4double Ekin = Kin->GetParticleGun()->GetParticleEnergy();
//  G4double Ekin = Kin->GetParticleGun()->GetParticleEnergy();
  G4cout << "*** Particle gun energy: " << Ekin << G4endl;
//  
//   G4SDManager* fSDM = G4SDManager::GetSDMpointer();
//   G4RunManager* fRM = G4RunManager::GetRunManager();
//   G4int collectionID1 = fSDM->GetCollectionID("RPD1");
//   const G4Event* currentEvent = fRM->GetCurrentEvent();
//   G4HCofThisEvent* HCofEvent = currentEvent->GetHCofThisEvent();
//   SDHitCollection* myCollection = (SDHitCollection*)(HC0fEvent->GetHC
//(collectionID1));
  myana->Book(Ekin);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RunAction::FillPerEvent(G4int nhitrpd1, G4int nhitrpd2,G4int
 nhitrpd3, G4int nhitrpd4, G4int nhitrpd5,G4int nhitrpd6,G4int
 nhitrpd7,G4int nhitrpd8,G4int nhitrpd9,G4int nhitrpd10, G4int
 nhitrpd11,G4int nhitrpd12,G4int nhitrpd13, G4int nhitrpd14,G4int
 nhitrpd15,G4int nhitrpd16)
{
  rpd1+=nhitrpd1;
  rpd2+=nhitrpd2;
  rpd3+=nhitrpd3;
  rpd4+=nhitrpd4;
  rpd5+=nhitrpd5;
  rpd6+=nhitrpd6;
  rpd7+=nhitrpd7;
  rpd8+=nhitrpd8;
  rpd9+=nhitrpd9;
  rpd10+=nhitrpd10;
  rpd11+=nhitrpd11;
  rpd12+=nhitrpd12;
  rpd13+=nhitrpd13;
  rpd14+=nhitrpd14;
  rpd15+=nhitrpd15;
  rpd16+=nhitrpd16;

}

void ExN02RunAction::FillPMTPerEvent(G4int nhitpmt1, G4int nhitpmt2,G4int
 nhitpmt3, G4int nhitpmt4, G4int nhitpmt5,G4int nhitpmt6,G4int
 nhitpmt7,G4int nhitpmt8,G4int nhitpmt9,G4int nhitpmt10, G4int
 nhitpmt11,G4int nhitpmt12,G4int nhitpmt13, G4int nhitpmt14,G4int
 nhitpmt15,G4int nhitpmt16)
{
  pmt1+=nhitpmt1;
  pmt2+=nhitpmt2;
  pmt3+=nhitpmt3;
  pmt4+=nhitpmt4;
  pmt5+=nhitpmt5;
  pmt6+=nhitpmt6;
  pmt7+=nhitpmt7;
  pmt8+=nhitpmt8;
  pmt9+=nhitpmt9;
  pmt10+=nhitpmt10;
  pmt11+=nhitpmt11;
  pmt12+=nhitpmt12;
  pmt13+=nhitpmt13;
  pmt14+=nhitpmt14;
  pmt15+=nhitpmt15;
  pmt16+=nhitpmt16;

}

void ExN02RunAction::EndOfRunAction(const G4Run* aRun) 
{ 
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  G4cout<<"RPD1 had"<<" "<<rpd1<<" "<<"hits."<<G4endl;
 // myana->FillNHitsRPD(rpd1,rpd2,rpd3,rpd4,rpd5,rpd6,rpd7,rpd8,rpd9,rpd10,rpd11,rpd12,rpd13,rpd14,rpd15,rpd16);
  
  myana->FillNHitsPMT(pmt1,pmt2,pmt3,pmt4,pmt5,pmt6,pmt7,pmt8,pmt9,pmt10,pmt11,pmt12,pmt13,pmt14,pmt15,pmt16);
//  myana->FillNHitsPMT(pmt1,pmt2,pmt3,pmt4,pmt5,pmt6,pmt7,pmt8,pmt9,pmt10,pmt11,pmt12,pmt13,pmt14,pmt15,pmt16);

  G4cout<<"pmt1 had"<<" "<<pmt1<<" "<<"hits."<<G4endl;
  G4cout<<"pmt2 had"<<" "<<pmt2<<" "<<"hits."<<G4endl;
  G4cout<<"pmt3 had"<<" "<<pmt3<<" "<<"hits."<<G4endl;
  G4cout<<"pmt4 had"<<" "<<pmt4<<" "<<"hits."<<G4endl;
  G4cout<<"pmt5 had"<<" "<<pmt5<<" "<<"hits."<<G4endl;
  G4cout<<"pmt6 had"<<" "<<pmt6<<" "<<"hits."<<G4endl;
  G4cout<<"pmt7 had"<<" "<<pmt7<<" "<<"hits."<<G4endl;
  G4cout<<"pmt8 had"<<" "<<pmt8<<" "<<"hits."<<G4endl;
  G4cout<<"pmt9 had"<<" "<<pmt9<<" "<<"hits."<<G4endl;
  G4cout<<"pmt10 had"<<" "<<pmt10<<" "<<"hits."<<G4endl;
  G4cout<<"pmt11 had"<<" "<<pmt11<<" "<<"hits."<<G4endl;
  G4cout<<"pmt12 had"<<" "<<pmt12<<" "<<"hits."<<G4endl;
  G4cout<<"pmt13 had"<<" "<<pmt13<<" "<<"hits."<<G4endl;
  G4cout<<"pmt14 had"<<" "<<pmt14<<" "<<"hits."<<G4endl;
  G4cout<<"pmt15 had"<<" "<<pmt15<<" "<<"hits."<<G4endl;
  G4cout<<"pmt16 had"<<" "<<pmt16<<" "<<"hits."<<G4endl;
  G4cout << " Processed "<< NbOfEvents << " events." <<G4endl;
  myana->Save();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



