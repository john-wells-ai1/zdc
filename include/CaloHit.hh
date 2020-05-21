// $Id: CaloHit.hh 100 2010-01-26 16:12:59Z adotti $
#ifndef CaloHit_h
#define CaloHit_h 1

/**
 * @file
 * @brief Define user class CaloHit.
 *
 * We define "our" hit content : the layer number and the energy deposited in the layer
 */

#include "G4VHit.hh"
#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
//#include "G4SDManager.hh"

/*!
 * \brief This class stores information of a hit.
 *
 * It contains
 *  - Hadronic calorimeter layer number
 *  - Deposited energy in layer
 */
class CaloHit : public G4VHit {
public:
  /// Constructor
//  CaloHit(const G4int layer);
  CaloHit();
  CaloHit(G4int z);
  /// Destructor
  ~CaloHit();
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
  void          AddEdep(const double e){ eDep += e; }

  G4double      GetEdep()        const { return eDep;}
//  G4int         GetLayerNumber() const { return layerNumber; }
  //@}
      inline void SetCellID(G4int z) { cellID = z; }
      inline G4int GetCellID() const { return cellID; }
       void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
       G4ThreeVector GetPos() const { return fPos; };

private:
//  const G4int   layerNumber;
  G4double      eDep;
  G4int cellID;
  G4ThreeVector fPos;
};

// Define the "hit collection" using the template class G4THitsCollection:
//typedef G4THitsCollection<CaloHit> CaloHitCollection;
typedef G4THitsCollection<CaloHit> CaloHitCollection;


// -- new and delete overloaded operators:
extern G4Allocator<CaloHit> CaloHitAllocator;

inline void* CaloHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) CaloHitAllocator.MallocSingle();
  return aHit;
}
inline void CaloHit::operator delete(void *aHit)
{
  CaloHitAllocator.FreeSingle((CaloHit*) aHit);
}

#endif
