#include "ExN02EmCalorimeter.hh"
#include "ExN02EmCalorimeterHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4ios.hh"

ExN02EmCalorimeter::ExN02EmCalorimeter(G4String name)
    : G4VSensitiveDetector(name) {
  G4String HCname;
  collectionName.insert(HCname = "EMcalorimeterColl");
  HCID = -1;
}

ExN02EmCalorimeter::~ExN02EmCalorimeter() { ; }

void ExN02EmCalorimeter::Initialize(G4HCofThisEvent* HCE) {
  hitsCollection = new ExN02EmCalorimeterHitsCollection(SensitiveDetectorName,
                                                        collectionName[0]);
  if (HCID < 0) {
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection);
  }
  HCE->AddHitsCollection(HCID, hitsCollection);

  // fill calorimeter hits with zero energy deposition
  for (int i = 0; i < 80; i++) {
    ExN02EmCalorimeterHit* aHit = new ExN02EmCalorimeterHit(i);
    hitsCollection->insert(aHit);
  }
}

G4bool ExN02EmCalorimeter::ProcessHits(G4Step* aStep,
                                       G4TouchableHistory* /*ROhist*/) {
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep == 0.) return true;

  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4TouchableHistory* theTouchable =
      (G4TouchableHistory*)(preStepPoint->GetTouchable());
  G4VPhysicalVolume* thePhysical = theTouchable->GetVolume();
  G4int copyNo = thePhysical->GetCopyNo();

  ExN02EmCalorimeterHit* aHit = (*hitsCollection)[copyNo];
  // check if it is first touch
  if (!(aHit->GetLogV())) {
    // fill volume information
    aHit->SetLogV(thePhysical->GetLogicalVolume());
    G4AffineTransform aTrans = theTouchable->GetHistory()->GetTopTransform();
    aTrans.Invert();
    aHit->SetRot(aTrans.NetRotation());
    aHit->SetPos(aTrans.NetTranslation());
  }
  // add energy deposition
  aHit->AddEdep(edep);

  return true;
}

void ExN02EmCalorimeter::EndOfEvent(G4HCofThisEvent* /*HCE*/) { ; }
