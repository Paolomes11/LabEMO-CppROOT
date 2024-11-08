#include <TFile.h>
#include <TH1F.h>
#include <TMath.h>
#include <TRandom.h>
#include <iostream>

// clang-format off
    R__LOAD_LIBRARY(root_files/ParticleType_cpp.so)
    R__LOAD_LIBRARY(root_files/ResonanceType_cpp.so)
    R__LOAD_LIBRARY(root_files/Particle_cpp.so)
// clang-format on

#include "Particle.hpp"
void random_generation()
{
  Int_t Nbase = 100;
  Int_t Nmax  = 140;
  Int_t nGen  = 1E5;

  Particle::AddParticleType("Pi+", 0.13957, 1.);
  Particle::AddParticleType("Pi-", 0.13957, -1.);
  Particle::AddParticleType("K+", 0.49367, 1.);
  Particle::AddParticleType("K-", 0.49367, -1.);
  Particle::AddParticleType("P+", 0.93827, 1.);
  Particle::AddParticleType("P-", 0.93827, -1.);
  Particle::AddParticleType("K*", 0.89166, 0., 0.050);

  gRandom->SetSeed();

  // Histograms
  TH1F* histo_particles = new TH1F("histo_particles", "Particles Distribution", 7, 1, 8);
  histo_particles->GetXaxis()->SetBinLabel(1, "Pi+");
  histo_particles->GetXaxis()->SetBinLabel(2, "Pi-");
  histo_particles->GetXaxis()->SetBinLabel(3, "K+");
  histo_particles->GetXaxis()->SetBinLabel(4, "K-");
  histo_particles->GetXaxis()->SetBinLabel(5, "P+");
  histo_particles->GetXaxis()->SetBinLabel(6, "P-");
  histo_particles->GetXaxis()->SetBinLabel(7, "K*");
  TH1F* histo_azimutal           = new TH1F("histo_azimutal", "Azimutal Angle Distribution", 100, 0, 7);
  TH1F* histo_polar              = new TH1F("histo_polar", "Polar Angle Distribution", 100, 0, 4);
  TH1F* histo_impulse            = new TH1F("histo_impulse", "Impulse Distribution", 100, 7, 7);
  TH1F* histo_transverse_impulse = new TH1F("histo_transverse_impulse", "Transverse Impulse Distribution", 100, 7, 7);
  TH1F* histo_energy             = new TH1F("histo_energy", "Energy Distribution", 100, 7, 7);
  TH1F* histo_invmass            = new TH1F("histo_invmass", "Invariant Mass Distribution", 200, 7, 7);
  histo_invmass->Sumw2();
  TH1F* histo_invmass_disc = new TH1F("histo_invmass_disc", "Discordant Invariant Mass Distribution", 1000, 7, 7);
  histo_invmass_disc->Sumw2();
  TH1F* histo_invmass_conc = new TH1F("histo_invmass_conc", "Concordant Invariant Mass Distribution", 1000, 7, 7);
  histo_invmass_conc->Sumw2();
  TH1F* histo_invmass_Pi_K_disc =
      new TH1F("histo_invmass_Pi_K_disc", "Pion and Kaon Discordant Invariant Mass Distribution", 1000, 7, 7);
  histo_invmass_Pi_K_disc->Sumw2();
  TH1F* histo_invmass_Pi_K_conc =
      new TH1F("histo_invmass_Pi_K_conc", "Pion and Kaon Concordant Invariant Mass Distribution", 200, 7, 7);
  histo_invmass_Pi_K_conc->Sumw2();
  TH1F* histo_invmass_Ks_prod = new TH1F("histo_invmass_Ks_prod", "K* Products Invariant Mass Distribution", 100, 7, 7);
  histo_invmass_Ks_prod->Sumw2();

// Generate events
#pragma omp parallel for
  for (Int_t i = 0; i < nGen; i++) {
    Particle* EventParticle[Nmax];
    Int_t Decay_index = Nbase;
    Particle::SetNParticles(0);

    // Generate particles in event
    for (Int_t j = 0; j < Nbase; j++) {
      EventParticle[j] = new Particle("buffer");

      // Set random impulse
      double phi       = gRandom->Uniform(0, TMath::TwoPi());
      double theta     = gRandom->Uniform(0, TMath::Pi());
      double impulse   = gRandom->Exp(1); // 1GeV
      double impulse_x = impulse * TMath::Sin(theta) * TMath::Cos(phi);
      double impulse_y = impulse * TMath::Sin(theta) * TMath::Sin(phi);
      double impulse_z = impulse * TMath::Cos(theta);
      EventParticle[j]->SetP(impulse_x, impulse_y, impulse_z);

      // Set type with probability
      double chPart = gRandom->Uniform(0, 1);
      // double chPart = gRandom->Rndm();
      int rangeIndex = (chPart >= 0.4) + (chPart >= 0.8) + (chPart >= 0.85) + (chPart >= 0.9) + (chPart >= 0.945)
                     + (chPart >= 0.99) + (chPart >= 1.0);

      switch (rangeIndex) {
      case 0:
        EventParticle[j]->SetIndex("Pi+");
#pragma omp critical
        histo_particles->Fill(1);
        break;
      case 1:
        EventParticle[j]->SetIndex("Pi-");
#pragma omp critical
        histo_particles->Fill(2);
        break;
      case 2:
        EventParticle[j]->SetIndex("K+");
#pragma omp critical
        histo_particles->Fill(3);
        break;
      case 3:
        EventParticle[j]->SetIndex("K-");
#pragma omp critical
        histo_particles->Fill(4);
        break;
      case 4:
        EventParticle[j]->SetIndex("P+");
#pragma omp critical
        histo_particles->Fill(5);
        break;
      case 5:
        EventParticle[j]->SetIndex("P-");
#pragma omp critical
        histo_particles->Fill(6);
        break;
      case 6:
        EventParticle[j]->SetIndex("K*");
#pragma omp critical
        histo_particles->Fill(7);
        double kdecay = gRandom->Uniform(0, 1);
        if (kdecay <= 0.5) {
          EventParticle[Decay_index]     = new Particle("K+");
          EventParticle[Decay_index + 1] = new Particle("Pi-");
#pragma omp critical
          {
            histo_particles->Fill(2);
            histo_particles->Fill(3);
          }
        } else {
          EventParticle[Decay_index]     = new Particle("K-");
          EventParticle[Decay_index + 1] = new Particle("Pi+");
#pragma omp critical
          {
            histo_particles->Fill(1);
            histo_particles->Fill(4);
          }
        }
        EventParticle[j]->Decay2body(*EventParticle[Decay_index], *EventParticle[Decay_index + 1]);

#pragma omp critical
        histo_invmass_Ks_prod->Fill(EventParticle[Decay_index]->InvMass(*EventParticle[Decay_index + 1]));
        Decay_index += 2;
        break;
      }

#pragma omp critical
      {
        histo_azimutal->Fill(phi);
        histo_polar->Fill(theta);
        histo_impulse->Fill(impulse);
        histo_transverse_impulse->Fill(TMath::Sqrt(TMath::Power(impulse_x, 2) + TMath::Power(impulse, 2)));
        histo_energy->Fill(EventParticle[j]->GetEnergy());
      }
    }

    for (Int_t j = 0; j < Particle::GetNParticles(); j++) {
      int idx_j    = EventParticle[j] ? EventParticle[j]->GetIndex() : 6;
      bool valid_j = (EventParticle[j] != nullptr) & (idx_j != 6);
      for (Int_t k = j + 1; k < Particle::GetNParticles(); k++) {
        int idx_k       = EventParticle[k] ? EventParticle[k]->GetIndex() : 6;
        bool valid_k    = (EventParticle[k] != nullptr) & (idx_k != 6);
        double inv_mass = (EventParticle[j]->InvMass(*EventParticle[k]) * (valid_j & valid_k));
        histo_invmass->Fill(inv_mass);

        bool mega_bool_conc =
            ((idx_j == 0) & (idx_k == 4)) | ((idx_j == 2) & (idx_k == 4)) | ((idx_j == 4) & (idx_k == 2))
            | ((idx_j == 4) & (idx_k == 0)) | ((idx_j == 1) & (idx_k == 5)) | ((idx_j == 3) & (idx_k == 5))
            | ((idx_j == 5) & (idx_k == 1)) | ((idx_j == 5) & (idx_k == 3)) | ((idx_j == 0) & (idx_k == 0))
            | ((idx_j == 2) & (idx_k == 2)) | ((idx_j == 4) & (idx_k == 4)) | ((idx_j == 1) & (idx_k == 1))
            | ((idx_j == 3) & (idx_k == 3)) | ((idx_j == 5) & (idx_k == 5));

        if (mega_bool_conc == true) {
#pragma omp critical
          histo_invmass_conc->Fill(inv_mass);
        }

        bool mega_bool_disc =
            ((idx_j == 0) & (idx_k == 1)) | ((idx_j == 0) & (idx_k == 5)) | ((idx_j == 1) & (idx_k == 0))
            | ((idx_j == 1) & (idx_k == 4)) | ((idx_j == 2) & (idx_k == 3)) | ((idx_j == 2) & (idx_k == 5))
            | ((idx_j == 3) & (idx_k == 2)) | ((idx_j == 3) & (idx_k == 4)) | ((idx_j == 4) & (idx_k == 1))
            | ((idx_j == 4) & (idx_k == 3)) | ((idx_j == 4) & (idx_k == 5)) | ((idx_j == 5) & (idx_k == 0))
            | ((idx_j == 5) & (idx_k == 2)) | ((idx_j == 5) & (idx_k == 4));

        if (mega_bool_disc == true) {
#pragma omp critical
          histo_invmass_disc->Fill(inv_mass);
        }

        bool cond_Pi_K_conc = ((idx_j == 0) & (idx_k == 2)) | ((idx_j == 2) & (idx_k == 0))
                            | ((idx_j == 1) & (idx_k == 3)) | ((idx_j == 3) & (idx_k == 1));

        if (cond_Pi_K_conc == true) {
#pragma omp critical
          {
            histo_invmass_conc->Fill(inv_mass);
            histo_invmass_Pi_K_conc->Fill(inv_mass);
          }
        }
        bool cond_Pi_K_disc = ((idx_j == 0) & (idx_k == 3)) | ((idx_j == 3) & (idx_k == 0))
                            | ((idx_j == 1) & (idx_k == 2)) | ((idx_j == 2) & (idx_k == 1));

        if (cond_Pi_K_disc == true) {
#pragma omp critical
          {
            histo_invmass_disc->Fill(inv_mass);
            histo_invmass_Pi_K_disc->Fill(inv_mass);
          }
        }
      }
    }
  }

  TFile* file = new TFile("histograms.root", "RECREATE");

  // Crash check
  if (!file || file->IsZombie()) {
    std::cerr << "Error: Failed to open histograms.root!" << std::endl;
  } else {
    std::cout << "File histograms.root opened successfully!" << std::endl;
  }

  histo_particles->Write();
  histo_azimutal->Write();
  histo_polar->Write();
  histo_impulse->Write();
  histo_transverse_impulse->Write();
  histo_energy->Write();
  histo_invmass->Write();
  histo_invmass_disc->Write();
  histo_invmass_conc->Write();
  histo_invmass_Pi_K_disc->Write();
  histo_invmass_Pi_K_conc->Write();
  histo_invmass_Ks_prod->Write();
  file->Close();
  delete file;
}
