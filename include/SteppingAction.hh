//
//*********************************************************************
//*********************************************************************

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class ExN02DetectorConstruction;
class ExN02EventAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(ExN02DetectorConstruction*, ExN02EventAction*, HistoManager*);
  virtual ~SteppingAction();

  void UserSteppingAction(const G4Step*);

  G4double getAttenuation(const G4Step*, G4double, G4double, G4double);
  G4double getBirkL3(const G4Step*, G4double, G4double, G4double);

private:
  ExN02DetectorConstruction* detector;
  ExN02EventAction*          eventaction;  
  HistoManager*         histo;
  int                 oldEvtNumber;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
