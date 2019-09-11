#ifndef HitMy_h
#define HitMy_h 1

//G4
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

//my
#include "HitDataStructure.hh"

class HitMy : public G4VHit
{

public:
  HitMy();
  ~HitMy();
  HitMy(const HitMy&);
  const HitMy& operator=(const HitMy&);
  G4int operator==(const HitMy&) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  void Draw();
  void Print();

public:

  HitData myData;

private:

};

typedef G4THitsCollection<HitMy> HitsCollection;

extern G4Allocator<HitMy> HitAllocator;

inline void* HitMy::operator new(size_t)
{
  void *aHit;
  aHit = (void *) HitAllocator.MallocSingle();
  return aHit;
}

inline void HitMy::operator delete(void *aHit)
{
  HitAllocator.FreeSingle((HitMy*) aHit);
}

#endif
