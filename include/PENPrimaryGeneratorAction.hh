#ifndef PENPrimaryGeneratorAction_h
#define PENPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PENPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	PENPrimaryGeneratorAction();
	virtual ~PENPrimaryGeneratorAction();

public:
	virtual void GeneratePrimaries(G4Event*);

	void  SetSourceType(G4int newType);
	G4int GetSourceType(void){return fSourceType;};
	void SetPhotonWavelength(G4double newValue);

private:
	G4ParticleGun*  fParticleGun;
	G4int           fSourceType;
	G4double		fPhotonWavelength;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*PENPrimaryGeneratorAction_h*/
