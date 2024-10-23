#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Particle.hpp"
#include "doctest.h"
#include <sstream>

TEST_CASE("Tests of Particle")
{
  SUBCASE("Test of Getters functions")
  {
    Particle::AddParticleType("Pi+", 18.34, +1, 22.);
    Particle::AddParticleType("Pi-", 18.34, +1, 22.);
    Particle::AddParticleType("K+", 1.34, +2, 2.);
    Particle::AddParticleType("K-", 1.34, +2, 2.);
    Particle::AddParticleType("P+", 1.34, +2, 2.);
    Particle::AddParticleType("P-", 1.34, +2, 2.);
    Particle::AddParticleType("K*", 1.34, +2, 2.);
    Particle::AddParticleType("a", 1.34, +2, 2.);
    Particle::AddParticleType("b", 0.0, +2, 2.); 
    Particle::AddParticleType("c", 1.34, +2, 2.); // non prende nuoi valori di width

    std::unique_ptr<Particle> particle[2];
    particle[0] = std::make_unique<Particle>("Pi+", 0., 1., 2.);
    particle[1] = std::make_unique<Particle>("K+", 1.1, 0.5, 3.7);

    CHECK(particle[0]->GetPx() == doctest::Approx(0.));
    CHECK(particle[0]->GetPy() == doctest::Approx(1.));
    CHECK(particle[0]->GetPz() == doctest::Approx(2.));
    CHECK(particle[0]->GetMass() == doctest::Approx(18.34));

    CHECK(particle[1]->GetPx() == doctest::Approx(1.1));
    CHECK(particle[1]->GetPy() == doctest::Approx(0.5));
    CHECK(particle[1]->GetPz() == doctest::Approx(3.7));
    CHECK(particle[1]->GetMass() == doctest::Approx(1.34));
  }

  SUBCASE("Test of Energy and Invariant functions")
  {
    std::unique_ptr<Particle> particle[3];
    particle[0] = std::make_unique<Particle>("Pi+", 0., 1., 2.);
    particle[1] = std::make_unique<Particle>("K+", 1.1, 0.5, 3.7);
    particle[2] = std::make_unique<Particle>("K+", 1., 0., 2.);

    CHECK(particle[0]->GetEnergy() == doctest::Approx(18.4758));
    CHECK(particle[0]->InvMass(*particle[2]) == doctest::Approx(20.6513));

    CHECK(particle[1]->GetEnergy() == doctest::Approx(4.1165));
    CHECK(particle[1]->InvMass(*particle[2]) == doctest::Approx(2.8378));
  }

  SUBCASE("Test of Setters functions")
  {
    std::unique_ptr<Particle> particle[2];
    particle[0] = std::make_unique<Particle>("Pi+", 0., 1., 2.);
    particle[1] = std::make_unique<Particle>("K+", 1.1, 0.5, 3.7);

    particle[0]->SetIndex(3);
    particle[0]->SetP(1., 1., 1.);
    CHECK(particle[0]->GetMass() == doctest::Approx(1.34));
    CHECK(particle[0]->GetPx() == doctest::Approx(1.));
    CHECK(particle[0]->GetPy() == doctest::Approx(1.));
    CHECK(particle[0]->GetPz() == doctest::Approx(1.));

    particle[1]->SetIndex("Pi+");
    particle[1]->SetP(0., 1.5, 2.);
    CHECK(particle[1]->GetMass() == doctest::Approx(18.34));
    CHECK(particle[1]->GetPx() == doctest::Approx(0.));
    CHECK(particle[1]->GetPy() == doctest::Approx(1.5));
    CHECK(particle[1]->GetPz() == doctest::Approx(2.));
  }

  SUBCASE("Test of Decay2body function")
  {
    std::unique_ptr<Particle> particle[3];
    particle[0] = std::make_unique<Particle>("Pi+", 0., 1., 2.);
    particle[1] = std::make_unique<Particle>("K+", 0., 0., 0.);
    particle[2] = std::make_unique<Particle>("P+", 0., 0., 0.);
    CHECK(particle[0]->Decay2body(*particle[1], *particle[2])==0);
  }
}

TEST_CASE("Test of ParticleType")
{
  ParticleType PT("P", 14.2, +1);
  CHECK(PT.GetName() == "P");
  CHECK(PT.GetMass() == doctest::Approx(14.2));
  CHECK(PT.GetCharge() == +1);
  CHECK(PT.GetWidth() == 0);
}

TEST_CASE("Test of ResonanceType")
{
  ResonanceType RT("R", 14.2, +1, 1.1);
  CHECK(RT.GetName() == "R");
  CHECK(RT.GetMass() == doctest::Approx(14.2));
  CHECK(RT.GetCharge() == +1);
  CHECK(RT.GetWidth() == doctest::Approx(1.1));
}

TEST_CASE("Test of Errors launch")
{
  SUBCASE("Constructor Error")
  {
    std::stringstream buffer;
    std::streambuf* old = std::cerr.rdbuf(buffer.rdbuf());

    std::unique_ptr<Particle> particle = std::make_unique<Particle>("Nano", 0., 1., 2.);

    std::cerr.rdbuf(old);
    CHECK(buffer.str() == "ERROR: particle type 'Nano' not found!\n");
  }
  SUBCASE("AddParticleType Error 1")
  {
    std::stringstream buffer;
    std::streambuf* old = std::cerr.rdbuf(buffer.rdbuf());

    Particle::AddParticleType("a", 1.34, +2, 2.);

    std::cerr.rdbuf(old);
    CHECK(buffer.str() == "ERROR: particle type 'a' already exist!\n\n");
  }
  SUBCASE("AddParticleType Error 2")
  {
    std::stringstream buffer;
    std::streambuf* old = std::cerr.rdbuf(buffer.rdbuf());

    Particle::AddParticleType("d", 1.34, +2, 2.);

    std::cerr.rdbuf(old);
    CHECK(buffer.str() == "ERROR: max number of types reached!\n\n");
  }
  SUBCASE("SetIndex 1 Error")
  {
    std::stringstream buffer;
    std::streambuf* old = std::cerr.rdbuf(buffer.rdbuf());

    std::unique_ptr<Particle> particle = std::make_unique<Particle>("Pi+", 0., 1., 2.);
    particle->SetIndex(11);

    std::cerr.rdbuf(old);
    CHECK(buffer.str()
          == "ERROR: The value of Index is invalid (greater or equal to 10 or containing another particle)\n");
  }
  SUBCASE("SetIndex 2 Error")
  {
    std::stringstream buffer;
    std::streambuf* old = std::cerr.rdbuf(buffer.rdbuf());

    std::unique_ptr<Particle> particle = std::make_unique<Particle>("Pi+", 0., 1., 2.);
    particle->SetIndex("d");

    std::cerr.rdbuf(old);
    CHECK(buffer.str() == "ERROR: particle type 'd' doesn't exist!\n");
  }
  SUBCASE("PrintParticleProperties Error")
  {
    std::stringstream buffer;
    std::streambuf* old = std::cerr.rdbuf(buffer.rdbuf());

    std::unique_ptr<Particle> particle = std::make_unique<Particle>("d", 0., 1., 2.);
    particle->PrintParticleProperties();

    std::cerr.rdbuf(old);
    CHECK(buffer.str() == "ERROR: particle type 'd' not found!\nERROR: The given particle doesn't exist!\n\n");
  }
  SUBCASE("GetP Errors")
  {
    std::stringstream buffer;
    std::streambuf* old = std::cerr.rdbuf(buffer.rdbuf());

    std::unique_ptr<Particle> particle = std::make_unique<Particle>("d", 0., 1., 2.);
    particle->GetPx();
    particle->GetPy();
    particle->GetPz();

    std::cerr.rdbuf(old);
    CHECK(buffer.str()
          == "ERROR: particle type 'd' not found!\nERROR: The given particle doesn't exist (Px)\nERROR: The given "
             "particle doesn't exist (Py)\nERROR: The given particle doesn't exist (Pz)\n");
  }
  SUBCASE("GetMass Error")
  {
    std::stringstream buffer;
    std::streambuf* old = std::cerr.rdbuf(buffer.rdbuf());

    std::unique_ptr<Particle> particle = std::make_unique<Particle>("d", 0., 1., 2.);
    particle->GetMass();

    std::cerr.rdbuf(old);
    CHECK(buffer.str() == "ERROR: particle type 'd' not found!\nERROR: The given particle doesn't exist (Mass)\n\n");
  }
  SUBCASE("Decay2body Errors"){
    std::unique_ptr<Particle> particle[4];
    particle[0] = std::make_unique<Particle>("b", 0., 1., 2.);
    particle[1] = std::make_unique<Particle>("K-", 0., 1., 2.);
    particle[2] = std::make_unique<Particle>("K+", 0., 0., 0.);
    particle[3] = std::make_unique<Particle>("P+", 0., 0., 0.);
    CHECK(particle[0]->Decay2body(*particle[1], *particle[2])==1);
    CHECK(particle[1]->Decay2body(*particle[1], *particle[2])==2);
  }
}
