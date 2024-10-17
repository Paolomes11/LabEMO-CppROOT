#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP

class ParticleType
{
public:
    ParticleType(char &fName, double fMass, int fCharge); // ctor
    virtual ~ParticleType() = default;

    // getters
    const char &GetName() const;
    double GetMass() const;
    int GetCharge() const;

    // print attributes
    virtual void Print() const;

private:
    const char fName_;
    const double fMass_;
    const int fCharge_;
};

#endif