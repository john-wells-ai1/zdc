//
// ********************************************************************
//
// ********************************************************************

#include "SteppingAction.hh"
#include "ExN02DetectorConstruction.hh"
#include "ExN02EventAction.hh"
#include "HistoManager.hh"
#include "G4Material.hh"
#include "G4Step.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"

//added for wls
#include "G4Run.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"


#include "G4ParticleTypes.hh"



#include "G4ProcessManager.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"

#include "G4ios.hh"
#include <sstream>

// Constructor determination with the pointers assignment
//
// detector(det)=> detector=det and the same for
// eventaction(evt) => eventaction=evt

SteppingAction::SteppingAction(ExN02DetectorConstruction* det,
                               ExN02EventAction* evt, HistoManager* hist)
  :detector(det), eventaction(evt), histo(hist), oldEvtNumber(0)
{}

// Distructor

SteppingAction::~SteppingAction()
{}

// Determination of SteppingAction function

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{

  // Geometrical informations are available from preStepPoint.
  // G4VTouchable and its derivates keep these geometrical informations.
  //---------------------------------------------------------------------
  //   G4TouchableHandle touch = aStep->GetPreStepPoint()->GetTouchableHandle();
  //    G4int volCopyNum = touch->GetVolume()->GetCopyNo();
  //const G4VTouchable* touchable = aStep->GetPostStepPoint()->GetTouchable();

  // G4int volCopyNum = touchable->GetVolume()->GetCopyNo();
  // G4int ReplicaNum = touchable->GetReplicaNumber(1);
  //G4cout<<"SteppingAction::UserSteppingAction: Volume: "<<touchable->GetVolume()->GetName()<<G4endl;
  //G4cout<<"SteppingAction::UserSteppingAction: copyno: "<<volCopyNum<<G4endl;
  // Get volume on the current step
  //--------------------------------
  // GetPreStepPoint() - member of G4Step
  // GetVolume() - member of G4Track
  // GetTouchableHandle() - member of G4Track
  G4VPhysicalVolume* volume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  G4VPhysicalVolume* postvolume = aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume();

  //G4VPhysicalVolume* volume = touchable->GetVolume();

  // Information on the current step
  //---------------------------------
  //  G4double edep = aStep->GetTotalEnergyDeposit();
  //G4double stepl  = aStep->GetStepLength();
  //G4double charge = aStep->GetTrack()->GetDefinition()->GetPDGCharge();
  G4double pnum = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
  G4String pname = aStep->GetTrack()->GetDefinition()->GetParticleName();
  // G4double totalP = aStep->GetTrack()->GetDynamicParticle()->GetTotalMomentum();
  G4double totalE = aStep->GetTrack()->GetTotalEnergy();
  // G4Material* mat = aStep->GetTrack()->GetMaterial();
  // G4MaterialPropertiesTable* propertiesTable = mat->GetMaterialPropertiesTable();

  // To get the copy number of the mother volume (layer number)
  //         (Sens -> Gap -> Wrap -> Layer)
  // ( depth = 0   ->  1  ->  2   ->   3  )
  // G4int copyNumber = touchable->GetCopyNumber(3);
  //
  // Check, in which volume the present step is ?
  // GetEcal(), GetHcal() and GetZero() member functions of
  // DetectorConstruction-class which return pointer to physics volume
  //
  // AddEcal(edep), AddHcal(edep) and AddZero(edep) are member functions
  // of EventAction-class, which accumulate deposited energy


  G4String name = volume->GetName();
  G4String det4 = "RPD1";
  G4String det3 = "RPD2";
  G4String det2 = "RPD3";
  G4String det1 = "RPD4";
  G4String det8 = "RPD5";
  G4String det7 = "RPD6";
  G4String det6 = "RPD7";
  G4String det5 = "RPD8";
  G4String det12 = "RPD9";
  G4String det11 = "RPD10";
  G4String det10 = "RPD11";
  G4String det9 = "RPD12";
  G4String det16 = "RPD13";
  G4String det15 = "RPD14";
  G4String det14 = "RPD15";
  G4String det13 = "RPD16";
  G4String fair = "Air";
  G4String proQuartz = "pQuartz";
  G4String wlsmat1 = "Pethylene";
  G4String wlsmat2 = "PMMA";  
  G4String pmtmat = "G4_Pb";
  G4String Aluminum = "G4_Al";

  G4String pmt1 = "PhotonDet1";
  G4String pmt2 = "PhotonDet2";
  G4String pmt3 = "PhotonDet3";
  G4String pmt4 = "PhotonDet4";
  G4String pmt5 = "PhotonDet5";
  G4String pmt6 = "PhotonDet6";
  G4String pmt7 = "PhotonDet7";
  G4String pmt8 = "PhotonDet8";
  G4String pmt9 = "PhotonDet9";
  G4String pmt10 = "PhotonDet10";
  G4String pmt11 = "PhotonDet11";
  G4String pmt12 = "PhotonDet12";
  G4String pmt13 = "PhotonDet13";
  G4String pmt14 = "PhotonDet14";
  G4String pmt15 = "PhotonDet15";
  G4String pmt16 = "PhotonDet16";
  
  G4String wls1 = "WLSRPD1";
  G4String clad1 = "Clad1";
  G4String wls2 = "WLSRPD2";
  G4String clad2 = "Clad2";
    G4String wls3 = "WLSRPD3";
  G4String clad3 = "Clad3";
    G4String wls4 = "WLSRPD4";
  G4String clad4 = "Clad4";
    G4String wls5 = "WLSRPD5";
  G4String clad5 = "Clad5";
    G4String wls6 = "WLSRPD6";
  G4String clad6 = "Clad6";
    G4String wls7 = "WLSRPD7";
  G4String clad7 = "Clad7";
    G4String wls8 = "WLSRPD8";
  G4String clad8 = "Clad8";
    G4String wls9 = "WLSRPD9";
  G4String clad9 = "Clad9";
    G4String wls10 = "WLSRPD10";
  G4String clad10 = "Clad10";
    G4String wls11 = "WLSRPD11";
  G4String clad11 = "Clad11";
    G4String wls12 = "WLSRPD12";
  G4String clad12 = "Clad12";
    G4String wls13 = "WLSRPD13";
  G4String clad13 = "Clad13";
    G4String wls14 = "WLSRPD14";
  G4String clad14 = "Clad14";
    G4String wls15 = "WLSRPD15";
  G4String clad15 = "Clad15";
    G4String wls16 = "WLSRPD16";
  G4String clad16 = "Clad16";
    G4String wls17 = "WLSRPD17";
  G4String clad17 = "Clad17";
    G4String wls18 = "WLSRPD18";
  G4String clad18 = "Clad18";
    G4String wls19 = "WLSRPD19";
  G4String clad19 = "Clad19";
    G4String wls20 = "WLSRPD20";
  G4String clad20 = "Clad20";
  G4String wls21 = "WLSRPD21";
  G4String clad21 = "Clad21";
  G4String wls22 = "WLSRPD22";
  G4String clad22 = "Clad22";
  G4String wls23 = "WLSRPD23";
  G4String clad23 = "Clad23";
  G4String wls24 = "WLSRPD24";
  G4String clad24 = "Clad24";
  G4String wls25 = "WLSRPD25";
  G4String clad25 = "Clad25";
  G4String wls26 = "WLSRPD26";
  G4String clad26 = "Clad26";
  G4String wls27 = "WLSRPD27";
  G4String clad27 = "Clad27";
  G4String wls28 = "WLSRPD28";
  G4String clad28 = "Clad28";
  G4String wls29 = "WLSRPD29";
  G4String clad29 = "Clad29";
  G4String wls30 = "WLSRPD30";
  G4String clad30 = "Clad30";
  G4String wls31 = "WLSRPD31";
  G4String clad31 = "Clad31";
  G4String wls32 = "WLSRPD32";
  G4String clad32 = "Clad32";
  G4String wls33 = "WLSRPD33";
  G4String clad33 = "Clad33";
  G4String wls34 = "WLSRPD34";
  G4String clad34 = "Clad34";
  G4String wls35 = "WLSRPD35";
  G4String clad35 = "Clad35";
  G4String wls36 = "WLSRPD36";
  G4String clad36 = "Clad36";
  G4String wls37 = "WLSRPD37";
  G4String clad37 = "Clad37";
  G4String wls38 = "WLSRPD38";
  G4String clad38 = "Clad38";
  G4String wls39 = "WLSRPD39";
  G4String clad39 = "Clad39";
  G4String wls40 = "WLSRPD40";
  G4String clad40 = "Clad40";
  G4String wls41 = "WLSRPD41";
  G4String clad41 = "Clad41";
  G4String wls42 = "WLSRPD42";
  G4String clad42 = "Clad42";
  G4String wls43 = "WLSRPD43";
  G4String clad43 = "Clad43";
  G4String wls44 = "WLSRPD44";
  G4String clad44 = "Clad44";
  G4String wls45 = "WLSRPD45";
  G4String clad45 = "Clad45";
  G4String wls46 = "WLSRPD46";
  G4String clad46 = "Clad46";
  G4String wls47 = "WLSRPD47";
  G4String clad47 = "Clad47";
  G4String wls48 = "WLSRPD48";
  G4String clad48 = "Clad48";
  G4String wls49 = "WLSRPD49";
  G4String clad49 = "Clad49";
  G4String wls50 = "WLSRPD50";
  G4String clad50 = "Clad50";
  G4String wls51 = "WLSRPD51";
  G4String clad51 = "Clad51";
  G4String wls52 = "WLSRPD52";
  G4String clad52 = "Clad52";
  G4String wls53 = "WLSRPD53";
  G4String clad53 = "Clad53";
  G4String wls54 = "WLSRPD54";
  G4String clad54 = "Clad54";
  G4String wls55 = "WLSRPD55";
  G4String clad55 = "Clad55";
  G4String wls56 = "WLSRPD56";
  G4String clad56 = "Clad56";
  G4String wls57 = "WLSRPD57";
  G4String clad57 = "Clad57";
  G4String wls58 = "WLSRPD58";
  G4String clad58 = "Clad58";
  G4String wls59 = "WLSRPD59";
  G4String clad59 = "Clad59";
  G4String wls60 = "WLSRPD60";
  G4String clad60 = "Clad60";
  G4String wls61 = "WLSRPD61";
  G4String clad61 = "Clad61";
  G4String wls62 = "WLSRPD62";
  G4String clad62 = "Clad62";
  G4String wls63 = "WLSRPD63";
  G4String clad63 = "Clad63";
  G4String wls64 = "WLSRPD64";
  G4String clad64 = "Clad64";



  if (aStep->GetPreStepPoint()->GetStepStatus()!=fWorldBoundary && aStep->GetPostStepPoint()->GetStepStatus()!=fWorldBoundary)
    {
      G4Material* PreMaterial = aStep->GetPreStepPoint()->GetMaterial();
      G4Material* PostMaterial = aStep->GetPostStepPoint()->GetMaterial();

      //  G4int copyNumber = touchable->GetCopyNumber();
      G4double eDeposit = aStep->GetTotalEnergyDeposit();

      G4ThreeVector PreHitPos = aStep->GetPreStepPoint()->GetPosition();
      G4ThreeVector ThisHitPos = aStep->GetPostStepPoint()->GetPosition();
      G4ThreeVector PreMomentum = aStep->GetTrack()->GetMomentum();


/////////////////////////**********************///////////////////////
//      G4ThreeVector Energy = aStep->GetEnergy();
   //if(PostMaterial->GetName()==Aluminum&&pname=="opticalphoton"){G4cout<<"blah"<<G4endl;}
 /*  	 if(pname=="opticalphoton" && PreMaterial->GetName()!="wlsmat" && PostMaterial->GetName()!="Lead")
		 {  
*///		 if(pname=="e+"||pname=="e-"||pname=="pi+"||pname=="pi-")
//		 {G4Track* theTrack = aStep->GetTrack();
//		 theTrack->SetTrackStatus(fStopAndKill);}
/*
		 G4cout<<"prematerial of kill event was "<<PreMaterial->GetName()<<" and post was "<<PostMaterial->GetName()<<G4endl;
		 G4cout<<"i killed an event"<<G4endl;}
		 
G4cout<<"prematerial of not kill event was "<<PreMaterial->GetName()<<" and post was "<<PostMaterial->GetName()<<G4endl; 
*/
//if(pname=="opticalphoton" && PostMaterial->GetName()=="Air")
// {  G4Track* theTrack = aStep->GetTrack();
//		 theTrack->SetTrackStatus(fStopAndKill);}
//if(pname=="opticalphoton") G4cout<<"energy is "<<totalE<<G4endl;
//if(PostMaterial->GetName()==wlsmat && pname=="opticalphoton" )
//{	G4cout<<"deposit is "<<eDeposit<<" and particle type is "<<pname<<" energy is "<<totalE<<G4endl; }
//if(pname=="opticalphoton" && (PreMaterial->GetName()==wlsmat1 || PreMaterial->GetName()==wlsmat2) && PostMaterial->GetName()==pmtmat )
//{G4cout<<"hit a pmt "<<eDeposit<<G4endl;}


/*
 if(pname=="gamma")
  {  G4Track* theTrack = aStep->GetTrack();
		 theTrack->SetTrackStatus(fStopAndKill);
 G4cout<<"I killed a gamma"<<G4endl;
 }

*/		 
//if(pname=="opticalphoton" && (PreMaterial->GetName()=="Air" || PreMaterial->GetName()=="Lead" || PreMaterial->GetName()=="EMQuartz"))
// {  G4Track* theTrack = aStep->GetTrack();
//		 theTrack->SetTrackStatus(fStopAndKill);}

//if(pname=="opticalphoton" && PreMaterial->GetName()=="wlsmat" && PostMaterial->GetName()=="Lead") G4cout<<"energy is "<<totalE<<" it started in "<<PreMaterial->GetName()<<" which should match " <<volume->GetName()<<G4endl ;


      //if(ThisHitPos.z()<60.8 && PreHitPos.z()>59.2)

	

/*
cout pre/postmat
prem==air && post !=air    **if that works then dont need to collect at surfce (I know that Never works!!)

*/
//tried pQuartz....cout pre and postMats
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  






     if (PreHitPos.z() > 55.495*mm)		//Now surface is staggered!!	
								//...why again was step Within error bound.. what is MAX step size??
	{

//          if(PreMaterial->GetName()==fair && PostMaterial->GetName()==proQuartz)
//    		  {

			if (ThisHitPos.y() > -42.999*mm && ThisHitPos.y() < 39.999*mm )	//High to low check or vv is faster?
			{
			
				if ((ThisHitPos.x() > -39.999*mm && ThisHitPos.x() < -19.2495*mm && ThisHitPos.z() < 55.505*mm)
					|| (ThisHitPos.x() > -19.2495*mm && ThisHitPos.x() < -1.500*mm && ThisHitPos.z() < 56.63*mm)
					|| (ThisHitPos.x() > -1.500*mm && ThisHitPos.x() < 22.2495*mm && ThisHitPos.z() < 57.755*mm)
					|| (ThisHitPos.x() > 22.2495*mm && ThisHitPos.x() < 42.999*mm && ThisHitPos.z() < 58.88*mm))
		
		  		{
		    			  //G4cout<<PreMaterial->GetName()<<" "<<PostMaterial->GetName()<<" "<<name<<G4endl;
		      			if(postvolume->GetName())
		        		{
		          			G4String postname = postvolume->GetName();
		          			
						if( postname==det1 || postname==det2 || postname==det3 || postname==det4
				              || postname==det5 || postname==det6 || postname==det7 || postname==det8
				              || postname==det9 || postname==det10 || postname==det11 || postname==det12
				              || postname==det13 || postname==det14 || postname==det15 || postname==det16)
					            {
			      //                      G4cout<<"The particle name is"<<" "<<pname<<" and its number is "<<pnum<<" preposition was "<<PreHitPos.z()<<" and the postposition was "<<ThisHitPos.z()<<" and it deposited this much energy "<<eDeposit<<" but the total energy was "<<totalE<<" and it was heading this way "<<PreMomentum.z()<<G4endl;
			      //G4cout<<"post name was"<<" "<<postname<<G4endl;
		              //G4cout<<"SteppingAction::UserSteppingAction: The name of the volume hit is"<<" "<<name<<", and energy: "<<totalE<<" and the prematerial was "<<PreMaterial->GetName()<<G4endl;
		              //      G4cout<<"The pre hit was in "<<aStep->GetPreStepPoint()->GetMaterial()->GetName()<<" "<<"and the post hit was in "<<aStep->GetPostStepPoint()->GetMaterial()->GetName()<<G4endl;
		            				  histo->FillPositionResolutionTruth(ThisHitPos.x(), ThisHitPos.y(), ThisHitPos.z(), totalE, pnum);
		            				}//end of det checks  
					     }//end of post name check
				   }//end of x/z grid checks
			 }//end of y grid check
//               }//end of Air first then quartz check
        }//end of PreHit z position check



      if (PreHitPos.z()>59.99 && PreMaterial->GetName()==proQuartz && eDeposit> 0.01*eV)
	{
//	  G4cout<<"The particle was "<<pname<<" and its energy deposit was "<<eDeposit<<"and it hit "<<volume->GetName()<<G4endl;
	  if (name==det1) histo->FillRPDHitEnergies(eDeposit,pnum,0);//RPD1 Hits
	  else if (name==det2) histo->FillRPDHitEnergies(eDeposit,pnum,1);//RPD2 Hits
	  else if (name==det3) histo->FillRPDHitEnergies(eDeposit,pnum,2);//RPD3 Hits
	  else if (name==det4) histo->FillRPDHitEnergies(eDeposit,pnum,3);//RPD4 Hits
	  else if (name==det5) histo->FillRPDHitEnergies(eDeposit,pnum,4);//RPD5 Hits
	  else if (name==det6) histo->FillRPDHitEnergies(eDeposit,pnum,5);//RPD6 Hits
	  else if (name==det7) histo->FillRPDHitEnergies(eDeposit,pnum,6);//RPD7 Hits
	  else if (name==det8) histo->FillRPDHitEnergies(eDeposit,pnum,7);//RPD8 Hits
	  else if (name==det9) histo->FillRPDHitEnergies(eDeposit,pnum,8);//RPD9 Hits
	  else if (name==det10) histo->FillRPDHitEnergies(eDeposit,pnum,9);//RPD10 Hits
	  else if (name==det11) histo->FillRPDHitEnergies(eDeposit,pnum,10);//RPD11 Hits
	  else if (name==det12) histo->FillRPDHitEnergies(eDeposit,pnum,11);//RPD12 Hits
	  else if (name==det13) histo->FillRPDHitEnergies(eDeposit,pnum,12);//RPD13 Hits
	  else if (name==det14) histo->FillRPDHitEnergies(eDeposit,pnum,13);//RPD14 Hits
	  else if (name==det15) histo->FillRPDHitEnergies(eDeposit,pnum,14);//RPD15 Hits
	  else if (name==det16) histo->FillRPDHitEnergies(eDeposit,pnum,15);//RPD16 Hits
	}


//	G4cout<<"optical photon with energy = "<< eDeposit<< "MeV"<<G4endl;
	



//	Edep(2:6eV)

 if (pname ="opticalphoton" && PostMaterial->GetName()==pmtmat && eDeposit>0.001*eV && eDeposit < 7.0*eV)

	{
	  if (postvolume->GetName()==pmt1) {histo->FillPMTHitEnergies(eDeposit,0);     
			histo->FillPMTChannelEnergy(0);}//RPD1 Hits
	 
	  else if (postvolume->GetName()==pmt2) {histo->FillPMTHitEnergies(eDeposit,1);     
			histo->FillPMTChannelEnergy(1);}//RPD2 Hits

	  else if (postvolume->GetName()==pmt3) {histo->FillPMTHitEnergies(eDeposit,2);     
			histo->FillPMTChannelEnergy(2);}
//		G4cout<<"pmt3 photon with energy = "<<totalE<<G4endl;     

	  else if (postvolume->GetName()==pmt4) {histo->FillPMTHitEnergies(eDeposit,3);     
			histo->FillPMTChannelEnergy(3);}//RPD4 Hits

	  else if (postvolume->GetName()==pmt5) {histo->FillPMTHitEnergies(eDeposit,4);   
			histo->FillPMTChannelEnergy(4);}//RPD5 Hits

	  else if (postvolume->GetName()==pmt6) {histo->FillPMTHitEnergies(eDeposit,5);     
			histo->FillPMTChannelEnergy(5);}//RPD6 Hits

	  else if (postvolume->GetName()==pmt7) {histo->FillPMTHitEnergies(eDeposit,6);     
			histo->FillPMTChannelEnergy(6);}
//		G4cout<<"pmt7 photon with energy = "<<totalE<<G4endl;    

	  else if (postvolume->GetName()==pmt8) {histo->FillPMTHitEnergies(eDeposit,7);     
			histo->FillPMTChannelEnergy(7);}//RPD8 Hits

	  else if (postvolume->GetName()==pmt9 ){histo->FillPMTHitEnergies(eDeposit,8);     
			histo->FillPMTChannelEnergy(8);}//RPD9 Hits

	  else if (postvolume->GetName()==pmt10 ){histo->FillPMTHitEnergies(eDeposit,9);     
			histo->FillPMTChannelEnergy(9);}//RPD10 Hits

	  else if (postvolume->GetName()==pmt11 ){histo->FillPMTHitEnergies(eDeposit,10);     
			histo->FillPMTChannelEnergy(10);}
//		G4cout<<"pmt11 photon with energy = "<<totalE<<G4endl;     

	  else if (postvolume->GetName()==pmt12) {histo->FillPMTHitEnergies(eDeposit,11);     
			histo->FillPMTChannelEnergy(11);}//RPD12 Hits

	  else if (postvolume->GetName()==pmt13) {histo->FillPMTHitEnergies(eDeposit,12);     
			histo->FillPMTChannelEnergy(12);}//RPD13 Hits

	  else if (postvolume->GetName()==pmt14) {histo->FillPMTHitEnergies(eDeposit,13);     
			histo->FillPMTChannelEnergy(13);}//RPD14 Hits

	  else if (postvolume->GetName()==pmt15) {histo->FillPMTHitEnergies(eDeposit,14);     
			histo->FillPMTChannelEnergy(14);}
//		G4cout<<"pmt15 photon with energy = "<<totalE<<G4endl;     

	  else if (postvolume->GetName()==pmt16) {histo->FillPMTHitEnergies(eDeposit,15);     
			histo->FillPMTChannelEnergy(15);}//RPD16 Hits
	}



    }//make sure steps are defined
}
