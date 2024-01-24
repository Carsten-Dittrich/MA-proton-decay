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

#include "../fit_functions.h"


void fourT_detsim(){
  //File Oeffnen
  auto f1 = TFile::Open("~/Carsten/masterthesis/data/muon/single/seed500/sample_detsim_user_mu_seed500.root", "READ");
  
  TCanvas *c1 = new TCanvas("hittime","hittime",1200,800);
  c1->SetTickx();
  c1->SetTicky();
  c1->SetGridx();
  c1->SetGridy();
  
  TPad *pad1 = new TPad("pad1","pad1",0,0.33,1,1);
  TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.33);
  
  pad1->SetBottomMargin(0.00001);
  pad1->SetBorderMode(0);
  pad1->SetLogy();
  pad2->SetTopMargin(0.00001);
  pad2->SetBottomMargin(0.1);
  pad2->SetBorderMode(0);
  pad1->Draw();
  pad2->Draw();
  
  pad1->cd();
  
  ////////// Histogramm //////////
  // Anzahl Bins
  int bins = 800;
  // Min und Max Wert
  float min = 0.0;
  float max = 800.0;
  // Histogramm
  TH1F* h1 = new TH1F("h1", "", bins, min, max);

  // HitTime Variable
  std::vector<float> ** hitTime = new std::vector<float> *;
  *hitTime = 0;
  TTree *tree = (TTree*) f1->Get("evt");// evt-Tree aus dem File holen
  tree->SetBranchAddress("hitTime", hitTime); // verknüpfe den Branch des Trees mit der zugehörigen Variable
  
  // Fuellen des Histograms
  int evtnr = tree->GetEntries();

  //Schleife über alle Events
  for (int i = 0; i < evtnr; i++){
    //hole den i-ten Eintrag aus dem Tree
    tree->GetEntry(i);
    int vectorsize = (*hitTime)->size();
    for (int j = 0; j < vectorsize; j++){
      h1->Fill((*hitTime)->at(j));
    }
  }

  ////////// FITTING //////////
  // Bounds to fit function
  Double_t fit_min = 95.;
  Double_t fit_max = 400.;

  TF1* fit_single_peak_fourT = new TF1("ftsingle_peak_fourT", single_peak_fourT, fit_min, fit_max, 11);
  //(Eigenname des Fits, Name der Funktion, Fitminimum (in x), Fitmaximum (in x), Anzahl an Parametern)
  fit_single_peak_fourT->SetNpx(500); //Number of points, default 100, better resolution
  // Parameterwerte und -bereiche festlegen
  Double_t a = 100000;
  Double_t a_min = 100;
  Double_t a_max = 10000000;
  Double_t mu = 100;
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
  Double_t p_min = 0.;
  Double_t p_max = 1.;
  Double_t q = 0.205;
  Double_t q_min = 0.;
  Double_t q_max = 1.;
  Double_t r = 0.06;
  Double_t r_min = 0.;
  Double_t r_max = 1.;
  Double_t s = 0.028;
  Double_t s_min = 0.;
  Double_t s_max = 1.;
  fit_single_peak_fourT->SetParameters(a, mu, sig, tau1, tau2, tau3, tau4, p, q, r, s);//par = [a, mu, sig, tau1, tau2, tau3, tau4, p, q, r, s]
  fit_single_peak_fourT->SetParLimits(0, a_min, a_max);
  fit_single_peak_fourT->SetParLimits(1, mu_min, mu_max);
  fit_single_peak_fourT->SetParLimits(2, sig_min, sig_max);
  fit_single_peak_fourT->SetParLimits(3, tau1, tau1);
  fit_single_peak_fourT->SetParLimits(4, tau2, tau2);
  fit_single_peak_fourT->SetParLimits(5, tau3, tau3);
  fit_single_peak_fourT->SetParLimits(6, tau4, tau4);
  fit_single_peak_fourT->SetParLimits(7, p_min, p_max);
  fit_single_peak_fourT->SetParLimits(8, p_min, p_max);
  fit_single_peak_fourT->SetParLimits(9, p_min, p_max);
  fit_single_peak_fourT->SetParLimits(10, p_min, p_max);
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
 
  //Canvasfenster fuellen
  h1->SetStats(0);
  h1->SetLineWidth(3);
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
  leg->AddEntry(fit_single_peak_fourT, "Fit", "l");
  leg->Draw();
  
  
  
  
  pad2->cd();
  
  TH1F *h2 = new TH1F("h2","residual",bins,min,max);
  h2->GetXaxis()->SetLabelFont(63);
  h2->GetXaxis()->SetLabelSize(16);
  h2->GetXaxis()->SetTitle("");
  h2->GetYaxis()->SetLabelFont(63);
  h2->GetYaxis()->SetLabelSize(16);
  
  for (Int_t i=1;i<=bins;i++) {
    Double_t counts = h1->GetBinContent(i);
    Double_t fit_counts = fit_single_peak_fourT->Eval(h1->GetBinCenter(i));
    
    if (fit_counts>1) {
      Double_t residual = (counts - fit_counts)/fit_counts;
      h2->SetBinContent(i,residual);
    }
    else {
      Double_t residual = 0;
      h2->SetBinContent(i,residual);
    }
    
  }
  
  h2->Draw();
  
  // Legende
  
  h2->SetStats(0);
  
  //TLegend *leg2 = new TLegend(0.7,0.7,0.9,0.9); //x1,y1,x2,y2 
  //leg2->AddEntry(h2, "Residuals", "f");
  //leg2->Draw();
  c1->cd();
  
  
  
  //c1->SaveAs("hittime_muon_fit.pdf");
}

