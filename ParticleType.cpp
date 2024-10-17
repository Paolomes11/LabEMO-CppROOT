#include "ParticleType.hpp"
#include <iostream>
#include <iomanip>

// ctor
ParticleType::ParticleType(char &fName, double fMass, int fCharge) : fName_(fName), fMass_(fMass), fCharge_(fCharge)
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
    std::cout << std::left << std::setw(8) << "fName: " << std::setw(8) << fName_ << '\n' << std::setw(8) << "fCharge: " << std::setw(8) << fCharge_ << '\n' << std::setw(8) << "fMass: " << std::setw(8) << fMass_ << '\n' << std::endl;
}