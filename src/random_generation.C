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

  AddParticleType("pioni_positivi", 0.13957, 1.);
  AddParticleType("pioni_negativi", 0.13957, -1.);
  AddParticleType("kaoni_positivi", 0.49367, 1.);
  AddParticleType("kaoni_negativi", 0.49367, -1.);
  AddParticleType("protoni_positivi", 0.93827, 1.);
  AddParticleType("protoni_negativi", 0.93827, -1.);
  AddParticleType("K*", 0.89166, 0., 0.050);

  gRandom→SetSeed();

TH1F *histo_particles = new TH1F("histo_particles", "Particles Distribution", 7);
TH1F *histo_azimutal = new TH1F("histo_azimutal", "Azimutal Angle Distribution", 7);
TH1F *histo_polar = new TH1F("histo_polar", "Polar Angle Distribution", 7);
TH1F *histo_impulse = new TH1F("histo_impulse", "Impulse Distribution", 7);
TH1F *histo_transverse_impulse = new TH1F("histo_transverse_impulse", "Transverse Impulse Distribution", 7);
TH1F *histo_energy = new TH1F("histo_energy", "Energy Distribution", 7);

  Particle* EventParticle[N];
  for (int i = 1; i < nGen; i++) {
    for (int j = 0; j < 100; j++) {
      // Part 3 Set name with probability
      double chPart = TRandom::Uniform(0, 1);
      // double chPart = TRandom::Rndm();
      if (chPart < 0.4) {
        EventParticle[j] = new Particle("Pi+");
      } else if (chPart >= 0.4 && chPart < 0.8) {
        EventParticle[j] = new Particle("Pi-");
      } else if (chPart >= 0.8 && chPart < 0.85) {
        EventParticle[j] = new Particle("K+");
      } else if (chPart >= 0.85 && chPart < 0.9) {
        EventParticle[j] = new Particle("K-");
      } else if (chPart >= 0.9 && chPart < 0.945) {
        EventParticle[j] = new Particle("P+");
      } else if (chPart >= 0.945 && chPart < 0.99) {
        EventParticle[j] = new Particle("P-");
      } else if (chPart >= 0.99) {
        EventParticle[j] = new Particle("K*");
      }

      // Part 2 set random impulse
      double phi       = TRandom::Uniform(0, TMath::2pi);
      double theta     = TRandom::Uniform(0, TMath::pi);
      double impulse   = TRandom::Exp(1); // 1GeV
      double impulse_x = impulse * TMath::sin(theta) * TMath::cos(phi);
      double impulse_y = impulse * TMath::sin(theta) * TMath::sin(phi);
      double impulse_z = impulse * TMath::cos(theta);
      EventParticle[j]->SetP(impulse_x, impulse_y, impulse_z);
    }
  }
}