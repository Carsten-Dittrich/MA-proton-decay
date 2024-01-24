/*
STILL IN PROGRESS / WORK ONGOING
This macro uses the output txt files of the extractor.C macro as input
It plots the chi square values of the single and the double peak fit into 1 histogram
Executable via: root plotting.C
*/
#include <iostream>
#include <fstream>
#include <vector>
#include "TH1F.h"
#include "TCanvas.h"

void plotting() {
    // Datei einbinden
    const char* fileName = "chi_square_muon_edge_merged.txt";
    std::ifstream inputFile(fileName);

    if (!inputFile.is_open()) {
        std::cerr << "Fehler beim Öffnen der Datei: " << fileName << std::endl;
        return;
    }

    // histogramm fur "chi^2/NDF kaon" und "chi^2/NDF atmospheric"
    TH1F* histKaon = new TH1F("chi2Kaon", "chi^2/NDF Kaon", 150, 0, 15);
    TH1F* histAtmospheric = new TH1F("chi2Atmospheric", "chi^2/NDF Atmospheric", 150, 0, 15);

    std::string line;

    // Durchlaufe die Zeilen der Datei
    while (std::getline(inputFile, line)) {
        if (line.find("chi^2/NDF kaon = ") != std::string::npos) {
            // Extrahiere den Wert aus der Zeile
            double value;
            if (sscanf(line.c_str() + 18, "%lf", &value) == 1) {
                histKaon->Fill(value);
            }
        } else if (line.find("chi^2/NDF atmospheric = ") != std::string::npos) {
            // Extrahiere den Wert aus der Zeile
            double value;
            if (sscanf(line.c_str() + 24, "%lf", &value) == 1) {
                histAtmospheric->Fill(value);
            }
        }
    }

    // Schließe die Datei
    inputFile.close();

    // Erstelle ein Canvas und zeichne das Histogramm
    TCanvas* canvas = new TCanvas("canvas", "canvas", 1680, 1050);
    canvas->SetTitle("Kaon events");
    histKaon->SetLineColor(kBlue);
    //histKaon->GetYaxis()->SetRangeUser(0, 200);
    histKaon->Draw();
    // Beschrifte die Achsen
    histKaon->GetXaxis()->SetTitle("#chi^{2}/NDF");
    histKaon->GetYaxis()->SetTitle("Counts / (0.1 a.u.)");
    // Erstelle eine Legende
    TLegend* leg = new TLegend(0.7, 0.8, 0.9, 0.9);//x1,y1,x2,y2    
    leg->AddEntry(histKaon, Form("Double peak (Mean=%.2f)", histKaon->GetMean()), "l");
    leg->Draw();
    
    
    histAtmospheric->SetLineColor(kRed);
    histAtmospheric->Draw("SAME");// Damit beide Histogramme im selben Diagramm sind

    leg->AddEntry(histAtmospheric, Form("Single peak (Mean=%.2f)", histAtmospheric->GetMean()), "l");
    

    // Zeige das Diagramm im ROOT-Fenster
    canvas->Update();
    canvas->Draw();
}
