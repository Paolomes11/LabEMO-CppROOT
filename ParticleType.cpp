#include "ParticleType.hpp"
#include <iostream>
#include <iomanip>

// ctor
ParticleType::ParticleType(const char* name, double mass, int charge) : fName(name), fMass(mass), fCharge(charge)
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

const char* ParticleType::GetName() const
{
    return fName;
}

// printer
void ParticleType::Print() const
{
    std::cout << '\n'
              << std::left << std::setw(8) << "fName: " << std::setw(8) << *fName << '\n'
              << std::setw(8) << "fCharge: " << std::setw(8) << fCharge << '\n'
              << std::setw(8) << "fMass: " << std::setw(8) << fMass << std::endl;
}