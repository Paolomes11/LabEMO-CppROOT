#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1F.h>
#include <TLegend.h>
#include <iostream>

void histo_analyzer() {
    TFile *file = TFile::Open("histograms.root");
    if (!file || file->IsZombie()) {
    std::cerr << "Error opening file!" << std::endl;
    return;
  }

    TH1F* histograms[5];

  histograms[0]  = (TH1F*)file->Get("histo_invmass_disc");
  histograms[1]  = (TH1F*)file->Get("histo_invmass_conc");
  histograms[2]  = (TH1F*)file->Get("histo_invmass_Pi_K_disc");
  histograms[3]  = (TH1F*)file->Get("histo_invmass_Pi_K_conc");
  histograms[4]  = (TH1F*)file->Get("histo_invmass_Ks_prod");

  if (histograms[0]->GetNbinsX() != histograms[1]->GetNbinsX()
      || histograms[2]->GetNbinsX() != histograms[3]->GetNbinsX()) {
    std::cerr << "Histograms have different binning!" << std::endl;
    return;
  }

  TH1F* result1_2 = (TH1F*)histograms[0]->Clone("result1");
  result1_2->Add(histograms[1], -1);

  TH1F* result3_4 = (TH1F*)histograms[2]->Clone("result3");
  result3_4->Add(histograms[3], -1);

  TCanvas* canvas = new TCanvas("canvas", "Analisis of K*", 800, 600);
  result1_2->SetLineColor(kBlue);
  result3_4->SetLineColor(kRed);
  histograms[4]->SetLineColor(kGreen);

  result1_2->Draw();
  result3_4->Draw("SAME");
  histograms[4]->Draw("SAME");

  TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->AddEntry(result1_2, "Subtract (1-2)", "l");
  legend->AddEntry(result3_4, "Subtract (3-4)", "l");
  legend->AddEntry(histograms[4], "K* Decay", "l");
  legend->Draw();

  TF1* gaussianFit = new TF1("gaussianFit", "gaus", result3_4->GetXaxis()->GetXmin(), result3_4->GetXaxis()->GetXmax());
  result3_4->Fit(gaussianFit, "R");

  double mass  = gaussianFit->GetParameter(1);
  double width = gaussianFit->GetParameter(2);
  double chi2  = gaussianFit->GetChisquare();
  int ndf      = gaussianFit->GetNDF();
  double prob  = gaussianFit->GetProb();

    std::cout << "Mass of K*: " << gaussianFit->GetParameter(1) << " GeV" << std::endl;
    std::cout << "Width of K*: " << width << " GeV" << std::endl;
    std::cout << "Chi2/NDF: " << chi2 << "/" << ndf << std::endl;
    std::cout << "Probability of fit: " << prob << std::endl;

  canvas->SaveAs("result_analisis.png");
}
