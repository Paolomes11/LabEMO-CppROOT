#include "ResonanceType.hpp"
#include <iostream>

//ctor
ResonanceType::ResonanceType(const char &Name, double Mass, int Charge, double Width): ParticleType(Name, Mass, Charge), fWidth(Width) {};

//dtro

//getter
double ResonanceType::GetWidth() const{
   return fWidth;
}

//printer ovveride
void ResonanceType::Print() const{
    ParticleType::Print();
    std::cout << "fWidth: " << fWidth <<std::endl;
}
