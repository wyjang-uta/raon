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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fSensorPV(nullptr)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Vacuum vessel parameters
  G4double pVacVesselRmin = 499.4*cm; // 4.994*m
  G4double pVacVesselRmax = 5.00*m;
  G4double pVacVesselDz    = 5.00*m;  // length
  G4double pVacVesselSphi  = 0; // radian
  G4double pVacVesselDphi  = 2.0*M_PI;

  G4double pVacVesselUpStreamEndcapRmin = 0.;
  G4double pVacVesselUpStreamEndcapRmax = pVacVesselRmin;
  G4double pVacVesselUpStreamEndcapDz = 0.6*cm;

  G4Material* vacuumVessel_mat = nist->FindOrBuildMaterial("G4_Fe");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 12.0*m;
  G4double world_sizeZ  = 12.0*m;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  G4Box* solidWorld =
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking


  //
  // Vacuum Vessel
  //

  G4Tubs* solidVacuumVessel = new G4Tubs(
      "vessel",
      pVacVesselRmin,
      pVacVesselRmax,
      pVacVesselDz,   // half-length
      pVacVesselSphi,
      pVacVesselDphi);

  G4LogicalVolume* logicalVacuumVessel =
    new G4LogicalVolume(solidVacuumVessel,
        vacuumVessel_mat,
        "vessel");

  new G4PVPlacement(0,
      G4ThreeVector(0, 0, 0),
      logicalVacuumVessel,
      "physVessel",
      logicWorld,
      false,
      0,
      checkOverlaps);

  G4Tubs* solidVacuumVessel_UpstreamEndcap = new G4Tubs(
      "vesselUpstreamEndcap",
      pVacVesselUpStreamEndcapRmin,
      pVacVesselUpStreamEndcapRmax,
      pVacVesselUpStreamEndcapDz,
      0.,
      2.0*M_PI);
  G4LogicalVolume* logicalVacuumVesselUpstreamEndcap =
    new G4LogicalVolume(solidVacuumVessel_UpstreamEndcap,
        vacuumVessel_mat,
        "vesselUpstreamEndcap");
  new G4PVPlacement(0,
      G4ThreeVector(0, 0, -499.7*cm),
      logicalVacuumVesselUpstreamEndcap,
      "physVesselUpstreamEndcap",
      logicWorld,
      false,
      0,
      checkOverlaps);

  G4Tubs* solidVacuumVessel_DownstreamEndcap = new G4Tubs(
      "vesselDownstreamEndcap",
      pVacVesselUpStreamEndcapRmin,
      pVacVesselUpStreamEndcapRmax,
      pVacVesselUpStreamEndcapDz,
      0.,
      2.0*M_PI);
  G4LogicalVolume* logicalVacuumVesselDownstreamEndcap =
    new G4LogicalVolume(solidVacuumVessel_DownstreamEndcap,
        vacuumVessel_mat,
        "vesselDownstreamEndcap");
  new G4PVPlacement(0,
      G4ThreeVector(0, 0, 499.7*cm),
      logicalVacuumVesselDownstreamEndcap,
      "physVesselDownstreamEndcap",
      logicWorld,
      false,
      0,
      checkOverlaps);

  G4Tubs* solidDownstreamSensorVolume = new G4Tubs(
      "DownstreamSensorVolume",
      pVacVesselUpStreamEndcapRmin,
      pVacVesselUpStreamEndcapRmax,
      pVacVesselUpStreamEndcapDz,
      0.,
      2.0*M_PI);
  G4LogicalVolume* logicalDownstreamSensorVolume =
    new G4LogicalVolume(solidDownstreamSensorVolume,
        world_mat, // vacuum
        "downstreamSensorVolume");
  fSensorPV = new G4PVPlacement(0,
              G4ThreeVector(0, 0, 499.7*cm + pVacVesselUpStreamEndcapDz),
              logicalDownstreamSensorVolume,
              "physDownstreamSensorVolume",
              logicWorld,
              false,
              0,
              checkOverlaps);
  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
