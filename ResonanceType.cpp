#include "ResonanceType.hpp"
#include <iostream>

//ctor
ResonanceType::ResonanceType(const char &fName, double fMass, int fCharge, double fWidth): ParticleType(&fName, fMass, fCharge), fWidth_(fWidth) {};

//dtro

//getter
double ResonanceType::GetWidth() const{
   return fWidth_;
}

//printer ovveride
void ResonanceType::Print() const{
    ParticleType::Print();
    std::cout << "fWidth: " << fWidth_ <<std::endl;
}
