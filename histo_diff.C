#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TLegend.h>
#include <iostream>

void analisis()
{
  TFile* file = TFile::Open("histograms.root");
  if (!file || file->IsZombie()) {
    std::cerr << "Error opening file!" << std::endl;
    return;
  }

  TH1D* histo_invmass_disc      = (TH1D*)file->Get("histo_invmass_disc");
  TH1D* histo_invmass_conc      = (TH1D*)file->Get("histo_invmass_conc");
  TH1D* histo_invmass_Pi_K_disc = (TH1D*)file->Get("histo_invmass_Pi_K_disc");
  TH1D* histo_invmass_Pi_K_conc = (TH1D*)file->Get("histo_invmass_Pi_K_conc");
  TH1D* histo_invmass_Ks_prod   = (TH1D*)file->Get("histo_invmass_Ks_prod");

  if (histo_invmass_disc->GetNbinsX() != histo_invmass_conc->GetNbinsX()
      || histo_invmass_Pi_K_disc->GetNbinsX() != histo_invmass_Pi_K_conc->GetNbinsX()) {
    std::cerr << "Histograms have different binning!" << std::endl;
    return;
  }

  TH1D* result1_2 = (TH1D*)histo_invmass_disc->Clone("result1");
  result1_2->Add(histo_invmass_conc, -1);

  TH1D* result3_4 = (TH1D*)histo_invmass_Pi_K_disc->Clone("result3");
  result3_4->Add(histo_invmass_Pi_K_conc, -1);

  TCanvas* canvas = new TCanvas("canvas", "Analisis of K*", 800, 600);
  result1_2->SetLineColor(kBlue);
  result3_4->SetLineColor(kRed);
  histo_invmass_Ks_prod->SetLineColor(kGreen);

  result1_2->Draw();
  result3_4->Draw("SAME");
  histo_invmass_Ks_prod->Draw("SAME");

  TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->AddEntry(result1_2, "Subtract (1-2)", "l");
  legend->AddEntry(result3_4, "Subtract (3-4)", "l");
  legend->AddEntry(histo_invmass_Ks_prod, "K* Decay", "l");
  legend->Draw();

  TF1* gaussianFit = new TF1("gaussianFit", "gaus", result3_4->GetXaxis()->GetXmin(), result3_4->GetXaxis()->GetXmax());
  result3_4->Fit(gaussianFit, "R");

  double mass  = gaussianFit->GetParameter(1);
  double width = gaussianFit->GetParameter(2);
  double chi2  = gaussianFit->GetChisquare();
  int ndf      = gaussianFit->GetNDF();
  double prob  = gaussianFit->GetProb();

  std::cout << "Mass of K*: " << mass << " GeV" << std::endl;
  std::cout << "Width of K*: " << width << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << chi2 << "/" << ndf << std::endl;
  std::cout << "Probability of fit: " << prob << std::endl;

  canvas->SaveAs("result_analisis.png");
}
