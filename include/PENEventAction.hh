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
    void    AddDepositedEnergy(G4double newEnergy){fDepositedEnergy += newEnergy;};
    void    AddWavelength(G4double newWavelength);
    void    AddIWavelength(G4double startWavelength);
<<<<<<< HEAD
=======
    void    AddBAWavelength(G4double boundaryAbsorbedWavelength);
>>>>>>> 52466bc0b086cdf657bc5c728c091019141b5de2

  private:
    PENRunAction* 	fRunAction;
    G4int           fDetectedPhotons;
    G4double        fDepositedEnergy;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
