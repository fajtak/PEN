#include "PENActionInitialization.hh"
#include "PENPrimaryGeneratorAction.hh"
#include "PENRunAction.hh"
#include "PENEventAction.hh"
#include "PENSteppingAction.hh"
// #include "OpNoviceStackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENActionInitialization::PENActionInitialization()
 : G4VUserActionInitialization()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENActionInitialization::~PENActionInitialization()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENActionInitialization::BuildForMaster() const
{
  SetUserAction(new PENRunAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENActionInitialization::Build() const
{
	PENRunAction* pPENRunAction = new PENRunAction();
	PENEventAction* pPENEventAction = new PENEventAction(pPENRunAction);

  SetUserAction(new PENPrimaryGeneratorAction());
  SetUserAction(pPENRunAction);
  SetUserAction(pPENEventAction);
  SetUserAction(new PENSteppingAction(pPENEventAction));
  // SetUserAction(new PENStackingAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
