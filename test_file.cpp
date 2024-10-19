#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Particle.hpp"
#include "doctest.h"

TEST_CASE("Test of Getters functions")
{
  // test for error managemente of adding particle
  Particle::AddParticleType("a", 18.34, +1, 22.); // TO FIX THE NUMBER OF CHARS
  Particle::AddParticleType("e", 18.34, +1, 22.);
  Particle::AddParticleType("i", 1.34, +2, 2.);
  Particle::AddParticleType("o", 1.34, +2, 2.);
  Particle::AddParticleType("u", 1.34, +2, 2.);

  Particle* particle[2];
  particle[0] = new Particle("a", 0., 1., 2.);
  particle[1] = new Particle("i", 1.1, 0.5, 3.7);

  CHECK(particle[0]->GetPx() == doctest::Approx(0.));
  CHECK(particle[0]->GetPy() == doctest::Approx(1.));
  CHECK(particle[0]->GetPz() == doctest::Approx(2.));
  CHECK(particle[0]->GetMass() == doctest::Approx(18.34));

  CHECK(particle[1]->GetPx() == doctest::Approx(1.1));
  CHECK(particle[1]->GetPy() == doctest::Approx(0.5));
  CHECK(particle[1]->GetPz() == doctest::Approx(3.7));
  CHECK(particle[1]->GetMass() == doctest::Approx(1.34));

  for (int i = 0; i < 2; ++i) {
    delete particle[i];
  }
}

TEST_CASE("Test of Energy and Invariant functions")
{
  Particle* particle[3];
  particle[0] = new Particle("a", 0., 1., 2.);
  particle[1] = new Particle("i", 1.1, 0.5, 3.7);
  particle[2] = new Particle("i", 1., 0., 2.);

  CHECK(particle[0]->fEnergy() == doctest::Approx(18.4758));
  CHECK(particle[0]->InvMass(*particle[2]) == doctest::Approx(20.6513));

  CHECK(particle[1]->fEnergy() == doctest::Approx(4.1165));
  CHECK(particle[1]->InvMass(*particle[2]) == doctest::Approx(2.8378));

  for (int i = 0; i < 3; ++i) {
    delete particle[i];
  }
}

TEST_CASE("Test of Setters functions")
{
  Particle* particle[2];
  particle[0] = new Particle("a", 0., 1., 2.);
  particle[1] = new Particle("i", 1.1, 0.5, 3.7);

  particle[0]->SetIndex(1);
  particle[0]->SetP(1., 1., 1.);
  CHECK(particle[0]->GetMass() == doctest::Approx(18.34));
  CHECK(particle[0]->GetPx() == doctest::Approx(1.));
  CHECK(particle[0]->GetPy() == doctest::Approx(1.));
  CHECK(particle[0]->GetPz() == doctest::Approx(1.));

  particle[1]->SetIndex("i");
  particle[1]->SetP(0., 1.5, 2.);
  CHECK(particle[1]->GetMass() == doctest::Approx(1.34));
  CHECK(particle[1]->GetPx() == doctest::Approx(0.));
  CHECK(particle[1]->GetPy() == doctest::Approx(1.5));
  CHECK(particle[1]->GetPz() == doctest::Approx(2.));

  for (int i = 0; i < 2; ++i) {
    delete particle[i];
  }
}
