#include "ResonanceType.hpp"

ResonanceType::ResonanceType(const char &fName, double fMass, int fCharge, double fWidth): fWidth_{fWidth}, ParticleType(&fName, fMass, fCharge) {};

double ResonanceType::GetWidth() const{
   return fWidth_;
}

void ResonanceType::Print() const override{
    std::cout << "fName: " << fName_ << "fCharge: " << fCharge_ << "fMass: " << fMass_ << "fWidth: " << fWidth_ <<std::endl;
}
