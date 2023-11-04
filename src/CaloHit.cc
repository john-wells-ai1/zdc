// $Id: CaloHit.cc 100 2010-01-26 16:12:59Z adotti $
/**
 * @file
 * @brief Implementation of user class CaloHit.
 */

#include "CaloHit.hh"
#include "G4UnitsTable.hh"
// -- one more nasty trick for new and delete operator overloading:
G4Allocator<CaloHit> CaloHitAllocator;

CaloHit::CaloHit(G4int z) {
  cellID = z;
  eDep = 0.;
  fPos = G4ThreeVector(0., 0., 0.);
}
CaloHit::CaloHit() {
  cellID = -1;
  eDep = 0.;
  fPos = G4ThreeVector(0., 0., 0.);
}
CaloHit::~CaloHit() {}

void CaloHit::Print() {
  //---------------------
  // Exercise 1 of task 4c
  //---------------------
  // Add some print out on screen: layer number and energy deposited
  //	G4cout<<"CaloHit::Print: edep: "<<edep<<G4endl;
  // need this as private in CaloHit.hh:
  //  private:
  G4double edep;
  G4cout << "CaloHit: cellID=" << cellID
         << " Edep=" << G4BestUnit(eDep, "Energy, ")
         << " Position: " << G4BestUnit(fPos, "Length") << G4endl;
}
