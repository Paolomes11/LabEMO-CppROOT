#ifndef RESONANCETYPE_HPP
#define RESONANCETYPE_HPP
#include "ParticleType.hpp"

class ResonanceType : public virtual ParticleType
{
 public:
  ResonanceType(const char* Name, const double Mass, const int Charge, const double Width);

  // Getter
  double GetWidth() const override;

  // Print attributes override
  void Print() const override;

 private:
  double const fWidth;
};

#endif