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

  // This routine records energy, momentum, and hit position of neutrons that are coming into the vacuum chamber.
  // In order to identify neutrons coming into the chamber, we compare the step volume of the post-step point and the pre-step point.
  // If the post-step point is "sensor_volume" but the pre-step point is some other volume, we accept those neutrons.
  //
  // To do: is there double counting issue?
  // I think we can prevent this by rejecting the case that the pre-step volume and the post-step volume both are the 'sensor_volume'.
  //
  if( presp == nullptr ) return;
  if( postsp == nullptr ) return;
  if( step->GetTrack()->GetParticleDefinition()->GetPDGEncoding() != 2112 ) return; // only neutrons
  if( postsp->GetTouchableHandle()->GetVolume() == nullptr ) return;
  if( presp->GetTouchableHandle()->GetVolume() == nullptr ) return;
  if( postsp->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() != "sensor_volume" ) return; // save only neutrons passing decay chamber
  if(
      postsp->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "sensor_volume" &&
      presp->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "sensor_volume" )
    return; // we reject this case to prevent double counting issue.

  if( presp->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "Dump")
  {
    evt_type = 1;  // This is for the case that neutron comes from the dump.
  }
  if( presp->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName() == "World" )
  {
    evt_type = 2; // This is for the case that neutrons are coming from the world volume.
  }

  G4double KE=(G4double)postsp->GetKineticEnergy()/CLHEP::MeV;    // Kinetic energy
  G4double TE=(G4double)postsp->GetTotalEnergy()/CLHEP::MeV;      // Total energy
  G4double vx=(G4double)postsp->GetPosition().getX()/CLHEP::m;    // Position where the neutron was produced.
  G4double vy=(G4double)postsp->GetPosition().getY()/CLHEP::m;
  G4double vz=(G4double)postsp->GetPosition().getZ()/CLHEP::m;
  G4double px=(G4double)postsp->GetMomentum().getX()/CLHEP::MeV;  // Momentum of the neutron.
  G4double py=(G4double)postsp->GetMomentum().getY()/CLHEP::MeV;
  G4double pz=(G4double)postsp->GetMomentum().getZ()/CLHEP::MeV;
  G4double gt=(G4double)postsp->GetGlobalTime()/CLHEP::ns;
  G4double lt=(G4double)postsp->GetLocalTime()/CLHEP::ns;
  //G4double theta=acos(pz/sqrt(px*px+py*py+pz*pz));                // Direction of the neutron with respect to the z-axis (beam direction).

  // event type
  analysisManager->FillNtupleDColumn(0, (G4double)evt_type );
  // kineticEnergy
  analysisManager->FillNtupleDColumn(1, KE);
  // total energy
  analysisManager->FillNtupleDColumn(2, TE);
  // vertex coordinates
  analysisManager->FillNtupleDColumn(3, vx);
  analysisManager->FillNtupleDColumn(4, vy);
  analysisManager->FillNtupleDColumn(5, vz);
  // momentum
  analysisManager->FillNtupleDColumn(6, px);
  analysisManager->FillNtupleDColumn(7, py);
  analysisManager->FillNtupleDColumn(8, pz);
  // time info
  analysisManager->FillNtupleDColumn(9, gt);
  analysisManager->FillNtupleDColumn(10, lt);
  analysisManager->AddNtupleRow();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

