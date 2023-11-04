#include "ExN02DetectorConstruction.hh"
#include "ExN02DetectorMessenger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02DetectorMessenger::ExN02DetectorMessenger(ExN02DetectorConstruction* myDet)
    : myDetector(myDet) {
  N02Dir = new G4UIdirectory("/N02/");
  N02Dir->SetGuidance("UI commands specific to this example.");

  detDir = new G4UIdirectory("/N02/det/");
  detDir->SetGuidance("detector control.");

  StepMaxCmd = new G4UIcmdWithADoubleAndUnit("/N02/det/stepMax", this);
  StepMaxCmd->SetGuidance("Define a step max");
  StepMaxCmd->SetParameterName("stepMax", false);
  StepMaxCmd->SetUnitCategory("Length");
  StepMaxCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02DetectorMessenger::~ExN02DetectorMessenger() {

  delete StepMaxCmd;
  delete detDir;
  delete N02Dir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02DetectorMessenger::SetNewValue(G4UIcommand* command,
                                         G4String newValue) {

  if (command == StepMaxCmd) {
    myDetector->SetMaxStep(StepMaxCmd->GetNewDoubleValue(newValue));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
