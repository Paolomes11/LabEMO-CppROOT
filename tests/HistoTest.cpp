#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TMath.h>

TEST_CASE("Tests for Histograms")
{
  TFile* file = TFile::Open("generated_files/histograms.root");
  if (!file || file->IsZombie()) {
    std::cerr << "Error opening file!" << std::endl;
    return;
  }

  // Initialize histograms
  TString names[12] = {"histo_particles",         "histo_azimutal",           "histo_polar",
                       "histo_impulse",           "histo_transverse_impulse", "histo_energy",
                       "histo_invmass",           "histo_invmass_disc",       "histo_invmass_conc",
                       "histo_invmass_Pi_K_disc", "histo_invmass_Pi_K_conc",  "histo_invmass_Ks_prod"};
  TH1F* MyHist[12];
  for (int i = 0; i < 12; i++) {
    MyHist[i] = (TH1F*)file->Get(names[i]);
  }

  SUBCASE("Test for num of entries")
  {
    CHECK(abs(MyHist[0]->GetEntries() - 1E7 - 1E5) <= 1E5); // (1E7 + 1E5) +- 1E5

    for (int i = 1; i < 6; i++) {
      CHECK(MyHist[i]->GetEntries() == 1E7);
    }

    CHECK(4.950E8 <= MyHist[6]->GetEntries());
    CHECK(MyHist[6]->GetEntries() <= 5.778E8);

    for (int i = 1; i <= 2; i++) {
      CHECK(2.428E8 <= MyHist[6 + i]->GetEntries());
      CHECK(MyHist[6 + i]->GetEntries() <= 2.834E8);
      CHECK(4.229E7 <= MyHist[8 + i]->GetEntries());
      CHECK(MyHist[8 + i]->GetEntries() <= 4.936E7);
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

  SUBCASE("Testing the mean of the Exponential Impulse Distribution")
  {
    TF1* expo_fit = new TF1("exponentialFit", "expo", MyHist[3]->GetBinLowEdge(MyHist[3]->FindFirstBinAbove(0)),
                            MyHist[3]->GetXaxis()->GetBinUpEdge(MyHist[3]->FindLastBinAbove(0)));
    MyHist[3]->Fit(expo_fit, "R");

    double mean       = expo_fit->GetParameter(1);
    double mean_error = expo_fit->GetParError(1);

    CHECK(abs(mean + 1) < mean_error);
  }

  SUBCASE("Testing the Maximum point of the Difference Histograms for K* mass")
  {
    TF1* gaus_fit =
        new TF1("gaussianFit", "gaus", MyHist[11]->GetXaxis()->GetXmin(), MyHist[11]->GetXaxis()->GetXmax());
    MyHist[11]->Fit(gaus_fit, "R");

    double mean       = gaus_fit->GetParameter(1);
    double mean_error = gaus_fit->GetParError(1);
    double sigma      = gaus_fit->GetParameter(2);

    CHECK(abs(mean - 0.89166) < mean_error * 3); // 3 for 99%

    Int_t max       = MyHist[11]->GetMaximumBin();
    double low_edge = MyHist[11]->GetXaxis()->GetBinLowEdge(max);
    double max_edge = MyHist[11]->GetXaxis()->GetBinUpEdge(max);
    CHECK(0.89166 > low_edge - 3 * sigma);
    CHECK(0.89166 < max_edge + 3 * sigma);
  }

  file->Close();
}
