#include "Particle.hpp"

int main()
{
  char name1 = 'N';
  char name2 = 'T';
  ParticleType* p[2];
  p[0] = new ParticleType(&name1, 14.5, +1);
  p[1] = new ResonanceType(&name2, 17.5, -1, 24.3);

  for (int i = 0; i < 2; ++i) {
    p[i]->Print();
    delete p[i]; // TO RETHINK AFTER
  }

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

  Particle::PrintParticleTypes();

  Particle* particle[2];
  particle[0] = new Particle("pollo");
  particle[1] = new Particle("a");
  for (int i = 0; i < 2; ++i) {
    particle[i]->PrintParticleProperties();
    delete particle[i];
  }

  return 0;
}