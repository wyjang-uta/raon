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
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();


  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 11.0*m;
  G4double world_sizeZ  = 1.5*m;
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
  // W dump
  //
  G4double r_dump = 0.5 * m;
  G4double l_dump = 1.0 * m;
  G4Material* dump_mat = nist->FindOrBuildMaterial("G4_W");

  G4Tubs* solidDump =
    new G4Tubs("WDump",                    //its name
        0,               // inner radius
        r_dump,          // outer radius
        0.5 * l_dump,    // half-length
        0,
        2.* M_PI * rad);

  G4LogicalVolume* logicDump =
    new G4LogicalVolume(solidDump,            //its solid
                        dump_mat,             //its material
                        "Dump");              //its name

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicDump,               //its logical volume
                    "Dump",                  //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  //
  // Neutron Moderator
  //
  G4double r_mod = 0.5 * m;
  G4double l_mod = 0.2 * m; // thickness of the moderator is 20 cm
  // Polyurethane C3H8N2O1
  // Element definition from https://apc.u-paris.fr/~franco/g4doxy4.10/html/class_materials.html
  // See the constructor source code line 54.
  //
  //                                          Z, A
  G4Element* C = new G4Element("Carbon", "C", 6, 12.011*g/mole);
  G4Element* H = new G4Element("Hydrogen", "H", 1, 1.00794*g/mole);
  G4Element* N = new G4Element("Nitrogen", "N", 7, 14.00674*g/mole);
  G4Element* O = new G4Element("Oxygen", "O", 8, 15.9994*g/mole);
  G4Material* polyurethane = new G4Material(
      "Polyurethane",   // its name
      1100*kg/m3,       // its density
      4,                // its number of elements
      kStateSolid);     // its state
  G4int natoms=3;
  polyurethane->AddElement(C, natoms);
  natoms=8;
  polyurethane->AddElement(H, natoms);
  natoms=2;
  polyurethane->AddElement(N, natoms);
  natoms=1;
  polyurethane->AddElement(O, natoms);

  G4Tubs* solid_mod_volume =
    new G4Tubs("mod_volume",
        0.,           // inner radius
        r_mod,        // outer raduis
        0.5 * l_mod,  // half-length of the moderator
        0.,           // starting phi
        2.* M_PI * rad ); // ending phi (full cylinder)
  G4LogicalVolume* logic_mod_volume =
    new G4LogicalVolume(solid_mod_volume,
        polyurethane,
        "mod_volume");
  new G4PVPlacement(
      0,                                              // no rotation
      G4ThreeVector(0, 0, 0.5 * ( l_dump + l_mod ) ), // translation
      logic_mod_volume,                               // its logical volume
      "mod_volume",                                   // its name
      logicWorld,                                     // its mother volume
      false,                                          // no boolean operation
      0,                                              // a copy number
      checkOverlaps);                                 // overlaps checking

  //
  // Front Endcap of Vacuum Vessel
  //  - It works as a virtual detection volume to know
  //  whether a particle is passing through this membrane
  //  and so it incident to the vacuum vessel.
  G4double l_sensor_volume = 1.0 * mm;
  G4double r_sensor_volume = 10.0 * m;
  G4Tubs* solid_sensor_volume =
    new G4Tubs("sensor_volume",
        0.,                       // inner radius
        r_sensor_volume,                   // outer radius
        0.5 * l_sensor_volume,    // half-length
        0.,
        2. * M_PI * rad );
  G4LogicalVolume* logic_sensor_volume =
    new G4LogicalVolume(solid_sensor_volume,
        world_mat,
        "sensor_volume");
  new G4PVPlacement(0,        // no rotation
      G4ThreeVector(0, 0, 0.5 * ( l_dump + 2*l_mod + l_sensor_volume ) ),        // where to put?
      logic_sensor_volume,    // its logical volume
      "sensor_volume",        // its name
      logicWorld,             // its mother volume
      false,                  // no boolean operation
      0,                      // a copy number
      checkOverlaps);         // overlaps checking

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
