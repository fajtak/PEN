#include "PENSteppingAction.hh"
#include "PENEventAction.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENSteppingAction::PENSteppingAction(PENEventAction* eventAction)
: fOneStepPrimaries(false), fEventAction(eventAction)
{
	fExpectedNextStatus = Undefined;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENSteppingAction::~PENSteppingAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENSteppingAction::UserSteppingAction(const G4Step * theStep)
{
	fEventAction->AddDepositedEnergy(theStep->GetTotalEnergyDeposit()/MeV);

	G4Track* theTrack = theStep->GetTrack();

	if ( theTrack->GetCurrentStepNumber() == 1 ) fExpectedNextStatus = Undefined;

	G4StepPoint* thePrePoint = theStep->GetPreStepPoint();
	G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();

	G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
	G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();

	G4OpBoundaryProcessStatus boundaryStatus=Undefined;
	static G4ThreadLocal G4OpBoundaryProcess* boundary=NULL;

	if (theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition() && theTrack->GetCurrentStepNumber() == 1)
		fEventAction->AddIWavelength(G4double(1240/(theTrack->GetVertexKineticEnergy()/eV)));

	//G4OpBoundaryProcessStatus boundaryStatus2 = Absorption;
	//G4cout << boundaryStatus2.name() << G4endl;

  //find the boundary process only once
	if(!boundary){
		G4ProcessManager* pm
		= theStep->GetTrack()->GetDefinition()->GetProcessManager();
		G4int nprocesses = pm->GetProcessListLength();
		G4ProcessVector* pv = pm->GetProcessList();
		G4int i;
		for( i=0;i<nprocesses;i++){
			if((*pv)[i]->GetProcessName()=="OpBoundary"){
				boundary = (G4OpBoundaryProcess*)(*pv)[i];
				break;
			}
		}
	}

  	if(!thePostPV)
  	{//out of world
  		fExpectedNextStatus=Undefined;
  		return;
  	}

  	G4ParticleDefinition* particleType = theTrack->GetDefinition();
  	if(particleType==G4OpticalPhoton::OpticalPhotonDefinition())
  	{
	  	boundaryStatus=boundary->GetStatus();

	    //Check to see if the partcile was actually at a boundary
	    //Otherwise the boundary status may not be valid
	    //Prior to Geant4.6.0-p1 this would not have been enough to check
	  	if(thePostPoint->GetStepStatus()==fGeomBoundary){
	  		if(fExpectedNextStatus==StepTooSmall){
	  			if(boundaryStatus!=StepTooSmall){
	  				G4ExceptionDescription ed;
	  				ed << "PENSteppingAction::UserSteppingAction(): "
		  			<< "No reallocation step after reflection!"
		  			<< G4endl;
		  			G4Exception("PENSteppingAction::UserSteppingAction()", "PENExpl01",
	  				FatalException,ed,
	  				"Something is wrong with the surface normal or geometry");
	  			}
	  		}
	  		fExpectedNextStatus=Undefined;
	  		switch(boundaryStatus){
		  		case Absorption:
		  			//G4cout << "Photon absorbed" << G4endl;
		  			break;
		      	case Detection: //Note, this assumes that the volume causing detection
		                      //is the photocathode because it is the only one with
		                      //non-zero efficiency
		      	{
			        //Triger sensitive detector manually since photon is
			        //absorbed but status was Detection
			        // G4SDManager* SDman = G4SDManager::GetSDMpointer();
			        // G4String sdName="/PENDet/pmtSD";
			        // PENPMTSD* pmtSD = (PENPMTSD*)SDman->FindSensitiveDetector(sdName);
			        // if(pmtSD)pmtSD->ProcessHits_constStep(theStep,NULL);
			        // trackInformation->AddTrackStatusFlag(hitPMT);
			      	//G4cout << "Photon detected" << G4endl
							fEventAction->AddWavelength(G4double(1240/(theTrack->GetKineticEnergy()/eV)));
			      	fEventAction->AddDetectedPhoton();
			      	break;
		      	}
		      	case FresnelReflection:
		      	case TotalInternalReflection:
		      	case LambertianReflection:
		      	case LobeReflection:
		      	case SpikeReflection:
		      	case BackScattering:
		      	//trackInformation->IncReflections();
		      	fExpectedNextStatus=StepTooSmall;
		      		break;
		      	default:
		      		break;
	  		}
		}
	}

}
