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

void edep(){
  //File Oeffnen
  auto f1 = TFile::Open("/home/studente15/Carsten/masterthesis/data/hepevt_TopMuon_TopRock_nooptical_gaus_50k_user.root", "READ");

  ////////// Histogramm //////////
  // Anzahl Bins
  int bins = 200;
  // Min und Max Wert
  float min = 0.0;
  float max = 10000.0;
  // Min und Max Wert von Interessensbereich
  float minE = 200.0;
  float maxE = 1000.0;
  // Histogramm
  TH1F* h1 = new TH1F("h1", "", bins, min, max);

  float edep;
  TTree *tree = (TTree*) f1->Get("evt");// evt-Tree aus dem File holen
  tree->SetBranchAddress("edep", &edep);// verknüpfe den Branch des Trees mit der zugehörigen Variable
  
  ///// Fuellen des Histograms /////
  int evtnr = tree->GetEntries();
  // Schleife über alle Events
  for (int i = 0; i < evtnr; i++){
    //hole den i-ten Eintrag aus dem Tree
    tree->GetEntry(i);
    h1->Fill(edep);
  }
  
  ////////// Plotting //////////
  //Canvas erzeugen
  auto c1=new TCanvas("edep","edep",1680,1050);
  c1->SetTickx();
  c1->SetTicky();
  c1->SetGridx();
  c1->SetGridy();
  //c1->SetLogy();

  //Canvasfenster fuellen
  h1->SetStats(0);
  h1->SetLineWidth(2);
  // X-Achse
  h1->GetXaxis()->SetTitle("#lower[0.1]{#scale[1.2]{ Deposited Energy (MeV) }}");
  h1->GetXaxis()->CenterTitle();
  // Y-Achse
  h1->GetYaxis()->SetTitle("#scale[1.2]{ Events / (50 MeV) }");
  h1->GetYaxis()->CenterTitle();
  h1->GetYaxis()->SetMaxDigits(3);
  h1->GetYaxis()->SetRangeUser(0,320);
  
  h1->Draw("p E1");
  // Schattierung
  TH1F* shadedHist = (TH1F*)h1->Clone("shadedHist");
  for (int bin = 1; bin <= shadedHist->GetNbinsX(); ++bin) {
    double binCenter = shadedHist->GetBinCenter(bin);
    if (binCenter < minE || binCenter > maxE) {
        shadedHist->SetBinContent(bin, 0);
    }
  }
  shadedHist->GetXaxis()->SetRangeUser(minE, maxE);
  shadedHist->SetFillColor(kGreen-6);
  shadedHist->SetLineWidth(0);
  shadedHist->SetFillStyle(3001);
  
  shadedHist->Draw("same");
  
  // Linien
  TLine* line1 = new TLine( minE, 0, minE, 320);
  line1->SetLineWidth(2);
  line1->Draw();
  TLine* line2 = new TLine( maxE, 0, maxE, 320);
  line2->SetLineWidth(2);
  line2->Draw();
  // Legende
  TLegend *leg = new TLegend(0.68,0.8,0.9,0.9);
  leg->AddEntry(h1, "Events", "PE");
  leg->AddEntry(shadedHist, "Events with 0.2 GeV < E_{dep} < 1 GeV", "f");
  leg->AddEntry(line1, "Borders of ROI: 0.2 GeV and 1 GeV", "l");
  leg->Draw();

  //c1->SaveAs("edep_10000.pdf");
}
