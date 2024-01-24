// Skript to extract only the hitTime-Branch
// Usage by: root -l -b 'RootBranchExtractor.C("INPUT.root","OUTPUT.root")'


#include "TFile.h"
#include "TTree.h"
#include <vector>

void RootBranchExtractor(const char* inputFileName, const char* outputFileName)
{
  // Input-Datei öffnen
  TFile* inputFile = new TFile(inputFileName, "READ");
  if (!inputFile || inputFile->IsZombie()) {
      printf("Fehler beim Öffnen der Eingabedatei '%s'\n", inputFileName);
      return;
  }

  // Input-Tree aus der Datei lesen
  TTree* inputTree = (TTree*) inputFile->Get("evt");
  if (!inputTree) {
    printf("Der Input-Tree 'evt' konnte nicht gefunden werden.\n");
    inputFile->Close();
    return;
  }

  // Variable für den HitTime-Zweig erstellen
  std::vector<double>* hitTime = nullptr;
  int evtID;

  // Input-Branch mit dem HitTime-Namen verknüpfen
  inputTree->SetBranchAddress("hitTime", &hitTime);
  inputTree->SetBranchAddress("evtID", &evtID);

  // Output-Datei erstellen und den Input-Tree kopieren
  TFile* outputFile = new TFile(outputFileName, "RECREATE");
  TTree* outputTree = new TTree("evt", "Event Tree");

  // Output-Branch mit dem HitTime-Namen erstellen und an den Output-Tree anschließen
  TBranch* outputBranchhitTime = outputTree->Branch("hitTime", &hitTime);
  TBranch* outputBranchevtID = outputTree->Branch("evtID", &evtID);

  // Anzahl der Einträge im Input-Tree
  Long64_t numEntries = inputTree->GetEntries();

  // Schleife über die Einträge im Input-Tree
  for (Long64_t i = 0; i < numEntries; ++i) {
    // Eintrag im Input-Tree laden
    inputTree->GetEntry(i);

    // Eintrag in den Output-Tree schreiben
    outputTree->Fill();
  }

  // Output-Tree in die Output-Datei schreiben
  outputTree->Write();

  // Dateien schließen
  outputFile->Close();
  inputFile->Close();
}