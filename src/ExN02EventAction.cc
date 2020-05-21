
#include "ExN02EventAction.hh"
//#include "ExN02EmCalorimeterHit.hh"
#include "ExN02RunAction.hh"
#include "ExN02DetectorConstruction.hh"
#include "HistoManager.hh"
//#include "CaloHit.hh"
#include "G4ThreeVector.hh"

#include "G4ios.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "G4DigiManager.hh"
#include "G4UIcommand.hh"

#include "Randomize.hh"
#include <iomanip>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02EventAction::ExN02EventAction(ExN02RunAction* run,ExN02DetectorConstruction* det,HistoManager* hist)
  :runAct(run),detCon(det),myana(hist),printModulo(20)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02EventAction::~ExN02EventAction()
{
  //We build the digitization module
  //    SiDigitizer* digitizer = new SiDigitizer("SiDigitizer");
  //    G4DigiManager * digiManager = G4DigiManager::GetDMpointer();
  //    digiManager->AddNewModule( digitizer );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02EventAction::BeginOfEventAction(const G4Event*)
{
  for (G4int i=0;i<16;i++)
    {
      if(i<16)
        {
          Erpd[i]=0;
          Epmt[i]=0;
        }
      nhitRPD[i]=0;
      nhitPMT[i]=0;
    }
  //    if ( hitsCollID == -1 )
  //    {
  //       G4SDManager * SDman = G4SDManager::GetSDMpointer();
  //        hitsCollID = SDman->GetCollectionID("RPD1/CaloHitCollection");
  //    }
  //    G4int hitsCollID2 = -1;
  //    hitsCollID2 = SDman->GetCollectionID("RPD2/CaloHitCollection");
  //    G4int hitsCollID3 = -1;
  //    hitsCollID3 = SDman->GetCollectionID("RPD3/CaloHitCollection");
  //    G4int hitsCollID4 = -1;
  //    hitsCollID4 = SDman->GetCollectionID("RPD4/CaloHitCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02EventAction::EndOfEventAction(const G4Event* evt)
{
  G4int event_id = evt->GetEventID();

  // get number of stored trajectories
  //
  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  // periodic printing
  //
  if (event_id < 100 || event_id%100 == 0) {
    G4cout << ">>> Event " << evt->GetEventID() <<" has " << n_trajectories << " trajectories stored in this event." << G4endl;
  }

  //Digitize!!
  //    G4DigiManager * digiManager = G4DigiManager::GetDMpointer();
  //    SiDigitizer* digiModule = static_cast<SiDigitizer*>( digiManager->FindDigitizerModule("SiDigitizer") );
  //    if ( digiModule )
  //    {
  //            digiModule->Digitize();
  //    }

  //Store information
  //    if ( rootSaver )
  //    {
  //Retrieve digits collection
  //            G4int digiCollID = digiManager->GetDigiCollectionID( digitsCollName );
  //            const SiDigiCollection* digits = static_cast<const SiDigiCollection*>( digiManager->GetDigiCollection(digiCollID) );
  ///Retrieve hits collections
  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4HCofThisEvent* hitsCollections = evt->GetHCofThisEvent();
  G4int hitsCollID = -1;
  CaloHitCollection* hits = 0;
  G4String det[16];
  G4String PhotonDet[16];  
  for(int intCell = 0 ; intCell < 16 ; ++intCell ){
    det[intCell] = "RPD"+G4UIcommand::ConvertToString(intCell+1);
    // Get hit collection (RPD 1 has intCell 0, RPD14 has intCell 13, etc)
    G4String CollName=det[intCell]+"/SDCaloHitCollection";
    //   G4cout<<"EventAction::EndOfEventAction: going to get collection with name: "<<CollName<<G4endl;
    hitsCollID = SDman->GetCollectionID(CollName);
    if ( hitsCollections )
      {
        //                 G4cout<<"EventAction::EndOfEventAction: going to get hits with ID: "<<hitsCollID<<", and CollName: "<<CollName<<G4endl;
        hits = static_cast<CaloHitCollection*>( hitsCollections->GetHC(hitsCollID) );
      }
    // stuff from ExN04 that tells me things aren't quite up to snuff:
    //int n_hit = hits->entries();
    //               G4cout << " EventAction::EndOfEventAction:    " << n_hit
    //         << " hits are stored in "<<CollName<< G4endl;
    // end stuff from ExN04
    if ( hits ) //hits container found we can proceed
      {
        //Loop on all hits
        //Now we loop on hits and add the energy to our arrays,
        //basically we are transforming the hit container in a plain array
        //              G4cout<<"EventAction::EndOfEventAction: got a hit"<<G4endl;
        typedef std::vector<CaloHit*>::const_iterator hitsVector_it;
        hitsVector_it hit = hits->GetVector()->begin();
        //              hit->Print();
        hitsVector_it end = hits->GetVector()->end();
        for ( ; hit != end ; ++hit)
          {
            G4double edep = (*hit)->GetEdep();
            //G4int Replica = (*hit)->GetCellID(); //!! there is no more replica or layer, or whatever, it should be 0!
            if(edep >0)
              {
                Erpd[intCell]=edep;
                G4cout<<"Erpd"<<intCell<<" is: "<<Erpd[intCell]<<G4endl;
                nhitRPD[intCell]+=1;     
              }
            // end of looping over the hits loop
          }
        // end of hit container found loop
      }
    // end of 16 cell for loop
  }

  for(int intCell = 0 ; intCell < 16 ; ++intCell ){
      PhotonDet[intCell] = "PhotonDet"+G4UIcommand::ConvertToString(intCell+1);
    G4String CollName=PhotonDet[intCell]+"/SDCaloHitCollection";
    hitsCollID = SDman->GetCollectionID(CollName);
    if ( hitsCollections )
      {
        hits = static_cast<CaloHitCollection*>( hitsCollections->GetHC(hitsCollID) );
      }
    if ( hits ) //hits container found we can proceed
      {
        typedef std::vector<CaloHit*>::const_iterator hitsVector_it;
        hitsVector_it hit = hits->GetVector()->begin();
        hitsVector_it end = hits->GetVector()->end();
        for ( ; hit != end ; ++hit)
          {
            G4double edepPmt = (*hit)->GetEdep();
            if(edepPmt >0)
              {
                Epmt[intCell]=edepPmt;
                G4cout<<"Epmt"<<intCell<<" is: "<<Epmt[intCell]<<G4endl;
                nhitPMT[intCell]+=1;               
              }
            // end of looping over the hits loop
          }
        // end of hit container found loop
      }
    // end of 16 cell for loop
  }
  runAct->FillPerEvent(nhitRPD[0],nhitRPD[1],nhitRPD[2],nhitRPD[3],
		       nhitRPD[4],nhitRPD[5],nhitRPD[6],nhitRPD[7],
		       nhitRPD[8],nhitRPD[9],nhitRPD[10],nhitRPD[11],
		       nhitRPD[12],nhitRPD[13],nhitRPD[14],nhitRPD[15]);
		 

  myana->FillEnergyRPD1(Erpd[0],Erpd[1],Erpd[2],Erpd[3]);

  myana->FillEnergyRPD2(Erpd[4],Erpd[5],Erpd[6],Erpd[7]);

  myana->FillEnergyRPD3(Erpd[8],Erpd[9],Erpd[10],Erpd[11]);

  myana->FillEnergyRPD4(Erpd[12],Erpd[13],Erpd[14],Erpd[15]);

  myana->FillChannelEnergy(Erpd[0],Erpd[1],Erpd[2],Erpd[3],
				Erpd[4],Erpd[5],Erpd[6],Erpd[7],
				Erpd[8],Erpd[9],Erpd[10],Erpd[11],
				Erpd[12],Erpd[13],Erpd[14],Erpd[15]);
				
				
  runAct->FillPMTPerEvent(nhitPMT[0],nhitPMT[1],nhitPMT[2],nhitPMT[3],
		       nhitPMT[4],nhitPMT[5],nhitPMT[6],nhitPMT[7],
		       nhitPMT[8],nhitPMT[9],nhitPMT[10],nhitPMT[11],
		       nhitPMT[12],nhitPMT[13],nhitPMT[14],nhitPMT[15]);
		 

  myana->FillEnergyPMT1(Epmt[0],Epmt[1],Epmt[2],Epmt[3]);

  myana->FillEnergyPMT2(Epmt[4],Epmt[5],Epmt[6],Epmt[7]);

  myana->FillEnergyPMT3(Epmt[8],Epmt[9],Epmt[10],Epmt[11]);

  myana->FillEnergyPMT4(Epmt[12],Epmt[13],Epmt[14],Epmt[15]);

/*  myana->FillPMTChannelEnergy(Epmt[0],Epmt[1],Epmt[2],Epmt[3],
				Epmt[4],Epmt[5],Epmt[6],Epmt[7],
				Epmt[8],Epmt[9],Epmt[10],Epmt[11],
				Epmt[12],Epmt[13],Epmt[14],Epmt[15]);
*/				
  //            Get Postion and Momentum of primary
  //            This is needed to store in ntuple info @ z=0
  const G4ThreeVector& pos = evt->GetPrimaryVertex()->GetPosition();
  const G4ThreeVector& mom = evt->GetPrimaryVertex()->GetPrimary()->GetMomentum();
  G4cout<<"EndOfEventAction: primary position: "<<pos<<", primary momentum: "<<mom<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
