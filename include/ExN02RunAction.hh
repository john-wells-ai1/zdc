#ifndef ExN02RunAction_h
#define ExN02RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class ExN02DetectorConstruction;
class ExN02PrimaryGeneratorAction;
class HistoManager;

class G4Run;

class ExN02RunAction : public G4UserRunAction
{
public:
  ExN02RunAction(ExN02DetectorConstruction*, ExN02PrimaryGeneratorAction*, HistoManager*);
  ~ExN02RunAction();

public:
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
  void FillPerEvent(G4int, G4int, G4int, G4int,G4int,G4int,G4int,G4int,G4int,G4int,G4int,G4int,G4int,G4int,G4int,G4int);
  void FillPMTPerEvent(G4int, G4int, G4int, G4int,G4int,G4int,G4int,G4int,G4int,G4int,G4int,G4int,G4int,G4int,G4int,G4int);	
	
private:

  G4double sumEcal, sum2Ecal;
  G4double sumHcal, sum2Hcal;
  G4double sumZero, sum2Zero;
  G4int rpd1, rpd2, rpd3,rpd4,rpd5,rpd6,rpd7,rpd8,rpd9,rpd10,rpd11,rpd12,rpd13,rpd14,rpd15,rpd16;
  G4int pmt1, pmt2, pmt3,pmt4,pmt5,pmt6,pmt7,pmt8,pmt9,pmt10,pmt11,pmt12,pmt13,pmt14,pmt15,pmt16;

  ExN02DetectorConstruction*   Det;
  ExN02PrimaryGeneratorAction* Kin;
  HistoManager*           myana;
  HistoManager*           mypmtana;  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif





