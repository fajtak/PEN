// Make this appear first!
#include "G4Timer.hh"

#include "PENRunAction.hh"

#include "G4Run.hh"

#include "PENAnalysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENRunAction::PENRunAction()
 : G4UserRunAction(),
   fTimer(0)
{
  fTimer = new G4Timer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENRunAction::~PENRunAction()
{
  delete fTimer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  fTimer->Start();
  G4AnalysisManager* man = G4AnalysisManager::Instance();

  man->OpenFile("penOutput");

  man->CreateH1("1","N Photons Detected",500,0,5000);
  man->CreateH1("2","Deposited energy [MeV]",1000,0,10);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENRunAction::EndOfRunAction(const G4Run* aRun)
{
  fTimer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent()
         << " " << *fTimer << G4endl;
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
