#ifndef PENActionInitialization_h
#define PENActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class B4DetectorConstruction;

/// Action initialization class.
///

class PENActionInitialization : public G4VUserActionInitialization
{
  public:
    PENActionInitialization();
    virtual ~PENActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif
