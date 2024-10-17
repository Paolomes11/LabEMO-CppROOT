#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "ResonanceType.hpp"

class Particle
{
public:
    Particle(const char *name, double px = 0., double py = 0., double pz = 0.);

    // setter
    void SetfIndex(int index);
    void SetfIndex(const char *name);

    static void AddParticleType(const char *name, double mass, int charge, double width = 0.);
    void SetIndex(const char *name);
    void SetIndex(int index);

    static void PrintParticleTypes();
    void PrintParticleProperties();

private:
    int fIndex;
    double fPx;
    double fPy;
    double fPz;

    static const int fMaxNumParticleType = 10;
    static ParticleType *fParticleType[fMaxNumParticleType];
    static int fNParticleType;

    static int FindParticle(const char *name);
};

#endif
