// Global variables
std::vector<double> Chi2_proton_event;


void fit_double_peak_threeT(std::string inputFilePath);

void fit_double_peak_threeT(std::string inputFilePath){
  // Open the file
  TFile* file = new TFile(inputFilePath.c_str(),"READ");
  // Get the evt-Tree of the file
  TTree *tree = (TTree*) file->Get("evt");
  // Determine number of events
  int evtnr = tree->GetEntries();
  
  // Loop over evtIDs
  for (int evtID = 0; evtID < evtnr; evtID++) {
    // Create canvas
    TCanvas* c1 = new TCanvas("c1", "c1", 800, 600);
    ///// Histogram /////
    // Number bins
    int bins = 200;//800
    // Bounds to the histogram
    float min = 0.0;
    float max = 400.0;
    TH1F* h1 = new TH1F("h1", "", bins, min, max);
    // Filling the single events into h1
    tree->Draw("hitTime>>h1", Form("evtID == %d", evtID));

    // Shift x-axis
    double firstNonZeroBinCenter = 0.0;
    for (int i = 1; i <= h1->GetNbinsX(); ++i) {
      if (h1->GetBinContent(i) != 0.0) {
        firstNonZeroBinCenter = h1->GetBinCenter(i);
        break;
      }
    }
    double shiftAmount = -firstNonZeroBinCenter+5;
    h1->GetXaxis()->SetLimits(h1->GetXaxis()->GetXmin() + shiftAmount, h1->GetXaxis()->GetXmax() + shiftAmount);

    ////////// FITTING //////////
    // Bounds to fit function
    Double_t fit_min = 0.;
    Double_t fit_max = 154.;//305

    TF1* fit_double_peak_threeT = new TF1("ftdouble_peak_threeT", double_peak_threeT, fit_min, fit_max, 10);
    //(Name of fit, name of function, Fit minimum, Fit maximum, Number of parameter)
    fit_double_peak_threeT->SetNpx(500); //Number of points, default 100, better resolution
    // Set parameter values and range
    // par = [a, b, mu1, mu2, sig, tau1, tau2, tau3, p, q]
    Double_t a = 10000.;
    Double_t a_min = 0.;
    Double_t a_max = 10000000.;
    Double_t b = 100000.;
    Double_t b_min = 0.;
    Double_t b_max = 10000000.;
    Double_t mu1 = 5;
    Double_t mu1_min = 0.;
    Double_t mu1_max = 2000.;
    Double_t mu2 = 20;
    Double_t mu2_min = 0.;
    Double_t mu2_max = 2000.;
    Double_t sig = 1.;
    Double_t sig_min = 0.;
    Double_t sig_max = 10.;
    Double_t tau1 = 4.6;
    Double_t tau1_min = 0.;
    Double_t tau1_max = 1000.;
    Double_t tau2 = 15.1;
    Double_t tau2_min = 0.;
    Double_t tau2_max = 1000.;
    Double_t tau3 = 76.1;
    Double_t tau3_min = 0.;
    Double_t tau3_max = 1000.;
    Double_t p1 = 0.7;
    Double_t p1_min = 0.;
    Double_t p1_max = 1.;
    Double_t q1 = 0.2;
    Double_t q1_min = 0.;
    Double_t q1_max = 1.;
    fit_double_peak_threeT->SetParameters(a, b, mu1, mu2, sig, tau1, tau2, tau3, p1, q1);
    fit_double_peak_threeT->SetParLimits(0, a_min, a_max); 
    fit_double_peak_threeT->SetParLimits(1, b_min, b_max); 
    fit_double_peak_threeT->SetParLimits(2, mu1_min, mu1_max);
    fit_double_peak_threeT->SetParLimits(3, mu2_min, mu2_max);
    fit_double_peak_threeT->SetParLimits(4, sig_min, sig_max);
    fit_double_peak_threeT->SetParLimits(5, tau1_min, tau1_max);
    fit_double_peak_threeT->SetParLimits(6, tau2_min, tau2_max);
    fit_double_peak_threeT->SetParLimits(7, tau3_min, tau3_max);
    fit_double_peak_threeT->SetParLimits(8, p1_min, p1_max);
    fit_double_peak_threeT->SetParLimits(9, q1_min, q1_max);
    // Set parameter names
    fit_double_peak_threeT->SetParName(0, "amplitude a");
    fit_double_peak_threeT->SetParName(1, "amplitude b");
    fit_double_peak_threeT->SetParName(2, "mu 1");
    fit_double_peak_threeT->SetParName(3, "mu 2");
    fit_double_peak_threeT->SetParName(4, "sigma");
    fit_double_peak_threeT->SetParName(5, "tau 1");
    fit_double_peak_threeT->SetParName(6, "tau 2");
    fit_double_peak_threeT->SetParName(7, "tau 3");
    fit_double_peak_threeT->SetParName(8, "ratio p");
    fit_double_peak_threeT->SetParName(9, "ratio q");
    
    /* // Einzelkomponenten
    // Peak 1; //par = [a, mu, sig, tau1, tau2, tau3, p1, q1]
    TF1* component1 = new TF1("component1", single_peak_threeT, fit_min, fit_max, 8);
    component1->SetNpx(500);
    component1->SetLineColor(kGreen+1);
    // Peak 2; //par = [a, mu, sig, tau1, tau2, tau3, p, q]
    TF1* component2 = new TF1("component2", single_peak_threeT, fit_min, fit_max, 8);
    component2->SetNpx(500);
    component2->SetLineColor(kGreen+2);*/

    // Fit
    cout << "---------- Fitting parameters ----------" << endl;
    h1->Fit(fit_double_peak_threeT, "+", "", fit_min, fit_max);

    ////////// Plotting //////////  
/*
    h1->SetStats(0);
    h1->SetLineWidth(3);
    fit_double_peak_threeT->SetLineWidth(4);
    // X-Achse
    h1->GetXaxis()->SetTitle("#lower[0.1]{#scale[1.2]{ Hittime [ns] }}");
    h1->GetXaxis()->CenterTitle();
    // Y-Achse
    h1->GetYaxis()->SetTitle("#scale[1.2]{ Events / (0.5 ns) }");
    h1->GetYaxis()->CenterTitle();
    h1->GetYaxis()->SetMaxDigits(3);
    // Legende
    TLegend *leg = new TLegend(0.7,0.8,0.9,0.9); //x1,y1,x2,y2 
    leg->AddEntry(h1, "Events", "f");
    leg->AddEntry(fit_double_peak_threeT, "Fit", "l");
    leg->Draw();

    c1->SaveAs(Form("hitTime_evtID%d.png", evtID));
*/

    Double_t ChiS_kaon = fit_double_peak_threeT->GetChisquare();
    Double_t NDF_kaon = fit_double_peak_threeT->GetNDF();
    Double_t Chi_NDF_kaon = fit_double_peak_threeT->GetChisquare()/fit_double_peak_threeT->GetNDF();
    cout << " " << endl;
    cout << "---------- Goodness of fit ----------" << endl;
    cout << "chi^2 kaon = " << ChiS_kaon << endl;
    cout << "NDF kaon = " << NDF_kaon << endl;
    cout << "chi^2/NDF kaon = " << Chi_NDF_kaon << endl;

    // Writing the chi^2 value into the vector
    Chi2_proton_event.push_back(Chi_NDF_kaon);
    // Progress
    int evtnr_total= evtnr * 2;
    int evtID_total = evtID + 1 + evtnr;
    cout << "+++++ progress:" << evtID_total << "/" << evtnr_total << " +++++" <<endl;
    

    c1->Clear();
  }
}
