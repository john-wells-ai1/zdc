//
// ====================================================================

#include "HistoManager.hh"
#include "HistoMessenger.hh"
#include "ExN02RunAction.hh"
#include "CalorimeterSD.hh"
#include "Randomize.hh"
#include "G4Poisson.hh"


// ROOT headers
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TTree.h"
//#include "TLatex.h"

#include <stdio.h>

// ====================================================================

HistoManager::HistoManager()
{
 for(G4int ih=0; ih<nhist; ih++) 
   {
     histo[ih]=0;
	 pmthisto[ih]=0;     
     EachEDep[ih]=0;
     EachPMTEDep[ih]=0;
   }

 fileName = "Quartz_0_0_neutron_2pt55TeV_100.root";

 RunNumber = 0;
 gROOT->Reset();                         // ROOT style
 histoMessenger = new HistoMessenger(this);
}

HistoManager::~HistoManager()
{
  for(G4int ih=0; ih<nhist; ih++) delete histo[ih];
  for(G4int ih=0; ih<nhist; ih++) delete pmthisto[ih];  
  delete nHit;

  delete PosRes;
  delete ZPos;
  delete ThreeDHit;
  delete GammaSpectra;
  delete GammaSmallSpectra;
  delete ESpectra;
  delete PionSpectra;
  delete NeutronSpectra;
  delete ProtonSpectra;
  delete SpeciesDistribution;
  delete ElectronHitEnergy;
  delete ChannelEnergy;
  delete PMTChannelEnergy;  
  delete histoMessenger;
}

// histos definition
//-------------------
void HistoManager::Book(G4double Ekin)
{
// Turn on histo errors
//----------------------
 TH1::SetDefaultSumw2(true);

  //Define the E distribution histos
  for(G4int ihist=0;ihist<nhist;ihist++)
    {
      sprintf(name,"RPD_%d_Edist",ihist+1);
      sprintf(title,"RPD_%d Energy Distribution of Hits",ihist+1);
      histo[ihist] = new TH1D(name,title,10000,0.0*MeV,1000.0*MeV);
      histo[ihist]->SetXTitle("Energy (MeV)");
      histo[ihist]->SetYTitle("Counts");
      histo[ihist]->SetStats(1);
   
///New
      histo[ihist]->SetOption("hist");

      
      //Shows energy of each step in each individual pad
      sprintf(name1,"RPD_%d_EdistPerHit",ihist+1);
      sprintf(title1,"#splitline{Energy Distribution of Individual Hits in RPD%d:}{2.55 TeV Neutrons, n=10 Events,Position=[3.0,3.0,-6.0cm]}",ihist+1);
      EachEDep[ihist] = new TH1D(name1,title1,10000,0.0*MeV,5.0*MeV);
      EachEDep[ihist]->SetXTitle("Energy (MeV)");
      EachEDep[ihist]->SetYTitle("Counts");
      EachEDep[ihist]->SetStats(1);
      
      EachEDep[ihist]->SetOption("hist9");
      
    }

  for(G4int ihist=0;ihist<nhist;ihist++)
    {
      sprintf(name,"PMT_%d_Edist",ihist+1);
      sprintf(title,"PMT_%d Energy Distribution of Hits",ihist+1);
      pmthisto[ihist] = new TH1D(name,title,10000,0.0*eV,1000.0*eV);
      pmthisto[ihist]->SetXTitle("Energy (MeV)");
      pmthisto[ihist]->SetYTitle("Counts");
      pmthisto[ihist]->SetStats(1);
   
///New
      pmthisto[ihist]->SetOption("hist");

      
      //Shows energy of each step in each individual pad
      sprintf(name1,"PMT_%d_EdistPerHit",ihist+1);
      sprintf(title1,"#splitline{Energy Distribution of Individual Hits in PMT%d:}{2.55 TeV Neutrons, n=10 Events,Position=[3.0,3.0,-6.0cm]}",ihist+1);
      EachPMTEDep[ihist] = new TH1D(name1,title1,10000,0.0*eV,5.0*eV);
      EachPMTEDep[ihist]->SetXTitle("Energy (MeV)");
      EachPMTEDep[ihist]->SetYTitle("Counts");
      EachPMTEDep[ihist]->SetStats(1);
      
      EachPMTEDep[ihist]->SetOption("hist9");  
    }


  //N Hit Histogram
  nHit = new TH2I("PMT_nHit","#splitline{Hits Depositing Energy: 2.55 TeV Neutrons,}{ n=10 Events, Position=[3.0,3.0,-6.0 cm]}",4,0,4,4,0,4);
  nHit->GetXaxis()->SetBinLabel(4,"Column1");
  nHit->GetXaxis()->SetBinLabel(3,"Column2");
  nHit->GetXaxis()->SetBinLabel(2,"Column3");
  nHit->GetXaxis()->SetBinLabel(1,"Column4");

  nHit->GetYaxis()->SetBinLabel(4,"Row1");
  nHit->GetYaxis()->SetBinLabel(3,"Row2");
  nHit->GetYaxis()->SetBinLabel(2,"Row3");
  nHit->GetYaxis()->SetBinLabel(1,"Row4");
 
  nHit->SetOption("textcolz");
  nHit->SetStats(0);

  //PMT Channel Energy Each Event
  PMTChannelEnergy = new TH2D("PMT_TotalChannelEnergy","#splitline{Total Energy Deposited, MeV: 2.55 TeV Neutrons,}{n=10 Events, Position=[3.0,3.0,-6.0 cm]}",4,0,4,4,0,4);
  PMTChannelEnergy->GetXaxis()->SetBinLabel(4,"Column1");
  PMTChannelEnergy->GetXaxis()->SetBinLabel(3,"Column2");
  PMTChannelEnergy->GetXaxis()->SetBinLabel(2,"Column3");
  PMTChannelEnergy->GetXaxis()->SetBinLabel(1,"Column4");

  PMTChannelEnergy->GetYaxis()->SetBinLabel(4,"Row1");
  PMTChannelEnergy->GetYaxis()->SetBinLabel(3,"Row2");
  PMTChannelEnergy->GetYaxis()->SetBinLabel(2,"Row3");
  PMTChannelEnergy->GetYaxis()->SetBinLabel(1,"Row4");

  PMTChannelEnergy->SetOption("textcolz");
  //Channel Energy Each Event
  ChannelEnergy = new TH2D("RPD_TotalChannelEnergy","#splitline{Total Energy Deposited, MeV: 2.55 TeV Neutrons,}{n=10 Events, Position=[3.0,3.0,-6.0 cm]}",4,0,4,4,0,4);
  ChannelEnergy->GetXaxis()->SetBinLabel(4,"Column1");
  ChannelEnergy->GetXaxis()->SetBinLabel(3,"Column2");
  ChannelEnergy->GetXaxis()->SetBinLabel(2,"Column3");
  ChannelEnergy->GetXaxis()->SetBinLabel(1,"Column4");

  ChannelEnergy->GetYaxis()->SetBinLabel(4,"Row1");
  ChannelEnergy->GetYaxis()->SetBinLabel(3,"Row2");
  ChannelEnergy->GetYaxis()->SetBinLabel(2,"Row3");
  ChannelEnergy->GetYaxis()->SetBinLabel(1,"Row4");

  ChannelEnergy->SetOption("textcolz");


  
//Position Resolution Histo
  PosRes = new TH2D("PositionResolution","#splitline{Geant4 Shower Profile: 2.55 TeV Neutrons,}{n=10 Events, Position=[3.0,3.0,-6.0 cm]}",1000,-4.25*cm,4.25*cm,1000,-4.25*cm,4.25*cm);
  PosRes->SetXTitle("X Position, mm");
  PosRes->SetYTitle("Y Position, mm");
  PosRes->SetOption("CONT4Z");
  //PosRes->SetOption("SURF2Z");
  //  PosRes->SetOption("COLZ");

  //Z position of hits 
  ZPos = new TH1D("ZPosition","ZPosition",1000,5*cm,8*cm);
  ZPos->SetXTitle("Z Position Of Shower Hit");
  ZPos->SetYTitle("Counts");

  //3D Vizualization of a Hit
  ThreeDHit = new TH3D("IncidentHitPositions","3DHits",100,5*cm,8*cm,100,-6*cm,6*cm,100,-10*cm,10*cm);
  ThreeDHit->SetXTitle("Z Position of Incident Particle");
  ThreeDHit->SetYTitle("X Position of Incident Particle");
  ThreeDHit->SetZTitle("Y Position of Incident Particle");
  ThreeDHit->SetOption("GLCOLZ");
  

  //Incident Gammas
  GammaSpectra = new TH1D("GammaSpectrum","Energies of Gammas Incident on the RPD",1000,0.0*MeV,3000*MeV);
  GammaSpectra->SetXTitle("Photon Energy (Rest+Kinetic) (MeV)");
  GammaSpectra->SetYTitle("Raw Counts");
  GammaSpectra->SetMarkerSize(0.8);

  GammaSmallSpectra = new TH1D("GammaSmallSpectrum","Energies of Gammas Incident on the RPD",100,0.0*MeV,20*MeV);
  GammaSmallSpectra->SetXTitle("Photon Energy (Rest+Kinetic) (MeV)");
  GammaSmallSpectra->SetYTitle("Raw Counts");
  GammaSmallSpectra->SetMarkerSize(0.8);
  GammaSmallSpectra->SetOption("PL");

  //Incident Electrons or Positrons
  ESpectra = new TH1D("EPlusEMinusSpectrum","Energies of e^{+} or e^{-} Incident on the RPD",1000,0.0*MeV,1000*MeV);
  ESpectra->SetXTitle("Electron Energy (rest+kinetic) (MeV)");
  ESpectra->SetYTitle("Raw Counts");
  ESpectra->SetMarkerSize(0.8);

  //Incident Pions
  PionSpectra = new TH1D("PionSpectrum","Energies of #pi^{+} or #pi^{-} Incident on the RPD",1000,100.0*MeV,5000*MeV);
  PionSpectra->SetXTitle("Pion Energy (Rest+Kinetic) (MeV)");
  PionSpectra->SetYTitle("Raw Counts");
  PionSpectra->SetMarkerSize(0.8);

  //Incident Neutrons or Anti Neutrons
  NeutronSpectra = new TH1D("NeutronSpectrum","Energies of n or #bar{n} Incident on the RPD",1000,0.0*MeV,5000*MeV);
  NeutronSpectra->SetXTitle("Neutron Energy (rest+kinetic) (MeV)");
  NeutronSpectra->SetYTitle("Raw Counts");
  NeutronSpectra->SetMarkerSize(0.8);

  //Incident Protons or Anti Protons
  ProtonSpectra = new TH1D("ProtonSpectrum","Energies of p or #bar{p} Incident on the RPD",1000,0.0*MeV,5000*MeV);
  ProtonSpectra->SetXTitle("Proton Energy (rest+kinetic) (MeV)");
  ProtonSpectra->SetYTitle("Raw Counts");
  ProtonSpectra->SetMarkerSize(0.8);

  //Species Distribution
  SpeciesDistribution = new TH1D("Species Distribution","#splitline{Normalized Distribution of Particle Species: 2.55 TeV Neutrons,}{n=10 Events, Position=[3.0,3.0,-6.0 cm]}",6,0,6);
  SpeciesDistribution->GetXaxis()->SetBinLabel(1,"Gamma");
  SpeciesDistribution->GetXaxis()->SetBinLabel(2,"e^{+} or e^{-}");
  SpeciesDistribution->GetXaxis()->SetBinLabel(3,"#pi^{+} or #pi^{-}");
  SpeciesDistribution->GetXaxis()->SetBinLabel(4,"n or #bar{n}");
  SpeciesDistribution->GetXaxis()->SetBinLabel(5,"p or #bar{p}");
  SpeciesDistribution->GetXaxis()->SetBinLabel(6,"other"); 
  SpeciesDistribution->SetMarkerSize(1.5);
  SpeciesDistribution->SetMarkerStyle(22);
  SpeciesDistribution->SetYTitle("#frac{1}{N_{total}} N_{species}");
  SpeciesDistribution->SetOption("TEXT0");
  SpeciesDistribution->SetStats(0);

  //Electron Hit Energies
  ElectronHitEnergy = new TH1D("ElectronHitEnergies","Individual Hit Energies of e^{+} or e^{-} in the RPD",1000,0.0*MeV,10.0*MeV);
  ElectronHitEnergy->SetMarkerSize(0.8);
  ElectronHitEnergy->SetXTitle("e^{+} or e^{-} Hit Energy (MeV)");
  ElectronHitEnergy->SetYTitle("Counts");
  return;
}

//==> reset histos ===============================

  void HistoManager::Clear()
{
  for(G4int ih=0;ih<nhist;ih++) 
    {
      histo[ih]=0;
      pmthisto[ih]=0;
      EachEDep[ih]=0;
      EachPMTEDep[ih]=0;
    }
  nHit=0;
  PosRes=0;
  ZPos=0;
  ThreeDHit=0;
  ElectronHitEnergy=0;
  GammaSpectra=0;
  GammaSmallSpectra=0;
  ESpectra=0;
  PionSpectra=0;
  NeutronSpectra=0;
  ProtonSpectra=0;
  ChannelEnergy=0;
  PMTChannelEnergy=0;
  SpeciesDistribution=0;
  return;
}

//==> write out histos in the output file ===============

//  void HistoManager::Save(G4String fileName)
  void HistoManager::Save()
{

  TFile* file = new TFile(fileName, "RECREATE", "Geant4 ROOT analysis");

  for(G4int ih=0; ih<nhist; ih++) 
    {
      histo[ih]->Write();
      EachEDep[ih]->Write();
      EachPMTEDep[ih]->Write();
      pmthisto[ih]->Write();
    }
  nHit->Write();
  PosRes->Write();
  ZPos->Write();
  ThreeDHit->Write();
  GammaSpectra->Write();
  GammaSmallSpectra->Write();
  ESpectra->Write();
  PionSpectra->Write();
  ElectronHitEnergy->Write();
  NeutronSpectra->Write();
  ProtonSpectra->Write();
  SpeciesDistribution->Scale(1./SpeciesDistribution->GetEntries());
  SpeciesDistribution->Write();
  ChannelEnergy->Write();
  PMTChannelEnergy->Write();
  file-> Close();
  delete file;

  return;
}

//ElectronsInRPD histo fill function
/*void HistoManager:FillElectronsInRPD(G4double EErpd)
   {
     ElectronsInRPD->Fill(EErpd);
   }
*/
//==> Fill histo with deposited energy ====================
//Top Row of the RPD
void HistoManager::FillEnergyRPD1(G4double Erpd1, G4double Erpd2, G4double Erpd3, G4double Erpd4)
   {
     if (Erpd1>0) histo[0]-> Fill(Erpd1);
     if (Erpd2>0) histo[1]-> Fill(Erpd2);
     if (Erpd3>0) histo[2]-> Fill(Erpd3);   
     if (Erpd4>0) histo[3]-> Fill(Erpd4);
   }
////////////////////////////////////////////////////////////
//Second Row of RPD
void HistoManager::FillEnergyRPD2(G4double Erpd1, G4double Erpd2, G4double Erpd3, G4double Erpd4)
   {
     
     if (Erpd1>0) histo[4]-> Fill(Erpd1);
     if (Erpd2>0) histo[5]-> Fill(Erpd2);
     if (Erpd3>0) histo[6]-> Fill(Erpd3);   
     if (Erpd4>0) histo[7]-> Fill(Erpd4);
   }
//////////////////////////////////////////////////////////////
//Third Row of RPD
void HistoManager::FillEnergyRPD3(G4double Erpd1, G4double Erpd2, G4double Erpd3, G4double Erpd4)
   {
     
     if (Erpd1>0) histo[8]-> Fill(Erpd1);
     if (Erpd2>0) histo[9]-> Fill(Erpd2);
     if (Erpd3>0) histo[10]-> Fill(Erpd3);   
     if (Erpd4>0) histo[11]-> Fill(Erpd4);

     //tree_RPD-> Fill();
   }
///////////////////////////////////////////////////////////
//Fourth Row of RPD
void HistoManager::FillEnergyRPD4(G4double Erpd1, G4double Erpd2, G4double Erpd3, G4double Erpd4)
   {
     
     if (Erpd1>0) histo[12]-> Fill(Erpd1);
     if (Erpd2>0) histo[13]-> Fill(Erpd2);
     if (Erpd3>0) histo[14]-> Fill(Erpd3);   
     if (Erpd4>0) histo[15]-> Fill(Erpd4);

//     tree_RPD-> Fill();
   }
   
//==> Fill PMT histo with deposited energy ====================
//Top Row of the PMT
void HistoManager::FillEnergyPMT1(G4double Epmt1, G4double Epmt2, G4double Epmt3, G4double Epmt4)
   {
     if (Epmt1>0) pmthisto[0]-> Fill(Epmt1);

     if (Epmt2>0) pmthisto[1]-> Fill(Epmt2);

     if (Epmt3>0) pmthisto[2]-> Fill(Epmt3);   

     if (Epmt4>0) pmthisto[3]-> Fill(Epmt4);

   }
////////////////////////////////////////////////////////////
//Second Row of PMT
void HistoManager::FillEnergyPMT2(G4double Epmt1, G4double Epmt2, G4double Epmt3, G4double Epmt4)
   {
     
     if (Epmt1>0) pmthisto[4]-> Fill(Epmt1);

     if (Epmt2>0) pmthisto[5]-> Fill(Epmt2);

     if (Epmt3>0) pmthisto[6]-> Fill(Epmt3);   

     if (Epmt4>0) pmthisto[7]-> Fill(Epmt4);

   }
//////////////////////////////////////////////////////////////
//Third Row of PMT
void HistoManager::FillEnergyPMT3(G4double Epmt1, G4double Epmt2, G4double Epmt3, G4double Epmt4)
   {
     
     if (Epmt1>0) pmthisto[8]-> Fill(Epmt1);

     if (Epmt2>0) pmthisto[9]-> Fill(Epmt2);

     if (Epmt3>0) pmthisto[10]-> Fill(Epmt3);   

     if (Epmt4>0) pmthisto[11]-> Fill(Epmt4);



   }
///////////////////////////////////////////////////////////
//Fourth Row of PMT
void HistoManager::FillEnergyPMT4(G4double Epmt1, G4double Epmt2, G4double Epmt3, G4double Epmt4)
   {
     
     if (Epmt1>0) pmthisto[12]-> Fill(Epmt1);

     if (Epmt2>0) pmthisto[13]-> Fill(Epmt2);

     if (Epmt3>0) pmthisto[14]-> Fill(Epmt3);   

     if (Epmt4>0) pmthisto[15]-> Fill(Epmt4);

   }
   
///////////////////////////////////////////////////////////
//Fill total hits in each RPD channel
/*void HistoManager::FillNHitsRPD(G4int rpd1, G4int rpd2, G4int rpd3, G4int rpd4, G4int rpd5, G4int rpd6, G4int rpd7, G4int rpd8, G4int rpd9, G4int rpd10, G4int rpd11, G4int rpd12, G4int rpd13, G4int rpd14, G4int rpd15, G4int rpd16)
{

  nHit->SetBinContent(1,4,rpd1);/////////////////////START FIRST ROW///////////////////////
  nHit->SetBinContent(2,4,rpd2);
  nHit->SetBinContent(3,4,rpd3);
  nHit->SetBinContent(4,4,rpd4);//////////////////////END FIRST ROW////////////////////////
  
  nHit->SetBinContent(1,3,rpd5);/////////////////////START SECOND ROW///////////////////
  nHit->SetBinContent(2,3,rpd6);
  nHit->SetBinContent(3,3,rpd7);
  nHit->SetBinContent(4,3,rpd8);////////////////////END SECOND ROW/////////////////////
  
  
  nHit->SetBinContent(1,2,rpd9);///////////////////START THIRD ROW////////////////////
  nHit->SetBinContent(2,2,rpd10);
  nHit->SetBinContent(3,2,rpd11);
  nHit->SetBinContent(4,2,rpd12);//////////////////END THIRD ROW//////////////////////
  

  nHit->SetBinContent(1,1,rpd13);/////////////////START FOURTH ROW///////////////////
  nHit->SetBinContent(2,1,rpd14);
  nHit->SetBinContent(3,1,rpd15);
  nHit->SetBinContent(4,1,rpd16);////////////////END FOURTH ROW/////////////////////////

}
*/

void HistoManager::FillNHitsPMT(G4int pmt1, G4int pmt2, G4int pmt3, G4int pmt4, G4int pmt5, G4int pmt6, G4int pmt7, G4int pmt8, G4int pmt9, G4int pmt10, G4int pmt11, G4int pmt12, G4int pmt13, G4int pmt14, G4int pmt15, G4int pmt16)
{

  nHit->SetBinContent(1,4,pmt1);/////////////////////START FIRST ROW///////////////////////
  nHit->SetBinContent(2,4,pmt2);
  nHit->SetBinContent(3,4,pmt3);
  nHit->SetBinContent(4,4,pmt4);//////////////////////END FIRST ROW////////////////////////
  
  nHit->SetBinContent(1,3,pmt5);/////////////////////START SECOND ROW///////////////////
  nHit->SetBinContent(2,3,pmt6);
  nHit->SetBinContent(3,3,pmt7);
  nHit->SetBinContent(4,3,pmt8);////////////////////END SECOND ROW/////////////////////
  
  
  nHit->SetBinContent(1,2,pmt9);///////////////////START THIRD ROW////////////////////
  nHit->SetBinContent(2,2,pmt10);
  nHit->SetBinContent(3,2,pmt11);
  nHit->SetBinContent(4,2,pmt12);//////////////////END THIRD ROW//////////////////////
  

  nHit->SetBinContent(1,1,pmt13);/////////////////START FOURTH ROW///////////////////
  nHit->SetBinContent(2,1,pmt14);
  nHit->SetBinContent(3,1,pmt15);
  nHit->SetBinContent(4,1,pmt16);////////////////END FOURTH ROW/////////////////////////

}


void HistoManager::FillPositionResolutionTruth(G4double X, G4double Y, G4double Z,G4double hitenergy,G4String particleid)
{
  PosRes->Fill(X,Y,hitenergy);
  ZPos->Fill(Z);
  ThreeDHit->Fill(Z,X,Y);
  if (particleid=="opticalphoton") 
    {
      GammaSpectra->Fill(hitenergy);
	  GammaSpectra->Fill(hitenergy);
      GammaSmallSpectra->Fill(hitenergy);
      SpeciesDistribution->Fill(0.5,1);
    }
  else if (particleid=="11" || particleid=="-11")
    {
      ESpectra->Fill(hitenergy);
      SpeciesDistribution->Fill(1.5,1);
    }
  else if (particleid=="211" || particleid=="-211") 
    {
      PionSpectra->Fill(hitenergy);
      SpeciesDistribution->Fill(2.5,1);
    }
  else if (particleid=="2112" || particleid=="-2112")
    {
      NeutronSpectra->Fill(hitenergy);
      SpeciesDistribution->Fill(3.5,1);
    }
  else if (particleid=="2212" || particleid=="-2212") 
    {
      ProtonSpectra->Fill(hitenergy); 
      SpeciesDistribution->Fill(4.5,1);
    }
  else SpeciesDistribution->Fill(5.5,1); 
}



void HistoManager::FillChannelEnergy(G4double erpd1, G4double erpd2, G4double erpd3, G4double erpd4, G4double erpd5, G4double erpd6, G4double erpd7, G4double erpd8, G4double erpd9, G4double erpd10, G4double erpd11, G4double erpd12, G4double erpd13, G4double erpd14, G4double erpd15, G4double erpd16)
{
  //First Row (Top)
  ChannelEnergy->Fill(0.5,3.5,erpd1);//Total Event Energy For channel 1
  ChannelEnergy->Fill(1.5,3.5,erpd2);//Total Event Energy For channel 2 
  ChannelEnergy->Fill(2.5,3.5,erpd3);//Total Event Energy For channel 3 
  ChannelEnergy->Fill(3.5,3.5,erpd4);//Total Event Energy For channel 4 

  //Second Row (Second From Top)
  ChannelEnergy->Fill(0.5,2.5,erpd5);//Total Event Energy For channel 5 
  ChannelEnergy->Fill(1.5,2.5,erpd6);//Total Event Energy For channel 6 
  ChannelEnergy->Fill(2.5,2.5,erpd7);//Total Event Energy For channel 7 
  ChannelEnergy->Fill(3.5,2.5,erpd8);//Total Event Energy For channel 8 
 
  //Third Row (Third From Top)
  ChannelEnergy->Fill(0.5,1.5,erpd9);//Total Event Energy For channel 9 
  ChannelEnergy->Fill(1.5,1.5,erpd10);//Total Event Energy For channel 10 
  ChannelEnergy->Fill(2.5,1.5,erpd11);//Total Event Energy For channel 11
  ChannelEnergy->Fill(3.5,1.5,erpd12);//Total Event Energy For channel 12 

  //Fourth Row (Bottom Row)
  ChannelEnergy->Fill(0.5,0.5,erpd13);//Total Event Energy For channel 13 
  ChannelEnergy->Fill(1.5,0.5,erpd14);//Total Event Energy For channel 14 
  ChannelEnergy->Fill(2.5,0.5,erpd15);//Total Event Energy For channel 15 
  ChannelEnergy->Fill(3.5,0.5,erpd16);//Total Event Energy For channel 16 
}

void HistoManager::FillPMTChannelEnergy(G4int ipmt)
{ 
 //First Row (Top)
switch (ipmt)
{
case 0:
  {PMTChannelEnergy->Fill(0.5,3.5,1);     break;}//  1
case 1:
  {PMTChannelEnergy->Fill(1.5,3.5,1);     break;}//  2 
case 2:
  {PMTChannelEnergy->Fill(2.5,3.5,1);     break;}//  3 
case 3:
  {PMTChannelEnergy->Fill(3.5,3.5,1);     break;}//  4 

  //Second Row (Second From Top)
case 4:
  {PMTChannelEnergy->Fill(0.5,2.5,1);     break;}//  5 
case 5:
  {PMTChannelEnergy->Fill(1.5,2.5,1);     break;}//  6 
case 6:
  {PMTChannelEnergy->Fill(2.5,2.5,1);     break;}//  7 
case 7:
  {PMTChannelEnergy->Fill(3.5,2.5,1);     break;}//  8 
 
  //Third Row (Third From Top)
case 8:
  {PMTChannelEnergy->Fill(0.5,1.5,1);     break;}//  9 
case 9:
  {PMTChannelEnergy->Fill(1.5,1.5,1);     break;}//  10 
case 10:
  {PMTChannelEnergy->Fill(2.5,1.5,1);     break;}//  11
case 11:
  {PMTChannelEnergy->Fill(3.5,1.5,1);     break;}//  12 

  //Fourth Row (Bottom Row)
case 12:
  {PMTChannelEnergy->Fill(0.5,0.5,1);     break;}//  13 
case 13:
  {PMTChannelEnergy->Fill(1.5,0.5,1);     break;}//  14 
case 14:
  {PMTChannelEnergy->Fill(2.5,0.5,1);     break;}//  15 
case 15:
  {PMTChannelEnergy->Fill(3.5,0.5,1);     break;}//  16 
	}
}

/*void HistoManager::FillZPosition(G4double Z)
{
  ZPos->Fill(Z);
}	
*/

void HistoManager::FillRPDHitEnergies(G4double edep,G4double pid,G4int rpdid)
{
  if (pid==11 || pid==-11)
    {
      ElectronHitEnergy->Fill(edep);
    }
  
  EachEDep[rpdid]->Fill(edep);
  
}

void HistoManager::FillPMTHitEnergies(G4double edepPMT,G4int pmtid)
{
  EachPMTEDep[pmtid]->Fill(edepPMT); 
}

// Set Job Run Number 
//--------------------

  void HistoManager::SetJobRunNumber(G4int Value)
  {
    RunNumber = Value;
  }

// Set ROOT file name
//--------------------
  void HistoManager::SetFileName(G4String userFile)
  {
    fileName = userFile;
  }

//===========================================================
