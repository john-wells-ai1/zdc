// $Id: SiHit.cc 100 2010-01-26 16:12:59Z adotti $
/**
 * @file
 * @brief Implementation of user class SimpleHit.
 */

#include "SimpleHit.hh"

// -- one more nasty trick for new and delete operator overloading:
G4Allocator<SimpleHit> SiHitAllocator;

SimpleHit::SimpleHit(const G4int strip, const G4int plane, const G4bool primary)
    : stripNumber(strip),
      planeNumber(plane),
      isPrimary(primary)  // <<-- note BTW this is the only way to initialize a
                          // "const" member
{
  eDep = 0.0;
}

SimpleHit::~SimpleHit() {}

void SimpleHit::Print() {
  G4cout << "Hit: Plane= " << planeNumber << " adsorber= " << stripNumber
         << " E= " << eDep / MeV
         << " MeV isPrimary=" << (isPrimary ? "true" : "false") << G4endl;
  G4cout << "     Pos=" << position << " ( " << position.perp() << ")"
         << G4endl;
}
