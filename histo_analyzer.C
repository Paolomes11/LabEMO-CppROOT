#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TLegend.h>
#include <iostream>

// TO SEE INCLUDES

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

  // Histos entries
  for (int i = 0; i < 12; ++i) {
    if (i < 6) {
      std::cout << "Histo " << histograms[i]->GetName() << " entries: " << histograms[i]->GetEntries() << std::endl;
    } else {
      std::cout << "Histo " << histograms_invmass[i - 6]->GetName()
                << " entries: " << histograms_invmass[i - 6]->GetEntries() << std::endl;
    }
  }
  std::cout << std::endl;

  // Histo_Particle Bin Contents
  std::cout << "Histo histo_particles bin entries" << std::endl;
  for (int i = 1; i < 8; ++i) {
    std::cout << "Bin " << i << " content:" << histograms[0]->GetBinContent(i) << "\nBin " << i
              << " error:" << histograms[0]->GetBinError(i) << std::endl;
  }
  std::cout << std::endl;

  // Uniform Fits
  TF1* uniformFit1 =
      new TF1("uniformFit1", "pol0", histograms[1]->GetXaxis()->GetXmin(), histograms[1]->GetXaxis()->GetXmax());
  histograms[1]->Fit(uniformFit1, "R");
  TF1* uniformFit2 =
      new TF1("uniformFit1", "pol0", histograms[1]->GetXaxis()->GetXmin(), histograms[1]->GetXaxis()->GetXmax());
  histograms[1]->Fit(uniformFit2, "R");

  std::cout << "Results of Fit on Histogram histo_azimutal" << std::endl;
  std::cout << "Uniform Fit Parameter: " << uniformFit1->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << uniformFit1->GetChisquare() / uniformFit1->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << uniformFit1->GetProb() << '\n' << std::endl;

  std::cout << "Results of Fit on Histogram histo_polar" << std::endl;
  std::cout << "Uniform Fit Parameter: " << uniformFit2->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << uniformFit2->GetChisquare() / uniformFit2->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << uniformFit2->GetProb() << '\n' << std::endl;

  // Exponential FIts
  TF1* expoFit =
      new TF1("uniformFit1", "expo", histograms[3]->GetXaxis()->GetXmin(), histograms[3]->GetXaxis()->GetXmax());
  histograms[3]->Fit(expoFit, "R");

  std::cout << "Results of Fit on Histogram histo_impulse" << std::endl;
  std::cout << "Exponential Fit Tau: " << expoFit->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << expoFit->GetChisquare() / expoFit->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << expoFit->GetProb() << '\n' << std::endl;

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

  // Chi2 test
  Double_t chi2comparison_1 =
      result1_2->Chi2Test(result3_4, "CHI2/NDF"); // valutazione p-value(se non va bene che ne faccio, faccio esplodere
                                                  // il PC?) TO SEE (ora restituisce il Chi2/ndf)
  Double_t chi2comparison_2 = result1_2->Chi2Test(histograms_invmass[5], "CHI2/NDF");
  Double_t chi2comparison_3 = result3_4->Chi2Test(histograms_invmass[5], "CHI2/NDF");

  std::cout << "Chi Squared Test between Difference Concordant and Discordant Particle Invariant Mass and Difference "
               "Concorde and Discord Particle Pi-K Invariant Mass:"
            << chi2comparison_1 << std::endl;
  std::cout << "Chi Squared Test between Difference Concordant and Discordant Particle Invariant Mass and K* products "
               "Invariant Mass:"
            << chi2comparison_1 << std::endl;
  std::cout
      << "Chi Squared Test between Difference Concordant and Discordant Particle Pi-K Invariant Mass and K* products "
         "Invariant Mass:"
      << chi2comparison_1 << std::endl;

  //  Gaussian Fits
  TF1* gaussianFit1_2 =
      new TF1("gaussianFit1_2", "gaus", result1_2->GetXaxis()->GetXmin(), result1_2->GetXaxis()->GetXmax());
  result3_4->Fit(gaussianFit1_2, "R");
  TF1* gaussianFit3_4 =
      new TF1("gaussianFit3_4", "gaus", result3_4->GetXaxis()->GetXmin(), result3_4->GetXaxis()->GetXmax());
  result3_4->Fit(gaussianFit3_4, "R");

  std::cout << "Results of Fit on Histogram Difference Concordant and Discordant Particle Invariant Mass" << std::endl;
  std::cout << "Mass of K*: " << gaussianFit1_2->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Width of K*: " << gaussianFit1_2->GetParameter(2) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << gaussianFit1_2->GetChisquare() / gaussianFit1_2->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << gaussianFit1_2->GetProb() << '\n' << std::endl;

  std::cout << "Results of Fit on Histogram Difference Concordant and Discordant Particle Pi-K Invariant Mass"
            << std::endl;
  std::cout << "Mass of K*: " << gaussianFit3_4->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Width of K*: " << gaussianFit3_4->GetParameter(2) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << gaussianFit3_4->GetChisquare() / gaussianFit3_4->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << gaussianFit3_4->GetProb() << '\n' << std::endl;

  // Canvas1
  TCanvas* canvas1 = new TCanvas("canvas1", "Particle Types", 800, 600);
  histograms[0]->SetLineColor(kRed);
  histograms[0]->Draw();
  canvas1->SaveAs("particles_distribution.png");

  // Canvas2
  TCanvas* canvas2 = new TCanvas("canvas2", "Azimutal Angle", 800, 600);
  canvas2->Divide(2, 1);
  histograms[1]->SetLineColor(kRed);
  histograms[1]->Draw();
  histograms[1]->SetTitle("Azimutal Angle Distribution Fit");
  histograms[1]->Draw();
  uniformFit1->SetLineColor(kBlue);
  uniformFit1->Draw("SAME");
  canvas2->SaveAs("azimutal_distribution.png");

  // Canvas3
  TCanvas* canvas3 = new TCanvas("canvas3", "Polar Angle", 800, 600);
  canvas2->Divide(2, 1);
  histograms[2]->SetLineColor(kRed);
  histograms[2]->Draw();
  histograms[2]->SetTitle("Polar Angle Distribution Fit");
  histograms[2]->Draw();
  uniformFit2->SetLineColor(kBlue);
  uniformFit2->Draw("SAME");
  canvas3->SaveAs("polar_distribution.png");

  // Canvas4
  TCanvas* canvas4 = new TCanvas("canvas4", "Particle Impulses", 800, 600);
  canvas2->Divide(2, 1);
  histograms[3]->SetLineColor(kRed);
  histograms[3]->Draw();
  histograms[3]->SetTitle("Impulse Distribution Fit");
  histograms[3]->Draw();
  expoFit->SetLineColor(kBlue);
  expoFit->Draw("SAME");
  canvas4->SaveAs("impulse_distribution.png");

  // Canvas5
  TCanvas* canvas5 = new TCanvas("canvas5", "Particle Transverse Impulses", 800, 600);
  histograms[4]->SetLineColor(kRed);
  histograms[4]->Draw();
  canvas5->SaveAs("transverse_impulse_distribution.png");

  // Canvas6
  TCanvas* canvas6 = new TCanvas("canvas6", "Particle Energies", 800, 600);
  histograms[5]->SetLineColor(kRed);
  histograms[5]->Draw();
  canvas6->SaveAs("energy_distribution.png");

  // Canvas7
  TCanvas* canvas7 = new TCanvas("canvas7", "Particle Invariant Masses", 800, 600);
  histograms_invmass[0]->SetLineColor(kRed);
  histograms_invmass[0]->Draw();
  canvas7->SaveAs("invmass_distribution.png");

  // Canvas8
  TCanvas* canvas8 = new TCanvas("canvas8", "Particle Concordant and Discordant Invariant Masses", 800, 600);
  canvas2->Divide(2, 1);
  histograms_invmass[1]->SetLineColor(kRed);
  histograms_invmass[1]->Draw();
  histograms_invmass[2]->SetLineColor(kGreen);
  histograms_invmass[2]->Draw();
  canvas4->SaveAs("conc_disc_invmass_distribution.png");

  // Canvas9
  TCanvas* canvas9 = new TCanvas("canvas9", "Particle Concordant and Discordant Pi-K Invariant Masses", 800, 600);
  canvas2->Divide(2, 1);
  histograms_invmass[3]->SetLineColor(kRed);
  histograms_invmass[3]->Draw();
  histograms_invmass[4]->SetLineColor(kGreen);
  histograms_invmass[4]->Draw();
  canvas4->SaveAs("conc_disc_PiK_invmass_distribution.png");

  // Canvas10
  TCanvas* canvas10 = new TCanvas("canvas10", "Analisis of K*", 800, 600);
  canvas2->Divide(3, 1);
  result1_2->SetLineColor(kBlue);
  result1_2->SetTitle("Difference Concordant and Discordant Particle Invariant Mass");
  result1_2->Draw();
  gaussianFit1_2->Draw("SAME");
  result3_4->SetLineColor(kRed);
  result3_4->SetTitle("Difference Concordant and Discordant Particle Pi-K Invariant Mass");
  result3_4->Draw();
  gaussianFit3_4->Draw("SAME");
  histograms_invmass[5]->SetLineColor(kGreen);
  histograms_invmass[5]->Draw();
  TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->AddEntry(result1_2, "Subtract (1-2)", "l");
  legend->AddEntry(result3_4, "Subtract (3-4)", "l");
  legend->AddEntry(histograms_invmass[5], "K* Decay", "l");
  legend->Draw();
  canvas10->SaveAs("Ks_distribution_analysis.png");
}
