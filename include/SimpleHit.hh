// $Id: SimpleHit.hh 100 2010-01-26 16:12:59Z adotti $
#ifndef SimpleHit_h
#define SimpleHit_h 1

/**
 * @file
 * @brief Define user class SimpleHit.
 *
 * We define "our" hit format : it is caracterized by its plane and strip
 * numbers, and an energy value, the accumulated energy in this strip
 */

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"

/*!
 * \brief This class stores information of a hit.
 *
 * It contains
 *  - strip and plane number
 *  - deposited energy
 *  - position information
 */
class SimpleHit : public G4VHit {
public:
  /// Constructor
  SimpleHit(const G4int strip, const G4int plane, const G4bool isPrimary);
  /// Destructor
  ~SimpleHit();
  //! Print on screen a Hit
  void Print();
  
public:
  //! \name The new and delete operators are overloaded for performances reasons:
  /*! -- Tricky business here... :-(, but provided for you below */
  //@{
  inline void *operator    new(size_t);
  inline void  operator delete(void *aHit);
  //@}

public:
  //! \name  simple set and get methods
  //@{
  void          AddEdep(const double e)                { eDep += e; }
  void          SetPosition(const G4ThreeVector & pos) { position = pos; }

  G4double      GetEdep()        const { return eDep;}
  G4ThreeVector GetPosition()    const { return position; }
  G4int         GetAdsorberNumber() const { return stripNumber; }
  G4int         GetPlaneNumber() const { return planeNumber; }
  G4bool	    GetIsPrimary()   const { return isPrimary; }
  //@}

private:
  const G4int   stripNumber, planeNumber;
  G4double      eDep;
  G4ThreeVector position;
  const G4bool  isPrimary;
};

// Define the "hit collection" using the template class G4THitsCollection:
typedef G4THitsCollection<SimpleHit> SDHitCollection;


// -- new and delete overloaded operators:

extern G4Allocator<SimpleHit> SiHitAllocator;

inline void* SimpleHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) SiHitAllocator.MallocSingle();
  return aHit;
}
inline void SimpleHit::operator delete(void *aHit)
{
  SiHitAllocator.FreeSingle((SimpleHit*) aHit);
}

#endif
