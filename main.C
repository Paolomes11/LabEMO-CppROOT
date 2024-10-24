// clang-format off
R__LOAD_LIBRARY(src/Particle_cpp.so)
R__LOAD_LIBRARY(src/ParticleType_cpp.so)
R__LOAD_LIBRARY(src/ResonanceType_cpp.so)
// clang-format on

void testso()
{
  Int_t nEv = 1E5;
  Int_t nParticle = 120;
  Particle EventParticles[nParticle];

  Particle::AddParticleType("a", 1.34, +2, 2.);
  Particle* A = new Particle("a");
  A->SetP(1., 1., 1.);
  A->GetPx();
}