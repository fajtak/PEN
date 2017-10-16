#ifndef PENEventAction_h
#define PENEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class PENRunAction;

/// Event action class
///

class PENEventAction : public G4UserEventAction
{
  public:
    PENEventAction(PENRunAction* runAction);
    virtual ~PENEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void    AddDetectedPhoton(void){fDetectedPhotons++;};
    G4int   GetNumberDetectedPhotons(void){return fDetectedPhotons;};

  private:
    PENRunAction* 	fRunAction;
    G4int           fDetectedPhotons;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
