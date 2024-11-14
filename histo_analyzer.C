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
  TString names[6]          = {"histo_particles", "histo_azimutal",           "histo_polar",
                               "histo_impulse",   "histo_transverse_impulse", "histo_energy"};
  TString invnames[6]       = {"histo_invmass",           "histo_invmass_disc",      "histo_invmass_conc",
                               "histo_invmass_Pi_K_disc", "histo_invmass_Pi_K_conc", "histo_invmass_Ks_prod"};
  TString particle_names[7] = {"Pi+", "Pi-", "K+", "K-", "P+", "P-", "K*"};

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
    std::cout << "Bin " << particle_names[i - 1] << " content: " << histograms[0]->GetBinContent(i) << "\nBin "
              << particle_names[i - 1] << " error :" << histograms[0]->GetBinError(i) << std::endl;
  }
  std::cout << std::endl;

  // Uniform and Exponential Fits
  TF1* Fits[3]; // 1-2 uniform, 3 exponential
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
      std::cout << "\nResults of Fit on Histogram histo_azimutal:" << std::endl;
    } else if (i == 1) {
      std::cout << "\nResults of Fit on Histogram histo_polar:" << std::endl;
    } else {
      std::cout << "\nResults of Fit on Histogram histo_impulse:" << std::endl;
    }
    Fits[i]->Print();
    if (i < 2) {
      std::cout << "Fit Parameter: " << Fits[i]->GetParameter(0) << std::endl;
      std::cout << "Fit Parameter Error: " << Fits[i]->GetParError(0) << std::endl;
    } else {
      std::cout << "Fit Parameter: " << Fits[i]->GetParameter(1) << std::endl;
      std::cout << "Fit Parameter Error: " << Fits[i]->GetParError(1) << std::endl;
    }
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
    hist_clones[i]->SetBins(
        100, histograms_invmass[5]->GetXaxis()->GetBinLowEdge(histograms_invmass[5]->FindFirstBinAbove(0)),
        histograms_invmass[5]->GetXaxis()->GetBinUpEdge(histograms_invmass[5]->FindLastBinAbove(0)));
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
  TString gaussian_names[2] = {"gaussianFits1_2", "gaussianFits3_4"};
  TF1* gaussianFits[2];

  for (int i = 0; i < 2; i++) {
    gaussianFits[i] =
        new TF1(gaussian_names[i], "gaus", results[i]->GetXaxis()->GetXmin(), results[i]->GetXaxis()->GetXmax());
    results[i]->Fit(gaussianFits[i], "R");

    if (i == 0) {
      std::cout << "\nResults of Fit on Histogram Difference Concordant and Discordant Particle Invariant Mass"
                << std::endl;
    } else {
      std::cout << "\nResults of Fit on Histogram Difference Concordant and Discordant Particle Pi-K Invariant Mass"
                << std::endl;
    }
    gaussianFits[i]->Print();
    std::cout << "Mass of K*: " << gaussianFits[i]->GetParameter(1) << " GeV/c2" << std::endl;
    std::cout << "Mass of K* Error: " << gaussianFits[i]->GetParError(1) << " GeV/c2" << std::endl;
    std::cout << "Width of K*: " << gaussianFits[i]->GetParameter(2) << " GeV/c2" << std::endl;
    std::cout << "Width of K* Error: " << gaussianFits[i]->GetParError(2) << " GeV/c2" << std::endl;
    std::cout << "Chi2/NDF: " << gaussianFits[i]->GetChisquare() / gaussianFits[i]->GetNDF() << std::endl;
    std::cout << "Probability of fit: " << gaussianFits[i]->GetProb() << '\n' << std::endl;
  }

  // Canvases
  TString canvas_name    = "canvas";
  TString canvas_des[10] = {"Particle Types",
                            "Azimutal Angle",
                            "Polar Angle",
                            "Particle Impulses",
                            "Particle Transverse Impulses",
                            "Particle Energies",
                            "Particle Invariant Masses",
                            "Particle Concordant and Discordant Invariant Masses",
                            "Particle Concordant and Discordant Pi-K Invariant Masses",
                            "Analisis of K*"};
  TString canvas_dow[10] = {
      "particles_distribution",  "azimutal_distribution",           "polar_distribution",
      "impulse_distribution",    "transverse_impulse_distribution", "energy_distribution",
      "invmass_distribution",    "conc_disc_invmass_distribution",  "conc_disc_PiK_invmass_distribution",
      "Ks_distribution_analysis"};
  TString titles[2] = {"Difference Concordant and Discordant Particle Invariant Mass",
                       "Difference Concordant and Discordant Particle Pi-K Invariant Mass"};
  TCanvas* canvases[10];
  EColor colors[2] = {kBlue, kGreen};

  for (int i = 0; i < 10; i++) {
    if (i < 7) {
      canvases[i] = new TCanvas(canvas_name + i + 1, canvas_des[i], 800, 600);
      histograms[i]->SetLineColor(kBlue);
      histograms[i]->Draw();
    } else if (i == 7 || i == 8) {
      canvases[i] = new TCanvas(canvas_name + i + 1, canvas_des[i], 1200, 600);
      canvases[i]->Divide(2, 1);
      for (int j = 0; j < 2; j++) {
        histograms_invmass[(i - 7) * 2 + j + 1]->SetLineColor(colors[j]);
        canvases[i]->cd(j + 1);
        histograms_invmass[(i - 7) * 2 + j + 1]->Draw();
      }
    } else {
      canvases[i] = new TCanvas(canvas_name + i + 1, canvas_des[i], 1600, 600);
      canvases[i]->Divide(3, 1);
      for (int j = 0; j < 2; j++) {
        results[j]->SetLineColor(colors[0]);
        results[j]->SetTitle(titles[j]);
        canvases[i]->cd(j + 1);
        results[j]->Draw();
        gaussianFits[j]->Draw("SAME");
      }
      histograms_invmass[5]->SetLineColor(colors[1]);
      canvases[i]->cd(3);
      histograms_invmass[5]->Draw();
    }

    canvases[i]->SaveAs("generated_files/png_files/" + canvas_dow[i] + ".png");
  }
}
