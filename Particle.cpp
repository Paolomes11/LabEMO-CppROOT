#include "Particle.hpp"
#include <cstring>
#include <iomanip>
#include <iostream>

ParticleType* Particle::fParticleType[fMaxNumParticleType] = {nullptr};
int Particle::fNParticleType                               = 0;

Particle::Particle(const char* name, double px, double py, double pz)
    : fPx(px)
    , fPy(py)
    , fPz(pz)
{
  fIndex = FindParticle(name);

  if (fIndex == -1) {
    std::cerr << "ERROR: particle type '" << name << "' not found!"
              << std::endl;
  }
}

int Particle::FindParticle(const char* name)
{
  for (int i = 0; i < fMaxNumParticleType; ++i) {
    if (fParticleType[i] != nullptr
        && std::strcmp(fParticleType[i]->GetName(), name) == 0) {
      return i;
    }
  }
  return -1;
}

void Particle::AddParticleType(const char* name, double mass, int charge,
                               double width)
{
  if (FindParticle(name) != -1) {
    std::cerr << "ERROR: particle type '" << name << "' already exist!"
              << std::endl;
    return;
  }

  if (fNParticleType >= fMaxNumParticleType) {
    std::cerr << "ERROR: max number of types reached!" << std::endl;
    return;
  }

  ParticleType* newParticleType;
  if (width > 0) {
    newParticleType = new ResonanceType(name, mass, charge, width);
  } else {
    newParticleType = new ParticleType(name, mass, charge);
  }

  for (int i = 0; i < fMaxNumParticleType; ++i) {
    if (fParticleType[i] == nullptr) {
      fParticleType[i] = newParticleType;
      fNParticleType++;
      return;
    }
  }
}

// setter
void Particle::SetIndex(int index)
{
  if (index < 10 && fParticleType[index] != nullptr) {
    fIndex = index;
  } else {
    std::cerr << "ERROR: The value of Index is invalid (greater or equal to 10 "
                 "or containing another particle)"
              << std::endl;
  }
}

void Particle::SetIndex(const char* name)
{
  fIndex = FindParticle(name);
}

void Particle::PrintParticleTypes()
{
  for (int i = 0; i < fMaxNumParticleType && fParticleType[i] != nullptr; ++i) {
    std::cout << "Index: " << i;
    fParticleType[i]->Print();
    std::cout << std::endl;
  }
}

void Particle::PrintParticleProperties()
{
  if (fIndex != -1) {
    std::cout << std::left << std::setw(8) << "Index: " << std::setw(8)
              << fIndex << '\n'
              << std::setw(8) << "Name: " << std::setw(8)
              << fParticleType[fIndex]->GetName() << '\n'
              << std::setw(8) << "Impulse: " << fPx << ", " << fPy << ", "
              << fPz << std::endl;
  } else {
    std::cout << "ERROR: The given particle doesn't exist!" << '\n' << '\n';
  }
}
