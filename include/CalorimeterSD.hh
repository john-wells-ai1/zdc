// $Id: SensitiveDetector.hh 22 2009-12-22 12:36:46Z schaelic $

#ifndef CalorimeterSD_h
#define CalorimeterSD_h 1

/**
 * @file
 * @brief Defines SensitiveDetector class.
 */

#include "G4VSensitiveDetector.hh"
//class ExN02DetectorConstruction;
//class ExN02RunAction;

class G4Step;
#include "CaloHit.hh"              // <<- the hit "format" we define
class G4HCofThisEvent;           // <<- means "H(it) C(ollections) of This Event"
class G4TouchableHistory;
class HistoManager;

/*!
 * \brief Defines sensitve part of detector geometry.
 *
 * Stores Hits with 
 *  * deposited energy
 *  * position
 * in <i>Hit Collections of This Event</i>
 *
 * /sa ProcessHits()
 */
class CalorimeterSD : public G4VSensitiveDetector
{
public:
  /// Constructor
  CalorimeterSD(G4String SDname);
  /// Destructor
  ~CalorimeterSD();

public:
  /// @name methods from base class G4VSensitiveDetector
  //@{
  /// Mandatory base class method : it must to be overloaded:
   G4bool ProcessHits(G4Step *step, G4TouchableHistory *ROhist);

  /// (optional) method of base class G4VSensitiveDetector
  void Initialize(G4HCofThisEvent* HCE);
  /// (optional) method of base class G4VSensitiveDetector
  void EndOfEvent(G4HCofThisEvent* HCE);
  //@}


private:
  typedef std::map<G4int,CaloHit*> hitMap_t; //< Helper mapping layer number with hit
  hitMap_t hitMap;
  CaloHitCollection* hitCollection;          //< Collection of calorimetric hits
  G4int HCID;
  G4String det[16];
  G4String PhotonDet[16];
  
};

#endif
