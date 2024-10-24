#include "Particle.hpp"
#include "ParticleType.hpp"
#include "ResonanceType.hpp"
#include <array>

void random_generation {
AddParticleType("pioni_positivi", 0.13957, 1.)
AddParticleType("pioni_negativi", 0.13957, -1.)
AddParticleType("kaoni_positivi", 0.49367, 1.)
AddParticleType("kaoni_negativi", 0.49367, -1.)
AddParticleType("protoni_positivi", 0.93827, 1.)
AddParticleType("protoni_negativi", 0.93827, 1.)
AddParticleType("K*", 0.89166, 0., 0.050)
gRandom→SetSeed();
int nGen=1E5;
std::array<Particle, N> EventParticles;

TH1F *histo_particles = new TH1F("histo_particles", "Particles Distribution", 7);
TH1F *histo_azimutal = new TH1F("histo_azimutal", "Azimutal Angle Distribution", 7);
TH1F *histo_polar = new TH1F("histo_polar", "Polar Angle Distribution", 7);
TH1F *histo_impulse = new TH1F("histo_impulse", "Impulse Distribution", 7);
TH1F *histo_transverse_impulse = new TH1F("histo_transverse_impulse", "Transverse Impulse Distribution", 7);
TH1F *histo_energy = new TH1F("histo_energy", "Energy Distribution", 7);

for (int i=1, i<nGen, i++)
{
int N=120;
for (i=1, i<100, i++)
{
double phi = TRandom::Uniform(0, TMath::2pi);
double theta = TRandom::Uniform(0, TMath::pi);
double impulse = TRandom::Exp(1);
double impulse_x=impulse*TMath::sin(theta)*TMath::cos(phi);
double impulse_y=impulse*TMath::sin(theta)*TMath::sin(phi);
double impulse_z=impulse*TMath::cos(theta);

}
}
}