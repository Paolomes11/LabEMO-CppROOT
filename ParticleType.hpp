#ifndef PARTICLETYPE_HPP
#define PARTICLETYPE_HPP

class ParticleType
{
public:
    ParticleType(const char &fName, double fMass, int fCharge); // ctor
    

private:
    char *fName_;
    double fMass_;
    int fCharge_;
};

#endif