#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP

class ParticleType
{
 public:
  ParticleType(const char* name, double mass, int charge);
  virtual ~ParticleType() = default;

  // Getters
  const char* GetName() const;
  double GetMass() const;
  int GetCharge() const;
  virtual double GetWidth() const;

  // Print attributes
  virtual void Print() const;

 private:
  const char* fName;
  const double fMass;
  const int fCharge;
};

#endif