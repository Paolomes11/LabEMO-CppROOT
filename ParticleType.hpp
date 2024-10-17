#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP

class ParticleType
{
public:
    ParticleType(const char &fName, double fMass, int fCharge); // ctor
    virtual ~ParticleType();

    // getters
    char &GetName() const;
    double GetMass() const;
    int GetCharge() const;

    // print attributes
    virtual void Print() const;

private:
    char fName_;
    double fMass_;
    int fCharge_;
};

#endif