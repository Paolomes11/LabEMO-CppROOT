#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "ResonanceType.hpp"

class Particle
{
 public:
  Particle(const char* name, double px = 0., double py = 0., double pz = 0.);
  ~Particle();

  // setter
  int SetIndex(const char* name);
  int SetIndex(int index);
  int SetP(double px, double py, double pz);

  // static
  static int AddParticleType(const char* name, double mass, int charge, double width = 0.);
  static int PrintParticleTypes();
  static int GetNParticles();
  static void SetNParticles(int n);

  // printer of particle properties
  int PrintParticleProperties();

  // invariant mass and Energy
  double GetEnergy() const;
  double InvMass(Particle& p) const;

  // getters
  double GetPx() const;
  double GetPy() const;
  double GetPz() const;
  double GetMass() const;
  int GetIndex() const;

  // function for decaying 1
  int Decay2body(Particle& dau1, Particle& dau2) const;

 private:
  int fIndex;
  double fPx;
  double fPy;
  double fPz;

  // calculate the squared module of a vector
  double fModule2(double x, double y, double z) const;

  static int fNParticles;
  static const int fMaxNumParticleType = 10;
  static ParticleType* fParticleType[fMaxNumParticleType];
  static int fNParticleType;

  static int FindParticle(const char* name);

  // function for decaying 2
  void Boost(double bx, double by, double bz);
};

#endif
