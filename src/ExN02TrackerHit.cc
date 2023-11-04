#include "ExN02TrackerHit.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"

G4Allocator<ExN02TrackerHit> ExN02TrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerHit::ExN02TrackerHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerHit::~ExN02TrackerHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02TrackerHit::ExN02TrackerHit(const ExN02TrackerHit& right) : G4VHit() {
  trackID = right.trackID;
  chamberNb = right.chamberNb;
  edep = right.edep;
  pos = right.pos;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const ExN02TrackerHit& ExN02TrackerHit::operator=(
    const ExN02TrackerHit& right) {
  trackID = right.trackID;
  chamberNb = right.chamberNb;
  edep = right.edep;
  pos = right.pos;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int ExN02TrackerHit::operator==(const ExN02TrackerHit& right) const {
  return (this == &right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerHit::Draw() {
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if (pVVisManager) {
    G4Circle circle(pos);
    circle.SetScreenSize(2.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1., 0., 0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02TrackerHit::Print() {
  G4cout << "  trackID: " << trackID << "  chamberNb: " << chamberNb
         << "  energy deposit: " << G4BestUnit(edep, "Energy")
         << "  position: " << G4BestUnit(pos, "Length") << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
