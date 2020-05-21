#include "CalorimeterSD.hh"
#include "CaloHit.hh"
#include "HistoManager.hh"
#include "G4Step.hh"
#include "Randomize.hh"

#include "G4HCofThisEvent.hh"

#include "G4HCtable.hh"
#include "G4SDManager.hh"
#include "G4TouchableHistory.hh"
#include "G4HCofThisEvent.hh"
#include "G4HCtable.hh"
#include "G4UnitsTable.hh"
#include "G4UIcommand.hh"
//#include "Analysis.hh"
#include "HistoManager.hh"


CalorimeterSD::CalorimeterSD(G4String SDname)
  : G4VSensitiveDetector(SDname)
{
  // G4cout<<"Creating SD with name:"<<SDname<<G4endl;

  // 'collectionName' is a protected data member of base class G4VCalorimeterSD.
  // Here we declare the name of the collection we will be using.
  collectionName.insert("SDCaloHitCollection");
  HCID = -1;
  // Note that we may add as many collection names we would wish: ie
  // a sensitive detector can have many collections.


}

CalorimeterSD::~CalorimeterSD()
{ }

G4bool CalorimeterSD::ProcessHits(G4Step *step, G4TouchableHistory *)
{
  // step is guaranteed to be in Scintillator volume : no need to check for volume
  
  G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
  G4ThreeVector PreHitPos = step->GetPreStepPoint()->GetPosition();
  G4ThreeVector PostHitPos = step->GetPostStepPoint()->GetPosition();
  G4double stepLength = step->GetStepLength();

  // energy deposit in this step 
  G4VPhysicalVolume* thePhysical = touchable->GetVolume();
 // G4int copyNo = touchable->GetReplicaNumber(1);
//  G4cout<<"CalorimeterSD::ProcessHits copyNo: "<<copyNo<<G4endl;
 // G4int layerIndex = copyNo;
// G4int copyNo = 0;
  G4int layerIndex = 0;
  G4double edep = step->GetTotalEnergyDeposit();
//  G4cout<<"CalorimeterSD::ProcessHits energy deposit: "<<edep<<G4endl;



  if (edep <= 0.) return false;

//new
	//check if edep is from primary or secondary:gmake
	G4String isPri = step->GetTrack()->GetTrackID() == 1 ? "Yes" : "No";
	//--------------
	//Exercise 2 task4a
	//-------------
  G4String name = thePhysical->GetName();
  for(int intCell = 0 ; intCell < 16 ; ++intCell ){
    det[intCell] = "RPD"+G4UIcommand::ConvertToString(intCell+1);
    if(name==det[intCell]) {
//    	G4cout<<" CalorimeterSD::ProcessHits: RPD"<<intCell+1<<" (name: "<<name<<") Edep="<<G4BestUnit(edep,"Energy")<<", in Geant4 units energy is: "<<edep<<" isPrimary? "<<isPri;
//	G4cout<<" (particle name="<<step->GetTrack()->GetDefinition()->GetParticleName()<<")"<<" and was located at"<<PreHitPos.z()<<" its post position was "<<PostHitPos.z()<<" with step length "<<stepLength<<" and parent "<<step->GetTrack()->GetParentID()<<G4endl;
	   // layerIndex = intCell;
    }
  }
    for(int intCell = 0 ; intCell < 16 ; ++intCell ){
    det[intCell] = "PhotonDet"+G4UIcommand::ConvertToString(intCell+1);
    if(name==det[intCell]) {
//    	G4cout<<" CalorimeterSD::ProcessHits: PhotonDet"<<intCell+1<<" (name: "<<name<<") Edep="<<G4BestUnit(edep,"Energy")<<", in Geant4 units energy is: "<<edep<<" isPrimary? "<<isPri;
//	G4cout<<" (particle name="<<step->GetTrack()->GetDefinition()->GetParticleName()<<")"<<" and was located at"<<PreHitPos.z()<<" its post position was "<<PostHitPos.z()<<" with step length "<<stepLength<<" and parent "<<step->GetTrack()->GetParentID()<<G4endl;
	   // layerIndex = intCell;
    }
  }
  hitMap_t::iterator it = hitMap.find(layerIndex);
// method from A01EmCalorimeter.cc
   CaloHit* aHit = (*hitCollection)[layerIndex];    
  aHit->AddEdep(edep);
  aHit->SetPos(step->GetPostStepPoint()->GetPosition());
// add some crap about selecting only electrons or something
  return true;
}

void CalorimeterSD::Initialize(G4HCofThisEvent* HCE)
{
  // ------------------------------
  // -- Creation of the collection
  // ------------------------------
  // -- collectionName[0] is "SDHitCollection", as declared in constructor
  std::cout<<"CalorimeterSD::Initialize: create new hitcollection "<<GetName()<<" with collection name: "<<collectionName[0]<<std::endl;

  // ----------------------------------------------------------------------------
  // -- and attachment of this collection to the "Hits Collection of this Event":
  // ----------------------------------------------------------------------------
  // -- To insert the collection, we need to get an index for it. This index
  // -- is unique to the collection. It is provided by the GetCollectionID(...)
  // -- method (which calls what is needed in the kernel to get this index).
  hitCollection = new CaloHitCollection(SensitiveDetectorName, collectionName[0]);
  if (HCID<0) {
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(hitCollection);
  }
  HCE->AddHitsCollection(HCID, hitCollection);
 for(int i=0;i<16;i++){
     CaloHit* aHit = new CaloHit(i);
     hitCollection->insert(aHit);
     det[i]="Test";
  }
  //Reset map of hits
	hitMap.clear();
}

void CalorimeterSD::EndOfEvent(G4HCofThisEvent* HE)
{
  // test output of hits
//  G4cout << "CalorimeterSD::EndOfEvent method of SD `" << GetName() << "' called." << G4endl;
//    hitCollection->PrintAllHits();
// -- we could have attached the collection to the G4HCofThisEvent in this
  // -- method as well (instead of Initialize). PrintAllHits()
}
