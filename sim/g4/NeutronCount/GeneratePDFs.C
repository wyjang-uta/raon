void GeneratePDFs()
{
  TFile fInput("RAONSCL2_POT_5E7.root", "READ");

  const Double_t dE = 5; // unit in MeV
  const Double_t Ei = 5; // unit in MeV
  const Double_t Ef = 600; // unit in MeV
  const Int_t nRuns = (Int_t)((Ef-Ei)/dE); // 119 runs

  Int_t     event_type;
  Double_t  kineticEnergy;
  Double_t  totalEnergy;
  Double_t  hit_x, hit_y, hit_z;
  Double_t  px, py, pz;
  Double_t  theta, phi;
  ULong64_t nEntries;

  TH1D* hKineticEnergy[nRuns];
  TH1D* hx[nRuns];
  TH1D* hy[nRuns];
  TH1D* hz[nRuns];
  TH1D* hTheta[nRuns];
  TH1D* hPhi[nRuns];

  Double_t Elo;
  Double_t Ehi;
  for( Int_t i = 0; i < nRuns; i++ )
  {
    Elo = Ei + i * dE;
    Ehi = Elo + dE;
    hKineticEnergy[i] = new TH1D(
        Form("hKineticEnergy_%d", i),
        Form("Kinetic Energy Spectrum in Energy Range - Ki=%5.3f, Kf=%5.3f;Kinetic Energy (MeV);Entries", Elo, Ehi),
        120, 0, 600);
    hx[i]             = new TH1D(
        Form("hx_%d", i),
        Form("Spatial Distribution (x) of Neutrons Hitting Vacuum Chamber in Energy Range - Ki=%5.3f, Kf=%5.3f;Hit Position, #it{x} (m);Entries", Elo, Ehi),
        220, -11, 11);
    hy[i]             = new TH1D(
        Form("hy_%d", i),
        Form("Spatial Distribution (y) of Neutrons Hitting Vacuum Chamber in Energy Range - Ki=%5.3f, Kf=%5.3f;Hit Position, #it{y} (m);Entries", Elo, Ehi),
        220, -11, 11);
    hz[i]             = new TH1D(
        Form("hz_%d", i),
        Form("Spatial Distribution (z) of Neutrons Hitting Vacuum Chamber in Energy Range - Ki=%5.3f, Kf=%5.3f;Hit Position, #it{y} (m);Entries", Elo, Ehi),
        220, -11, 11);
    hTheta[i]         = new TH1D(
        Form("hTheta_%d", i),
        Form("Angular Distribution (#theta) of Neutrons Hitting Vacuum Chamber in Energy Range - Ki=%5.3f, Kf=%5.3f;Angle w.r.to z-axis, #theta (deg.);Entries/0.1 (deg)", Elo, Ehi),
        360, 0, 360);
    hPhi[i]           = new TH1D(
        Form("hPhi_%d", i),
        Form("Angular Distribution (#phi) of Neutrons Hitting Vacuum Chamber in Energy Range -Ki=%5.3f, Kf=%5.3f;Azimuthal angle, #phi (deg.);Entries/0.1 (deg)", Elo, Ehi),
        360, -180, 180);
  }

  TTree* fTree = (TTree*)fInput.Get("RAON-NeutronCount");

  fTree->SetBranchAddress("kineticEnergy", &kineticEnergy);
  fTree->SetBranchAddress("totalEnergy", &totalEnergy);
  fTree->SetBranchAddress("hit_x", &hit_x);
  fTree->SetBranchAddress("hit_y", &hit_y);
  fTree->SetBranchAddress("hit_z", &hit_z);
  fTree->SetBranchAddress("px", &px);
  fTree->SetBranchAddress("py", &py);
  fTree->SetBranchAddress("pz", &pz);

  nEntries = fTree->GetEntries();

  Int_t binNumber;
  for(ULong64_t i = 0; i < nEntries; i++)
  {
    fTree->GetEntry(i);
    theta = acos( pz / sqrt( px*px + py*py + pz*pz ) ) * TMath::RadToDeg();
    phi   = atan2( py, pz ) * TMath::RadToDeg();

    binNumber = Int_t(kineticEnergy)/Int_t(dE);

    hKineticEnergy[binNumber]->Fill(kineticEnergy);
    hx[binNumber]->Fill(hit_x);
    hy[binNumber]->Fill(hit_y);
    hz[binNumber]->Fill(hit_z);
    hTheta[binNumber]->Fill(theta);
    hPhi[binNumber]->Fill(phi);
  }

  TFile* fOutput = new TFile("output_GeneratePDFs.root", "recreate");
  for(Int_t i = 0; i < nRuns; i++)
  {
    hKineticEnergy[i]->Write();
    hx[i]->Write();
    hy[i]->Write();
    hz[i]->Write();
    hTheta[i]->Write();
    hPhi[i]->Write();
  }
  fOutput->Close();
  fInput.Close();
}
