#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TLegend.h>
#include <iostream>

// TO SEE INCLUDES

void histo_analyzer()
{
  TFile* file = TFile::Open("generated_files/histograms.root");
  if (!file || file->IsZombie()) {
    std::cerr << "Error opening file!" << std::endl;
    return;
  }

  // Some starter values
  TString names[6]    = {"histo_particles", "histo_azimutal",           "histo_polar",
                         "histo_impulse",   "histo_transverse_impulse", "histo_energy"};
  TString invnames[6] = {"histo_invmass",           "histo_invmass_disc",      "histo_invmass_conc",
                         "histo_invmass_Pi_K_disc", "histo_invmass_Pi_K_conc", "histo_invmass_Ks_prod"};

  // Get histograms
  TH1F* histograms[6];
  TH1F* histograms_invmass[6];

  for (int i = 0; i < 6; ++i) {
    histograms[i]         = (TH1F*)file->Get(names[i]);
    histograms_invmass[i] = (TH1F*)file->Get(invnames[i]);
  }

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
    std::cout << "Bin " << i << " content: " << histograms[0]->GetBinContent(i) << "\nBin " << i
              << " error :" << histograms[0]->GetBinError(i) << std::endl;
  }
  std::cout << std::endl;

  // Uniform and Exponential Fits
  TF1* Fits[2]; // 1-2 uniform, 3 exponential
  TString Fits_name[3] = {"uniformFit1", "uniformFit2", "exponentialFit"};

  for (int i = 0; i < 3; i++) {
    if (i < 2) {
      Fits[i] = new TF1(Fits_name[i], "pol0",
                        histograms[i + 1]->GetXaxis()->GetBinLowEdge(histograms[i + 1]->FindFirstBinAbove(0)),
                        histograms[i + 1]->GetXaxis()->GetBinUpEdge(histograms[i + 1]->FindLastBinAbove(0)));
    } else {
      Fits[i] = new TF1(Fits_name[i], "expo", histograms[i + 1]->GetBinLowEdge(histograms[i + 1]->FindFirstBinAbove(0)),
                        histograms[i + 1]->GetXaxis()->GetBinUpEdge(histograms[i + 1]->FindLastBinAbove(0)));
    }

    histograms[i + 1]->Fit(Fits[i], "R");

    if (i == 0) {
      std::cout << "Results of Fit on Histogram histo_azimutal" << std::endl;
    } else if (i == 1) {
      std::cout << "Results of Fit on Histogram histo_polar" << std::endl;
    } else {
      std::cout << "Results of Fit on Histogram histo_impulse" << std::endl;
    }

    std::cout << "Fit Parameter: " << Fits[i]->GetParameter(0) << std::endl;
    std::cout << "Chi2/NDF: " << Fits[i]->GetChisquare() / Fits[i]->GetNDF() << std::endl;
    std::cout << "Probability of fit: " << Fits[i]->GetProb() << '\n' << std::endl;
  }

  // Inv_Mass Histos Subtraction
  TString result_names[2] = {"result1_2", "result3_4"};
  TString clone_names[2]  = {"hist1_clone", "hist2_clone"};
  TH1F* results[2];
  TH1F* hist_clones[2];

  for (int i = 0; i < 2; i++) {
    results[i] = (TH1F*)histograms_invmass[2 * i + 1]->Clone(result_names[i]);
    results[i]->Add(histograms_invmass[2 * (i + 1)], -1);
    // For Chi2 test
    hist_clones[i] = (TH1F*)histograms_invmass[i]->Clone(clone_names[i]);
    hist_clones[i]->SetBins(100, 0.7, 1.1);
  }

  // Chi2 Test
  Double_t chi2comparison_1 = hist_clones[0]->Chi2Test(hist_clones[1], "CHI2/NDF");
  Double_t chi2comparison_2 = hist_clones[0]->Chi2Test(histograms_invmass[5], "CHI2/NDF");
  Double_t chi2comparison_3 = hist_clones[1]->Chi2Test(histograms_invmass[5], "CHI2/NDF");

  std::cout << "Chi Squared Test between Difference Concordant and Discordant Particle Invariant Mass and Difference "
               "Concorde and Discord Particle Pi-K Invariant Mass:"
            << chi2comparison_1 << std::endl;
  std::cout << "Chi Squared Test between Difference Concordant and Discordant Particle Invariant Mass and K* products "
               "Invariant Mass:"
            << chi2comparison_2 << std::endl;
  std::cout
      << "Chi Squared Test between Difference Concordant and Discordant Particle Pi-K Invariant Mass and K* products "
         "Invariant Mass:"
      << chi2comparison_3 << std::endl;

  //  Gaussian Fits

  TF1* gaussianFit1_2 =
      new TF1("gaussianFit1_2", "gaus", results[0]->GetXaxis()->GetXmin(), results[0]->GetXaxis()->GetXmax());
  results[0]->Fit(gaussianFit1_2, "R");
  TF1* gaussianFit3_4 =
      new TF1("gaussianFit3_4", "gaus", results[1]->GetXaxis()->GetXmin(), results[1]->GetXaxis()->GetXmax());
  results[1]->Fit(gaussianFit3_4, "R");

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
  histograms[0]->SetLineColor(kBlue);
  histograms[0]->Draw();
  canvas1->SaveAs("generated_files/png_files/particles_distribution.png");

  // Canvas2
  TCanvas* canvas2 = new TCanvas("canvas2", "Azimutal Angle", 800, 600);
  histograms[1]->SetLineColor(kBlue);
  histograms[1]->Draw();
  canvas2->SaveAs("generated_files/png_files/azimutal_distribution.png");

  // Canvas3
  TCanvas* canvas3 = new TCanvas("canvas3", "Polar Angle", 800, 600);
  histograms[2]->SetLineColor(kBlue);
  histograms[2]->Draw();
  canvas3->SaveAs("generated_files/png_files/polar_distribution.png");

  // Canvas4
  TCanvas* canvas4 = new TCanvas("canvas4", "Particle Impulses", 800, 600);
  histograms[3]->SetLineColor(kBlue);
  histograms[3]->Draw();
  canvas4->SaveAs("generated_files/png_files/impulse_distribution.png");

  // Canvas5
  TCanvas* canvas5 = new TCanvas("canvas5", "Particle Transverse Impulses", 800, 600);
  histograms[4]->SetLineColor(kBlue);
  histograms[4]->Draw();
  canvas5->SaveAs("generated_files/png_files/transverse_impulse_distribution.png");

  // Canvas6
  TCanvas* canvas6 = new TCanvas("canvas6", "Particle Energies", 800, 600);
  histograms[5]->SetLineColor(kBlue);
  histograms[5]->Draw();
  canvas6->SaveAs("generated_files/png_files/energy_distribution.png");

  // Canvas7
  TCanvas* canvas7 = new TCanvas("canvas7", "Particle Invariant Masses", 800, 600);
  histograms_invmass[0]->SetLineColor(kBlue);
  histograms_invmass[0]->Draw();
  canvas7->SaveAs("generated_files/png_files/invmass_distribution.png");

  // Canvas8
  TCanvas* canvas8 = new TCanvas("canvas8", "Particle Concordant and Discordant Invariant Masses", 1200, 600);
  canvas8->Divide(2, 1);
  histograms_invmass[1]->SetLineColor(kBlue);
  canvas8->cd(1);
  histograms_invmass[1]->Draw();
  histograms_invmass[2]->SetLineColor(kGreen);
  canvas8->cd(2);
  histograms_invmass[2]->Draw();
  canvas8->SaveAs("generated_files/png_files/conc_disc_invmass_distribution.png");

  // Canvas9
  TCanvas* canvas9 = new TCanvas("canvas9", "Particle Concordant and Discordant Pi-K Invariant Masses", 1200, 600);
  canvas9->Divide(2, 1);
  histograms_invmass[3]->SetLineColor(kBlue);
  canvas9->cd(1);
  histograms_invmass[3]->Draw();
  histograms_invmass[4]->SetLineColor(kGreen);
  canvas9->cd(2);
  histograms_invmass[4]->Draw();
  canvas9->SaveAs("generated_files/png_files/conc_disc_PiK_invmass_distribution.png");

  // Canvas10
  TCanvas* canvas10 = new TCanvas("canvas10", "Analisis of K*", 1600, 600);
  canvas10->Divide(3, 1);
  results[0]->SetLineColor(kBlue);
  results[0]->SetTitle("Difference Concordant and Discordant Particle Invariant Mass");
  canvas10->cd(1);
  results[0]->Draw();
  gaussianFit1_2->Draw("SAME");
  results[1]->SetLineColor(kBlue);
  results[1]->SetTitle("Difference Concordant and Discordant Particle Pi-K Invariant Mass");
  canvas10->cd(2);
  results[1]->Draw();
  gaussianFit3_4->Draw("SAME");
  histograms_invmass[5]->SetLineColor(kGreen);
  canvas10->cd(3);
  histograms_invmass[5]->Draw();
  canvas10->SaveAs("generated_files/png_files/Ks_distribution_analysis.png");
}
