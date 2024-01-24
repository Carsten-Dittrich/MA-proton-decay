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

void RvsEdep(){
  // Öffnen der ROOT-Datei
  TFile* file = TFile::Open("/home/studente15/Carsten/masterthesis/data/hepevt_TopMuon_hepevt_TopMuon_TopRock_nooptical_gaus_broader_50k_0_user.root", "READ");

  // Laden der Bäume aus der Datei
  TTree* geninfoTree = (TTree*) file->Get("geninfo");
  TTree* evtTree = (TTree*) file->Get("evt");
  if (!geninfoTree || !evtTree) {
    printf("Die Bäume 'geninfo' oder 'evt' konnten nicht gefunden werden.\n");
    file->Close();
    return;
  }

  // Erstellen des TH2F Histogramms
  // TF2F(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup)
  TH2F* hist = new TH2F("hist", "", 100, 0, 10000, 200, 0, 20000);
  
  // Min und Max Wert von Interessensbereich
  float minE = 100.0;
  float maxE = 600.0;
  
  // Anzahl der Events mit edep=0 zählen
  Int_t numZeroEdep = 0;

  // Variablen zum Speichern der Branch-Daten
  Float_t InitX, InitY, edep;

  // Verknüpfen der Branches mit den Variablen
  geninfoTree->SetBranchAddress("InitX", &InitX);
  geninfoTree->SetBranchAddress("InitY", &InitY);
  evtTree->SetBranchAddress("edep", &edep);

  // Anzahl der Einträge im Tree
  Long64_t numEntries = geninfoTree->GetEntries();

  // Schleife über die Einträge im Tree
  for (Long64_t i = 0; i < numEntries; ++i) {
    // Eintrag im geninfo-Tree laden
    geninfoTree->GetEntry(i);
 
    // Eintrag im evt-Tree laden
    evtTree->GetEntry(i);

    // Nur Punkte plotten, bei denen edep nicht null ist
    //if (edep != 0) {
    if (edep > 50) {
      // Punkte in das Histogramm eintragen
      hist->Fill(edep, sqrt(InitX*InitX + InitY*InitY));
    }
    else {
      // Wenn edep=0, die Anzahl erhöhen
      numZeroEdep++;
    }
  }

  // Erstellen des Canvas XXXX 1920 × 1080
  TCanvas* canvas = new TCanvas("canvas", "Tree Plots", 1400, 1000);
  canvas->SetRightMargin(0.15);  // Increase the right margin
  //canvas->SetLogz();
  
  hist->SetStats(0);

  // X-Achse
  hist->GetXaxis()->SetTitle("#lower[0.2]{#scale[1.2]{E_{dep} (MeV) }}");
  hist->GetXaxis()->CenterTitle();
  //hist->GetXaxis()->SetMaxDigits(3);
  // Y-Achse
  hist->GetYaxis()->SetTitle("#scale[1.2]{ R_{xy} (mm) }");
  hist->GetYaxis()->CenterTitle();
  hist->GetYaxis()->SetMaxDigits(3);
  // Z-Achse
  hist->GetZaxis()->SetTitle("#scale[1.2]{ Events / (100 MeV #bullet 100 mm) }");
  hist->GetZaxis()->CenterTitle();
  
  // Zeichnen des Histogramms
  hist->Draw("colz");
  hist->SetContour(100);
  // Linien
  TLine* line1 = new TLine( minE, 0, minE, 20000);
  line1->SetLineWidth(2);
  line1->Draw();
  TLine* line2 = new TLine( maxE, 0, maxE, 20000);
  line2->SetLineWidth(2);
  line2->Draw();
  TLine* line3 = new TLine( 0, 17400, 10000, 17400);
  line3->SetLineWidth(2);
  line3->Draw();

  // Anzeigen des Plots
  canvas->Draw();
  /*// Legende; x1 y1 x2 y2
  TLegend *leg = new TLegend(0.8,0.8,0.9,0.9);
  leg->AddEntry(hist, "Events", "");
  leg->AddEntry(line1, "Borders of ROI: 0.2 GeV and 1 GeV", "l");
  leg->Draw();*/
  
  printf("Anzahl der Events mit edep=0: %d\n", numZeroEdep);
  canvas->SaveAs("RvsEdep_2000_less50.pdf");
}
