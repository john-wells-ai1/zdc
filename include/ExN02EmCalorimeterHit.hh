#ifndef ExN02EmCalorimeterHit_h
#define ExN02EmCalorimeterHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class ExN02EmCalorimeterHit : public G4VHit
{
  public:

      ExN02EmCalorimeterHit();
      ExN02EmCalorimeterHit(G4int z);
      virtual ~ExN02EmCalorimeterHit();
      ExN02EmCalorimeterHit(const ExN02EmCalorimeterHit &right);
      const ExN02EmCalorimeterHit& operator=(const ExN02EmCalorimeterHit &right);
      int operator==(const ExN02EmCalorimeterHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      virtual void Draw();
      virtual void Print();

  private:
      G4int cellID;
      G4double edep;
      G4ThreeVector pos;
      G4RotationMatrix rot;
      const G4LogicalVolume* pLogV;

  public:
      inline void SetCellID(G4int z) { cellID = z; }
      inline G4int GetCellID() const { return cellID; }
      inline void SetEdep(G4double de) { edep = de; }
      inline void AddEdep(G4double de) { edep += de; }
      inline G4double GetEdep() const { return edep; }
      inline void SetPos(G4ThreeVector xyz) { pos = xyz; }
      inline G4ThreeVector GetPos() const { return pos; }
      inline void SetRot(G4RotationMatrix rmat) { rot = rmat; }
      inline G4RotationMatrix GetRot() const { return rot; }
      inline void SetLogV(G4LogicalVolume* val) { pLogV = val; }
      inline const G4LogicalVolume* GetLogV() const { return pLogV; }
};

typedef G4THitsCollection<ExN02EmCalorimeterHit> ExN02EmCalorimeterHitsCollection;

extern G4Allocator<ExN02EmCalorimeterHit> ExN02EmCalorimeterHitAllocator;

inline void* ExN02EmCalorimeterHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*)ExN02EmCalorimeterHitAllocator.MallocSingle();
  return aHit;
}

inline void ExN02EmCalorimeterHit::operator delete(void* aHit)
{
  ExN02EmCalorimeterHitAllocator.FreeSingle((ExN02EmCalorimeterHit*) aHit);
}

#endif