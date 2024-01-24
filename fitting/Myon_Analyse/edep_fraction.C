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

void edep_fraction(){
  // File Oeffnen
  auto f1 = TFile::Open("/home/studente15/Carsten/masterthesis/data/hepevt_TopMuon_hepevt_TopMuon_TopRock_nooptical_gaus_broader_50k_0_user.root", "READ");

  ////////// Histogramm //////////
  // Anzahl Bins
  int bins = 40;//200
  // Min und Max Wert
  float min = 0.0;
  float max = 2000.0;//10000
  // Min und Max Wert von Interessensbereich
  float minE = 100.0;
  float maxE = 600.0;
  // Histogramm
  TH1F* h1 = new TH1F("h1", "", bins, min, max);

  // edep Variable
  float edep;
  TTree *tree = (TTree*) f1->Get("evt");// evt-Tree aus dem File holen
  tree->SetBranchAddress("edep", &edep); // verknüpfe den Branch des Trees mit der zugehörigen Variable
  
  ///// Fuellen des Histograms /////
  int evtnr = tree->GetEntries();
  
  // Variablen
  Int_t numZeroEdep = 0;
  Int_t numInterestedEdep = 0;
  
  // Schleife über alle Events
  for (int i = 0; i < evtnr; i++){
    // hole den i-ten Eintrag aus dem Tree
    tree->GetEntry(i);
    // Nur Punkte plotten, bei denen edep nicht null ist sonst numZeroEdep erhöhen
    if (edep != 0) {
      h1->Fill(edep);
    }
    else {
      numZeroEdep++;
    }    
  }
  
  ////////// Calculating muon fraction //////////
  
  // Schleife über alle Events
  for (int i = 0; i < evtnr; i++){
    // hole den i-ten Eintrag aus dem Tree
    tree->GetEntry(i);
    // Nur Punkte plotten, bei denen edep nicht null ist
    if (edep > minE && edep < maxE) {
      numInterestedEdep++;
    } 
  }
  
  Int_t numTotalEvt = 50000 - numZeroEdep;
  double RatioRange = (double)numInterestedEdep/(double)numTotalEvt * 100;
  
  printf("Number of events with edep=0: %d\n", numZeroEdep);
  printf("Number of events with edep>0: %d\n", numTotalEvt);
  printf("Number of events inside the range: %d\n", numInterestedEdep);  
  cout << "Ratio of events inside the range: " << RatioRange << "%" << endl;
  
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
  //h1->SetFillColor(kGreen-9);
  // X-Achse
  h1->GetXaxis()->SetTitle("#lower[0.2]{#scale[1.2]{E_{dep} (MeV) }}");
  h1->GetXaxis()->CenterTitle();
  // Y-Achse
  h1->GetYaxis()->SetTitle("#scale[1.2]{ Events / (50 MeV) }");
  h1->GetYaxis()->CenterTitle();
  h1->GetYaxis()->SetMaxDigits(1);
  h1->GetYaxis()->SetRangeUser(0,400);
  h1->Draw("pe1");
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
  TLine* line1 = new TLine( minE, 0, minE, 400);
  line1->SetLineWidth(2);
  line1->Draw();
  TLine* line2 = new TLine( maxE, 0, maxE, 400);
  line2->SetLineWidth(2);
  line2->Draw();
  // Legende; x1 y1 x2 y2
  TLegend *leg = new TLegend(0.6,0.75,0.9,0.9);
  leg->AddEntry(h1, "Events", "pe");
  leg->AddEntry(shadedHist, "Events with 0.1 GeV < E_{dep} < 0.6 GeV ", "f");
  leg->AddEntry(line1, "Borders of ROI: 0.1 GeV and 0.6 GeV ", "l");
  leg->AddEntry((TObject*)0, TString::Format("Ratio ROI/Total:  %.2f %%", RatioRange), "");
  leg->Draw();

  c1->SaveAs("edep_frac_2000.pdf");
}
        
