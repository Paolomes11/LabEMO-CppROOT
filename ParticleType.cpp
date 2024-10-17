#include "ParticleType.hpp"
#include <iostream>

// ctor
ParticleType::ParticleType(const char &fName, double fMass, int fCharge) : fName_(fName), fMass_(fMass), fCharge_(fCharge)
{
}

// dtor
// ParticleType::~ParticleType

// getters
int ParticleType::GetCharge() const
{
    return fCharge_;
}

double ParticleType::GetMass() const
{
    return fMass_;
}

const char &ParticleType::GetName() const
{
    return fName_;
}

// printer
void ParticleType::Print() const
{
    std::cout << "fName: " << fName_ << "fCharge: " << fCharge_ << "fMass: " << fMass_ << std::endl;
}