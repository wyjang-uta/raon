void analyze()
{
  TFile fInput("RAONSCL2.root", "READ");

  Int_t     event_type;
  Double_t  kineticEnergy;
  Double_t  totalEnergy;
  Double_t  hit_x, hit_y, hit_z;
  Double_t  px, py, pz;
  Double_t  theta, phi;
  ULong64_t nEntries;

  TH1D* hKineticEnergy = new TH1D("hKineticEnergy", ";Kinetic Energy (MeV);Entries", 120, 0, 600);
  TH1D* hTotalEnergy  = new TH1D("hTotalEnergy",  ";Total Energy (MeV);Entries", 308, 0, 1540); // 1540 = 600 + 940, 940 ~ 939 MeV = M_n, neutron mass
  TH1D* hx = new TH1D("hx", ";Hit Position, #it{x} (m);Entries", 220, -11, 11);
  TH1D* hy = new TH1D("hy", ";Hit Position, #it{y} (m);Entries", 220, -11, 11);
  TH1D* hz = new TH1D("hz", ";Hit Position, #it{z} (m);Entries", 220, -11, 11);
  TH2D* hx_y = new TH2D("hx_y", ";Hit Position, #it{x} (cm);Hit Position, #it{y} (m)", 220, -11, 11, 220, -11, 11);
  TH2D* hz_y = new TH2D("hy_z", ";Hit Position, #it{z} (cm);Hit Position, #it{y} (m)", 220, -11, 11, 220, -11, 11);
  TH1D* hTheta = new TH1D("hTheta", ";Angle w.r.to z-axis, #theta (deg.);Entries/0.1 (deg)", 3600, 0, 360);
  TH1D* hPhi  = new TH1D("hPhi", ";Azimuthal angle, #phi (deg.);Entries/0.1 (deg)", 3600, -180, 180);

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


  for(ULong64_t i = 0; i < nEntries; i++)
  {
    fTree->GetEntry(i);
    theta = acos( pz / sqrt( px*px + py*py + pz*pz ) ) * TMath::RadToDeg();
    phi   = atan2( py, pz ) * TMath::RadToDeg();
    hKineticEnergy->Fill(kineticEnergy);
    hTotalEnergy->Fill(totalEnergy);
    hx->Fill(hit_x);
    hy->Fill(hit_y);
    hz->Fill(hit_z);
    hx_y->Fill(hit_x, hit_y);
    hz_y->Fill(hit_z, hit_z);
  }

  TFile* fOutput = new TFile("output_analysis.root", "recreate");
  hKineticEnergy->Write();
  hTotalEnergy->Write();
  hx->Write();
  hy->Write();
  hz->Write();
  hx_y->Write();
  hz_y->Write();
  fOutput->Close();
  fInput.Close();
}
