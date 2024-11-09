#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "TFile.h"
#include "TH1F.h"
#include "doctest.h"

TEST_CASE("Tests for Histograms")
{
  TFile* file = TFile::Open("generated_files/histograms.root");
  if (!file || file->IsZombie()) {
    std::cerr << "Error opening file!" << std::endl;
    return;
  }

  // Inizializza gli istogrammi
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

  SUBCASE("Test for num of entries")
  {
    CHECK(abs(MyHist[0]->GetEntries() - 1E7 - 2*1E5) < 1E5); // STATIC CAST?
    for (int i = 1; i < 6; i++) {
      CHECK(MyHist[i]->GetEntries() == 1E7);
    }
  }

  file->Close();
}
