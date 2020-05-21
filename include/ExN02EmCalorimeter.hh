//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
// $Id: ExN02EmCalorimeter.hh,v 1.3 2002/12/13 11:34:28 gunter Exp $
// --------------------------------------------------------------
//
#ifndef ExN02EmCalorimeter_h
#define ExN02EmCalorimeter_h 1

#include "G4VSensitiveDetector.hh"
#include "ExN02EmCalorimeterHit.hh"
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class ExN02EmCalorimeter : public G4VSensitiveDetector
{

  public:
      ExN02EmCalorimeter(G4String name);
      virtual ~ExN02EmCalorimeter();

      virtual void Initialize(G4HCofThisEvent*HCE);
      virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
      virtual void EndOfEvent(G4HCofThisEvent*HCE);

  private:
      ExN02EmCalorimeterHitsCollection* hitsCollection;
      G4int HCID;
};




#endif

