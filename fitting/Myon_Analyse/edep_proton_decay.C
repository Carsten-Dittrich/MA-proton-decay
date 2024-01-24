/* 
This macro takes detsim_user.root files as input
It plots the deposited energy
Executable via: root edep_proton_decay.C
*/
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

void edep_proton_decay() {
  // Erstelle ein TCanvas
  TCanvas *canvas = new TCanvas("canvas", "Edep Histogram", 1680, 1050);

  // number of bins
  int bins = 100;
  // binning range
  float min = 0.0;
  float max = 1000.0;
  // creating the histogram
  TH1F *h1 = new TH1F("edep", "Energy Deposited", bins, min, max);

  // analyze the different ROOT-files
  for (int i = 0; i <= 0; i++) {
    // Creating the file name
    TString fileName = Form("/home/studente15/Carsten/masterthesis/data/kaon/sample_detsim_user_random_100_kaon_%d.root", i);
    // open the file
    TFile *file = new TFile(fileName, "READ"); // Öffne die ROOT-Datei zum Lesen

    // Does the file exist
    if (!file || file->IsZombie()) {
      std::cerr << "Error when opening file: " << fileName << std::endl;
      continue; // Falls ein Fehler auftritt, gehe zur nächsten Datei
    }

    // get the evt-tree from the file
    TTree *tree = (TTree *)file->Get("evt");
    // Does the tree exist
    if (!tree) {
      std::cerr << "The evt doesn't exist for the file: " << fileName << std::endl;
      file->Close();
      continue;
    }

    // edep variable
    float edep;

    // link the tree's branch to the corresponding variable
    tree->SetBranchAddress("edep", &edep);
        
    // Fill the histogram
    int numEntries = tree->GetEntries();
    for (int entry = 0; entry < numEntries; entry++) {
      tree->GetEntry(entry);
      h1->Fill(edep);
    }
        
    file->Close();
  }

  // draw the histogram
  h1->Draw();
  //h1->Draw("p E1");
  // Add lines
  TLine* line10Percent = new TLine(100, 0, 100, 310);
  line10Percent->SetLineColor(kBlack);
  line10Percent->SetLineWidth(3);
  line10Percent->Draw();
    
  TLine* line95Percent = new TLine(600, 0, 600, 310);
  line95Percent->SetLineColor(kBlack);
  line95Percent->SetLineWidth(3);
  line95Percent->Draw();
    
  h1->SetTitle("");
  h1->SetStats(0);
  h1->SetLineWidth(2);
  // x-axis
  h1->GetXaxis()->SetTitle("#lower[0.1]{#scale[1.2]{ Deposited Energy (MeV) }}");
  h1->GetXaxis()->CenterTitle();
  // y-axis
  h1->GetYaxis()->SetTitle("#scale[1.2]{ Events / (10 MeV) }");
  h1->GetYaxis()->CenterTitle();
  h1->GetYaxis()->SetMaxDigits(3);
  h1->GetYaxis()->SetRangeUser(0,310);
  // Legende
  TLegend *leg = new TLegend(0.7,0.8,0.9,0.9); //x1,y1,x2,y2 
  leg->AddEntry(h1, "Events", "f");
  leg->AddEntry(line10Percent, "Cut Values", "l");
  leg->Draw();
  // show canvas
  canvas->Update();
  canvas->Modified();
  //canvas->SaveAs("edep_histogram.pdf");
}
