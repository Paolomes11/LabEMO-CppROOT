#include "Particle.hpp"

int main()
{
  char name1 = 'N';
  char name2 = 'T';
  std::unique_ptr<ParticleType> p[2];
  p[0] = std::make_unique<ParticleType>(&name1, 14.5, +1);
  p[1] = std::make_unique<ResonanceType>(&name2, 17.5, -1, 24.3);

  for (int i = 0; i < 2; ++i) {
    p[i]->Print();
  }

  // test for error managemente of adding particle
  Particle::AddParticleType("a", 18.34, +1, 22.); // TO FIX THE NUMBER OF CHARS
  Particle::AddParticleType("e", 18.34, +1, 22.);
  Particle::AddParticleType("i", 1.34, +2, 2.);
  Particle::AddParticleType("o", 1.34, +2, 2.);
  Particle::AddParticleType("u", 1.34, +2, 2.);
  Particle::AddParticleType("u", 1.34, +2, 2.);
  Particle::AddParticleType("u", 1.34, +2, 2.);
  Particle::AddParticleType("u", 1.34, +2, 2.);
  Particle::AddParticleType("u", 1.34, +2, 2.);
  Particle::AddParticleType("u", 1.34, +2, 2.);
  Particle::AddParticleType("u", 1.34, +2, 2.);

  // test for some member functions of Particle
  Particle::PrintParticleTypes();

  std::unique_ptr<Particle> particle[2];
  particle[0] = std::make_unique<Particle>("pollo");
  particle[1] = std::make_unique<Particle>("a", 0., 1., 2.);
  for (int i = 0; i < 2; ++i) {
    particle[i]->PrintParticleProperties();
    particle[i]->GetPx();
    particle[i]->GetPy();
    particle[i]->GetPz();
    particle[i]->GetMass();
  }

  return 0;
}