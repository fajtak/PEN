#include "PENEventAction.hh"
#include "PENRunAction.hh"
#include "PENAnalysis.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENEventAction::PENEventAction(PENRunAction* runAction)
: G4UserEventAction(),
fRunAction(runAction)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENEventAction::~PENEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENEventAction::BeginOfEventAction(const G4Event* myEvent)
{    
	fDetectedPhotons = 0;
	if (myEvent->GetEventID() % 100000 == 0)
		G4cout << "event no.: " << myEvent->GetEventID() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENEventAction::EndOfEventAction(const G4Event* myEvent)
{   
	auto analysisManager = G4AnalysisManager::Instance(); 

	analysisManager->FillH1(0,fDetectedPhotons);
	G4cout << fDetectedPhotons << G4endl;
}