// clang-format off
R__LOAD_LIBRARY(src/Particle_cpp.so)
R__LOAD_LIBRARY(src/ParticleType_cpp.so)
R__LOAD_LIBRARY(src/ResonanceType_cpp.so)
// clang-format on

#include "Particle.hpp"
#include "ParticleType.hpp"
#include "ResonanceType.hpp"
#include <array>

void random_generation()
{
  int Nbase = 100;
  int Nmax  = 120;
  int nGen  = 1E5;

  AddParticleType("Pi+", 0.13957, 1.);
  AddParticleType("Pi-", 0.13957, -1.);
  AddParticleType("K+", 0.49367, 1.);
  AddParticleType("K-", 0.49367, -1.);
  AddParticleType("P+", 0.93827, 1.);
  AddParticleType("P-", 0.93827, -1.);
  AddParticleType("K*", 0.89166, 0., 0.050);

  gRandom→SetSeed();

  TH1F* histo_particles = new TH1F("histo_particles", "Particles Distribution", 7, 1, 7);
  TH1F* histo_azimutal  = new TH1F("histo_azimutal", "Azimutal Angle Distribution", 60, 0, TMath::2pi);
  TH1F* histo_polar     = new TH1F("histo_polar", "Polar Angle Distribution", 60, 0, TMath::pi);
  TH1F* histo_impulse   = new TH1F("histo_impulse", "Impulse Distribution", 60, 0, 3);
  TH1F* histo_transverse_impulse =
      new TH1F("histo_transverse_impulse", "Transverse Impulse Distribution", 60, 0, TMath::sqrt(2));
  TH1F* histo_energy       = new TH1F("histo_energy", "Energy Distribution", 60, 0, TMath::sqrt(2));
  TH1F* histo_invmass      = new TH1F("histo_invmass", "Invariant Mass Distribution", 60, 0, 6.1);
  TH1F* histo_invmass_disc = new TH1F("histo_invmass_disc", "Discordant Invariant Mass Distribution", 60, 0, 6.1);
  TH1F* histo_invmass_conc = new TH1F("histo_invmass_conc", "Concordant Invariant Mass Distribution", 60, 0, 6.1);
  TH1F* histo_invmass_Pi_K_disc =
      new TH1F("histo_invmass_Pi_K_disc", "Pion and Kaon Discordant Invariant Mass Distribution", 60, 0, 6.1);
  TH1F* histo_invmass_Pi_K_conc =
      new TH1F("histo_invmass_Pi_K_conc", "Pion and Kaon Concordant Invariant Mass Distribution", 60, 0, 6.1);
  TH1F* histo_invmass_Ks_prod =
      new TH1F("histo_invmass_Ks_prod", "K* Products Invariant Mass Distribution", 60, 0, 6.1);

  Particle* EventParticle[N];
  for (int i = 1; i < nGen; i++) {
    for (int j = 0; j < 100; j++) {
      EventParticle[j] = new Particle("a");
      // Part 2 set random impulse
      double phi       = TRandom::Uniform(0, TMath::2pi);
      double theta     = TRandom::Uniform(0, TMath::pi);
      double impulse   = TRandom::Exp(1); // 1GeV
      double impulse_x = impulse * TMath::sin(theta) * TMath::cos(phi);
      double impulse_y = impulse * TMath::sin(theta) * TMath::sin(phi);
      double impulse_z = impulse * TMath::cos(theta);
      EventParticle[j]->SetP(impulse_x, impulse_y, impulse_z);

      // Part 3 Set type with probability
      double chPart = TRandom::Uniform(0, 1);
      // double chPart = TRandom::Rndm();
      if (chPart < 0.4) {
        EventParticle[j]->SetIndex("Pi+");
      } else if (chPart >= 0.4 && chPart < 0.8) {
        EventParticle[j]->SetIndex("Pi-");
      } else if (chPart >= 0.8 && chPart < 0.85) {
        EventParticle[j]->SetIndex("K+");
      } else if (chPart >= 0.85 && chPart < 0.9) {
        EventParticle[j]->SetIndex("K-");
      } else if (chPart >= 0.9 && chPart < 0.945) {
        EventParticle[j]->SetIndex("P+");
      } else if (chPart >= 0.945 && chPart < 0.99) {
        EventParticle[j]->SetIndex("P-");
      } else if (chPart >= 0.99) {
        EventParticle[j]->SetIndex("K*");
      }

    }

    // Part 5 fill the histograms
   


  }
}