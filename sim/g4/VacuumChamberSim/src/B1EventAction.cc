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
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "B1EventAction.hh"
#include "B1RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{
  fEdep = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event* event)
{
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
  event->Print();

  /*
  for( unsigned long i = 0; i < event->GetTrajectoryContainer()->size(); i++)
  {
    G4cout << "TrackID: "    << event->GetTrajectoryContainer()->GetVector()->at(i)->GetTrackID() << G4endl;
    G4cout << "Name: "       << event->GetTrajectoryContainer()->GetVector()->at(i)->GetParticleName() << G4endl;
    G4cout << "InitMom:"     << event->GetTrajectoryContainer()->GetVector()->at(i)->GetInitialMomentum() << G4endl;
    G4cout << "ParentID: "   << event->GetTrajectoryContainer()->GetVector()->at(i)->GetParentID() << G4endl;
    G4cout << "PDGEncoding:" << event->GetTrajectoryContainer()->GetVector()->at(i)->GetPDGEncoding() << G4endl;
  }
  */
  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if( trajectoryContainer ) n_trajectories = trajectoryContainer->entries();

  G4int eventID = event->GetEventID();
  if( eventID < 100 || eventID % 100 == 0 ) {
    G4cout << "     " << n_trajectories
           << " trajectories stored in this event." << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
