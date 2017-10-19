#ifndef PENPrimaryGeneratorMessenger_h
#define PENPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PENPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PENPrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PENPrimaryGeneratorMessenger(PENPrimaryGeneratorAction* );
    virtual ~PENPrimaryGeneratorMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:
    PENPrimaryGeneratorAction* 		fPENAction;
    G4UIdirectory*                  fGunDir;
    //G4UIcmdWithADoubleAndUnit*      fPolarCmd;
    G4UIcmdWithAnInteger*			fSourceType;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
