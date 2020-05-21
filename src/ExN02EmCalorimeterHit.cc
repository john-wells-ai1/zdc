#include "ExN02EmCalorimeterHit.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4ios.hh"

G4Allocator<ExN02EmCalorimeterHit> ExN02EmCalorimeterHitAllocator;

ExN02EmCalorimeterHit::ExN02EmCalorimeterHit()
{
  cellID = -1;
  edep = 0.;
  pLogV = 0;
}

ExN02EmCalorimeterHit::ExN02EmCalorimeterHit(G4int z)
{
  cellID = z;
  edep = 0.;
  pLogV = 0;
}

ExN02EmCalorimeterHit::~ExN02EmCalorimeterHit()
{;}

ExN02EmCalorimeterHit::ExN02EmCalorimeterHit(const ExN02EmCalorimeterHit &right)
    : G4VHit() {
  cellID = right.cellID;
  edep = right.edep;
  pos = right.pos;
  rot = right.rot;
  pLogV = right.pLogV;
}

const ExN02EmCalorimeterHit& ExN02EmCalorimeterHit::operator=(const ExN02EmCalorimeterHit &right)
{
  cellID = right.cellID;
  edep = right.edep;
  pos = right.pos;
  rot = right.rot;
  pLogV = right.pLogV;
  return *this;
}

int ExN02EmCalorimeterHit::operator==(const ExN02EmCalorimeterHit &right) const
{
  return (cellID==right.cellID);
}

void ExN02EmCalorimeterHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager&&(edep>0.))
  {
    // Draw a calorimeter cell with a color corresponding to its energy deposit
    G4Transform3D trans(rot.inverse(),pos);
    G4VisAttributes attribs;
    const G4VisAttributes* pVA = pLogV->GetVisAttributes();
    if(pVA) attribs = *pVA;
    G4double rcol = edep/(0.7*GeV);
    if(rcol>1.) rcol = 1.;
    if(rcol<0.4) rcol = 0.4;
    G4Colour colour(rcol,0.,0.);
    attribs.SetColour(colour);
    attribs.SetForceWireframe(false);
    attribs.SetForceSolid(true);
    pVVisManager->Draw(*pLogV,attribs,trans);
  }
}

void ExN02EmCalorimeterHit::Print()
{
  G4cout << "  Cell[" << cellID << "] " << edep/MeV << " (MeV)" << G4endl;
}


