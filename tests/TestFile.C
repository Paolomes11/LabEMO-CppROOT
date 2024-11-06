#include "TApplication.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "iostream"

int TestFile()
{
  // Apri il file ROOT
  TFile* file = new TFile("histograms.root");

  TH1F* MyHist[12];

  // Leggi l'istogramma dal file
  MyHist[0]  = (TH1F*)file->Get("histo_particles");
  MyHist[1]  = (TH1F*)file->Get("histo_azimutal");
  MyHist[2]  = (TH1F*)file->Get("histo_polar");
  MyHist[3]  = (TH1F*)file->Get("histo_impulse");
  MyHist[4]  = (TH1F*)file->Get("histo_transverse_impulse");
  MyHist[5]  = (TH1F*)file->Get("histo_energy");
  MyHist[6]  = (TH1F*)file->Get("histo_invmass");
  MyHist[7]  = (TH1F*)file->Get("histo_invmass_disc");
  MyHist[8]  = (TH1F*)file->Get("histo_invmass_conc");
  MyHist[9]  = (TH1F*)file->Get("histo_invmass_Pi_K_disc");
  MyHist[10] = (TH1F*)file->Get("histo_invmass_Pi_K_conc");
  MyHist[11] = (TH1F*)file->Get("histo_invmass_Ks_prod");

  // Verifica se l'istogramma è stato letto correttamente
  for (Int_t i = 0; i < 12; i++) {
    if (MyHist[i] == nullptr) {
      std::cerr << "Errore: impossibile leggere l'istogramma" << i << std::endl;
      return 1;
    }
  }

  TCanvas* myCanvas = new TCanvas("myCanvas", "All histograms", 0, 0, 1600, 1200);
  myCanvas->Divide(4, 3);

  for (Int_t i = 0; i < 12; i++) {
    myCanvas->cd(i + 1);
    MyHist[i]->Draw();
  }

  myCanvas->Print("Histograms.gif");

  // Mantieni la finestra aperta fino a quando non viene chiusa dall'utente
  // gApplication->Run();

  // Chiudi il file
  file->Close();

  return 0;
}