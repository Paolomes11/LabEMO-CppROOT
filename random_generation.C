#include <TFile.h>
#include <TH1F.h>
#include <TMath.h>
#include <TRandom.h>

// clang-format off
    R__LOAD_LIBRARY(root_files/ParticleType_cpp.so)
    R__LOAD_LIBRARY(root_files/ResonanceType_cpp.so)
    R__LOAD_LIBRARY(root_files/Particle_cpp.so)
// clang-format on

#include "Particle.hpp"

void random_generation()
{
  Int_t Nbase = 100;
  Int_t Nmax  = 150;
  Int_t nGen  = 1E5;

  TString particle_names[7] = {"Pi+", "Pi-", "K+", "K-", "P+", "P-", "K*"};
  double masses[7]          = {0.13957, 0.13957, 0.49367, 0.49367, 0.93827, 0.93827, 0.89166};
  double charges[7]         = {1., -1., 1., -1., 1., -1., 0.};
  // Values for histograms
  TString histo_names[12] = {"histo_particles",         "histo_azimutal",           "histo_polar",
                             "histo_impulse",           "histo_transverse_impulse", "histo_energy",
                             "histo_invmass",           "histo_invmass_disc",       "histo_invmass_conc",
                             "histo_invmass_Pi_K_disc", "histo_invmass_Pi_K_conc",  "histo_invmass_Ks_prod"};
  TString histo_desc[12]  = {"Particles Distribution",
                             "Azimutal Angle Distribution",
                             "Polar Angle Distribution",
                             "Impulse Distribution",
                             "Transverse Impulse Distribution",
                             "Energy Distribution",
                             "Invariant Mass Distribution",
                             "Discordant Invariant Mass Distribution",
                             "Concordant Invariant Mass Distribution",
                             "Pion and Kaon Discordant Invariant Mass Distribution",
                             "Pion and Kaon Concordant Invariant Mass Distribution",
                             "K* Products Invariant Mass Distribution"};
  int histo_bins[12]      = {7, 10000, 10000, 1000, 1000, 1000, 300, 1000, 1000, 1000, 1000, 100};
  double min_bin[12]      = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.7};
  double max_bin[12]      = {8, 7, 3.5, 7, 8, 6, 7, 7, 7, 7, 7, 1.1};

  TH1F* AllHist[12];
  for(int i = 0; i < 12; ++i) {
    AllHist[i] = new TH1F(histo_names[i], histo_desc[i], histo_bins[i], min_bin[i], max_bin[i]);

    if(i > 5){
      AllHist[i]->Sumw2();
    }
  }

  for (int i = 0; i < 7; ++i) {
    if (i < 6) {
      Particle::AddParticleType(particle_names[i], masses[i], charges[i]);
    } else {
      Particle::AddParticleType(particle_names[i], masses[i], charges[i], 0.050);
    }
    AllHist[0]->GetXaxis()->SetBinLabel(i + 1, particle_names[i]);
  }

  gRandom->SetSeed();

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
      double chPart  = gRandom->Uniform(0, 1);
      int rangeIndex = (chPart >= 0.4) + (chPart >= 0.8) + (chPart >= 0.85) + (chPart >= 0.9) + (chPart >= 0.945)
                     + (chPart >= 0.99) + (chPart >= 1.0);

      switch (rangeIndex) {
      case 0:
        EventParticle[j]->SetIndex("Pi+");
#pragma omp critical
        AllHist[0]->Fill(1);
        break;
      case 1:
        EventParticle[j]->SetIndex("Pi-");
#pragma omp critical
        AllHist[0]->Fill(2);
        break;
      case 2:
        EventParticle[j]->SetIndex("K+");
#pragma omp critical
        AllHist[0]->Fill(3);
        break;
      case 3:
        EventParticle[j]->SetIndex("K-");
#pragma omp critical
        AllHist[0]->Fill(4);
        break;
      case 4:
        EventParticle[j]->SetIndex("P+");
#pragma omp critical
        AllHist[0]->Fill(5);
        break;
      case 5:
        EventParticle[j]->SetIndex("P-");
#pragma omp critical
        AllHist[0]->Fill(6);
        break;
      case 6:
        EventParticle[j]->SetIndex("K*");
#pragma omp critical
        AllHist[0]->Fill(7);

        double kdecay = gRandom->Uniform(0, 1);
        if (kdecay <= 0.5) {
          EventParticle[Decay_index]     = new Particle("K+");
          EventParticle[Decay_index + 1] = new Particle("Pi-");
        } else {
          EventParticle[Decay_index]     = new Particle("K-");
          EventParticle[Decay_index + 1] = new Particle("Pi+");
        }

        int decay = EventParticle[j]->Decay2body(*EventParticle[Decay_index], *EventParticle[Decay_index + 1]);

        if (decay == 0) {
#pragma omp critical
          AllHist[11]->Fill(EventParticle[Decay_index]->InvMass(*EventParticle[Decay_index + 1]));
          Decay_index += 2;
        } else {
          delete EventParticle[Decay_index];
          delete EventParticle[Decay_index + 1];
          Particle::RemNParticles(2);
        }

        break;
      }

#pragma omp critical
      {
        AllHist[1]->Fill(phi);
        AllHist[2]->Fill(theta);
        AllHist[3]->Fill(impulse);
        AllHist[4]->Fill(TMath::Sqrt(TMath::Power(impulse_x, 2) + TMath::Power(impulse, 2)));
        AllHist[5]->Fill(EventParticle[j]->GetEnergy());
      }
    }

    for (Int_t j = 0; j < Particle::GetNParticles(); j++) {
      int idx_j    = EventParticle[j] ? EventParticle[j]->GetIndex() : 6;
      bool valid_j = (EventParticle[j] != nullptr) & (idx_j != 6);
      for (Int_t k = j + 1; k < Particle::GetNParticles(); k++) {
        int idx_k       = EventParticle[k] ? EventParticle[k]->GetIndex() : 6;
        bool valid_k    = (EventParticle[k] != nullptr) & (idx_k != 6);
        double inv_mass = (EventParticle[j]->InvMass(*EventParticle[k]) * (valid_j & valid_k));

#pragma omp critical
        AllHist[6]->Fill(inv_mass);

        bool mega_bool_conc =
            ((idx_j == 0) & (idx_k == 4)) | ((idx_j == 2) & (idx_k == 4)) | ((idx_j == 4) & (idx_k == 2))
            | ((idx_j == 4) & (idx_k == 0)) | ((idx_j == 1) & (idx_k == 5)) | ((idx_j == 3) & (idx_k == 5))
            | ((idx_j == 5) & (idx_k == 1)) | ((idx_j == 5) & (idx_k == 3)) | ((idx_j == 0) & (idx_k == 0))
            | ((idx_j == 2) & (idx_k == 2)) | ((idx_j == 4) & (idx_k == 4)) | ((idx_j == 1) & (idx_k == 1))
            | ((idx_j == 3) & (idx_k == 3)) | ((idx_j == 5) & (idx_k == 5));

        if (mega_bool_conc == true) {
#pragma omp critical
          AllHist[8]->Fill(inv_mass);
        }

        bool mega_bool_disc =
            ((idx_j == 0) & (idx_k == 1)) | ((idx_j == 0) & (idx_k == 5)) | ((idx_j == 1) & (idx_k == 0))
            | ((idx_j == 1) & (idx_k == 4)) | ((idx_j == 2) & (idx_k == 3)) | ((idx_j == 2) & (idx_k == 5))
            | ((idx_j == 3) & (idx_k == 2)) | ((idx_j == 3) & (idx_k == 4)) | ((idx_j == 4) & (idx_k == 1))
            | ((idx_j == 4) & (idx_k == 3)) | ((idx_j == 4) & (idx_k == 5)) | ((idx_j == 5) & (idx_k == 0))
            | ((idx_j == 5) & (idx_k == 2)) | ((idx_j == 5) & (idx_k == 4));

        if (mega_bool_disc == true) {
#pragma omp critical
          AllHist[7]->Fill(inv_mass);
        }

        bool cond_Pi_K_conc = ((idx_j == 0) & (idx_k == 2)) | ((idx_j == 2) & (idx_k == 0))
                            | ((idx_j == 1) & (idx_k == 3)) | ((idx_j == 3) & (idx_k == 1));

        if (cond_Pi_K_conc == true) {
#pragma omp critical
          {
            AllHist[8]->Fill(inv_mass);
            AllHist[10]->Fill(inv_mass);
          }
        }
        bool cond_Pi_K_disc = ((idx_j == 0) & (idx_k == 3)) | ((idx_j == 3) & (idx_k == 0))
                            | ((idx_j == 1) & (idx_k == 2)) | ((idx_j == 2) & (idx_k == 1));

        if (cond_Pi_K_disc == true) {
#pragma omp critical
          {
            AllHist[7]->Fill(inv_mass);
            AllHist[9]->Fill(inv_mass);
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

  // Write histograms
  for(int i = 0; i < 12; ++i) {
    AllHist[i]->Write();
  }
  
  file->Close();
  delete file;
}
