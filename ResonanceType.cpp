#include "ResonanceType.hpp"

ResonanceType::ResonanceType(const char &fName, double fMass, int fCharge, double fWidth): fWidth_{fWidth}, ParticleType(&fName, fMass, fCharge) {};

double ResonanceType::GetWidth const{
   return fWidth_;
}
