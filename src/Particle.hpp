#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <cmath>
#include <cstring>
#include "ResonanceType.hpp"

class Particle
{
 public:
  Particle(const char* name, double px = 0., double py = 0., double pz = 0.);

  // Setter
  int SetIndex(const char* name);
  int SetIndex(int index);
  int SetP(double px, double py, double pz);

  // Static
  static int AddParticleType(const char* name, double mass, int charge, double width = 0.);
  static int PrintParticleTypes();
  static int GetNParticles();
  static void SetNParticles(int n);
  static void RemNParticles(int n);

  // Printer of particle properties
  int PrintParticleProperties();

  // Invariant mass and Energy
  double GetEnergy() const;
  double InvMass(Particle& p) const;

  // Getters
  double GetPx() const;
  double GetPy() const;
  double GetPz() const;
  double GetMass() const;
  int GetIndex() const;

  // Function for decaying 1
  int Decay2body(Particle& dau1, Particle& dau2) const;

 private:
  int fIndex;
  double fPx;
  double fPy;
  double fPz;

  // Calculate the squared module of a vector
  double fModule2(double x, double y, double z) const;

  static int fNParticles;
  static const int fMaxNumParticleType = 10;
  static ParticleType* fParticleType[fMaxNumParticleType];
  static int fNParticleType;

  static int FindParticle(const char* name);

  // Function for decaying 2
  void Boost(double bx, double by, double bz);
};

#endif
