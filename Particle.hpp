#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "ParticleType.hpp"

// class Particle : public ParticleType {
class Particle {
public:
Particle(int fIndex, double impulso_x, double impulso_y, double impulso_z);

static void AddParticleType(ParticleType* particleType);

private:
int fIndex_;
double impulso_x_;
double impulso_y_;
double impulso_z_;

static ParticleType* particleTypes_[6];
static int FindParticle(const char*name);
};

#endif