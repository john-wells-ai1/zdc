#ifndef Analysis_h
#define Analysis_h 1


#include "G4String.hh"
#include "G4VHitsCollection.hh"
#include <map>

class TH1;
class TTree;

class ExN02DetectorConstruction ;

#include "TH1.h"

struct hit_t {
   Int_t           run;
   Int_t           event;
   Int_t           layer;
   Int_t           adsorber;
   Double_t        x;
   Double_t        y;
   Double_t        z;
   Double_t        dE;
   Int_t           isEM;

  hit_t() : run(0), event(0), layer(0), adsorber(0),
	    x(0.),y(0.),z(0.),dE(0.),
	    isEM(0)
  {}
};

class Analysis {
  typedef std::map<G4String,TH1*> HistoMap;
  HistoMap histos;

  static Analysis* analysis;
  ExN02DetectorConstruction * myDet;

  double zoffset;
  double energy;

  Analysis();
  virtual ~Analysis();
  void Init();
public:
  void SetParticleEnergy(double e) { energy=e; };

  void BeginOfRun(G4int n);
  void BeginOfEvent(G4int n);
  void EndOfRun(G4int n);
  void EndOfEvent(G4VHitsCollection* HC);  // the analysis

  void SetDetectorConstruction(ExN02DetectorConstruction *);
  
  static Analysis* GetInstance();
  
};


#endif
