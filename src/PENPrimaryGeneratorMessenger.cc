#include "PENPrimaryGeneratorMessenger.hh"

#include "PENPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENPrimaryGeneratorMessenger::
  PENPrimaryGeneratorMessenger(PENPrimaryGeneratorAction* PENGun)
  : G4UImessenger(),
    fPENAction(PENGun)
{
  fGunDir = new G4UIdirectory("/PEN/gun/");
  fGunDir->SetGuidance("PrimaryGenerator control");

  fSourceType =
           new G4UIcmdWithAnInteger("/PEN/gun/sourceType",this);
  fSourceType->SetGuidance("Choose the type of source");
  fSourceType->SetParameterName("sourceType",true);
  fSourceType->SetDefaultValue(0);
  fSourceType->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENPrimaryGeneratorMessenger::~PENPrimaryGeneratorMessenger()
{
  delete fSourceType;
  delete fGunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENPrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{
  if( command == fSourceType ) {
      fPENAction->SetSourceType(fSourceType->GetNewIntValue(newValue));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
