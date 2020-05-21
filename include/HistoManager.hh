//
// ====================================================================

#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"

// ====================================================================

class TH1D;
class TH2D;
class TH3D;
class TH2F;
class TH2I;
class TTree;

class HistoMessenger;

 const G4int  nhist = 16; 

class HistoManager 
{

  public:

    HistoManager();
   ~HistoManager();

  void Book(G4double);
    void Clear();
    void Save();

  void FillRPDHitEnergies(G4double,G4double,G4int); //electrons in RPD
  void FillPMTHitEnergies(G4double,G4int); //photons in PMT
  void FillEnergyRPD1(G4double, G4double, G4double, G4double);//Top Row
  void FillEnergyRPD2(G4double, G4double, G4double, G4double);//Second Row
  void FillEnergyRPD3(G4double, G4double, G4double, G4double);//Third Row
  void FillEnergyRPD4(G4double, G4double, G4double, G4double);//Bottom Row
  void FillEnergyPMT1(G4double, G4double, G4double, G4double);//Top Row
  void FillEnergyPMT2(G4double, G4double, G4double, G4double);//Second Row
  void FillEnergyPMT3(G4double, G4double, G4double, G4double);//Third Row
  void FillEnergyPMT4(G4double, G4double, G4double, G4double);//Bottom Row
  void FillNHitsRPD(G4int, G4int, G4int, G4int,G4int, G4int, G4int, G4int,G4int, G4int, G4int, G4int,G4int, G4int, G4int, G4int);//Grab the total Number of Hits
  void FillNHitsPMT(G4int, G4int, G4int, G4int,G4int, G4int, G4int, G4int,G4int, G4int, G4int, G4int,G4int, G4int, G4int, G4int);
  void FillPositionResolutionTruth(G4double,G4double,G4double,G4double,G4String);//this is poorly named
  void FillChannelEnergy(G4double, G4double, G4double, G4double,G4double, G4double, G4double, G4double,G4double, G4double, G4double, G4double,G4double, G4double, G4double, G4double);
  void FillPMTChannelEnergy(G4int);
  




    void SetFileName(G4String);

    void SetJobRunNumber(G4int);

    G4int       GetJobRunNumber()    {return RunNumber;};
 
    G4String    GetfileName()        {return fileName;};

  private:

  TH1D*  histo[nhist];
  TH1D* EachEDep[nhist];
  TH1D* EachPMTEDep[nhist];  
  TH1D* pmthisto[nhist];
  TH2I*  nHit;
  TH2D*  PosRes;
  TH1D* ZPos;
  TH3D* ThreeDHit;
  TH1D* GammaSpectra;
  TH1D* ESpectra;
  TH1D* PionSpectra;
  TH1D* NeutronSpectra;
  TH1D* ProtonSpectra;
  TH1D* SpeciesDistribution;
  TH1D* GammaSmallSpectra;
  TH2D* ChannelEnergy;
  TH2D* PMTChannelEnergy;  
  //The Following Plots are Individual Hit Energies of Different Particles
  TH1D* ElectronHitEnergy;//For now this is the only one i care about
  TH1D* PionHitEnergy;
  TH1D* ProtonHitEnergy;
  TH1D* GammaHitEnergy;

  
  G4int    RunNumber;
  char   name[128];
  char   title[128];
  char   name1[128];
  char   title1[128];

    G4String  fileName ;
  HistoMessenger* histoMessenger;
};


#endif
//===============================================================
