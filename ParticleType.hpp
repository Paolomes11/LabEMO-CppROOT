#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP

class ParticleType
{
public:
    ParticleType(const char* name, double mass, int charge); // ctor
    virtual ~ParticleType() = default;

    // getters
    const char* GetName() const;
    double GetMass() const;
    int GetCharge() const;

    // print attributes
    virtual void Print() const;

private:
    const char *fName;
    const double fMass;
    const int fCharge;
};

#endif