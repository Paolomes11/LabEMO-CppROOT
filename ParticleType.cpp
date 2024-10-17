#include "ParticleType.hpp"
#include <iostream>

// ctor
ParticleType::ParticleType(const char &fName, double fMass, int fCharge) : fName_(fName), fMass_(fMass), fCharge_(fCharge) {
};

// dtor
// ParticleType::~ParticleType