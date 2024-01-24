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

void residual_orig(){
  //gStyle->SetOptStat(0);
  TCanvas *c1 = new TCanvas("c1","c1",800,800);
  
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
  
  Double_t xmin = 0.3;
  Double_t xmax = 6;
  TF1 *f1 = new TF1("f1","[0]*TMath::Gaus(x,[1],[2])",xmin,xmax);
  f1->SetParameters(1,1.7,1.5);
  TH1F *h1 = new TH1F("h1","NGC1275",100,xmin,xmax);
  h1->GetYaxis()->SetLabelFont(63);
  h1->GetYaxis()->SetLabelSize(16);
  h1->FillRandom("f1",1000);
  h1->Fit("f1");
  
  pad2->cd();
  
  TH1F *h2 = new TH1F("h2","residuals",100,xmin,xmax);
  h2->GetXaxis()->SetLabelFont(63);
  h2->GetXaxis()->SetLabelSize(16);
  h2->GetXaxis()->SetTitle("channel energy (keV)");
  h2->GetYaxis()->SetLabelFont(63);
  h2->GetYaxis()->SetLabelSize(16);
  for (Int_t i=1;i<=100;i++) {
    Double_t diff = h1->GetBinContent(i)-f1->Eval(h1->GetBinCenter(i));
    h2->SetBinContent(i,diff);
  }
  
  h2->Draw();
  c1->cd();

}
