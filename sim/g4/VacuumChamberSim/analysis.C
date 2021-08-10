Double_t GetShortestDistance(Double_t* r1, Double_t* r2, Double_t* p1, Double_t* p2, Double_t* s, Double_t* t);

void analysis(const char* filename="RAONSCL2_clone.root")
{
  cout << "DAMSA Analysis v0" << endl;
  TFile fInput(filename, "read");
  if( fInput.IsOpen() )
    cout << filename << " is successfully opened." << endl;
  TTree* fTree = (TTree*)fInput.Get("tRAONSCL2");

  Double_t e, px, py, pz;
  Double_t vx, vy, vz;
  Double_t gtime, ltime;
  Double_t v[3];
  Double_t p[3];
  char volumeName[32];
  Double_t t_e, t_px, t_py, t_pz;
  Double_t t_vx, t_vy, t_vz;
  Double_t t_gtime, t_ltime;
  Double_t t_v[3];
  Double_t t_p[3];
  Double_t s, t;
  char t_volumeName[32]; // temporary variables

  Double_t invm2, dt, dist;
  ULong64_t nEntries;

  fTree->SetBranchAddress("kineticEnergy", &e);
  fTree->SetBranchAddress("px", &px);
  fTree->SetBranchAddress("py", &py);
  fTree->SetBranchAddress("pz", &pz);
  fTree->SetBranchAddress("vertex_x", &vx);
  fTree->SetBranchAddress("vertex_y", &vy);
  fTree->SetBranchAddress("vertex_z", &vz);
  fTree->SetBranchAddress("gtime", &gtime);
  fTree->SetBranchAddress("ltime", &ltime);
  fTree->SetBranchAddress("volumeName", volumeName);

  TH1D* hinvm2_10ns  = new TH1D("hinvm2_10ns", ";invm2;Entries", 100, 0, 100);
  TH1D* hinvm2_5ns   = new TH1D("hinvm2_5ns", ";invm2;Entries", 100, 0, 100);
  TH1D* hinvm2_2ns   = new TH1D("hinvm2_2ns", ";invm2;Entries", 100, 0, 100);
  TH1D* hinvm2_1ns   = new TH1D("hinvm2_1ns", ";invm2;Entries", 100, 0, 100);
  TH1D* hinvm2_0_5ns = new TH1D("hinvm2_0_5ns", ";invm2;Entries", 100, 0, 100);
  TH1D* hinvm2_0_1ns = new TH1D("hinvm2_0_1ns", ";invm2;Entries", 100, 0, 100);
  TH2D* hdist_dt = new TH2D("hdist_dt", ";dist;#Delta t (ns)", 200, 0, 20, 100, 0, 1000);
  TH2D* htemp = new TH2D("htemp", "",
      1000, 0, 1000,  // invariant mass binning
      600, 0, 600     // dt binning
      );

  nEntries = fTree->GetEntries();
  for( ULong64_t i = 0; i < nEntries; i++ )
  {
    cout << "Scanning i = " << i << " / " << nEntries << " (" << (Double_t)i/(Double_t)nEntries*100. << "%)." << endl;
    fTree->GetEntry(i);
    t_e  = e;
    t_px = px;
    t_py = py;
    t_pz = pz;
    t_vx = vx;
    t_vy = vy;
    t_vz = vz;
    t_v[0] = vx;
    t_v[1] = vy;
    t_v[2] = vz;
    t_p[0] = px;
    t_p[1] = py;
    t_p[2] = pz;
    t_gtime = gtime;
    t_ltime = ltime;  // copy values to temporary variables

    if( strcmp(volumeName, "physDownstreamSensorVolume") != 0 ) continue; // select only events in sensor volume
    if( e < 5 ) continue; // 5 MeV threshold
    strncpy(t_volumeName, volumeName, sizeof(volumeName));

    // Second loop
    for( ULong64_t j = nEntries-1; j > i; j-- )
    {
      fTree->GetEntry(j);

      if( strcmp(volumeName, "physDownstreamSensorVolume") != 0 ) continue; // select only events in sensor volume
      if( e < 5 ) continue; // 5 MeV threshold

      v[0] = vx;
      v[1] = vy;
      v[2] = vz;
      p[0] = px;
      p[1] = py;
      p[2] = pz;

      invm2 = (t_e + e)*(t_e + e) -( (t_px + px)*(t_px + px) + (t_py + py)*(t_py + py) + (t_pz + pz)*(t_pz + pz) );
      dt = (t_gtime - gtime >= 0)?(t_gtime - gtime):(gtime - t_gtime);
      dist = GetShortestDistance(t_v, v, t_p, p, &s, &t);
      cout << "dist=" << dist << ", s=" << s << ", t=" << t << ", dt=" << dt << endl;

      htemp->Fill(invm2, dt);
      hdist_dt->Fill(dist, dt);
      if( dt < 10)    hinvm2_10ns->Fill(invm2);
      if( dt < 5 )    hinvm2_5ns->Fill(invm2);
      if( dt < 2 )    hinvm2_2ns->Fill(invm2);
      if( dt < 1 )    hinvm2_1ns->Fill(invm2);
      if( dt < 0.5 )  hinvm2_0_5ns->Fill(invm2);
      if( dt < 0.1 )  hinvm2_0_1ns->Fill(invm2);
    }
  }

  TFile* fOutput = new TFile("output_analysis_v0.root", "recreate");
  htemp->Write();
  hdist_dt->Write();
  hinvm2_10ns->Write();
  hinvm2_5ns->Write();
  hinvm2_2ns->Write();
  hinvm2_1ns->Write();
  hinvm2_0_5ns->Write();
  hinvm2_0_1ns->Write();
  fOutput->Close();
}

Double_t GetShortestDistance(Double_t* r1, Double_t* r2, Double_t* p1, Double_t* p2, Double_t* s, Double_t* t)
{
  Double_t p12  = p1[0]*p2[0] + p1[1]*p2[1] + p1[2]*p2[2];
  Double_t r2p2 = r2[0]*p2[0] + r2[1]*p2[1] + r2[2]*p2[2];
  Double_t r1p2 = r1[0]*p2[0] + r1[1]*p2[1] + r1[2]*p2[2];
  Double_t r2p1 = r2[0]*p1[0] + r2[1]*p1[1] + r2[2]*p1[2];
  Double_t r1p1 = r1[0]*p1[0] + r1[1]*p1[1] + r1[2]*p1[2];
  Double_t p1p1 = p1[0]*p1[0] + p1[1]*p1[1] + p1[2]*p1[2];
  Double_t p2p2 = p2[0]*p2[0] + p2[1]*p2[1] + p2[2]*p2[2];

  // Vertex parameter
  s[0] = (  p12 * (r2p2 - r1p2) - p2p2 * ( r2p1 - r1p1 ) ) / ( p12*p12 - p1p1*p2p2 );
  t[0] = ( p1p1 * (r2p2 - r1p2) -  p12 * ( r2p1 - r1p1 ) ) / ( p12*p12 - p1p1*p2p2 );

  // Distance calculator
  Double_t determinant =  ( r2[0] - r1[0] ) * ( p1[1] * p2[2] - p1[2] * p2[1] ) +
                          ( r2[1] - r1[1] ) * ( p1[2] * p2[0] - p2[2] * p1[0] ) +
                          ( r2[2] - r1[2] ) * ( p1[0] * p2[1] - p1[1] * p2[0] );
  determinant = fabs(determinant);
  Double_t denominator =  ( p1[1] * p2[2] - p1[2] * p2[1] ) * ( p1[1] * p2[2] - p1[2] * p2[1] ) +
                          ( p1[2] * p2[0] - p1[0] * p2[2] ) * ( p1[2] * p2[0] - p1[0] * p2[2] ) +
                          ( p1[0] * p2[1] - p1[1] * p2[0] ) * ( p1[0] * p2[1] - p1[1] * p2[0] );
  denominator = sqrt( denominator );
  return determinant/denominator;
}
