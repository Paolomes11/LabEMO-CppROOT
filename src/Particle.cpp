#include "Particle.hpp"
#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>

std::unique_ptr<ParticleType> Particle::fParticleType[fMaxNumParticleType] = {nullptr};
int Particle::fNParticleType                                               = 0;

Particle::Particle(const char* name, double px, double py, double pz)
    : fPx(px)
    , fPy(py)
    , fPz(pz)
{
  fIndex = FindParticle(name);

  if (fIndex == -1) {
    std::cerr << "ERROR: particle type '" << name << "' not found!" << std::endl;
  }
}

int Particle::FindParticle(const char* name)
{
  for (int i = 0; i < fMaxNumParticleType; ++i) {
    if (fParticleType[i] != nullptr && std::strcmp(fParticleType[i]->GetName(), name) == 0) {
      return i;
    }
  }
  return -1;
}

int Particle::AddParticleType(const char* name, double mass, int charge, double width)
{
  if (FindParticle(name) != -1) { // TO SEE
    std::cerr << "ERROR: particle type '" << name << "' already exist!" << '\n' << '\n';
    return 1;
  }

  if (fNParticleType >= fMaxNumParticleType) {
    std::cerr << "ERROR: max number of types reached!" << '\n' << '\n';
    return 2;
  }

  std::unique_ptr<ParticleType> newParticleType;
  if (width > 0) {
    newParticleType = std::make_unique<ResonanceType>(name, mass, charge, width);
  } else {
    newParticleType = std::make_unique<ParticleType>(name, mass, charge);
  }

  for (int i = 0; i < fMaxNumParticleType; ++i) {
    if (fParticleType[i] == nullptr) {
      fParticleType[i] = std::move(newParticleType);
      fNParticleType++;
      return 0;
    }
  }
}

// setter
int Particle::SetIndex(int index)
{
  if (index < 10 && fParticleType[index] != nullptr) {
    fIndex = index;
    return 0;
  } else {
    std::cerr << "ERROR: The value of Index is invalid (greater or equal to 10 "
                 "or containing another particle)"
              << std::endl;
    return 1;
  }
}

int Particle::SetIndex(const char* name)
{
  if (FindParticle(name) == -1) {
    std::cerr << "ERROR: particle type '" << name << "' doesn't exist!" << std::endl;
    return 1;
  } else {
    fIndex = FindParticle(name);
    return 0;
  }
}

int Particle::PrintParticleTypes()
{
  std::cout << "=============================\n";
  std::cout << "  Particle Types\n";
  std::cout << "=============================\n";
  for (int i = 0; i < fMaxNumParticleType && fParticleType[i] != nullptr; ++i) {
    std::cout << "Index: " << i << '\n';
    fParticleType[i]->Print();
    std::cout << "-----------------------------\n";
  }
  std::cout << "=============================\n";
  return 0;
}

int Particle::PrintParticleProperties()
{
  if (fIndex != -1) {
    std::cout << "=============================\n";
    std::cout << "  Particle Properties\n";
    std::cout << "=============================\n";
    std::cout << std::left << std::setw(20) << "Index:" << fIndex << '\n'
              << std::left << std::setw(20) << "Name:" << fParticleType[fIndex]->GetName() << '\n'
              << std::left << std::setw(20) << "Impulse (Px, Py, Pz):" << fPx << ", " << fPy << ", " << fPz << '\n'
              << "=============================\n";
    return 0;
  } else {
    std::cerr << "ERROR: The given particle doesn't exist!" << '\n' << '\n';
    return 1;
  }
}

// getters for impulse
double Particle::GetPx() const
{
  return fIndex != -1 ? fPx : (std::cerr << "ERROR: The given particle doesn't exist (Px)" << std::endl, -1);
}

double Particle::GetPy() const
{
  return fIndex != -1 ? fPy : (std::cerr << "ERROR: The given particle doesn't exist (Py)" << std::endl, -1);
}

double Particle::GetPz() const
{
  return fIndex != -1 ? fPz : (std::cerr << "ERROR: The given particle doesn't exist (Pz)" << std::endl, -1);
}

double Particle::GetMass() const
{
  if (fIndex != -1) {
    return fParticleType[fIndex]->GetMass(); // TO TEST
  } else {
    std::cerr << "ERROR: The given particle doesn't exist (Mass)" << '\n' << '\n';
    return -1;
  }
}

// Impulse setters
int Particle::SetP(double px, double py, double pz)
{
  fPx = px;
  fPy = py;
  fPz = pz;
  return 0;
}

// Energy and invariant mass
double Particle::GetEnergy() const // TO TEST
{
  return sqrt(pow(fParticleType[fIndex]->GetMass(), 2) + fModule2(fPx, fPy, fPz));
}

double Particle::InvMass(Particle& p) const // TO TEST
{
  return sqrt(pow(GetEnergy() + p.GetEnergy(), 2) - fModule2(fPx + p.fPx, fPy + p.fPy, fPz + p.fPz));
}

// Squared module
double Particle::fModule2(double x, double y, double z) const
{ // TO TEST
  return x * x + y * y + z * z;
}

// Functions for decaying
int Particle::Decay2body(Particle& dau1, Particle& dau2) const
{
  if (GetMass() == 0.0) {
    std::cerr << "Decayment cannot be preformed if mass is zero\n";
    return 1;
  }

  double massMot  = GetMass();
  double massDau1 = dau1.GetMass();
  double massDau2 = dau2.GetMass();

  if (fIndex > -1) { // add width effect

    // gaussian random numbers

    float x1, x2, w, y1;

    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w  = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w  = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;

    massMot += fParticleType[fIndex]->GetWidth() * y1;
  }

  if (massMot < massDau1 + massDau2) {
    std::cerr << "Decayment cannot be preformed because mass is too low in this channel\n";
    return 2;
  }

  double pout = sqrt((massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2))
                     * (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2)))
              / massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi   = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.SetP(pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi), pout * cos(theta));
  dau2.SetP(-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi), -pout * cos(theta));

  double energy = sqrt(fPx * fPx + fPy * fPy + fPz * fPz + massMot * massMot);

  double bx = fPx / energy;
  double by = fPy / energy;
  double bz = fPz / energy;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  return 0;
}

void Particle::Boost(double bx, double by, double bz)
{
  double energy = GetEnergy();

  // Boost this Lorentz vector
  double b2     = fModule2(bx, by, bz);
  double gamma  = 1.0 / sqrt(1.0 - b2);
  double bp     = bx * fPx + by * fPy + bz * fPz;
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  fPx += gamma2 * bp * bx + gamma * bx * energy;
  fPy += gamma2 * bp * by + gamma * by * energy;
  fPz += gamma2 * bp * bz + gamma * bz * energy;
}