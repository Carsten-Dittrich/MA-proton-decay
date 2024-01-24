#include "TFile.h"
#include "TTree.h"
#include "TObject.h"
#include "TString.h"
#include "TH1.h"
#include <TH2.h>
#include <TH3.h>
#include "TCanvas.h"
#include "TF1.h"
#include "TAttAxis.h"
#include "TLegend.h"
#include "TF1Convolution.h"
#include "TMath.h"
#include <TLine.h>
#include <TFrame.h>

#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

#include </home/studente15/Carsten/masterthesis/fitting/fit_functions.h>


void fourT_elec(){
  //File Oeffnen
  auto f1 = TFile::Open("~/Carsten/masterthesis/data/muon/single/seed500/sample_elecsim_user_mu_seed500.root", "READ");
  
  TCanvas *c1 = new TCanvas("hittime","hittime",1200,800);
  c1->SetTickx();
  c1->SetTicky();
  c1->SetGridx();
  c1->SetGridy();
  
  ////////// Histogramm //////////
  // Anzahl Bins
  int bins = 800;
  // Min und Max Wert
  float min = 200.0;
  float max = 1000.0;
  // Histogramm
  TH1F* h1 = new TH1F("h1", "", bins, min, max);

  // HitTime Variable
  double hittime;
  TTree *tree = (TTree*) f1->Get("SPMT");// SPMT-Tree aus dem File holen
  tree->SetBranchAddress("Time", &hittime); // verknüpfe den Branch des Trees mit der zugehörigen Variable
  
  // Anzahl Evts im Tree
  int evtnr = tree->GetEntries();

  // Fuellen des Histograms
  //Schleife über alle Events
  for (int i = 0; i < evtnr; i++){
    //hole den i-ten Eintrag aus dem Tree
    tree->GetEntry(i);
    h1->Fill(hittime);
  }
 /*
  ////////// FITTING //////////
  // Bounds to fit function
  Double_t fit_min = 360.;
  Double_t fit_max = 700.;

  TF1* fit_single_peak_fourT = new TF1("ftsingle_peak_fourT", single_peak_fourT, fit_min, fit_max, 11);
  //(Eigenname des Fits, Name der Funktion, Fitminimum (in x), Fitmaximum (in x), Anzahl an Parametern)
  fit_single_peak_fourT->SetNpx(500); //Number of points, default 100, better resolution
  // Parameterwerte und -bereiche festlegen
  Double_t a = 100000;
  Double_t a_min = 100;
  Double_t a_max = 10000000;
  Double_t mu = 350;
  Double_t mu_min = 0.;
  Double_t mu_max = 2000.;
  Double_t sig = 0.1;
  Double_t sig_min = 0.;
  Double_t sig_max = 2.;
  Double_t tau1 = 4.6;
  Double_t tau2 = 15.1;
  Double_t tau3 = 76.1;
  Double_t tau4 = 397.;
  Double_t p = 0.707;
  Double_t q = 0.205;
  Double_t r = 0.06;
  Double_t s = 0.028;
  fit_single_peak_fourT->SetParameters(a, mu, sig, tau1, tau2, tau3, tau4, p, q, r, s);//par = [a, mu, sig, tau1, tau2, tau3, tau4, p, q, r, s]
  fit_single_peak_fourT->SetParLimits(0, a_min, a_max);
  fit_single_peak_fourT->SetParLimits(1, mu_min, mu_max);
  fit_single_peak_fourT->SetParLimits(2, sig_min, sig_max);
  fit_single_peak_fourT->SetParLimits(3, tau1, tau1);
  fit_single_peak_fourT->SetParLimits(4, tau2, tau2);
  fit_single_peak_fourT->SetParLimits(5, tau3, tau3);
  fit_single_peak_fourT->SetParLimits(6, tau4, tau4);
  fit_single_peak_fourT->SetParLimits(7, p, p);
  fit_single_peak_fourT->SetParLimits(8, q, q);
  fit_single_peak_fourT->SetParLimits(9, r, r);
  fit_single_peak_fourT->SetParLimits(10, s, s);
  // Parameter benennen
  fit_single_peak_fourT->SetParName(0, "amplitude a");
  fit_single_peak_fourT->SetParName(1, "mu");
  fit_single_peak_fourT->SetParName(2, "sigma");
  fit_single_peak_fourT->SetParName(3, "tau 1");
  fit_single_peak_fourT->SetParName(4, "tau 2");
  fit_single_peak_fourT->SetParName(5, "tau 3");
  fit_single_peak_fourT->SetParName(6, "tau 4");
  fit_single_peak_fourT->SetParName(7, "ratio p");
  fit_single_peak_fourT->SetParName(8, "ratio q");
  fit_single_peak_fourT->SetParName(9, "ratio r");
  fit_single_peak_fourT->SetParName(10, "ratio s");


  ////////// Plotting //////////  

  fit_single_peak_fourT->SetLineWidth(4);
  h1->Fit(fit_single_peak_fourT, "+", "", fit_min, fit_max);

  Double_t ChiS = fit_single_peak_fourT->GetChisquare();
  Double_t NDF = fit_single_peak_fourT->GetNDF();
  Double_t Chi_NDF = fit_single_peak_fourT->GetChisquare()/fit_single_peak_fourT->GetNDF();
  cout << "chi^2 = " << ChiS << endl;
  cout << "NDF = " << NDF << endl;
  cout << "chi^2/NDF = " << Chi_NDF << endl;
  */
  //Canvasfenster fuellen
  h1->Draw();
  h1->SetStats(0);
  h1->SetLineWidth(3);
  // X-Achse
  h1->GetXaxis()->SetTitle("#lower[0.1]{#scale[1.2]{ Hittime [ns] }}");
  h1->GetXaxis()->CenterTitle();
  // Y-Achse
  h1->GetYaxis()->SetTitle("#scale[1.2]{ Events / (1. ns) }");
  h1->GetYaxis()->CenterTitle();
  h1->GetYaxis()->SetMaxDigits(3);
  // Legende
  TLegend *leg = new TLegend(0.7,0.8,0.9,0.9); //x1,y1,x2,y2 
  leg->AddEntry(h1, "Events", "f");
  //leg->AddEntry(fit_single_peak_fourT, "Fit", "l");
  leg->Draw();
  
  
  //c1->SaveAs("hittime_muon_fit.pdf");
}

