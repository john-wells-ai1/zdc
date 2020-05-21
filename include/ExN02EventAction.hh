#ifndef ExN02EventAction_h
#define ExN02EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;
class ExN02RunAction;
class ExN02DetectorConstruction;
class HistoManager;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02EventAction : public G4UserEventAction
{
  public:
    ExN02EventAction(ExN02RunAction*, ExN02DetectorConstruction*, HistoManager*);
   ~ExN02EventAction();

  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
  void AddEcal(G4double de) { EnergyEcal += de; };
  void AddZero(G4double de) { EnergyZero += de; };
  void AddHcal(G4double de) { EnergyHcal += de; };       
  void AddAbs(G4double de)  { EnergyAbs  += de; };

  void AddHcalRange(G4double dl, G4int lay_hcal) {
                                 RangeHcal += dl;
                                 RangeHcalLay[lay_hcal] +=dl; };

  void AddEcalRange(G4double dl, G4int lay_ecal) {
                                 RangeEcal += dl;
                                 RangeEcalLay[lay_ecal] +=dl; };

  void fillEcalStep(G4double dEstep, G4int Lbin, G4int Rbin) {
                                     dEdL[Lbin] += dEstep; 
                                     dEdR[Rbin] += dEstep; };

  void fillHcalStep(G4double dEstep, G4int Lbin, G4int Rbin) {
                                     dEdLHcal[Lbin] += dEstep;
                                     dEdRHcal[Rbin] += dEstep; };

  void fillAbsStep(G4double dEstep, G4int Lbin, G4int Rbin) {
                                    dEdLAbs[Lbin] += dEstep;
                                    dEdRAbs[Rbin] += dEstep; };

  void fillEcalCell(G4int Lcell, G4double dEstep) {dECellsEcal[Lcell] += dEstep;};

  void fillEcalHits(G4int Lhits, G4double dEstep) {dEHitsEcal[Lhits] += dEstep;};

  G4int   GetEventNb()              {return evtNbOld;};

private:

   ExN02RunAction*  runAct;
   ExN02DetectorConstruction* detCon;
   HistoManager* myana;


   G4double    EnergyEcal, EnergyHcal, EnergyZero, EnergyAbs;
   G4double    RangeHcal,  RangeEcal;
   G4int       nRtot, nLtot, nRtoth, nLayers;
   G4int       nRtotAbs, nLtotAbs;
   G4int       nEcalCells;
   G4double    *dEdL, *dEdR, *dEdRHcal, *RangeEcalLay;
   G4double    *dEdLAbs, *dEdRAbs;
   G4double    *dECellsEcal, *dEHitsEcal;
   G4double    dEdLHcal[20], RangeHcalLay[20];
   G4int       printModulo;                     
   G4int       evtNbOld;
  G4double Erpd[16];
  G4int nhitRPD[16];
  G4double Epmt[16];
  G4int nhitPMT[16];  

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
