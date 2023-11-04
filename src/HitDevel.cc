//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: HitDevel.cc,v 1.16 2009-10-30 14:59:59 allison Exp $
// GEANT4 tag $Name: geant4-09-04-patch-02 $
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN02DetectorConstruction.hh"
#include "ExN02EventAction.hh"
#include "ExN02PhysicsList.hh"
#include "ExN02PrimaryGeneratorAction.hh"
#include "ExN02RunAction.hh"
#include "HistoManager.hh"
#include "SteppingAction.hh"
//#include "ExN02SteppingAction.hh"
//#include "ExN02SteppingVerbose.hh"
#include "G4PhysListFactory.hh"
#include "G4StepLimiter.hh"
#include "G4UserLimits.hh"
#include "G4UserSpecialCuts.hh"
#include "G4VModularPhysicsList.hh"
//#include "G4StepLimiterBuilder.hh"'
#include "G4Cerenkov.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UItcsh.hh"
#include "G4UIterminal.hh"
#include "MyAdditionalPhysics.hh"
#include "Randomize.hh"
#include "time.h"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "FTFP_BERT.hh"
#include "FTFP_BERT_EMV.hh"
#include "LBE.hh"
#include "QGSP_BERT_EMV.hh"
#include "QGSP_BERT_HP.hh"
#include "QGSP_BIC.hh"
#include "QGSP_BIC_HP.hh"

#if defined(G4UI_USE_TCSH)
#include "G4UItcsh.hh"
#include "G4UIterminal.hh"
#elif defined(G4UI_USE_XM)
#include "G4UIXm.hh"
#elif defined(G4UI_USE_WIN32)
#include "G4UIWin32.hh"
#elif defined(G4UI_USE_QT)
#include "G4Qt.hh"
#include "G4UIQt.hh"
#else
#include "G4UIterminal.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv) {
  // User Verbose output class
  // Choose the Random engine

  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Set random seed with system time
  G4long seed = time(NULL);
  CLHEP::HepRandom::setTheSeed(seed);

  // Run manager
  //
  G4RunManager* runManager = new G4RunManager;

  // User Initialization classes (mandatory)
  ExN02DetectorConstruction* detector = new ExN02DetectorConstruction;
  runManager->SetUserInitialization(detector);

  QGSP_BERT_EMV* MyQGSP = new QGSP_BERT_EMV;
  ///  QGSP_BERT_EMV * MyQPSPWLS = new QGSP_BERT_EMV;
  MyQGSP->RegisterPhysics(new MyAdditionalPhysics("Cerenkov"));

  runManager->SetUserInitialization(MyQGSP);

  // Initilization of histograms
  //-----------------------------
  HistoManager* histo = new HistoManager();
  // User Action classes
  //
  ExN02PrimaryGeneratorAction* gen_action =
      new ExN02PrimaryGeneratorAction(detector);
  runManager->SetUserAction(gen_action);
  //
  ExN02RunAction* run_action = new ExN02RunAction(detector, gen_action, histo);
  runManager->SetUserAction(run_action);
  //
  ExN02EventAction* event_action =
      new ExN02EventAction(run_action, detector, histo);
  runManager->SetUserAction(event_action);
  //
  SteppingAction* stepping_action =
      new SteppingAction(detector, event_action, histo);
  runManager->SetUserAction(stepping_action);

  // Initialize G4 kernel
  //
  runManager->Initialize();

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc != 1)  // batch mode
  {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  } else  // interactive mode : define UI session
  {
    G4UIsession* session = 0;
#if defined(G4UI_USE_TCSH)
    session = new G4UIterminal(new G4UItcsh);
#elif defined(G4UI_USE_XM)
    session = new G4UIXm(argc, argv);
    UI->ApplyCommand("/control/execute visTutor/gui.mac");
#elif defined(G4UI_USE_WIN32)
    session = new G4UIWin32();
    UI->ApplyCommand("/control/execute visTutor/gui.mac");
#elif defined(G4UI_USE_QT)
    session = new G4UIQt(argc, argv);
    UI->ApplyCommand("/control/execute visTutor/gui.mac");
#else
    session = new G4UIterminal();
#endif
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute vis.mac");
#endif
    ui->SessionStart();
    delete ui;
#endif

#ifdef G4VIS_USE
    delete visManager;
#endif
  }

  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
