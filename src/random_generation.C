#include <TFile.h>
#include <TH1F.h>
#include <TMath.h>
#include <TRandom.h>
#include <TSystem.h>

// clang-format off
    R__LOAD_LIBRARY(ParticleType_cpp.so)
    R__LOAD_LIBRARY(ResonanceType_cpp.so)
    R__LOAD_LIBRARY(Particle_cpp.so)
// clang-format on

#include "Particle.hpp"

void random_generation()
{
  Int_t Nbase = 100;
  Int_t Nmax  = 120;
  Int_t nGen  = 1E5;

  Particle::AddParticleType("Pi+", 0.13957, 1.);
  Particle::AddParticleType("Pi-", 0.13957, -1.);
  Particle::AddParticleType("K+", 0.49367, 1.);
  Particle::AddParticleType("K-", 0.49367, -1.);
  Particle::AddParticleType("P+", 0.93827, 1.);
  Particle::AddParticleType("P-", 0.93827, -1.);
  Particle::AddParticleType("K*", 0.89166, 0., 0.050);

  gRandom->SetSeed();

  TH1F* histo_particles = new TH1F("histo_particles", "Particles Distribution", 7, 7, 7);
  histo_particles->GetXaxis()->SetBinLabel(1, "Pi+");
  histo_particles->GetXaxis()->SetBinLabel(2, "Pi-");
  histo_particles->GetXaxis()->SetBinLabel(3, "K+");
  histo_particles->GetXaxis()->SetBinLabel(4, "K-");
  histo_particles->GetXaxis()->SetBinLabel(5, "P+");
  histo_particles->GetXaxis()->SetBinLabel(6, "P-");
  histo_particles->GetXaxis()->SetBinLabel(7, "K*");
  TH1F* histo_azimutal           = new TH1F("histo_azimutal", "Azimutal Angle Distribution", 7, 7, 7);
  TH1F* histo_polar              = new TH1F("histo_polar", "Polar Angle Distribution", 7, 7, 7);
  TH1F* histo_impulse            = new TH1F("histo_impulse", "Impulse Distribution", 7, 7, 7);
  TH1F* histo_transverse_impulse = new TH1F("histo_transverse_impulse", "Transverse Impulse Distribution", 7, 7, 7);
  TH1F* histo_energy             = new TH1F("histo_energy", "Energy Distribution", 7, 7, 7);
  TH1F* histo_invmass            = new TH1F("histo_invmass", "Invariant Mass Distribution", 7, 7, 7);
  histo_invmass->Sumw2();
  TH1F* histo_invmass_disc = new TH1F("histo_invmass_disc", "Discordant Invariant Mass Distribution", 7, 7, 7);
  histo_invmass_disc->Sumw2();
  TH1F* histo_invmass_conc = new TH1F("histo_invmass_conc", "Concordant Invariant Mass Distribution", 7, 7, 7);
  histo_invmass_conc->Sumw2();
  TH1F* histo_invmass_Pi_K_disc =
      new TH1F("histo_invmass_Pi_K_disc", "Pion and Kaon Discordant Invariant Mass Distribution", 7, 7, 7);
  histo_invmass_Pi_K_disc->Sumw2();
  TH1F* histo_invmass_Pi_K_conc =
      new TH1F("histo_invmass_Pi_K_conc", "Pion and Kaon Concordant Invariant Mass Distribution", 7, 7, 7);
  histo_invmass_Pi_K_conc->Sumw2();
  TH1F* histo_invmass_Ks_prod = new TH1F("histo_invmass_Ks_prod", "K* Products Invariant Mass Distribution", 7, 7, 7);
  histo_invmass_Ks_prod->Sumw2();

  Particle* EventParticle[Nmax];
  for (Int_t i = 1; i < nGen; i++) {
    Int_t Decay_index = Nbase;
    for (Int_t j = 0; j < Nbase; j++) {
      EventParticle[j] = new Particle("buffer");
      // Part 2 set random impulse
      double phi       = gRandom->Uniform(0, TMath::TwoPi());
      double theta     = gRandom->Uniform(0, TMath::Pi());
      double impulse   = gRandom->Exp(1); // 1GeV
      double impulse_x = impulse * TMath::Sin(theta) * TMath::Cos(phi);
      double impulse_y = impulse * TMath::Sin(theta) * TMath::Sin(phi);
      double impulse_z = impulse * TMath::Cos(theta);
      EventParticle[j]->SetP(impulse_x, impulse_y, impulse_z);

      // Part 3 Set type with probability
      double chPart = gRandom->Uniform(0, 1);
      // double chPart = gRandom->Rndm();
      if (chPart < 0.4) {
        EventParticle[j]->SetIndex("Pi+");
        histo_particles->Fill(1);
      } else if (chPart >= 0.4 && chPart < 0.8) {
        EventParticle[j]->SetIndex("Pi-");
        histo_particles->Fill(2);
      } else if (chPart >= 0.8 && chPart < 0.85) {
        EventParticle[j]->SetIndex("K+");
        histo_particles->Fill(3);
      } else if (chPart >= 0.85 && chPart < 0.9) {
        EventParticle[j]->SetIndex("K-");
        histo_particles->Fill(4);
      } else if (chPart >= 0.9 && chPart < 0.945) {
        EventParticle[j]->SetIndex("P+");
        histo_particles->Fill(5);
      } else if (chPart >= 0.945 && chPart < 0.99) {
        EventParticle[j]->SetIndex("P-");
        histo_particles->Fill(6);
      } else if (chPart >= 0.99) {
        EventParticle[j]->SetIndex("K*");
        // Part 4
        EventParticle[j]->Decay2body(EventParticle[Decay_index];
                                     EventParticle[Decay_index + 1]); // TO SEE colpa di quel birichino di GIULIO
        histo_particles->Fill(7);
        histo_invmass_Ks_prod->Fill(EventParticle[Decay_index]->InvMass(*EventParticle[Decay_index + 1]));
        Decay_index += 2;
      }

      histo_azimutal->Fill(phi);
      histo_polar->Fill(theta);
      histo_impulse->Fill(impulse);
      histo_transverse_impulse->Fll(TMath::Sqrt(TMath::Power(impulse_x, 2) + TMath::Power(impulse, 2)));
      histo_energy->Fill(EventParticle[j]->GetEnergy());
    }

    for (Int_t j = 0; j < Nmax; j++) {
      if (EventParticle[j]->GetIndex() != 6) {
        for (Int_t k = j + 1; k < Nmax; k++) {
          if (EventParticle[k]->GetIndex() != 6) {
            histo_invmass->Fill(EventParticle[j]->InvMass(*EventParticle[k]));
            
            // da testare la velocità di esecuzione in confronto all'opzione di esplicitare tutte le combinazioni
            if (((EventParticle[j]->GetIndex()== 0 || EventParticle[j]->GetIndex()== 2 || EventParticle[j]->GetIndex()== 4) 
                  && (EventParticle[k]->GetIndex()== 0 || EventParticle[k]->GetIndex()== 2 || EventParticle[k]->GetIndex()== 4))
                || ((EventParticle[j]->GetIndex()== 1 || EventParticle[j]->GetIndex()== 3 || EventParticle[j]->GetIndex()== 5) 
                  && (EventParticle[k]->GetIndex()== 1 || EventParticle[k]->GetIndex()== 3 || EventParticle[k]->GetIndex()== 5))) {
              histo_invmass_conc->Fill(EventParticle[j]->InvMass(*EventParticle[k]));
            }

            if (((EventParticle[j]->GetIndex()== 0 || EventParticle[j]->GetIndex()== 2 || EventParticle[j]->GetIndex()== 4) 
                  && (EventParticle[k]->GetIndex()== 1 || EventParticle[k]->GetIndex()== 3 || EventParticle[k]->GetIndex()== 5))
                || ((EventParticle[j]->GetIndex()== 1 || EventParticle[j]->GetIndex()== 3 || EventParticle[j]->GetIndex()== 5) 
                  && (EventParticle[k]->GetIndex()== 0 || EventParticle[k]->GetIndex()== 2 || EventParticle[k]->GetIndex()== 4))) {
              histo_invmass_disc->Fill(EventParticle[j]->InvMass(*EventParticle[k]));
            }

            if ((EventParticle[j]->GetIndex() == 0 && EventParticle[k]->GetIndex() == 2)
                || (EventParticle[j]->GetIndex() == 2 && EventParticle[k]->GetMass() == 0)
                || (EventParticle[j]->GetIndex() == 1 && EventParticle[k]->GetIndex() == 3)
                || (EventParticle[j]->GetIndex() == 3 && EventParticle[k]->GetMass() == 1)) {
              histo_invmass_Pi_K_disc->Fill(EventParticle[j]->InvMass(*EventParticle[k]));
            }

            if ((EventParticle[j]->GetIndex() == 0 && EventParticle[k]->GetIndex() == 3)
                || (EventParticle[j]->GetIndex() == 3 && EventParticle[k]->GetMass() == 0)
                || (EventParticle[j]->GetIndex() == 1 && EventParticle[k]->GetIndex() == 2)
                || (EventParticle[j]->GetIndex() == 2 && EventParticle[k]->GetMass() == 1)) {
              histo_invmass_Pi_K_disc->Fill(EventParticle[j]->InvMass(*EventParticle[k]));
            }

          } else {
            continue;
          }
        }
      } else {
        continue;
      }
    }
  }

  TFile* file("histograms.root", "RECREATE");
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
  file.Close();
}