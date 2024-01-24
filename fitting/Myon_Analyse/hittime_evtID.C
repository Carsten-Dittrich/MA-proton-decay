//// Skript um Root file mit mehreren Events auszulesen und die einzelnen HitTimes zu plotten
//// Ausführbar durch bspw.: root hitime_evtID.C


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

void hittime_evtID(){
  // File Oeffnen
  TFile* file = TFile::Open("/home/studente15/Carsten/masterthesis/data/hepevt_TopMuon_TopRock_scint001_noCerenkov_gaus_17m_30_0_user.root", "READ");
  //TFile* file = new TFile(inputFilePath.c_str(),"READ");
  // evt-Tree aus dem File holen
  TTree *tree = (TTree*) file->Get("evt");
  // Anzahl events ermitteln
  int evtnr = tree->GetEntries();  
  cout << "evtnr = " << evtnr << endl;

  // Loop over evtIDs
  for (int evtID = 0; evtID < evtnr; evtID++) {
    // Create Canvas
    TCanvas* c1 = new TCanvas("c1", "c1", 1680, 1050);
    ///// Histogramm /////
    // Anzahl Bins
    int bins = 1000;
    // Min und Max Wert
    float min = 0.0;
    float max = 1000.0;
    TH1F* h1 = new TH1F("h1", "", bins, min, max);
    // h1 füllen für einzelne events
    tree->Draw("hitTime>>h1", Form("evtID == %d", evtID));

    ////////// Plotting //////////  

    h1->SetStats(0);
    h1->SetLineWidth(3);
  //h1->SetTitle("quenching changed minimal");
    // X-Achse
    h1->GetXaxis()->SetTitle("#lower[0.1]{#scale[1.2]{ Hittime [ns] }}");
    h1->GetXaxis()->CenterTitle();
    // Y-Achse
    h1->GetYaxis()->SetTitle("#scale[1.2]{ Events / (0.5 ns) }");
    h1->GetYaxis()->CenterTitle();
    h1->GetYaxis()->SetMaxDigits(3);
    // Legende
    TLegend *leg = new TLegend(0.7,0.85,0.9,0.9); //x1,y1,x2,y2 
    leg->AddEntry(h1, "Events", "f");
    leg->Draw();

    c1->SaveAs(Form("hitTime_evtID%d_17m.png", evtID));
    
    c1->Clear();
  }
  
}       
