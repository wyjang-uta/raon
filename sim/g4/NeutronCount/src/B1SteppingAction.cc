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
  G4StepPoint* presp = step->GetPreStepPoint();
  G4StepPoint* postsp = step->GetPostStepPoint();

  G4int evt_type = 0;

  if( presp == nullptr ) return;
  if( postsp == nullptr ) return;
  if( step->GetTrack()->GetParticleDefinition()->GetPDGEncoding() != 2112 ) return; // only neutrons
  if( postsp->GetTouchableHandle()->GetVolume() == nullptr ) return;
  if( presp->GetTouchableHandle()->GetVolume() == nullptr ) return;
  if( postsp->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() != "sensor_volume" ) return; // save only neutrons passing decay chamber
  if( presp->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "Dump")
  {
    evt_type = 1;  // This is for the case that neutron comes from the dump.
  }
  if( presp->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "World" )
  {
    evt_type = 2; // This is for the case that neutrons are coming from the world volume.
  }

  G4double KE=(G4double)postsp->GetKineticEnergy()/CLHEP::MeV;
  G4double TE=(G4double)postsp->GetTotalEnergy()/CLHEP::MeV;
  G4double vx=(G4double)postsp->GetPosition().getX()/CLHEP::m;
  G4double vy=(G4double)postsp->GetPosition().getY()/CLHEP::m;
  G4double vz=(G4double)postsp->GetPosition().getZ()/CLHEP::m;
  G4double px=(G4double)postsp->GetMomentum().getX()/CLHEP::MeV;
  G4double py=(G4double)postsp->GetMomentum().getY()/CLHEP::MeV;
  G4double pz=(G4double)postsp->GetMomentum().getZ()/CLHEP::MeV;
  G4double theta=acos(pz/sqrt(px*px+py*py+pz*pz));
  G4cout << "Sensor volume captured a neutron event!" << G4endl;
  G4cout << "K=" << KE << "/ E=" << TE <<
	  "/ vx=" << vx <<
	  "/ vy=" << vy <<
	  "/ vz=" << vz <<
	  "/ px=" << px <<
	  "/ py=" << py <<
	  "/ pz=" << pz <<
	  "/ theta=" << theta << G4endl;
  // event type
  analysisManager->FillNtupleDColumn(0, (G4double)evt_type );
  // kineticEnergy
  analysisManager->FillNtupleDColumn(1, (G4double)postsp->GetKineticEnergy()/CLHEP::MeV);
  // total energy
  analysisManager->FillNtupleDColumn(2, (G4double)postsp->GetTotalEnergy()/CLHEP::MeV);
  // vertex coordinates
  analysisManager->FillNtupleDColumn(3, (G4double)postsp->GetPosition().getX()/CLHEP::m);
  analysisManager->FillNtupleDColumn(4, (G4double)postsp->GetPosition().getY()/CLHEP::m);
  analysisManager->FillNtupleDColumn(5, (G4double)postsp->GetPosition().getZ()/CLHEP::m);
  // momentum
  analysisManager->FillNtupleDColumn(6, (G4double)postsp->GetMomentum().getX()/CLHEP::MeV);
  analysisManager->FillNtupleDColumn(7, (G4double)postsp->GetMomentum().getY()/CLHEP::MeV);
  analysisManager->FillNtupleDColumn(8, (G4double)postsp->GetMomentum().getZ()/CLHEP::MeV);
  // time info
  analysisManager->FillNtupleDColumn(9, (G4double)postsp->GetGlobalTime()/CLHEP::ns);
  analysisManager->FillNtupleDColumn(10, (G4double)postsp->GetLocalTime()/CLHEP::ns);
  analysisManager->AddNtupleRow();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

