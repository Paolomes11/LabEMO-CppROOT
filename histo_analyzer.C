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

  TH1F* histograms[6];
  histograms[0] = (TH1F*)file->Get("histo_particles");
  histograms[1] = (TH1F*)file->Get("histo_azimutal");
  histograms[2] = (TH1F*)file->Get("histo_polar");
  histograms[3] = (TH1F*)file->Get("histo_impulse");
  histograms[4] = (TH1F*)file->Get("histo_transverse_impulse");
  histograms[5] = (TH1F*)file->Get("histo_energy");

  TF1* uniformFit1 =
      new TF1("uniformFit1", "uniform1", histograms[1]->GetXaxis()->GetXmin(), histograms[1]->GetXaxis()->GetXmax());
  TF1* uniformFit2 =
      new TF1("uniformFit2", "uniform2", histograms[2]->GetXaxis()->GetXmin(), histograms[2]->GetXaxis()->GetXmax());

  std::cout << "Results of Fit on Histogram histo_azimutal" << std::endl;
  std::cout << "Mass of K*: " << uniformFit1->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Width of K*: " << uniformFit1->GetParameter(2) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << uniformFit1->GetChisquare() / uniformFit1->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << uniformFit1->GetProb() << '\n' << std::endl;
  
  std::cout << "Results of Fit on Histogram histo_polar" << std::endl;
  std::cout << "Mass of K*: " << uniformFit2->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Width of K*: " << uniformFit2->GetParameter(2) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << uniformFit2->GetChisquare() / uniformFit2->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << uniformFit1->GetProb() << '\n' << std::endl;


  TH1F* histograms_invmass[6];
  histograms_invmass[0] = (TH1F*)file->Get("histo_invmass");
  histograms_invmass[1] = (TH1F*)file->Get("histo_invmass_disc");
  histograms_invmass[2] = (TH1F*)file->Get("histo_invmass_conc");
  histograms_invmass[3] = (TH1F*)file->Get("histo_invmass_Pi_K_disc");
  histograms_invmass[4] = (TH1F*)file->Get("histo_invmass_Pi_K_conc");
  histograms_invmass[5] = (TH1F*)file->Get("histo_invmass_Ks_prod");

  if (histograms_invmass[1]->GetNbinsX() != histograms_invmass[2]->GetNbinsX()
      || histograms_invmass[3]->GetNbinsX() != histograms_invmass[4]->GetNbinsX()) {
    std::cerr << "histograms_invmass have different binning!" << std::endl;
    return;
  }

  TH1F* result1_2 = (TH1F*)histograms_invmass[1]->Clone("result1");
  result1_2->Add(histograms_invmass[2], -1);

  TH1F* result3_4 = (TH1F*)histograms_invmass[3]->Clone("result3");
  result3_4->Add(histograms_invmass[4], -1);

  TCanvas* canvas = new TCanvas("canvas", "Analisis of K*", 800, 600);
  result1_2->SetLineColor(kBlue);
  result3_4->SetLineColor(kRed);
  histograms_invmass[4]->SetLineColor(kGreen);

  result1_2->Draw();
  result3_4->Draw("SAME");
  histograms_invmass[5]->Draw("SAME");

  TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->AddEntry(result1_2, "Subtract (1-2)", "l");
  legend->AddEntry(result3_4, "Subtract (3-4)", "l");
  legend->AddEntry(histograms_invmass[5], "K* Decay", "l");
  legend->Draw();

  TF1* gaussianFit1_2 =
      new TF1("gaussianFit1_2", "gaus1_2", result1_2->GetXaxis()->GetXmin(), result1_2->GetXaxis()->GetXmax());
  result3_4->Fit(gaussianFit1_2, "R");

  std::cout << "Results of Fit on Histogram 1 - 2" << std::endl;
  std::cout << "Mass of K*: " << gaussianFit1_2->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Width of K*: " << gaussianFit1_2->GetParameter(2) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << gaussianFit1_2->GetChisquare() / gaussianFit1_2->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << gaussianFit1_2->GetProb() << '\n' << std::endl;

  TF1* gaussianFit3_4 =
      new TF1("gaussianFit3_4", "gaus3_4", result3_4->GetXaxis()->GetXmin(), result3_4->GetXaxis()->GetXmax());
  result3_4->Fit(gaussianFit3_4, "R");

  std::cout << "Results of Fit on Histogram 3 - 4" << std::endl;
  std::cout << "Mass of K*: " << gaussianFit3_4->GetParameter(1) << " GeV" << std::endl;
  std::cout << "Width of K*: " << gaussianFit3_4->GetParameter(2) << " GeV" << std::endl;
  std::cout << "Chi2/NDF: " << gaussianFit3_4->GetChisquare() / gaussianFit3_4->GetNDF() << std::endl;
  std::cout << "Probability of fit: " << gaussianFit3_4->GetProb() << std::endl;

  canvas->SaveAs("result_analisis.png");
}
