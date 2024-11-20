#include "ResonanceType.hpp"

ResonanceType::ResonanceType(const char* Name, double Mass, int Charge, double Width)
    : ParticleType(Name, Mass, Charge)
    , fWidth(Width) {};

// Getter
double ResonanceType::GetWidth() const
{
  return fWidth;
}

// Printer ovveride
void ResonanceType::Print() const
{
  ParticleType::Print();
  std::cout << std::left << std::setw(8) << "fWidth: " << std::setw(8) << fWidth << std::endl;
}
