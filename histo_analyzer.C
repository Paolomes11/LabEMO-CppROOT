#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TLegend.h>
#include <iostream>

void histo_analyzer()
{
  TFile* file = TFile::Open("histograms.root");
  if (!file || file->IsZombie()) {
    std::cerr << "Error opening file!" << std::endl;
    return;
  }

  // get histograms
  TH1F* histograms[6];
  histograms[0] = (TH1F*)file->Get("histo_particles");
  histograms[1] = (TH1F*)file->Get("histo_azimutal");
  histograms[2] = (TH1F*)file->Get("histo_polar");
  histograms[3] = (TH1F*)file->Get("histo_impulse");
  histograms[4] = (TH1F*)file->Get("histo_transverse_impulse");
  histograms[5] = (TH1F*)file->Get("histo_energy");

  TH1F* histograms_invmass[6];
  histograms_invmass[0] = (TH1F*)file->Get("histo_invmass");
  histograms_invmass[1] = (TH1F*)file->Get("histo_invmass_disc");
  histograms_invmass[2] = (TH1F*)file->Get("histo_invmass_conc");
  histograms_invmass[3] = (TH1F*)file->Get("histo_invmass_Pi_K_disc");
  histograms_invmass[4] = (TH1F*)file->Get("histo_invmass_Pi_K_conc");
  histograms_invmass[5] = (TH1F*)file->Get("histo_invmass_Ks_prod");

  // Histos Entries
  Float_t HistoEntries[12];
  HistoEntries[0]  = histograms[0]->GetEntries();
  HistoEntries[1]  = histograms[1]->GetEntries();
  HistoEntries[2]  = histograms[2]->GetEntries();
  HistoEntries[3]  = histograms[3]->GetEntries();
  HistoEntries[4]  = histograms[4]->GetEntries();
  HistoEntries[5]  = histograms[5]->GetEntries();
  HistoEntries[6]  = histograms_invmass[0]->GetEntries();
  HistoEntries[7]  = histograms_invmass[1]->GetEntries();
  HistoEntries[8]  = histograms_invmass[2]->GetEntries();
  HistoEntries[9]  = histograms_invmass[3]->GetEntries();
  HistoEntries[10] = histograms_invmass[4]->GetEntries();
  HistoEntries[11] = histograms_invmass[5]->GetEntries();

  for (int i = 0; i < 7; ++i) {
    if (i < 6) {
      std::cout << "Histo " << histograms[i]->GetName() << " Entries: " << HistoEntries[i] << std::endl;
    } else {
      std::cout << "Histo " << histograms_invmass[i]->GetName() << " Entries: " << HistoEntries[i] << std::endl;
    }
  }
  std::cout << std::endl;

  // Histo_Particle Bin Contents
  Float_t BinEntries[7];
  BinEntries[0] = histograms[0]->GetBinContent(1);
  BinEntries[1] = histograms[0]->GetBinContent(2);
  BinEntries[2] = histograms[0]->GetBinContent(3);
  BinEntries[3] = histograms[0]->GetBinContent(4);
  BinEntries[4] = histograms[0]->GetBinContent(5);
  BinEntries[5] = histograms[0]->GetBinContent(6);
  BinEntries[6] = histograms[0]->GetBinContent(7);

  Float_t BinError[7];
  BinError[0] = histograms[0]->GetBinError(1);
  BinError[1] = histograms[0]->GetBinError(2);
  BinError[2] = histograms[0]->GetBinError(3);
  BinError[3] = histograms[0]->GetBinError(4);
  BinError[4] = histograms[0]->GetBinError(5);
  BinError[5] = histograms[0]->GetBinError(6);
  BinError[6] = histograms[0]->GetBinError(7);

  for (int i = 0; i < 7; ++i) {
    std::cout << "Bin 'i' content:" << BinEntries[i] << "\nBin 'i' error:" << BinError[i] << std::endl;
  }
  std::cout << std::endl;

  // Uniform Fits
  TF1* uniformFit1 =
      new TF1("uniformFit1", "pol0", histograms[1]->GetXaxis()->GetXmin(), histograms[1]->GetXaxis()->GetXmax());
  histograms[1]->Fit(uniformFit1, "R");
  TF1* uniformFit2 =
      new TF1("uniformFit2", "pol0", histograms[2]->GetXaxis()->GetXmin(), histograms[2]->GetXaxis()->GetXmax());
  histograms[2]->Fit(uniformFit2, "R");

  std::cout << "Results of Fit on Histogram histo_azimutal" << std::endl;
  std::cout << "Uniform Fit Parameter: " << uniformFit1->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << uniformFit1->GetChisquare() / uniformFit1->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << uniformFit1->GetProb() << '\n' << std::endl;

  std::cout << "Results of Fit on Histogram histo_polar" << std::endl;
  std::cout << "Uniform Fit Parameter: " << uniformFit2->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << uniformFit2->GetChisquare() / uniformFit2->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << uniformFit2->GetProb() << '\n' << std::endl;

  // Exponential FIts
  TF1* expoFit1 =
      new TF1("uniformFit1", "expo", histograms[3]->GetXaxis()->GetXmin(), histograms[3]->GetXaxis()->GetXmax());
  histograms[3]->Fit(expoFit1, "R");
  TF1* expoFit2 =
      new TF1("uniformFit2", "pol0", histograms[4]->GetXaxis()->GetXmin(), histograms[4]->GetXaxis()->GetXmax());
  histograms[4]->Fit(expoFit2, "R");

  std::cout << "Results of Fit on Histogram histo_impulse" << std::endl;
  std::cout << "Exponential Fit Tau: " << expoFit1->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << expoFit1->GetChisquare() / expoFit1->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << expoFit1->GetProb() << '\n' << std::endl;

  std::cout << "Results of Fit on Histogram histo_transverse_impulse" << std::endl;
  std::cout << "Exponential Fit Tau: " << expoFit2->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << expoFit2->GetChisquare() / expoFit2->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << expoFit2->GetProb() << '\n' << std::endl;

  // Inv_Mass Histos Bin control
  if (histograms_invmass[1]->GetNbinsX() != histograms_invmass[2]->GetNbinsX()
      || histograms_invmass[3]->GetNbinsX() != histograms_invmass[4]->GetNbinsX()) {
    std::cerr << "histograms_invmass have different binning!" << std::endl;
    return;
  }

  // Inv_Mass Histos Subtraction
  TH1F* result1_2 = (TH1F*)histograms_invmass[1]->Clone("result1");
  result1_2->Add(histograms_invmass[2], -1);

  TH1F* result3_4 = (TH1F*)histograms_invmass[3]->Clone("result3");
  result3_4->Add(histograms_invmass[4], -1);
  //Chi2 test
  Double_t chi2comparison = result1_2->Chi2Test(result3_4);
  //valutazione p-value(se non va bene che ne faccio, faccio esplodere il PC?) TO SEE
  // Gaussian Fits
  TF1* gaussianFit1_2 =
      new TF1("gaussianFit1_2", "gaus", result1_2->GetXaxis()->GetXmin(), result1_2->GetXaxis()->GetXmax());
  result3_4->Fit(gaussianFit1_2, "R");
  TF1* gaussianFit3_4 =
      new TF1("gaussianFit3_4", "gaus", result3_4->GetXaxis()->GetXmin(), result3_4->GetXaxis()->GetXmax());
  result3_4->Fit(gaussianFit3_4, "R");

  std::cout << "Results of Fit on Histogram 1 - 2" << std::endl;
  std::cout << "Mass of K*: " << gaussianFit1_2->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Width of K*: " << gaussianFit1_2->GetParameter(2) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << gaussianFit1_2->GetChisquare() / gaussianFit1_2->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << gaussianFit1_2->GetProb() << '\n' << std::endl;

  std::cout << "Results of Fit on Histogram 3 - 4" << std::endl;
  std::cout << "Mass of K*: " << gaussianFit3_4->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Width of K*: " << gaussianFit3_4->GetParameter(2) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << gaussianFit3_4->GetChisquare() / gaussianFit3_4->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << gaussianFit3_4->GetProb() << '\n' << std::endl;

  // Canvas
  TCanvas* canvas = new TCanvas("canvas", "Analisis of K*", 800, 600);
  result1_2->SetLineColor(kBlue);
  result3_4->SetLineColor(kRed);
  histograms_invmass[4]->SetLineColor(kGreen);

  result1_2->Draw();
  result3_4->Draw("SAME");
  histograms_invmass[5]->Draw("SAME");

  // Legend
  TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->AddEntry(result1_2, "Subtract (1-2)", "l");
  legend->AddEntry(result3_4, "Subtract (3-4)", "l");
  legend->AddEntry(histograms_invmass[5], "K* Decay", "l");
  legend->Draw();

  canvas->SaveAs("result_analisis.png");
}
