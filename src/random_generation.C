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
  AddParticleType("protoni_negativi", 0.93827, 1.);
  AddParticleType("K*", 0.89166, 0., 0.050);

  gRandom→SetSeed();

  Particle* EventParticle[Nmax];
  for (int i = 1; i < nGen; i++) {
    double impulse_x;
    double impulse_y;
    double impulse_z;

    for (int j = 1, j < 100, j++) {
      double phi     = TRandom::Uniform(0, TMath::2pi);
      double theta   = TRandom::Uniform(0, TMath::pi);
      double impulse = TRandom::Exp(1); // 1GeV
      impulse_x      = impulse * TMath::sin(theta) * TMath::cos(phi);
      impulse_y      = impulse * TMath::sin(theta) * TMath::sin(phi);
      impulse_z      = impulse * TMath::cos(theta);
    }

    // Part 3
    double chPart = TRandom::Uniform(0, 1);
    // double chPart = TRandom::Rndm();
    if (chPart < 0.4) {
      EventParticle[i] = new Particle("Pi+", impulse_x, impulse_y, impulse_z);
    } else if (chPart >= 0.4 && chPart < 0.8) {
      EventParticle[i] = new Particle("Pi-", impulse_x, impulse_y, impulse_z);
    } else if (chPart >= 0.8 && chPart < 0.85) {
      EventParticle[i] = new Particle("K+", impulse_x, impulse_y, impulse_z);
    } else if (chPart >= 0.85 && chPart < 0.9) {
      EventParticle[i] = new Particle("K-", impulse_x, impulse_y, impulse_z);
    } else if (chPart >= 0.9 && chPart < 0.945) {
      EventParticle[i] = new Particle("P+", impulse_x, impulse_y, impulse_z);
    } else if (chPart >= 0.945 && chPart < 0.99) {
      EventParticle[i] = new Particle("P-", impulse_x, impulse_y, impulse_z);
    } else if (chPart >= 0.99) {
      EventParticle[i] = new Particle("K*", impulse_x, impulse_y, impulse_z);
    } 
  }
}