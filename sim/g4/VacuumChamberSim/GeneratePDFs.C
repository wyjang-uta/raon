void GeneratePDFs()
{
  TFile fInput("NeutronPDF_POT_5E7.root", "READ");

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
  Double_t Eavg;
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
        Form("Spatial Distribution (z) of Neutrons Hitting Vacuum Chamber in Energy Range - Ki=%5.3f, Kf=%5.3f;Hit Position, #it{z} (m);Entries", Elo, Ehi),
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
    phi   = atan2( py, px ) * TMath::RadToDeg();

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

  // Produce run macros
  ofstream o_macro[nRuns];

  // initialize output filestreams
  for(Int_t i = 0; i < nRuns; i++)
  {
    o_macro[i].open(Form("run%d.mac", i+1));
  }

  Double_t x_real, x_gps;
  Double_t y_real, y_gps;
  Double_t theta_real, theta_gps;

  for(Int_t i = 0; i < nRuns; i++)
  {
    Elo = Ei + i * dE;
    Ehi = Elo + dE;
    Eavg = 0.5*(Elo+Ehi);


        // Write down macro files

    // Common area
    o_macro[i] << "/run/initialize" << std::endl;
    o_macro[i] << "/control/verbose 0" << std::endl;
    o_macro[i] << "/run/verbose 0" << std::endl;
    o_macro[i] << "/event/verbose 0" << std::endl;
    o_macro[i] << "/tracking/verbose 0" << std::endl;
    o_macro[i] << "/gps/particle neutron" << std::endl;
    o_macro[i] << "/gps/pos/type Plane" << std::endl;
    o_macro[i] << "/gps/pos/shape Circle" << std::endl;
    o_macro[i] << "/gps/pos/centre 0. 0. -5. m" << std::endl;
    o_macro[i] << "/gps/pos/radius 5.5 m" << std::endl;

    // run specific area
    o_macro[i] << Form("/gps/ene/mono %f MeV", Eavg) << std::endl;

    // biasx
    // The first bin of x-histogram is 56-th bin of the histogram hx. (where x = -5.5 in meter).
    // and the last bin of x-histogram is 165-th bin.
    o_macro[i] << "/gps/hist/type biasx" << std::endl;
    for(Int_t j = 56; j <= 165; j++)
    {
      if( j == 56 ) // if we're filling in the first bin info, we should write bin lower edge. This is the rule required by Geant4.
      {
        x_real = hx[i]->GetBinLowEdge(j);
        x_gps = 0.5 + x_real/11.;
        o_macro[i] << "/gps/hist/point " << x_gps << " " << hx[i]->GetBinContent(j) << std::endl;
        continue;
      }
      else
      {
        // if this is not the first bin of GPS input, it should be a bin higher edge.
        // So we use the bin low edge of the next bin. This rule is required by Geant4.
        x_real = hx[i]->GetBinLowEdge(j+1);
        x_gps = 0.5 + x_real/11.;
        o_macro[i] << "/gps/hist/point " << x_gps << " " << hx[i]->GetBinContent(j) << std::endl;
      }
    }
    o_macro[i] << std::endl;

    // biasy
    // The first bin of y-histogram is 56-th bin of the histogram hy. (where y = -5.5 in meter).
    // and the last bin of y-histogram is 165-th bin.
    o_macro[i] << "/gps/hist/type biasy" << std::endl;
    for(Int_t j = 56; j <= 165; j++)
    {
      if( j == 56 ) // if we're filling in the first bin info, we should write bin lower edge. This is the rule required by Geant4.
      {
        y_real = hy[i]->GetBinLowEdge(j);
        y_gps = 0.5 + y_real/11.;
        o_macro[i] << "/gps/hist/point " << y_gps << " " << hy[i]->GetBinContent(j) << std::endl;
        continue;
      }
      else
      {
        // if this is not the first bin of GPS input, it should be a bin higher edge.
        // So we use the bin low edge of the next bin. This rule is required by Geant4.
        y_real = hy[i]->GetBinLowEdge(j+1);
        y_gps = 0.5 + y_real/11.;
        o_macro[i] << "/gps/hist/point " << y_gps << " " << hy[i]->GetBinContent(j) << std::endl;
      }
    }
    o_macro[i] << std::endl;

    // Gun direction part
    o_macro[i] << "/gps/ang/type user" << std::endl;
    o_macro[i] << "/gps/hist/type theta" << std::endl;
    // Loop over theta histogram bins!
    for( Int_t j = 0; j <= 90; j++) // Particles with theta > 90 does not incident to the detector because they're backward propagating particles.
    {
      if( j == 0 )
      {
        theta_gps = 0;
        o_macro[i] << "/gps/hist/point " << theta_gps << " " << 0 << std::endl;
        continue;
      }
      else
      {
        theta_real = TMath::DegToRad() * hTheta[i]->GetBinLowEdge(j+1);
        theta_gps  = theta_real;
        o_macro[i] << "/gps/hist/point " << theta_gps << " " << hTheta[i]->GetBinContent(j) << std::endl;
      }
    }

    o_macro[i] << "/tracking/verbose 0" << std::endl;

    // run!
    o_macro[i] << Form("/run/beamOn %d", (Int_t)hKineticEnergy[i]->GetEntries()) << std::endl;
  }

  // close the output filestreams.
  for(Int_t i = 0; i < nRuns; i++)
  {
    o_macro[i].close();
  }
  fInput.Close();
  fOutput->Close();
}
