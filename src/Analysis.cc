#include "Analysis.hh"

#include "SimpleHit.hh"

#include "TFile.h"
#include "TH1D.h"

#include "ExN02DetectorConstruction.hh"

Analysis* Analysis::analysis = 0;

Analysis::Analysis() 
  : myDet(0)
{
  zoffset=0.;
  energy=10.*GeV;

  TH1::StatOverflows(kTRUE); // include overflow and under flow in mean/rms calculations
}

void Analysis::Init() {
  double zmin=-1.*m, zmax=+1.*m;
  double rmin=0., rmax=+1.*m;
  
  double emin=0., emax=energy;

  if (myDet) {
//    zmax=myDet->GetCalorBoxInFullLength();
    zmin=0.;
    zoffset=zmax*0.5;
    rmax=50.*cm;
   
  }
 G4cout<<"Running Analysis.cc"<<G4endl;
  // event observables
  histos["lProfile"] = new TH1D("lProfile","longitudinal profile",100,zmin,zmax);
  histos["rProfile"] = new TH1D("rProfile","radial profile",100,rmin,rmax);

  // run oservables
  histos["visEne"] = new TH1D("visEne","total visible Energy",100,emin,emax);
  histos["visEne1"] = new TH1D("visEne1","total visible Energy in adsorber 1",100,emin,emax);
  histos["visEne2"] = new TH1D("visEne2","total visible Energy in adsorber 2",100,emin,emax);
}

Analysis::~Analysis() {
}

void Analysis::SetDetectorConstruction(ExN02DetectorConstruction * det) {
  myDet=det;
}

void Analysis::BeginOfRun(G4int n) {
  Init();
}

void Analysis::BeginOfEvent(G4int n){
  // clear event shape
}

void Analysis::EndOfRun(G4int n){
  char filename[256];
  sprintf(filename,"run_%d.root",n );
  TFile* file = TFile::Open(filename,"recreate");
  HistoMap::iterator it = histos.begin();
  for (;it!=histos.end();++it) {
    it->second->Write();
  }
  file->Close();
  std::cout<<" written "<<filename<<std::endl;


  it = histos.begin();
  for (;it!=histos.end();++it) {
    delete it->second;
  }
  histos.clear();
}
void Analysis::EndOfEvent(G4VHitsCollection* hitCollection)  // the analysis
{
  TH1* lProfile = histos["lProfile"];
  TH1* rProfile = histos["rProfile"];

  TH1* visEne = histos["visEne"];
  TH1* visEne1 = histos["visEne1"];
  TH1* visEne2 = histos["visEne2"];


  SDHitCollection* shc= (SDHitCollection*)hitCollection;
  double_t vis=0.,vis1=0.,vis2=0.;
  for (size_t i = 0; i < shc->GetSize(); ++i ) {
    //   G4cout<<"["<<i<<"]"<<G4endl;
    //   (*shc)[i]->Print();
    SimpleHit * sh = (*shc)[i];
    G4ThreeVector pos = sh->GetPosition();
    vis+=sh->GetEdep();
   G4cout<<"Got a hit collection, got the position of hit: "<<pos<<G4endl;
//    G4int k=sh->GetAdsorberNumber();
//    if (k==1) vis1+=sh->GetEdep();
//    if (k==2) vis2+=sh->GetEdep();;
 //   lProfile->Fill(pos.z()+zoffset,sh->GetEdep());
//    rProfile->Fill(pos.perp(),sh->GetEdep());
  }
  visEne->Fill(vis);
  visEne1->Fill(vis1);
  visEne2->Fill(vis2);
}

  
Analysis* Analysis::GetInstance()
{
  if (!analysis) {
    analysis= new Analysis();
  }
  return analysis;
}
