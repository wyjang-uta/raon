//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1SteppingAction.cc
/// \brief Implementation of the B1SteppingAction class

#include "B1SteppingAction.hh"
#include "B1EventAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleTypes.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::B1SteppingAction(B1EventAction* eventAction,
    const B1DetectorConstruction* detConstruction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fDetConstruction(detConstruction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  auto analysisManager = G4AnalysisManager::Instance();

  G4VPhysicalVolume* currentVolumePV = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  G4bool isFirstStepInSensorVolume = false;
  G4StepPoint* preStepPoint = step->GetPreStepPoint();

  if( currentVolumePV->GetName() == "physDownstreamSensorVolume" && step->IsFirstStepInVolume())
  {
    G4cout << "Sensor volume event found!" << G4endl;
    analysisManager->FillNtupleSColumn(0, step->GetTrack()->GetParticleDefinition()->GetParticleName());
    analysisManager->FillNtupleDColumn(1, preStepPoint->GetKineticEnergy()/CLHEP::MeV);
    analysisManager->FillNtupleDColumn(2, preStepPoint->GetMomentum().getX()/CLHEP::MeV);
    analysisManager->FillNtupleDColumn(3, preStepPoint->GetMomentum().getY()/CLHEP::MeV);
    analysisManager->FillNtupleDColumn(4, preStepPoint->GetMomentum().getZ()/CLHEP::MeV);
    analysisManager->FillNtupleDColumn(5, preStepPoint->GetPosition().getX()/CLHEP::cm);
    analysisManager->FillNtupleDColumn(6, preStepPoint->GetPosition().getY()/CLHEP::cm);
    analysisManager->FillNtupleDColumn(7, preStepPoint->GetPosition().getZ()/CLHEP::cm);
    analysisManager->AddNtupleRow();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

