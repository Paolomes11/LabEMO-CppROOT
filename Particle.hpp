#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "ParticleType.hpp"
#include "ResonanceType.hpp"

class Particle {
public:
    Particle(const char* name, double px = 0., double py = 0., double pz = 0.);

    static void AddParticleType(const char* name, double mass, int charge, double width = 0.);

private:
    int fIndex;  
    double fPx; 
    double fPy;  
    double fPz;  

    static const int fMaxNumParticleType = 10;  
    static ParticleType* fParticleType[fMaxNumParticleType];  
    static int fNParticleType;  

    static int FindParticle(const char* name);
};

#endif
