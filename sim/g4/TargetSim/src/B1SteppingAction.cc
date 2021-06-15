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

B1SteppingAction::B1SteppingAction()
: G4UserSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1SteppingAction::~B1SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1SteppingAction::UserSteppingAction(const G4Step* step)
{
  auto analysisManager = G4AnalysisManager::Instance();

  const std::vector<const G4Track*>* secondary = step->GetSecondaryInCurrentStep();
  for( size_t lp=0; lp < (*secondary).size(); lp++ )
  {
    // secondary particle name
    analysisManager->FillNtupleSColumn(0, (*secondary)[lp]->GetDefinition()->GetParticleName() );
    // production process of secondary particle
    analysisManager->FillNtupleSColumn(1, (*secondary)[lp]->GetCreatorProcess()->GetProcessName());
    // parent id
    analysisManager->FillNtupleDColumn(2, (*secondary)[lp]->GetParentID());
    // track id
    analysisManager->FillNtupleDColumn(3, (*secondary)[lp]->GetTrackID());
    // step number
    analysisManager->FillNtupleDColumn(4, (*secondary)[lp]->GetCurrentStepNumber());
    // kineticEnergy
    analysisManager->FillNtupleDColumn(5, (G4double)(*secondary)[lp]->GetKineticEnergy()/CLHEP::MeV);
    analysisManager->FillNtupleDColumn(6, (G4double)(*secondary)[lp]->GetTotalEnergy()/CLHEP::MeV);
    // vertex coordinates
    analysisManager->FillNtupleDColumn(7, (G4double)(*secondary)[lp]->GetPosition().getX()/CLHEP::cm);
    analysisManager->FillNtupleDColumn(8, (G4double)(*secondary)[lp]->GetPosition().getY()/CLHEP::cm);
    analysisManager->FillNtupleDColumn(9, (G4double)(*secondary)[lp]->GetPosition().getZ()/CLHEP::cm);
    // momentum
    analysisManager->FillNtupleDColumn(10, (G4double)(*secondary)[lp]->GetMomentum().getX()/CLHEP::MeV);
    analysisManager->FillNtupleDColumn(11, (G4double)(*secondary)[lp]->GetMomentum().getY()/CLHEP::MeV);
    analysisManager->FillNtupleDColumn(12, (G4double)(*secondary)[lp]->GetMomentum().getZ()/CLHEP::MeV);
    // time info
    analysisManager->FillNtupleDColumn(13, (G4double)(*secondary)[lp]->GetGlobalTime()/CLHEP::ns);
    analysisManager->FillNtupleDColumn(14, (G4double)(*secondary)[lp]->GetLocalTime()/CLHEP::ns);
    analysisManager->AddNtupleRow();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

