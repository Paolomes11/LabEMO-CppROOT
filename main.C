R__LOAD_LIBRARY(myRoot/Particle_cpp.so)
R__LOAD_LIBRARY(myRoot/ParticleType_cpp.so)
R__LOAD_LIBRARY(myRoot/ResonanceType_cpp.so)

void testso()
{
  Particle* A = new Particle("pollo");
  A->SetP(1., 1., 1.);
  A->GetPx();
}