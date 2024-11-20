#include <TCanvas.h>
#include <TColor.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TMath.h>
#include <TROOT.h>
#include <TStyle.h>
#include <iostream>
// TO SEE INCLUDES

void histo_analyzer()
{
  //Canvas Style
  TStyle* graphStyle = new TStyle("graphStyle", "Stile personalizzato");
  graphStyle->SetFrameBorderMode(0);
  graphStyle->SetFrameFillColor(0);
  graphStyle->SetCanvasBorderMode(0);
  graphStyle->SetCanvasColor(0);
  graphStyle->SetPadBorderMode(0);
  graphStyle->SetPadColor(0);
  graphStyle->SetStatColor(0);
  graphStyle->SetPadGridX(true);
  graphStyle->SetPadGridY(true);
  graphStyle->SetLabelFont(42, "");
  graphStyle->SetLabelFont(42, "XY");
  graphStyle->SetLabelSize(0.04, "XY");
  graphStyle->SetTitleFont(42, "");
  graphStyle->SetTitleFont(42, "XY");
  graphStyle->SetTitleSize(0.035, "XY");
  graphStyle->SetTitleOffset(1.3, "XY");
  graphStyle->SetTitleAlign(23);
  graphStyle->SetTitleSize(0.050, "");
  graphStyle->SetTitleX(0.5);
  graphStyle->SetTitleBorderSize(0);
  graphStyle->SetTitleFillColor(0);
  graphStyle->SetTitleBorderSize(0);
  graphStyle->SetTitleStyle(0);
  graphStyle->SetTitleOffset(0., "Y");
  graphStyle->SetPalette(57);
  graphStyle->SetLineWidth(2);
  graphStyle->SetOptFit(1111);
  graphStyle->SetOptStat(1111);
  graphStyle->SetStatBorderSize(1);
  graphStyle->SetStatY(0.935);
  graphStyle->SetStatFontSize(0.03);
  graphStyle->SetStatFont(42);
  graphStyle->SetFuncWidth(2);
  graphStyle->SetFuncColor(2);
  graphStyle->SetHistLineColor(kBlue + 2);
  gROOT->SetStyle("graphStyle");
  
  // Open file 
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
      std::cout << "Fit Mean: " << Fits[i]->GetParameter(1) << " GeV" << std::endl;
      std::cout << "Fit Mean Error: " << Fits[i]->GetParError(1) << " GeV" << std::endl;
    }
    std::cout << "Chi2/NDF: " << Fits[i]->GetChisquare() / Fits[i]->GetNDF() << std::endl;
    std::cout << "Probability of fit: " << Fits[i]->GetProb() * 100 << "%\n" << std::endl;
  }

  // Inv_Mass Histos Subtraction
  TString result_names[2] = {"Diff_InvMass_Disc_Conc", "Diff_InvMass_Disc_Conc_PiK"};
  TString clone_names[2]  = {"hist1_clone", "hist2_clone"};
  TH1F* results[2];
  TH1F* hist_clones[2];

  for (int i = 0; i < 2; i++) {
    results[i] = (TH1F*)histograms_invmass[2 * i + 1]->Clone(result_names[i]);
    results[i]->Add(histograms_invmass[2 * (i + 1)], -1);
  }

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
    std::cout << "Probability of fit: " << gaussianFits[i]->GetProb() * 100 << "%\n" << std::endl;
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
  TString titles[3] = {"Difference Concordant and Discordant Particle Invariant Mass",
                       "Difference Concordant and Discordant Particle Pi-K Invariant Mass", "entries"};
  TCanvas* canvases[10];
  EColor colors[3]   = {kBlue, kGreen, kSpring};
  int Cerulean_Color = TColor::GetColor(54, 126, 166);
  for (int i = 0; i < 10; i++) {
    if (i < 6) {
      canvases[i] = new TCanvas(canvas_name + i + 1, canvas_des[i], 1200, 700);
      histograms[i]->SetLineColor(colors[0]);
      histograms[i]->SetFillColorAlpha(Cerulean_Color, 0.1);
      histograms[i]->GetYaxis()->SetTitle(titles[2]);
      if (i == 0) {
        histograms[i]->GetXaxis()->SetTitle(canvas_des[i]);
      } else if (i == 1 || i == 2) {
        histograms[i]->GetXaxis()->SetTitle(canvas_des[i] + " (rad)");
      } else {
        histograms[i]->GetXaxis()->SetTitle(canvas_des[i] + " (GeV)");
      }
      histograms[i]->Draw();
    } else if (i == 6) {
      canvases[i] = new TCanvas(canvas_name + i + 1, canvas_des[i], 1200, 700);
      histograms_invmass[0]->SetLineColor(colors[0]);
      histograms_invmass[0]->GetYaxis()->SetTitle(titles[2]);
      histograms_invmass[0]->GetXaxis()->SetTitle(canvas_des[i] + " (GeV/c2)");
      histograms_invmass[0]->Draw();
    } else if (i == 7 || i == 8) {
      canvases[i] = new TCanvas(canvas_name + i + 1, canvas_des[i], 1700, 600);
      canvases[i]->Divide(2, 1);
      for (int j = 0; j < 2; j++) {
        canvases[i]->cd(j + 1);
        histograms_invmass[(i - 7) * 2 + j + 1]->SetLineColor(colors[j]);
        histograms_invmass[(i - 7) * 2 + j + 1]->GetYaxis()->SetTitle(titles[2]);
        histograms_invmass[(i - 7) * 2 + j + 1]->GetXaxis()->SetTitle(canvas_des[6] + " (GeV/c2)");
        histograms_invmass[(i - 7) * 2 + j + 1]->Draw();
      }
    } else {
      canvases[i] = new TCanvas(canvas_name + i + 1, canvas_des[i], 2100, 400);
      canvases[i]->Divide(3, 1);
      for (int j = 0; j < 2; j++) {
        canvases[i]->cd(j + 1);
        results[j]->SetTitle(titles[j]);
        results[j]->SetLineColor(colors[0]);
        results[j]->GetYaxis()->SetTitle(titles[2]);
        results[j]->GetXaxis()->SetTitle(canvas_des[6] + " (GeV/c2)");
        results[j]->Draw();
      }
      canvases[i]->cd(3);
      histograms_invmass[5]->SetLineColor(colors[1]);
      histograms_invmass[5]->GetYaxis()->SetTitle(titles[2]);
      histograms_invmass[5]->GetXaxis()->SetTitle(canvas_des[6] + " (GeV/c2)");
      histograms_invmass[5]->Draw();
      gPad->WaitPrimitive();
    }

    canvases[i]->SaveAs("generated_files/png_files/" + canvas_dow[i] + ".png");
    canvases[i]->SaveAs("generated_files/C_files/" + canvas_dow[i] + ".C");
    canvases[i]->SaveAs("generated_files/root_files/" + canvas_dow[i] + ".root");
  }
}
