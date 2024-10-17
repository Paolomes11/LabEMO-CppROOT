#include "ParticleType.hpp"
#include <iostream>

// ctor
ParticleType::ParticleType(const char &name, double mass, int charge) : fName(name), fMass(mass), fCharge(charge)
{
}

// dtor
// ParticleType::~ParticleType

// getters
int ParticleType::GetCharge() const
{
    return fCharge;
}

double ParticleType::GetMass() const
{
    return fMass;
}

const char &ParticleType::GetName() const
{
    return fName;
}

// printer
void ParticleType::Print() const
{
    std::cout << "fName: " << fName << "fCharge: " << fCharge << "fMass: " << fMass << std::endl;
}