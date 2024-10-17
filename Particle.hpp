#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "ParticleType.hpp"

class Particle {
public:
    Particle(const char* fName, double fPx = 0., double fPy = 0., double fPz = 0.);

    static void AddParticleType(const char* nome, double massa, int carica, double larghezza = 0.);

private:
    int fIndex_;  
    double fPx_; 
    double fPy_;  
    double fPz_;  

    static const int fMaxNumParticleType = 10;  
    static ParticleType* fParticleType_[fMaxNumParticleType];  
    static int fNParticleType;  

    static int FindParticle(const char* name);
};

#endif
