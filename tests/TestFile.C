#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include <iostream>

int TestFile()
{
  // Open file.root
  TFile* file = new TFile("histograms.root");
  if (!file || file->IsZombie()) {
    std::cerr << "Errore: impossibile aprire il file ROOT." << std::endl;
    return 1;
  }

  // Initialize histograms
  TH1F* MyHist[12];
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

  // Test if histograms are not null
  for (Int_t i = 0; i < 12; i++) {
    if (MyHist[i] == nullptr) {
      std::cerr << "Errore: impossibile leggere l'istogramma " << i << std::endl;
      return 1;
    }
  }

  // Crae il canvas
  TCanvas* myCanvas = new TCanvas("myCanvas", "All histograms", 0, 0, 1600, 1200);
  myCanvas->Divide(4, 3);

  for (Int_t i = 0; i < 12; i++) {
    myCanvas->cd(i + 1);
    MyHist[i]->Draw();
  }

  // Save the file as a gif
  myCanvas->Print("Histograms.gif");

  gPad->WaitPrimitive();

  // Close file
  file->Close();

  return 0;
}