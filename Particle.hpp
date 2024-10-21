#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "ResonanceType.hpp"
#include <memory>

class Particle
{
 public:
  Particle(const char* name, double px = 0., double py = 0., double pz = 0.);

  // setter
  void SetIndex(const char* name);
  void SetIndex(int index);
  void SetP(double px, double py, double pz);

  // static
  static void AddParticleType(const char* name, double mass, int charge, double width = 0.);
  static void PrintParticleTypes();

  // printer of particle properties
  void PrintParticleProperties();

  // invariant mass and Energy
  double fEnergy() const;
  double InvMass(Particle& p) const;

  // getters
  double GetPx() const;
  double GetPy() const;
  double GetPz() const;
  double GetMass() const;

 private:
  int fIndex;
  double fPx;
  double fPy;
  double fPz;

  // calculate the squared module of a vector
  double fModule2(double x, double y, double z) const;

  static const int fMaxNumParticleType = 10;
  static std::unique_ptr<ParticleType> fParticleType[fMaxNumParticleType];
  static int fNParticleType;

  static int FindParticle(const char* name);
};

#endif
