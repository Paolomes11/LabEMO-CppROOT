#include "ResonanceType.hpp"
#include <iostream>
#include <iomanip>

//ctor
ResonanceType::ResonanceType(char &fName, double fMass, int fCharge, double fWidth): ParticleType(fName, fMass, fCharge), fWidth_(fWidth) {};

//dtro

//getter
double ResonanceType::GetWidth() const{
   return fWidth_;
}

//printer ovveride
void ResonanceType::Print() const{
    ParticleType::Print();
    std::cout << std::right << std::setw(8) << "fWidth: " << fWidth_ <<std::endl;
}
