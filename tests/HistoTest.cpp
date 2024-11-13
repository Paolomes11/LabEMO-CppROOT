#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <TFile.h>
#include <TH1F.h>
#include <TMath.h>
#include <memory>

TEST_CASE("Tests for Histograms")
{
  std::unique_ptr<TFile> file = std::make_unique<TFile>("generated_files/histograms.root", "READ");
  if (!file || file->IsZombie()) {
    std::cerr << "Error opening file!" << std::endl;
    return;
  }

  // Inizializza gli istogrammi
  std::unique_ptr<TH1F> MyHist[12];
  MyHist[0]  = std::make_unique<TH1F>(*(TH1F*)file->Get("histo_particles"));
  MyHist[1]  = std::make_unique<TH1F>(*(TH1F*)file->Get("histo_azimutal"));
  MyHist[2]  = std::make_unique<TH1F>(*(TH1F*)file->Get("histo_polar"));
  MyHist[3]  = std::make_unique<TH1F>(*(TH1F*)file->Get("histo_impulse"));
  MyHist[4]  = std::make_unique<TH1F>(*(TH1F*)file->Get("histo_transverse_impulse"));
  MyHist[5]  = std::make_unique<TH1F>(*(TH1F*)file->Get("histo_energy"));
  MyHist[6]  = std::make_unique<TH1F>(*(TH1F*)file->Get("histo_invmass"));
  MyHist[7]  = std::make_unique<TH1F>(*(TH1F*)file->Get("histo_invmass_disc"));
  MyHist[8]  = std::make_unique<TH1F>(*(TH1F*)file->Get("histo_invmass_conc"));
  MyHist[9]  = std::make_unique<TH1F>(*(TH1F*)file->Get("histo_invmass_Pi_K_disc"));
  MyHist[10] = std::make_unique<TH1F>(*(TH1F*)file->Get("histo_invmass_Pi_K_conc"));
  MyHist[11] = std::make_unique<TH1F>(*(TH1F*)file->Get("histo_invmass_Ks_prod"));

  SUBCASE("Test for num of entries")
  {
    CHECK(abs(MyHist[0]->GetEntries() - 1E7 - 1E5) <= 1E5); // (1E7 + 1E5) +- 1E3

    for (int i = 1; i < 6; i++) {
      CHECK(MyHist[i]->GetEntries() == 1E7);
    }

    CHECK(4.950E8 <= MyHist[6]->GetEntries());
    CHECK(MyHist[6]->GetEntries() <= 5.778E8);

    for (int i = 1; i <= 2; i++) {
      CHECK(2.427E8 <= MyHist[6 + i]->GetEntries());
      CHECK(MyHist[6 + i]->GetEntries() <= 2.833E8);
      CHECK(4.227E7 <= MyHist[8 + i]->GetEntries());
      CHECK(MyHist[8 + i]->GetEntries() <= 4.934E7);
    }

    CHECK(99051 <= MyHist[11]->GetEntries());
    CHECK(MyHist[11]->GetEntries() <= 100949);
  }

  SUBCASE("Test for correct percentage")
  {
    int nBins          = MyHist[0]->GetNbinsX();
    double nEntries    = MyHist[0]->GetEntries();
    double prob[nBins] = {0.4, 0.4, 0.05, 0.05, 0.045, 0.045, 0.01};

    for (int i = 1; i <= nBins; ++i) {
      double iEntries  = MyHist[0]->GetBinContent(i);
      double iError    = 3 * MyHist[0]->GetBinError(i); // 3 for 99%
      double iRelative = iError / iEntries;
      CHECK(prob[i - 1] * nEntries == doctest::Approx(iEntries).epsilon(iRelative));
    }
  }

  file->Close();
}
